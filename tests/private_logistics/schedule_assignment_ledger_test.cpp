// Synthetic copied-value tests of the production ledger; not native lifecycle proof.
#include "schedule_assignment_ledger.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
namespace a=private_logistics_assignment;
namespace sch=private_logistics_schedule;
#define CHECK(x) do{if(!(x)){std::fprintf(stderr,"FAIL %d: %s\n",__LINE__,#x);std::exit(1);}}while(0)
#define TEST(name) static void name()
static sch::Snapshot Goods() {
    sch::Snapshot s{};s.status=sch::Status::Complete;s.count=3;
    s.rows[0]={0x10000,true,1,0,1,1};s.rows[1]={0x20000,true,0,1,1,1};
    s.rows[2]={0x30000,true,0,0,1,1};return s;
}
struct Fixture {
    sch::History history;
    a::Ledger ledger;
    sch::Snapshot s=Goods();
    std::uint64_t epoch=1;sch::Address root=0x40000,home=0x30000;
    unsigned sequence=0;
    a::Result Step(bool gap=false,bool fuelValid=true,float litres=100) {
        if(gap) history.Break();
        const auto previous=history.LastValidValues();++sequence;
        const auto d=history.Observe(s,epoch,root,home,sequence,sequence*1000);
        const auto c=d.endpointCompared?sch::ClassifyChanges(previous,s):sch::FieldChanges{};
        return ledger.Observe(s,d,c,epoch,root,home,{sequence,sequence*1000,fuelValid,litres});
    }
};
TEST(adopt_existing_does_not_invent_assignment_start) {
    Fixture f;auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Adopted);
    CHECK(f.ledger.Current().unknownStart && f.ledger.Current().id==1);
    r=f.Step(false,true,90);CHECK(r.count==0 && f.ledger.Current().last.sample==2);
    CHECK(f.ledger.Current().first.litres==100 && f.ledger.Current().last.litres==90);
}
TEST(empty_to_goods_opens_observed_candidate) {
    Fixture f;f.s.count=0;CHECK(f.Step().count==0);f.s=Goods();auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Opened);
    CHECK(!f.ledger.Current().unknownStart && f.ledger.Current().first.sample==2);
}
TEST(target_replacement_closes_then_opens_and_keeps_both_endpoints) {
    Fixture f;f.Step();f.Step(false,true,95);f.s.rows[1].target=0x50000;
    auto r=f.Step(false,true,90);CHECK(r.count==2);
    CHECK(r.events[0].kind==a::Kind::Closed && r.events[1].kind==a::Kind::Opened);
    CHECK(r.events[0].candidate.id==1 && r.events[1].candidate.id==2);
    CHECK(r.events[0].candidate.last.sample==2 && r.events[0].candidate.last.litres==95);
    CHECK(r.events[0].boundary.sample==3 && r.events[0].boundary.litres==90);
    CHECK(r.events[1].candidate.first.sample==3);
}
TEST(scalar_and_index_changes_keep_the_candidate) {
    Fixture f;f.Step();f.s.rows[0].value04=std::nextafter(1.0f,0.0f);f.s.index698=1;
    auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated);
    CHECK(f.ledger.Current().id==1 && f.ledger.Current().first.sample==1);
}
TEST(storage_context_and_row_order_have_distinct_meanings) {
    Fixture f;f.Step();f.s.begin=0x90000;f.s.end=0x90018;f.s.raw610=12;
    auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated);
    auto row=f.s.rows[0];f.s.rows[0]=f.s.rows[1];f.s.rows[1]=row;
    r=f.Step();CHECK(r.count==2 && r.events[0].kind==a::Kind::Closed);
}
TEST(empty_schedule_closes_without_any_cargo_empty_requirement) {
    Fixture f;f.Step();f.s.count=0;f.s.index698=2;auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Closed && !f.ledger.Active());
    CHECK(std::strcmp(r.events[0].reason,"observed_removed")==0);
    CHECK(f.Step().count==0); // no duplicate closing/payment-style summary
}
TEST(office_only_never_opens_goods_but_can_end_observed_structure) {
    Fixture f;f.s.count=1;f.s.rows[0]=Goods().rows[2];CHECK(f.Step().count==0);
    f.s=Goods();CHECK(f.Step().events[0].kind==a::Kind::Opened);
    f.s.count=1;f.s.rows[0]=Goods().rows[2];auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Closed && !f.ledger.Active());
    CHECK(std::strcmp(r.events[0].reason,"observed_office_only")==0);
}
TEST(flag_edit_loses_pattern_but_does_not_replace_same_targets) {
    Fixture f;f.Step();f.s.rows[0].flag00=0;auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated);
    CHECK(f.ledger.Current().id==1 && f.ledger.Current().roleUncertain);
    f.s.rows[0].flag00=1;f.Step();CHECK(f.ledger.Current().id==1);
}
TEST(config_availability_loss_does_not_create_removal) {
    Fixture f;f.Step();f.s.rows[0].configRead=false;auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated && f.ledger.Current().id==1);
    CHECK(f.ledger.Current().roleUncertain);
}
TEST(invalid_read_is_not_empty_or_removal_and_recovery_keeps_id) {
    Fixture f;f.Step();f.s.status=sch::Status::Unreadable;f.s.count=0;auto r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Uncertain && f.ledger.Active());
    CHECK(f.Step().count==0);f.s=Goods();r=f.Step();
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated);
    CHECK(f.ledger.Current().id==1 && f.ledger.Current().observationGap);
}
TEST(identity_switch_abandons_without_claiming_removal) {
    Fixture f;f.Step();f.root+=0x10000;auto r=f.Step();
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Abandoned && r.events[1].kind==a::Kind::Adopted);
    CHECK(r.events[0].candidate.root==0x40000 && f.ledger.Current().root==0x50000);
    CHECK(!r.events[0].boundary.fuelValid && r.events[0].boundary.litres==0);
    CHECK(f.ledger.Current().id==2 && f.ledger.Current().unknownStart);
}
TEST(invalid_other_identity_prevents_switch_back_join) {
    Fixture f;f.Step();f.root+=0x10000;f.s.status=sch::Status::Ineligible;
    auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Abandoned);
    f.root-=0x10000;f.s=Goods();r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Adopted);
    CHECK(f.ledger.Current().id==2);
}
TEST(load_change_never_reuses_old_candidate_id) {
    Fixture f;f.Step();++f.epoch;auto r=f.Step();
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Abandoned && r.events[1].kind==a::Kind::Adopted);
    CHECK(f.ledger.Current().epoch==2 && f.ledger.Current().id==2);
}
TEST(home_change_is_identity_not_completed_assignment) {
    Fixture f;f.Step();f.home=0x60000;auto r=f.Step();
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Abandoned && r.events[1].kind==a::Kind::Adopted);
}
TEST(pause_comparison_closes_observed_targets_but_marks_uncertainty) {
    Fixture f;f.Step();auto r=f.Step(true);CHECK(r.count==1 && r.events[0].kind==a::Kind::Updated);
    CHECK(f.ledger.Current().id==1 && f.ledger.Current().observationGap);
    f.s.rows[1].target=0x60000;r=f.Step(true);CHECK(r.count==2);
    CHECK(r.events[0].candidate.observationGap && r.events[1].candidate.observationGap);
    f.s.count=0;r=f.Step(true);CHECK(r.count==1 && r.events[0].kind==a::Kind::Closed);
}
TEST(explicit_capture_limit_gap_does_not_auto_close) {
    Fixture f;f.Step();f.ledger.Break();CHECK(f.ledger.Active());
    CHECK(f.ledger.Current().observationGap);CHECK(f.Step().count==0);
}
TEST(fuel_invalidity_does_not_become_zero_or_money) {
    Fixture f;f.Step(false,true,std::numeric_limits<float>::quiet_NaN());
    CHECK(!f.ledger.Current().first.fuelValid && f.ledger.Current().first.litres==0);
    f.Step(false,true,-1);CHECK(!f.ledger.Current().last.fuelValid);
    f.s.count=0;auto r=f.Step(false,false,45);
    CHECK(r.events[0].kind==a::Kind::Closed && !r.events[0].boundary.fuelValid);
}
TEST(newly_recognized_flags_adopt_unknown_start_not_open) {
    Fixture f;f.s.rows[0].flag00=0;CHECK(f.Step().count==0);
    f.s.rows[0].flag00=1;auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Adopted);
    CHECK(f.ledger.Current().unknownStart);
}
TEST(out_of_order_metadata_cannot_remove_a_candidate) {
    Fixture f;f.Step();sch::Delta d;d.endpointCompared=true;
    sch::FieldChanges c;c.comparable=true;c.count=true;auto empty=Goods();empty.count=0;
    auto r=f.ledger.Observe(empty,d,c,1,f.root,f.home,{1,999,true,50});
    CHECK(r.count==1 && r.events[0].kind==a::Kind::Uncertain && f.ledger.Current().id==1);
    CHECK(f.ledger.Current().last.sample==1 && f.ledger.Current().observationGap);
}
TEST(unavailable_comparison_abandons_not_settles) {
    Fixture f;f.Step();sch::Delta d;sch::FieldChanges c;
    auto r=f.ledger.Observe(f.s,d,c,1,f.root,f.home,{2,2000,true,50});
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Abandoned && r.events[1].kind==a::Kind::Adopted);
}
TEST(id_exhaustion_saturates_without_reuse) {
    a::Ledger ledger(UINT64_MAX-1);auto s=Goods();sch::Delta d;sch::FieldChanges c;
    auto r=ledger.Observe(s,d,c,1,1,2,{1,1000,true,10});
    CHECK(r.count==1 && ledger.Current().id==UINT64_MAX);
    d.endpointCompared=true;c.comparable=true;c.targets=true;s.rows[1].target=0x70000;
    r=ledger.Observe(s,d,c,1,1,2,{2,2000,true,9});
    CHECK(r.count==2 && r.events[0].kind==a::Kind::Closed && r.events[1].kind==a::Kind::IdExhausted);
    CHECK(!ledger.Active());
}
TEST(oversize_or_missing_identity_never_opens_a_candidate) {
    Fixture f;f.s.count=sch::MaxRows+1;CHECK(f.Step().count==0 && !f.ledger.Active());
    f.s=Goods();f.home=0;CHECK(f.Step().count==0 && !f.ledger.Active());
    f.home=0x30000;f.root=0;CHECK(f.Step().count==0 && !f.ledger.Active());
}
TEST(other_structural_plan_ends_candidate_without_inventing_goods) {
    Fixture f;f.Step();f.s.count=1;f.s.rows[0].target=0x90000;f.s.rows[0].flag00=0;
    auto r=f.Step();CHECK(r.count==1 && r.events[0].kind==a::Kind::Closed && !f.ledger.Active());
}
// ---- Endpoint estimate tests ----
TEST(net_change_consumption_negative_when_level_drops) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();f.Step(false,true,80);
    CHECK(f.ledger.Current().netChangeValid);
    CHECK(f.ledger.Current().netChange==-20.0); // 80-100: level dropped
}
TEST(net_change_refill_positive_when_level_rises) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();f.Step(false,true,120);
    CHECK(f.ledger.Current().netChangeValid);
    CHECK(f.ledger.Current().netChange==20.0); // 120-100: level rose (refill)
}
TEST(net_change_invalid_when_unknown_start) {
    Fixture f;f.s.rows[0].flag00=0;f.s.rows[1].flag00=0;f.s.rows[2].flag00=0;
    f.Step();f.s=Goods();auto r=f.Step(); // Adopted, unknownStart=true
    f.Step(false,true,80);
    CHECK(!f.ledger.Current().netChangeValid);
}
TEST(net_change_invalid_when_gap) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();f.Step(true);
    CHECK(!f.ledger.Current().netChangeValid);
}
TEST(net_change_reset_on_identity_change) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();f.Step(false,true,80);
    CHECK(f.ledger.Current().netChange==-20.0);
    f.root+=0x10000;auto r=f.Step(); // Abandoned + Adopted
    CHECK(r.count==2);
    CHECK(!f.ledger.Current().netChangeValid); // new candidate: unknownStart=true
    CHECK(f.ledger.Current().netChange==0.0);
}
TEST(net_change_preserved_across_scalar_index_changes) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();f.Step(false,true,90);
    f.s.rows[0].value04=std::nextafter(1.0f,0.0f);f.s.index698=1;
    f.Step(false,true,90); // Updated, last stays 90 (pass litres explicitly)
    CHECK(f.ledger.Current().netChangeValid);
    CHECK(f.ledger.Current().netChange==-10.0);
}
TEST(net_change_zero_when_first_equals_last) {
    Fixture f;f.s.count=0;f.Step();f.s=Goods();f.Step();
    CHECK(f.ledger.Current().netChangeValid);
    CHECK(f.ledger.Current().netChange==0.0);
}
// RUN entries are kept explicit so each test is selectable by the Python wrapper.
int main(int argc,char** argv) {
    unsigned count=0;
#define RUN(name) if(argc==1 || std::strcmp(argv[1],#name)==0){name();++count;std::puts("PASS " #name);}
    RUN(adopt_existing_does_not_invent_assignment_start);
    RUN(empty_to_goods_opens_observed_candidate);
    RUN(target_replacement_closes_then_opens_and_keeps_both_endpoints);
    RUN(scalar_and_index_changes_keep_the_candidate);
    RUN(storage_context_and_row_order_have_distinct_meanings);
    RUN(empty_schedule_closes_without_any_cargo_empty_requirement);
    RUN(office_only_never_opens_goods_but_can_end_observed_structure);
    RUN(flag_edit_loses_pattern_but_does_not_replace_same_targets);
    RUN(config_availability_loss_does_not_create_removal);
    RUN(invalid_read_is_not_empty_or_removal_and_recovery_keeps_id);
    RUN(identity_switch_abandons_without_claiming_removal);
    RUN(invalid_other_identity_prevents_switch_back_join);
    RUN(load_change_never_reuses_old_candidate_id);
    RUN(home_change_is_identity_not_completed_assignment);
    RUN(pause_comparison_closes_observed_targets_but_marks_uncertainty);
    RUN(explicit_capture_limit_gap_does_not_auto_close);
    RUN(fuel_invalidity_does_not_become_zero_or_money);
    RUN(newly_recognized_flags_adopt_unknown_start_not_open);
    RUN(out_of_order_metadata_cannot_remove_a_candidate);
    RUN(unavailable_comparison_abandons_not_settles);
    RUN(id_exhaustion_saturates_without_reuse);
    RUN(oversize_or_missing_identity_never_opens_a_candidate);
    RUN(other_structural_plan_ends_candidate_without_inventing_goods);
    RUN(net_change_consumption_negative_when_level_drops);
    RUN(net_change_refill_positive_when_level_rises);
    RUN(net_change_invalid_when_unknown_start);
    RUN(net_change_invalid_when_gap);
    RUN(net_change_reset_on_identity_change);
    RUN(net_change_preserved_across_scalar_index_changes);
    RUN(net_change_zero_when_first_equals_last);
    CHECK(count);std::printf("%u assignment ledger cases PASS (copied values only)\n",count);
}
