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
    CHECK(count);std::printf("%u schedule host scenarios PASS (not native lifecycle validation)\n",count);
}
