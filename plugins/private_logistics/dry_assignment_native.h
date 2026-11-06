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
    char path[MAX_PATH];const int n=std::snprintf(path,sizeof(path),"%s\\%s",dir,filename);
    if(n<0 || (size_t)n>=sizeof(path)) return -1;
    const DWORD attributes=GetFileAttributesA(path);
    if(attributes!=INVALID_FILE_ATTRIBUTES) return (attributes&FILE_ATTRIBUTE_DIRECTORY)?-1:1;
    const DWORD error=GetLastError();
    return (error==ERROR_FILE_NOT_FOUND || error==ERROR_PATH_NOT_FOUND)?0:-1;
}
#endif
