#define main ReaderHarnessMain
#include "panel_fuel_observer_test.cpp"
#undef main
#include "schedule_observer.h"
namespace sch=private_logistics_schedule;
struct ScheduleFixture : Fixture {
    Address targets=0xB0000,configs=0xC0000;
    ScheduleFixture() {
        Add(targets,sch::MaxRows*8);Add(configs,sch::MaxRows*sch::ConfigStride);
        Set(root,0x680,targets);Set(root,0x688,targets+16);Set(root,0x638,configs);
        Set(root,0x698,std::int32_t(1));Set(root,0x610,Address(0x123456));
        Set(root,0x650,Address(0xFEDCBA));Set(root,0x4A0,Address(0xFA000));
        Set(targets,0,Address(0xDEAD0000));Set(targets,8,Address(0xBEEF0000));
        Set(configs,0,std::uint8_t(1));Set(configs,4,1.0f);
        Set(configs,sch::ConfigStride+0xC8,std::uint8_t(1));
        Set(configs,sch::ConfigStride+0xCC,0.5f);
    }
    sch::Snapshot Now() {sch::Snapshot s;sch::Capture(Mem(),root,s);return s;}
};
TEST(schedule_selected_fields_mapping_no_pointee_following) {
    ScheduleFixture f;auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==2);
    CHECK(s.index698==1 && s.rows[0].target==0xDEAD0000 && s.rows[1].target==0xBEEF0000);
    CHECK(s.rows[0].configRead && s.rows[0].flag00==1 && s.rows[0].value04==1.0f);
    CHECK(s.rows[1].flagC8==1 && s.rows[1].valueCC==0.5f && s.raw650==0xFEDCBA);
}
TEST(schedule_special_last_row_does_not_read_config) {
    ScheduleFixture f;f.Set(f.root,0x5FD,std::uint8_t(1));
    f.denied.push_back({f.configs+sch::ConfigStride,sch::ConfigStride});
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.rows[0].configRead);
    CHECK(!s.rows[1].configRead && s.rows[1].target==0xBEEF0000);
    f.Set(f.root,0x688,f.targets+8);f.Set(f.root,0x638,Address(0));
    s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==1 && !s.rows[0].configRead);
}
TEST(schedule_empty_list_is_valid_not_completion) {
    ScheduleFixture f;f.Set(f.root,0x680,Address(0));f.Set(f.root,0x688,Address(0));
    f.Set(f.root,0x638,Address(0));auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==0);
    f.Set(f.root,0x680,f.targets);f.Set(f.root,0x688,f.targets);CHECK(f.Now().status==sch::Status::Complete);
}
TEST(schedule_null_target_and_unknown_index_preserved) {
    ScheduleFixture f;f.Set(f.targets,0,Address(0));f.Set(f.root,0x698,std::int32_t(-7));
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && !s.rows[0].target && s.index698==-7);
    f.Set(f.root,0x698,std::int32_t(999));CHECK(f.Now().index698==999);
}
TEST(schedule_missing_header_and_config_fail_closed) {
    ScheduleFixture f;f.denied.push_back({f.root+0x698,4});CHECK(f.Now().status==sch::Status::Unreadable);
    f.denied.clear();f.Set(f.root,0x638,Address(0));CHECK(f.Now().status==sch::Status::MissingConfigs);
    f.Set(f.root,0x638,f.configs);f.denied.push_back({f.configs+4,4});CHECK(f.Now().status==sch::Status::Unreadable);
}
TEST(schedule_bad_bounds_and_oversize_do_not_walk) {
    ScheduleFixture f;f.Set(f.root,0x688,f.targets-8);CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x688,f.targets+7);CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x680,Address(0));CHECK(f.Now().status==sch::Status::BadBounds);
    f.Set(f.root,0x680,f.targets);f.Set(f.root,0x688,f.targets+17*8);f.reads=0;
    CHECK(f.Now().status==sch::Status::TooManyRows && f.reads==8);
}
TEST(schedule_max_rows_and_checked_arithmetic) {
    ScheduleFixture f;f.Set(f.root,0x688,f.targets+sch::MaxRows*8);
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==sch::MaxRows);
    f.Set(f.root,0x638,UINTPTR_MAX-2);CHECK(f.Now().status==sch::Status::Unreadable);
    sch::Capture(f.Mem(),UINTPTR_MAX-0x100,s);CHECK(s.status==sch::Status::Unreadable);
}
TEST(schedule_nonfinite_no_partial_table) {
    ScheduleFixture f;f.Set(f.configs,sch::ConfigStride+0xCC,std::numeric_limits<float>::quiet_NaN());
    CHECK(f.Now().status==sch::Status::NonFinite);
    f.Set(f.configs,sch::ConfigStride+0xCC,std::numeric_limits<float>::infinity());
    CHECK(f.Now().status==sch::Status::NonFinite);
}
struct Mutation {
    ScheduleFixture f;
    Address address=0;unsigned hits=0;
    static bool Read(Address a,void* out,std::size_t n,void* user) {
        auto& m=*static_cast<Mutation*>(user);
        if(a==m.address && ++m.hits==2) {
            if(a==m.f.root+0x698)m.f.Set(m.f.root,0x698,std::int32_t(2));
            else m.f.Set(m.f.configs,4,0.3f);
        }
        return Fixture::Reader(a,out,n,&m.f);
    }
};
TEST(schedule_header_and_row_mutation_rejected) {
    Mutation m;sch::Snapshot s;m.address=m.f.root+0x698;
    sch::Capture({Mutation::Read,&m},m.f.root,s);CHECK(s.status==sch::Status::ChangedDuringRead);
    m.address=m.f.configs+4;m.hits=0;
    sch::Capture({Mutation::Read,&m},m.f.root,s);CHECK(s.status==sch::Status::ChangedDuringRead);
}
TEST(schedule_history_first_unchanged_and_index_only) {
    ScheduleFixture f;auto s=f.Now();sch::History h;
    auto d=h.Observe(s,1,f.root,f.home);CHECK(d.emit && d.baseline==sch::Baseline::First && !d.fields);
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
    s.index698=0;d=h.Observe(s,1,f.root,f.home);CHECK(d.emit && d.index && !d.fields && !d.storage);
    CHECK(d.baseline==sch::Baseline::None); // row advance, NOT job completion
}
TEST(schedule_history_field_storage_and_context_changes_separate) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.rows[0].value04=0.2f;auto d=h.Observe(s,1,f.root,f.home);CHECK(d.fields && !d.index && !d.storage);
    s.configs+=0x10000;d=h.Observe(s,1,f.root,f.home);CHECK(!d.fields && d.storage && !d.index);
    s.raw650=1;d=h.Observe(s,1,f.root,f.home);CHECK(d.context && !d.fields && !d.index);
}
TEST(schedule_history_list_clear_replacement_are_observed_fields_only) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.rows[1].target=0xEEEE;CHECK(h.Observe(s,1,f.root,f.home).fields);
    s.count=0;CHECK(h.Observe(s,1,f.root,f.home).fields);
    // No success/cancel reason/job generation exists in this class.
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
}
TEST(schedule_history_three_one_three_rows_and_same_count_replacement) {
    ScheduleFixture f;
    const Address originalFirst=0xDEAD0000, singleTarget=0xABCD0000;
    f.Set(f.root,0x688,f.targets+24);
    f.Set(f.targets,16,Address(0xCAFE0000));
    auto s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==3);
    sch::History h;h.Observe(s,1,f.root,f.home);
    // Advancing within the same three rows is not a content replacement.
    f.Set(f.root,0x698,std::int32_t(2));
    s=f.Now();auto d=h.Observe(s,1,f.root,f.home);
    CHECK(d.index && !d.fields && !d.storage);
    // A one-row list is a content change, not proof of a refuelling task.
    f.Set(f.root,0x688,f.targets+8);
    f.Set(f.root,0x698,std::int32_t(0));
    f.Set(f.targets,0,singleTarget);
    s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==1);
    d=h.Observe(s,1,f.root,f.home);CHECK(d.fields && d.storage && d.index);
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
    // Equal row count is not enough: a different target must be detected.
    f.Set(f.targets,0,Address(0xABCE0000));
    s=f.Now();d=h.Observe(s,1,f.root,f.home);
    CHECK(d.fields && !d.storage && !d.index);
    // Returning to three rows is another observed content change.
    f.Set(f.targets,0,originalFirst);f.Set(f.root,0x688,f.targets+24);
    s=f.Now();CHECK(s.status==sch::Status::Complete && s.count==3);
    d=h.Observe(s,1,f.root,f.home);CHECK(d.fields && d.storage && !d.index);
    CHECK(!h.Observe(s,1,f.root,f.home).emit);
}
TEST(schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    s.index698=0;auto d=h.Observe(s,2,f.root,f.home);CHECK(d.baseline==sch::Baseline::Epoch && !d.index);
    d=h.Observe(s,2,f.root+1,f.home);CHECK(d.baseline==sch::Baseline::Identity);
    d=h.Observe(s,2,f.root+1,f.home+1);CHECK(d.baseline==sch::Baseline::Identity);
    h.Break();s.count=0;d=h.Observe(s,2,f.root+1,f.home+1);CHECK(d.baseline==sch::Baseline::Gap && !d.fields);
    s.status=sch::Status::ChangedDuringRead;CHECK(h.Observe(s,2,f.root,f.home).baseline==sch::Baseline::Invalid);
    s=f.Now();d=h.Observe(s,2,f.root,f.home);CHECK(d.baseline==sch::Baseline::Gap && !d.fields);
}
TEST(schedule_identical_visible_schedule_is_not_new_job) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    // Even if an unseen cancellation/reassignment happened, equal selected
    // fields cannot reveal it. No synthesized generation counter or bill.
    for(unsigned i=0;i<1000;++i) CHECK(!h.Observe(s,1,f.root,f.home).emit);
}
TEST(schedule_gap_changed_endpoint_keeps_original_delta_guards) {
    ScheduleFixture f;auto s=f.Now();sch::History h;
    auto d=h.Observe(s,2,f.root,f.home,51,24980906);
    CHECK(!d.endpointCompared && d.baseline==sch::Baseline::First);
    const auto old=h.LastValidValues();
    h.Break();s.rows[1].target=0xABCE0000;s.rows[0].value04=0.50917542f;s.index698=0;
    d=h.Observe(s,2,f.root,f.home,53,25004531);
    CHECK(d.baseline==sch::Baseline::Gap && d.emit);
    CHECK(!d.fields && !d.storage && !d.index && !d.context);
    CHECK(d.endpointCompared && d.endpointAcrossGap && d.endpointFields && d.endpointIndex);
    CHECK(!d.endpointStorage && !d.endpointContext);
    CHECK(d.previousSample==51 && d.previousMs==24980906);
    CHECK(old.rows[1].target==0xBEEF0000 && old.rows[0].value04==1.0f);
    CHECK(!h.Observe(s,2,f.root,f.home,54,25005531).emit);
    h.Break();d=h.Observe(s,2,f.root,f.home,55,25006531);
    CHECK(d.previousSample==54 && d.previousMs==25005531); // last valid, not last emitted
    CHECK(!d.endpointFields && d.endpointAcrossGap);
}
TEST(schedule_gap_empty_then_three_rows_are_endpoint_differences) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home,1,1000);
    h.Break();s.count=0;s.end=s.begin;
    auto d=h.Observe(s,1,f.root,f.home,2,2000);
    CHECK(d.endpointFields && d.endpointStorage && d.endpointAcrossGap && !d.fields);
    CHECK(h.LastValidValues().count==0);
    f.Set(f.root,0x688,f.targets+24);f.Set(f.targets,16,Address(0xCAFE0000));s=f.Now();
    h.Break();d=h.Observe(s,1,f.root,f.home,3,3000);
    CHECK(d.endpointFields && d.endpointAcrossGap && d.previousSample==2 && !d.fields);
    CHECK(h.LastValidValues().count==3);
}
TEST(schedule_gap_unchanged_storage_index_context_are_independent) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home);
    h.Break();auto d=h.Observe(s,1,f.root,f.home);
    CHECK(d.endpointCompared && d.endpointAcrossGap && !d.endpointFields);
    CHECK(!d.endpointStorage && !d.endpointIndex && !d.endpointContext);
    h.Break();s.begin+=0x100;s.end+=0x100;s.configs+=0x100;d=h.Observe(s,1,f.root,f.home);
    CHECK(d.endpointStorage && !d.endpointFields && !d.endpointIndex && !d.endpointContext);
    CHECK(!d.storage); // original flags still guarded on a gap
    h.Break();++s.index698;d=h.Observe(s,1,f.root,f.home);
    CHECK(d.endpointIndex && !d.endpointFields && !d.endpointStorage);
    h.Break();++s.raw610;d=h.Observe(s,1,f.root,f.home);
    CHECK(d.endpointContext && !d.endpointFields && !d.endpointStorage && !d.endpointIndex);
}
TEST(schedule_endpoint_vehicle_home_and_load_never_join) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home,1,1000);
    auto d=h.Observe(s,1,f.root+1,f.home,2,2000);
    CHECK(d.baseline==sch::Baseline::Identity && !d.endpointCompared);
    d=h.Observe(s,1,f.root,f.home,3,3000);CHECK(!d.endpointCompared);
    d=h.Observe(s,1,f.root,f.home+1,4,4000);CHECK(!d.endpointCompared);
    d=h.Observe(s,2,f.root,f.home+1,5,5000);
    CHECK(d.baseline==sch::Baseline::Epoch && !d.endpointCompared);
}
TEST(schedule_endpoint_invalid_is_not_an_empty_schedule) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home,10,1000);
    auto bad=s;bad.status=sch::Status::Unreadable;bad.count=0;
    auto d=h.Observe(bad,1,f.root,f.home,11,2000);
    CHECK(d.baseline==sch::Baseline::Invalid && !d.endpointCompared);
    CHECK(h.LastValidValues().count==2);
    bad.status=sch::Status::TooManyRows;bad.count=sch::MaxRows+1;
    CHECK(!h.Observe(bad,1,f.root,f.home,12,3000).endpointCompared);
    h.Break();s.count=0;s.end=s.begin;d=h.Observe(s,1,f.root,f.home,13,4000);
    CHECK(d.endpointCompared && d.endpointAcrossGap && d.endpointFields);
    CHECK(d.previousSample==10 && d.previousMs==1000 && !d.fields);
    CHECK(!h.Observe(s,1,f.root,f.home,14,5000).emit);
}
TEST(schedule_endpoint_invalid_identity_cannot_resurrect_cache) {
    ScheduleFixture f;auto s=f.Now();auto bad=s;bad.status=sch::Status::Unreadable;sch::History h;
    h.Observe(s,1,f.root,f.home);
    h.Observe(bad,1,f.root+1,f.home);
    CHECK(!h.Observe(s,1,f.root,f.home).endpointCompared);
    h.Observe(bad,2,f.root,f.home);
    CHECK(!h.Observe(s,1,f.root,f.home).endpointCompared);
    h.Observe(bad,1,f.root,f.home+1);
    CHECK(!h.Observe(s,1,f.root,f.home).endpointCompared);
}
TEST(schedule_endpoint_equal_endpoints_do_not_invent_hidden_aba) {
    ScheduleFixture f;auto s=f.Now();sch::History h;h.Observe(s,1,f.root,f.home,1,1000);
    // Any unseen A->B->A remains unknown; equality proves neither presence nor absence.
    h.Break();h.Break();auto d=h.Observe(s,1,f.root,f.home,2,5000);
    CHECK(d.endpointCompared && d.endpointAcrossGap && !d.endpointFields);
    CHECK(!d.endpointIndex && !d.endpointStorage && !d.endpointContext);
    CHECK(!d.fields && !d.index && d.baseline==sch::Baseline::Gap);
}
int main(int argc,char** argv) {
    unsigned count=0;
#define RUN(name) do {if(argc==1 || std::string(argv[1])==#name){name();++count;std::printf("PASS %s\n",#name);}} while(0)
    RUN(schedule_selected_fields_mapping_no_pointee_following);
    RUN(schedule_special_last_row_does_not_read_config);
    RUN(schedule_empty_list_is_valid_not_completion);
    RUN(schedule_null_target_and_unknown_index_preserved);
    RUN(schedule_missing_header_and_config_fail_closed);
    RUN(schedule_bad_bounds_and_oversize_do_not_walk);
    RUN(schedule_max_rows_and_checked_arithmetic);
    RUN(schedule_nonfinite_no_partial_table);
    RUN(schedule_header_and_row_mutation_rejected);
    RUN(schedule_history_first_unchanged_and_index_only);
    RUN(schedule_history_field_storage_and_context_changes_separate);
    RUN(schedule_history_list_clear_replacement_are_observed_fields_only);
    RUN(schedule_history_three_one_three_rows_and_same_count_replacement);
    RUN(schedule_history_epoch_identity_and_gaps_rebaseline_no_deltas);
    RUN(schedule_identical_visible_schedule_is_not_new_job);
    RUN(schedule_gap_changed_endpoint_keeps_original_delta_guards);
    RUN(schedule_gap_empty_then_three_rows_are_endpoint_differences);
    RUN(schedule_gap_unchanged_storage_index_context_are_independent);
    RUN(schedule_endpoint_vehicle_home_and_load_never_join);
    RUN(schedule_endpoint_invalid_is_not_an_empty_schedule);
    RUN(schedule_endpoint_invalid_identity_cannot_resurrect_cache);
    RUN(schedule_endpoint_equal_endpoints_do_not_invent_hidden_aba);
    CHECK(count);std::printf("%u schedule host scenarios PASS (not native lifecycle validation)\n",count);
}
