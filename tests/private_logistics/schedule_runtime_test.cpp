// Production runtime through the same portable shim. No DLL/SEH/thread proof.
#define PANEL_RUNTIME_TEST_NO_MAIN
#include "panel_fuel_runtime_test.cpp"

int main() {
    Fixture fixture;
    for(const auto& r:fixture.regions) std::memcpy(g_exeBase+r.address,r.data.data(),r.data.size());
    const auto ptr=[&](Address a){return (Address)(g_exeBase+a);};
    const auto put=[&](Address a,Address v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    const auto put32=[&](Address a,int v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    const auto putFloat=[&](Address a,float v){std::memcpy(g_exeBase+a,&v,sizeof(v));};
    put(fixture.window+0x240,ptr(fixture.root));put(fixture.root+0x1708,ptr(fixture.meta));
    put(fixture.root+0x4F8,ptr(fixture.home));put(fixture.home+0x318,ptr(fixture.descriptor));
    put(fixture.home+0xC70,ptr(fixture.fleet));put(fixture.home+0xC78,ptr(fixture.fleet+8));put(fixture.fleet,ptr(fixture.root));
    auto* dos=(IMAGE_DOS_HEADER*)g_exeBase;dos->e_magic=IMAGE_DOS_SIGNATURE;dos->e_lfanew=0x80;
    auto* nt=(IMAGE_NT_HEADERS64*)(g_exeBase+0x80);nt->Signature=IMAGE_NT_SIGNATURE;
    nt->FileHeader.Machine=IMAGE_FILE_MACHINE_AMD64;nt->FileHeader.TimeDateStamp=0x6A3EB6AD;
    nt->OptionalHeader.Magic=IMAGE_NT_OPTIONAL_HDR64_MAGIC;
    const BYTE prologue[]={0x40,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57,0x48,0x8D,0xAC,0x24,0x50,0xD1,0xFF,0xFF};
    std::memcpy(g_exeBase+0x7D5460,prologue,sizeof(prologue));
    const BYTE scale[]={0,0,0x7A,0x44},format[]={0x25,0,0x2e,0,0x32,0,0x66,0,0x6c,0,0,0};
    std::memcpy(g_exeBase+0x90B07C,scale,sizeof(scale));std::memcpy(g_exeBase+0x8F3E98,format,sizeof(format));
    for(const auto& f:g_scheduleFingerprints)std::memcpy(g_exeBase+f.rva,f.bytes,f.size);
    g_scheduleObserverEnabled=1;
    g_exeBase[0x7E8438]^=1;PanelObserverStart();CHECK(g_panelStarted && !g_scheduleReady && installed==1);
    g_exeBase[0x7E8438]^=1; // no second hook/start; emulate fresh process gate result for remainder
    g_scheduleReady=ScheduleTargetMatches();CHECK(g_scheduleReady);
    std::puts("PASS schedule_runtime_mismatch_disables_schedule_not_fuel");
    Address targets=0xD0000,configs=0xD1000;
    put(fixture.root+0x680,ptr(targets));put(fixture.root+0x688,ptr(targets+16));put(fixture.root+0x638,ptr(configs));
    put(targets,Address(0xDEAD0000));put(targets+8,Address(0xBEEF0000));
    putFloat(configs+4,1.0f);putFloat(configs+0x198+0xCC,0.5f);
    void* context=(void*)ptr(fixture.context);void* window=(void*)ptr(fixture.window);
    auto capture=[&](){clockMs+=1000;PanelCapture(context,window);};
    auto drain=[&](){tick(nullptr,1,1);};
    auto contains=[&](const char* text){for(const auto& l:logs)if(l.find(text)!=std::string::npos)return true;return false;};
    logs.clear();capture();CHECK(logs.empty());CHECK(g_panelCount==1);
    CHECK(g_panelQueue[g_panelHead].schedule.status==private_logistics_schedule::Status::Complete);
    drain();CHECK(logs.size()==6 && contains("baseline=first_observation") && contains("target_raw=0xdead0000"));
    CHECK(contains("native_job_id=unverified buyer=unverified completion=unverified"));
    std::puts("PASS schedule_runtime_same_post_render_record_deferred_rows_and_truthful_labels");
    logs.clear();capture();drain();CHECK(logs.size()==2); // unchanged: fuel/object only
    put32(fixture.root+0x698,1);logs.clear();capture();drain();
    CHECK(logs.size()==6 && contains("observed_fields_changed=0 storage_changed=0 row_index_changed=1"));
    std::puts("PASS schedule_runtime_unchanged_suppressed_and_row_index_not_job_end");
    paused=true;capture();putFloat(configs+4,0.25f);capture();
    CHECK(g_panelCount==1 && g_panelQueue[g_panelHead].schedule.rows[0].value04==0.25f);
    logs.clear();paused=false;drain();CHECK(contains("baseline=diagnostic_gap") && contains("value04=0.25"));
    CHECK(contains("[PANEL SCHEDULE ENDPOINT]") && contains("endpoint_fields_differ=1"));
    CHECK(contains("compared_across_gap=1 continuity_valid=0"));
    CHECK(contains("[PANEL SCHEDULE PREVIOUS ROW]") && contains("values=cached_not_native_read"));
    std::puts("PASS schedule_runtime_paused_coalescing_carries_latest_schedule_and_rebaselines");
    // User-test shape: three rows are replaced in place across paused captures.
    put(fixture.root+0x688,ptr(targets+24));put(targets+16,ptr(fixture.home));
    logs.clear();capture();drain();
    const unsigned lastValidSample=g_panelSamples;
    paused=true;capture();put(targets+8,Address(0xABCE0000));putFloat(configs+4,0.50917542f);
    capture();logs.clear();paused=false;drain();
    CHECK(contains("baseline=diagnostic_gap observed_fields_changed=0 storage_changed=0 row_index_changed=0"));
    CHECK(contains("previous_rows=3 current_rows=3 endpoint_fields_differ=1 endpoint_storage_differ=0"));
    CHECK(contains(("previous_sample="+std::to_string(lastValidSample)+" ").c_str()));
    CHECK(contains("[PANEL SCHEDULE PREVIOUS ROW]") && contains("target_raw=0xbeef0000"));
    CHECK(contains("[PANEL SCHEDULE ROW]") && contains("target_raw=0xabce0000"));
    CHECK(contains("compared_across_gap=1 continuity_valid=0") && contains("native_event_time=unknown"));
    std::puts("PASS schedule_runtime_in_place_three_row_replacement_across_pause");
    paused=true;capture();put(fixture.root+0x688,ptr(targets));capture();
    logs.clear();paused=false;drain();
    CHECK(contains("previous_rows=3 current_rows=0 endpoint_fields_differ=1"));
    CHECK(contains("[PANEL SCHEDULE PREVIOUS ROW]") && !contains("[PANEL SCHEDULE ROW]"));
    CHECK(contains("baseline=diagnostic_gap observed_fields_changed=0"));
    logs.clear();capture();drain();CHECK(!contains("[PANEL SCHEDULE")); // unchanged empty quiet
    put(fixture.root+0x688,ptr(targets+16));logs.clear();capture();drain();
    CHECK(contains("baseline=none observed_fields_changed=1") && !contains("[PANEL SCHEDULE ENDPOINT]"));
    std::puts("PASS schedule_runtime_clear_across_pause_old_rows_and_recovery");
    // Classification is descriptive, using only the existing captured row fields.
    g_exeBase[configs]=1;g_exeBase[configs+0xC8]=0;
    g_exeBase[configs+0x198]=0;g_exeBase[configs+0x198+0xC8]=1;
    logs.clear();capture();drain();CHECK(contains("current_pattern=load_unload_pattern"));
    putFloat(configs+4,0.509832323f);logs.clear();capture();drain();
    CHECK(contains("structure_changed=0 config_availability_changed=0 task_flags_changed=0 scalar_settings_changed=1 settings_only=1"));
    CHECK(contains("native_job_boundary=unverified money_moved=0"));
    put(targets+8,Address(0xABCF0000));logs.clear();capture();drain();
    CHECK(contains("row_count_changed=0 ordered_targets_changed=1 structure_changed=1"));
    CHECK(contains("settings_only=0"));
    put32(fixture.root+0x698,0);logs.clear();capture();drain();
    CHECK(contains("active_index_only=1") && contains("scalar_settings_changed=0 settings_only=0"));
    g_exeBase[configs]=0;logs.clear();capture();drain();
    CHECK(contains("structure_changed=0 config_availability_changed=0 task_flags_changed=1 scalar_settings_changed=0 settings_only=1"));
    g_exeBase[configs]=1;logs.clear();capture();drain();
    g_exeBase[fixture.root+0x5FD]=1;logs.clear();capture();drain();
    CHECK(contains("structure_changed=0 config_availability_changed=1 task_flags_changed=0 scalar_settings_changed=0 settings_only=0"));
    g_exeBase[fixture.root+0x5FD]=0;logs.clear();capture();drain();
    CHECK(contains("config_availability_changed=1 task_flags_changed=0 scalar_settings_changed=0"));
    std::puts("PASS schedule_runtime_classifies_scalar_target_and_index_changes");
    put(fixture.root+0x688,ptr(targets+8));put(targets,ptr(fixture.home));
    g_exeBase[configs]=0;g_exeBase[configs+0xC8]=0;
    logs.clear();capture();drain();CHECK(contains("current_pattern=office_only_pattern"));
    put(fixture.root+0x688,ptr(targets));logs.clear();capture();drain();
    CHECK(contains("previous_pattern=office_only_pattern current_pattern=empty"));
    put(fixture.root+0x688,ptr(targets+16));put(targets,Address(0xDEAD0000));g_exeBase[configs]=1;
    logs.clear();capture();drain();CHECK(contains("current_pattern=load_unload_pattern"));
    std::puts("PASS schedule_runtime_classifies_office_empty_and_load_unload_patterns");
    paused=true;capture();putFloat(configs+4,0.51f);capture();logs.clear();paused=false;drain();
    CHECK(contains("comparison_available=1 compared_across_gap=1"));
    CHECK(contains("structure_changed=0 config_availability_changed=0 task_flags_changed=0 scalar_settings_changed=1 settings_only=1"));
    CHECK(contains("compared_across_gap=1 continuity_valid=0"));
    std::puts("PASS schedule_runtime_classification_preserves_gap_semantics");
    CHECK(g_scheduleLedger.Active());
    const auto originalId=g_scheduleLedger.Current().id;
    const auto firstLedgerSample=g_scheduleLedger.Current().first.sample;
    logs.clear();capture();drain();
    CHECK(logs.size()==2 && g_scheduleLedger.Current().last.sample==g_panelSamples);
    putFloat(configs+4,0.52f);put32(fixture.root+0x698,1);logs.clear();capture();drain();
    CHECK(contains("[PANEL ASSIGNMENT]") && contains("event=observed_update reason=same_observed_targets"));
    CHECK(!contains("event=observed_close") && !contains("event=observed_open"));
    CHECK(g_scheduleLedger.Current().id==originalId && g_scheduleLedger.Current().first.sample==firstLedgerSample);
    CHECK(contains("cost=not_computed settlement=none money_moved=0"));
    std::puts("PASS schedule_runtime_ledger_quiet_endpoints_and_scalar_index_keep_id");
    put(targets+8,Address(0xABC10000));logs.clear();capture();drain();
    CHECK(contains("event=observed_close reason=observed_targets_replaced"));
    CHECK(contains("event=observed_open reason=observed_appearance"));
    CHECK(g_scheduleLedger.Current().id==originalId+1);
    put(fixture.root+0x688,ptr(targets+15));logs.clear();capture();drain();
    CHECK(contains("event=observation_unavailable reason=invalid_capture_not_removal"));
    CHECK(!contains("event=observed_close") && g_scheduleLedger.Current().id==originalId+1);
    put(fixture.root+0x688,ptr(targets+16));logs.clear();capture();drain();
    CHECK(g_scheduleLedger.Current().id==originalId+1 && g_scheduleLedger.Current().observationGap);
    std::puts("PASS schedule_runtime_ledger_replacement_and_invalid_recovery");
    paused=true;capture();put(fixture.root+0x688,ptr(targets));capture();logs.clear();paused=false;drain();
    CHECK(contains("event=observed_close reason=observed_removed") && contains("observation_gap=1"));
    CHECK(!g_scheduleLedger.Active() && contains("current_pattern=empty"));
    logs.clear();capture();drain();CHECK(!contains("[PANEL ASSIGNMENT]"));
    put(fixture.root+0x688,ptr(targets+16));logs.clear();capture();drain();
    CHECK(g_scheduleLedger.Active() && contains("event=observed_open"));
    std::puts("PASS schedule_runtime_ledger_clear_across_pause_no_duplicate_close");
    PanelObserverLoading();drain();logs.clear();capture();drain();CHECK(contains("baseline=load_epoch"));
    CHECK(!contains("[PANEL SCHEDULE ENDPOINT]"));
    CHECK(contains("comparison_available=0") && contains("previous_pattern=unavailable"));
    CHECK(contains("event=abandon_unobserved reason=identity_or_load_changed") && contains("event=adopt_existing"));
    put(fixture.fleet,Address(0));logs.clear();capture();drain();
    CHECK(contains("status=not_unique_road_fleet_root") && !contains("[PANEL SCHEDULE ROW]"));
    CHECK(!contains("[PANEL SCHEDULE CLASSIFICATION]"));
    put(fixture.fleet,ptr(fixture.root));logs.clear();capture();drain();CHECK(contains("baseline=diagnostic_gap"));
    put(fixture.fleet+8,ptr(fixture.root));put(fixture.home+0xC78,ptr(fixture.fleet+16));
    logs.clear();capture();drain();CHECK(contains("status=not_unique_road_fleet_root"));
    put(fixture.home+0xC78,ptr(fixture.fleet+8));
    std::puts("PASS schedule_runtime_load_absent_and_duplicate_fleet_fail_closed");
    // Explicitly cover delayed capture-drop counter advance after a drain-side report.
    logs.clear();capture();drain();g_scheduleHistory.Break();capture();drain();
    ++g_panelDropped;drain(); // report at drain BEFORE the next accepted capture
    logs.clear();capture();drain();CHECK(contains("baseline=diagnostic_gap"));
    std::puts("PASS schedule_runtime_capture_drop_counter_advance_rebaselines");
    // Huge raw list is not a partial/empty schedule. Fuel continues unchanged.
    put(fixture.root+0x688,ptr(targets+17*8));logs.clear();capture();drain();
    CHECK(contains("status=too_many_rows_no_partial_table") && !contains("[PANEL SCHEDULE ROW]") && contains("[PANEL FUEL]"));
    put(fixture.root+0x688,ptr(targets+16));put(fixture.root+0x638,Address(1));
    logs.clear();capture();drain();CHECK(contains("status=unreadable") && !g_panelFault);
    put(fixture.root+0x638,ptr(configs));
    std::puts("PASS schedule_runtime_invalid_schedule_never_blocks_fuel_or_prints_partial_rows");
    // Inject a C++ exception through the shim at a schedule-only address.
    // This exercises the production isolated catch/latch, NOT Windows SEH.
    throwAtRead=ptr(fixture.root+0x698);logs.clear();capture();throwAtRead=0;drain();
    CHECK(g_scheduleFault && contains("guarded_schedule_fault_disabled") && contains("[PANEL FUEL]") && !g_panelFault);
    logs.clear();capture();drain();CHECK(contains("guarded_schedule_fault_disabled") && !g_panelFault);
    std::puts("PASS schedule_runtime_fault_latch_isolated_from_fuel_not_a_windows_seh_test");
    std::puts("16 schedule runtime host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");
}
