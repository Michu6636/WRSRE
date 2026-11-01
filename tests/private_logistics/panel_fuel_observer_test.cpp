#include "panel_fuel_observer.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <limits>
#include <vector>
#include <string>
#include <utility>
using namespace private_logistics_panel;
#define CHECK(x) do { if (!(x)) { std::fprintf(stderr,"line %d: %s\n",__LINE__,#x); std::exit(1); } } while (0)
struct Region { Address address; std::vector<unsigned char> data; };
struct Fixture {
    std::vector<Region> regions;
    Address window=0x10000, context=0x20000, root=0x30000, meta=0x40000;
    Address home=0x50000, descriptor=0x60000, fleet=0x70000, links=0x80000;
    Address imageBase=0x1000000;
    std::size_t reads = 0;
    std::vector<std::pair<Address,std::size_t>> denied;
    void Add(Address a, std::size_t n) { regions.push_back({a,std::vector<unsigned char>(n)}); }
    template<class T> void Set(Address a, Address off, T v) {
        for(auto& r:regions) if(a+off>=r.address && a+off+sizeof(T)<=r.address+r.data.size()) {
            std::memcpy(r.data.data()+a+off-r.address,&v,sizeof(T));return;
        }
        CHECK(false);
    }
    static bool Reader(Address a, void* out, std::size_t n, void* user) {
        auto& f=*static_cast<Fixture*>(user); ++f.reads;
        for(auto d:f.denied) if(a<d.first+d.second && d.first<a+n) return false;
        for(auto& r:f.regions) if(a>=r.address && a+n>=a && a+n<=r.address+r.data.size()) {
            std::memcpy(out,r.data.data()+a-r.address,n); return true;
        }
        return false;
    }
    Memory Mem() { return {Reader,this}; }
    Fixture() {
        Add(window,0x500);Add(context,0x600);Add(root,0x2000);Add(meta,0x9000);
        Add(home,0x1000);Add(descriptor,0x400);Add(fleet,MaxFleet*8);Add(links,0x100);
        Set(window,0x20,std::uint8_t(1)); Set(window,0x248,1);Set(window,0x24C,2);
        Set(window,0x240,root);Set(context,0x5B0,2);
        Set(root,0x1708,meta);Set(root,0x5F0,0.54954f);Set(meta,0x8684,0.7f);
        Set(root,0x4F8,home);Set(home,0x318,descriptor);Set(descriptor,0x360,43);
        Set(home,0xC70,fleet);Set(home,0xC78,fleet+8);Set(fleet,0,root);
    }
    Snapshot CaptureNow() { Snapshot s; CHECK(Capture(Mem(),context,window,imageBase,s));return s; }
    void Link(float raw, float capacity) {
        Address child=0x90000, childMeta=0xA0000;
        Add(child,0x2000);Add(childMeta,0x9000);
        Set(root,0x3A0,links);Set(root,0x3A8,links+8);Set(links,0,child);
        Set(child,0x1708,childMeta);Set(child,0x5F0,raw);Set(childMeta,0x8684,capacity);
        Set(child,0x4F8,home);Set(fleet,8,child);Set(home,0xC78,fleet+16);
    }
};
#define TEST(name) static void name()
TEST(literal_litres_and_identity) {
    Fixture f;auto s=f.CaptureNow();CHECK(s.status==Status::Complete && s.aggregateValid);
    CHECK(s.current==0.54954f && s.litresCandidate==0.54954f*1000.0f);
    char text[32];std::snprintf(text,sizeof(text),"%.2fl",double(s.litresCandidate));CHECK(std::string(text)=="549.54l");
    CHECK(s.count==1 && s.objects[0].match==Match::Found && s.objects[0].fleetIndex==0);
    CHECK(s.objects[0].occurrences==1 && s.ratioValid);
}
TEST(root_plus_child) {
    Fixture f; f.Link(0.1f,0.2f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.count==2 && s.current==0.54954f+0.1f);
    CHECK(s.objects[1].match==Match::Found && s.objects[1].fleetIndex==1);
}
TEST(nonpositive_capacity_excludes_current) {
    Fixture f;f.Link(500.0f,0.0f);auto s=f.CaptureNow();CHECK(s.current==0.54954f);
    f.Set(f.meta,0x8684,0.0f);s=f.CaptureNow();CHECK(s.current==0 && s.litresCandidate==0 && !s.ratioValid);
}
TEST(negative_raw_is_preserved_not_burn) {
    Fixture f;f.Set(f.root,0x5F0,-0.03f);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.current==0 && s.objects[0].raw==-0.03f);
}
TEST(nonfinite_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::quiet_NaN());auto s=f.CaptureNow();
    CHECK(s.status==Status::NonFinite && !s.aggregateValid);
    f.Set(f.root,0x5F0,1.0f);f.Set(f.meta,0x8684,std::numeric_limits<float>::infinity());
    CHECK(f.CaptureNow().status==Status::NonFinite);
}
TEST(arithmetic_overflow_is_unavailable) {
    Fixture f;f.Set(f.root,0x5F0,std::numeric_limits<float>::max());auto s=f.CaptureNow();
    CHECK(s.status==Status::Overflow && !s.aggregateValid);
}
TEST(optional_capacity_adjustment_is_not_guessed) {
    Fixture f;f.Set(f.root,0x19E0,f.links);f.Set(f.root,0x19E8,f.links+8);auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.adjustmentNeeded && !s.ratioValid);
    f.denied.push_back({f.root+0x19E0,16});s=f.CaptureNow();CHECK(s.aggregateValid && !s.adjustmentRead && !s.ratioValid);
}
TEST(window_gates_and_4f8_not_required) {
    Fixture f;Snapshot s;CHECK(Capture(f.Mem(),f.context,f.window,f.imageBase,s));
    f.Set(f.window,0x4F8,std::uint8_t(0));CHECK(Capture(f.Mem(),f.context,f.window,f.imageBase,s));
    for(Address off:{Address(0x34),Address(0x23C)}) {f.Set(f.window,off,9);CHECK(!Capture(f.Mem(),f.context,f.window,f.imageBase,s));f.Set(f.window,off,0);}
    f.Set(f.window,0x248,3);CHECK(!Capture(f.Mem(),f.context,f.window,f.imageBase,s));f.Set(f.window,0x248,1);
    f.Set(f.window,1,std::uint8_t(1));CHECK(!Capture(f.Mem(),f.context,f.window,f.imageBase,s));
    f.Set(f.window,1,std::uint8_t(0));f.Set(f.window,0x20,std::uint8_t(0));CHECK(!Capture(f.Mem(),f.context,f.window,f.imageBase,s));
}
TEST(invalid_links_never_produce_partial_total) {
    Fixture f;f.Set(f.root,0x3A0,f.links);f.Set(f.root,0x3A8,f.links+MaxObjects*8);
    auto s=f.CaptureNow();CHECK(s.status==Status::TooManyLinks && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links+1);s=f.CaptureNow();CHECK(s.status==Status::Unreadable && !s.aggregateValid);
    f.Set(f.root,0x3A8,f.links-8);CHECK(!f.CaptureNow().aggregateValid);
}
TEST(unreadable_current_keeps_identity_but_not_total) {
    Fixture f;f.denied.push_back({f.root+0x5F0,4});auto s=f.CaptureNow();
    CHECK(!s.aggregateValid && s.status==Status::Unreadable && !s.objects[0].quantityRead);
    CHECK(s.objects[0].match==Match::Found);
}
TEST(home_pointer_alone_is_not_membership) {
    Fixture f;f.Set(f.fleet,0,Address(0xDEADBEEF));auto s=f.CaptureNow();
    CHECK(s.objects[0].homeCandidate==f.home && s.objects[0].match==Match::Absent);
    f.Set(f.descriptor,0x360,52);CHECK(f.CaptureNow().objects[0].match==Match::NotRoadOffice);
}
TEST(fleet_bounds_and_duplicate_membership) {
    Fixture f;f.Set(f.home,0xC78,f.fleet+(MaxFleet+1)*8);
    CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+1);CHECK(f.CaptureNow().objects[0].match==Match::InvalidFleet);
    f.Set(f.home,0xC78,f.fleet+16);f.Set(f.fleet,8,f.root);auto s=f.CaptureNow();
    CHECK(s.objects[0].match==Match::Found && s.objects[0].occurrences==2);
    f.denied.push_back({f.fleet+8,8});CHECK(f.CaptureNow().objects[0].match==Match::Unavailable);
}
TEST(snapshot_reuse_clears_old_pointer_and_quantity) {
    Fixture f;Snapshot s=f.CaptureNow();CHECK(s.aggregateValid);
    f.Set(f.window,0x240,Address(0));CHECK(Capture(f.Mem(),f.context,f.window,f.imageBase,s));
    CHECK(!s.aggregateValid && s.root==0 && s.count==0 && s.objects[0].pointer==0);
}
TEST(address_overflow_does_not_call_reader) {
    Fixture f;float value=0;CHECK(!f.Mem().Get(UINTPTR_MAX-1,8,value));CHECK(f.reads==0);
    CHECK(!f.Mem().Get(0,0,value));CHECK(f.reads==0);
}
TEST(flags_are_evidence_not_literal_mode_proof) {
    Fixture f;f.Set(f.meta,0x8681,std::uint8_t(1));auto s=f.CaptureNow();
    CHECK(s.aggregateValid && s.flagsRead && s.flag8681==1 && s.flag8682==0);
    f.denied.push_back({f.meta+0x8682,1});CHECK(!f.CaptureNow().flagsRead);
}
TEST(capture_is_read_only_and_bounded) {
    Fixture f;auto before=f.regions;auto s=f.CaptureNow();CHECK(s.aggregateValid && f.reads<100);
    CHECK(before.size()==f.regions.size());for(std::size_t i=0;i<before.size();++i) CHECK(before[i].data==f.regions[i].data);
}
TEST(root_514_and_4f0_read_independently) {
    Fixture f;
    f.Set(f.root,0x514,std::uint8_t(0x42));
    f.Set(f.root,0x4F0,f.home);
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(s.root514Read && s.root514==0x42);
    CHECK(s.root4F0Read && s.root4F0==f.home);
}
TEST(root_514_read_fails_but_4f0_succeeds) {
    Fixture f;
    f.Set(f.root,0x4F0,f.home);
    f.denied.push_back({f.root+0x514,1});
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(!s.root514Read);
    CHECK(s.root4F0Read && s.root4F0==f.home);
}
TEST(global_reads_signed_int32_at_image_base) {
    Fixture f;f.Add(f.imageBase,0xA00000);
    f.Set(f.imageBase,0x9D54C0,std::int32_t(-7));
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(s.gateGlobalRead && s.gateGlobal==-7);
    f.Set(f.imageBase,0x9D54C0,std::int32_t(42));
    s=f.CaptureNow();
    CHECK(s.gateGlobalRead && s.gateGlobal==42);
    f.Set(f.imageBase,0x9D54C0,std::int32_t(0));
    s=f.CaptureNow();
    CHECK(s.gateGlobalRead && s.gateGlobal==0);
}
TEST(global_unavailable_when_image_base_zero) {
    Fixture f;
    Snapshot s;
    CHECK(Capture(f.Mem(),f.context,f.window,0,s));
    CHECK(s.aggregateValid);
    CHECK(!s.gateGlobalRead);
}
TEST(global_read_denied_does_not_invalidate_capture) {
    Fixture f;f.Add(f.imageBase,0xA00000);
    f.denied.push_back({f.imageBase+0x9D54C0,4});
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(!s.gateGlobalRead);
}
TEST(object_1794_and_8680_read_independently) {
    Fixture f;
    f.Link(0.1f,0.2f);
    f.Set(f.root,0x1794,std::uint32_t(5));
    f.Set(f.meta,0x8680,std::uint8_t(0));
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(s.objects[0].raw1794Read && s.objects[0].raw1794==5);
    CHECK(s.objects[0].proto8680Read && s.objects[0].proto8680==0);
    f.Set(0x90000,0x1794,std::uint32_t(3));
    f.Set(0xA0000,0x8680,std::uint8_t(1));
    s=f.CaptureNow();
    CHECK(s.objects[1].raw1794Read && s.objects[1].raw1794==3);
    CHECK(s.objects[1].proto8680Read && s.objects[1].proto8680==1);
}
TEST(object_read_failure_does_not_invalidate_capture) {
    Fixture f;
    f.Link(0.1f,0.2f);
    f.Set(f.root,0x1794,std::uint32_t(5));
    f.denied.push_back({f.root+0x1794,4});
    auto s=f.CaptureNow();
    CHECK(s.aggregateValid);
    CHECK(!s.objects[0].raw1794Read);
    CHECK(s.objects[1].quantityRead);
}
struct Case {const char* name;void(*run)();};
static const Case cases[]={
#define CASE(name) {#name,name}
CASE(literal_litres_and_identity),CASE(root_plus_child),CASE(nonpositive_capacity_excludes_current),
CASE(negative_raw_is_preserved_not_burn),CASE(nonfinite_is_unavailable),CASE(arithmetic_overflow_is_unavailable),
CASE(optional_capacity_adjustment_is_not_guessed),CASE(window_gates_and_4f8_not_required),
CASE(invalid_links_never_produce_partial_total),CASE(unreadable_current_keeps_identity_but_not_total),
CASE(home_pointer_alone_is_not_membership),CASE(fleet_bounds_and_duplicate_membership),
CASE(snapshot_reuse_clears_old_pointer_and_quantity),CASE(address_overflow_does_not_call_reader),
CASE(flags_are_evidence_not_literal_mode_proof),CASE(capture_is_read_only_and_bounded),
CASE(root_514_and_4f0_read_independently),CASE(root_514_read_fails_but_4f0_succeeds),
CASE(global_reads_signed_int32_at_image_base),CASE(global_unavailable_when_image_base_zero),
CASE(global_read_denied_does_not_invalidate_capture),CASE(object_1794_and_8680_read_independently),
CASE(object_read_failure_does_not_invalidate_capture)
};
int main(int argc,char**argv) {
    unsigned count=0;for(const auto& c:cases)if(argc==1 || std::strcmp(argv[1],c.name)==0) {c.run();std::printf("PASS %s\n",c.name);++count;}
    CHECK(count>0);std::printf("%u native scenarios PASS\n",count);return 0;
}
