// Host fixture for extracted, unmodified VehSlot + CensusFleets bodies.
// Services/native memory are synthetic; this is not Windows lifecycle proof.
#include <array>
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <vector>
using BYTE = unsigned char;
#define VEH_MAX 64
static void* g_vehPtr[VEH_MAX]{};
static double g_vehPeakTons[VEH_MAX]{}, g_vehFuel[VEH_MAX]{}, g_vehCap[VEH_MAX]{};
static unsigned g_vehSamples[VEH_MAX]{};
static bool g_vehFuelValid[VEH_MAX]{};
static unsigned char g_vehCapValid[VEH_MAX]{};
static int g_vehCount=0, g_censusLog=1;
static unsigned g_tankVehDaysRun=0, g_tankDaysRun=0, g_activeVehDaysRun=0, g_idleVehDaysRun=0;
static double g_burnTonsRun=0;
static unsigned g_refillsRun=0;
struct alignas(void*) Vehicle { std::array<BYTE,0x900> bytes{}; };
struct alignas(void*) Office { std::array<BYTE,0xD00> bytes{}; };
static Vehicle vehicles[128];
static Office offices[2];
static std::vector<void*> fleets[2];
static unsigned officeCount=0, reconciliations=0, warnings=0, lastOmitted=0;
static unsigned tankVisits[128]{}, scanVisits[128]{};
static size_t Count() { return officeCount; }
static void* ByIndex(size_t i) { return &offices[i]; }
struct EnterpriseApi { size_t (*getEnterpriseCount)(); void* (*getEnterpriseByIndex)(size_t); };
static EnterpriseApi api{Count,ByIndex};
static EnterpriseApi* g_enterprise=&api;
static bool TsmIsValidGamePtr(void* p) { return p!=nullptr; }
static bool ReadablePtr(const void* p,size_t) { return p!=nullptr; }
static void Logf(const char* format,...) {
    char line[1024]; va_list args; va_start(args,format);
    std::vsnprintf(line,sizeof(line),format,args); va_end(args);
    if (std::strstr(line,"[VEH COVERAGE]")) {
        ++warnings;
        assert(std::strstr(line,"max_slots=64") && std::strstr(line,"existing_slots_retained=1"));
        const char* p=std::strstr(line,"omitted_visits="); assert(p);
        assert(std::sscanf(p,"omitted_visits=%u",&lastOmitted)==1);
    }
}
static unsigned Id(void* p) {
    for(unsigned i=0;i<128;++i) if(p==&vehicles[i]) return i;
    assert(false); return 0;
}
static void BuildingName(void*,char* out,size_t size) { std::snprintf(out,size,"office"); }
static double CargoTons(void*,int* slots) { *slots=1; return 0.5; }
static void CargoDump(int slot,void*,int) { if(slot<0 || slot>=VEH_MAX) return; }
static double TankCapacity(void*) { return 1.0; }
static void ReadTank(int slot,void* p,int,double* burned,unsigned*) {
    if(slot<0 || slot>=VEH_MAX) return; // Same early-return contract as production.
    ++tankVisits[Id(p)]; g_vehFuelValid[slot]=true; g_vehFuel[slot]=0.5; *burned+=0.25;
}
static void ScanVehicle(int slot,void* p,int) {
    assert(slot>=0 && slot<VEH_MAX); ++scanVisits[Id(p)];
}
#include "production_census.inc"
static void Publish() {
    for(unsigned i=0;i<officeCount;++i) {
        void** begin=fleets[i].data(); void** end=begin;
        if(!fleets[i].empty()) end+=fleets[i].size();
        std::memcpy(offices[i].bytes.data()+0xC70,&begin,sizeof(begin));
        std::memcpy(offices[i].bytes.data()+0xC78,&end,sizeof(end));
    }
}
static void Reset() {
    g_enterprise=&api; g_censusLog=1; officeCount=2; reconciliations=warnings=lastOmitted=0;
    g_vehCount=0; g_tankVehDaysRun=g_tankDaysRun=g_activeVehDaysRun=g_idleVehDaysRun=0;
    g_burnTonsRun=0; g_refillsRun=0;
    for(int i=0;i<VEH_MAX;++i) {
        g_vehPtr[i]=nullptr; g_vehPeakTons[i]=g_vehFuel[i]=g_vehCap[i]=0;
        g_vehSamples[i]=0; g_vehFuelValid[i]=false; g_vehCapValid[i]=0;
    }
    for(unsigned i=0;i<128;++i) { tankVisits[i]=scanVisits[i]=0; vehicles[i]=Vehicle{}; }
    for(unsigned i=0;i<2;++i) { offices[i]=Office{}; fleets[i].clear(); }
}
static void Day(int day) {
    Publish(); CensusFleets(day);
    ++reconciliations; // Continuation sentinel, NOT the native target reconciler.
}
static void Fill(unsigned n) { for(unsigned i=0;i<n;++i) fleets[0].push_back(&vehicles[i]); }
int main() {
    Reset(); g_censusLog=0; Fill(1); Day(1);
    assert(g_vehCount==0 && reconciliations==1 && warnings==0);
    Reset(); officeCount=0; Day(1);
    assert(g_vehCount==0 && reconciliations==1 && warnings==0);
    Reset(); Fill(63); Day(1);
    assert(g_vehCount==63 && tankVisits[62]==1 && warnings==0 && g_tankDaysRun==1);
    Reset(); Fill(64); Day(1);
    assert(g_vehCount==64 && tankVisits[63]==1 && warnings==0 && g_burnTonsRun==16.0);
    // A full table must still visit supported identities after a rejected pointer.
    fleets[1]={&vehicles[64],&vehicles[0],&vehicles[65]}; Day(2);
    assert(g_vehCount==64 && tankVisits[64]==0 && scanVisits[65]==0);
    assert(tankVisits[0]==3 && scanVisits[0]==3 && g_vehSamples[0]==3);
    assert(warnings==1 && lastOmitted==2 && reconciliations==2 && g_tankDaysRun==2);
    // Repeated overflowing days report once per census, not once per omitted visit.
    Day(3);
    assert(warnings==2 && lastOmitted==2 && reconciliations==3 && tankVisits[64]==0);
    // Load-like pointer churn: preserve the lifetime table, do not recycle identities.
    fleets[0].clear(); fleets[1]={&vehicles[66],&vehicles[1]}; Day(4);
    assert(warnings==3 && lastOmitted==1 && tankVisits[66]==0 && tankVisits[1]==4);
    assert(g_vehCount==64 && g_vehPtr[1]==&vehicles[1] && reconciliations==4);
    // A full table is not itself an error: existing pointers still fit without warning.
    fleets[1]={&vehicles[2]}; Day(5);
    assert(warnings==3 && tankVisits[2]==4 && reconciliations==5);
    std::puts("8 extracted census capacity scenarios PASS");
}
