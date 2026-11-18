// Reuses existing world/type/position helpers. Never follows a saved target pointer.
#ifndef PRIVATE_LOGISTICS_DRY_ASSIGNMENT_NATIVE_H
#define PRIVATE_LOGISTICS_DRY_ASSIGNMENT_NATIVE_H
#include "dry_assignment_ledger.h"
#include <cstdio>
static bool DryIdentitySnapshot(uintptr_t address,private_logistics_dry::Identity& result) {
    result={};
    __try {
        if(!address || !g_exeBase || !g_enterprise ||
           g_enterprise->structSize<offsetof(TsmEnterpriseBaseApi,getBuildingObjectName)+sizeof(g_enterprise->getBuildingObjectName) ||
           !g_enterprise->getBuildingObjectName) return false;
        TsmBuildingSpan span{},check{};
        BYTE* game=TsmGameObject(g_exeBase);
        if(!TsmGetBuildings(game,&span) || !ReadablePtr(span.begin,span.count*sizeof(void*))) return false;
        unsigned matches=0;size_t index=0;
        for(size_t i=0;i<span.count;++i) if((uintptr_t)span.begin[i]==address) {++matches;index=i;}
        if(matches!=1) return false; // No dereference if removed, unknown or duplicated.
        void* target=(void*)address;int type=-1,type2=-1;float x=0,z=0,x2=0,z2=0;
        if(!AutoReadBuildingType(target,&type) || type<0 || !AutoBuildingPosition(target,&x,&z)) return false;
        const char* source=g_enterprise->getBuildingObjectName(target);char name[128]{};
        bool ended=false;
        for(unsigned i=0;source && i<sizeof(name);++i) {
            if(!ReadablePtr(source+i,1)) return false;
            name[i]=source[i];if(!name[i]) {ended=true;break;}
        }
        if(!ended || !AutoReadBuildingType(target,&type2) || !AutoBuildingPosition(target,&x2,&z2) ||
           type2!=type || x2!=x || z2!=z || !TsmGetBuildings(game,&check) ||
           span.begin!=check.begin || span.end!=check.end || span.begin[index]!=target) return false;
        result=private_logistics_dry::MakeIdentity(name,(std::uint32_t)type,x,z);
        return result.valid;
    } __except(EXCEPTION_EXECUTE_HANDLER) {return false;}
}
// 0 missing, 1 exists, -1 inaccessible/error. Called only on the file thread.
static int DryFileState(const char* dir,const char* filename) {
    if(!dir || !filename) return -1;
    // Build the path by hand instead of snprintf: snprintf can only *report*
    // truncation after the fact, so GCC 13+ rejects the call outright under
    // -Werror=format-truncation. This form has no format string to analyse and
    // keeps the previous limit exactly (reject when the joined length would
    // reach MAX_PATH).
    const std::size_t dirLength=std::strlen(dir),fileLength=std::strlen(filename);
    if(dirLength+1+fileLength>=(std::size_t)MAX_PATH) return -1;
    char path[MAX_PATH];
    std::memcpy(path,dir,dirLength);
    path[dirLength]='\\';
    std::memcpy(path+dirLength+1,filename,fileLength+1);   // copies the NUL
    const int n=(int)(dirLength+1+fileLength);
    if(n<0 || (std::size_t)n>=(std::size_t)MAX_PATH) return -1;
    const DWORD attributes=GetFileAttributesA(path);
    if(attributes!=INVALID_FILE_ATTRIBUTES) return (attributes&FILE_ATTRIBUTE_DIRECTORY)?-1:1;
    const DWORD error=GetLastError();
    return (error==ERROR_FILE_NOT_FOUND || error==ERROR_PATH_NOT_FOUND)?0:-1;
}
#endif
