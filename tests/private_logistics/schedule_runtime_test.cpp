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
    drain();CHECK(logs.size()==5 && contains("baseline=first_observation") && contains("target_raw=0xdead0000"));
    CHECK(contains("native_job_id=unverified buyer=unverified completion=unverified"));
    std::puts("PASS schedule_runtime_same_post_render_record_deferred_rows_and_truthful_labels");
    logs.clear();capture();drain();CHECK(logs.size()==2); // unchanged: fuel/object only
    put32(fixture.root+0x698,1);logs.clear();capture();drain();
    CHECK(logs.size()==5 && contains("observed_fields_changed=0 storage_changed=0 row_index_changed=1"));
    std::puts("PASS schedule_runtime_unchanged_suppressed_and_row_index_not_job_end");
    paused=true;capture();putFloat(configs+4,0.25f);capture();
    CHECK(g_panelCount==1 && g_panelQueue[g_panelHead].schedule.rows[0].value04==0.25f);
    logs.clear();paused=false;drain();CHECK(contains("baseline=diagnostic_gap") && contains("value04=0.25"));
    std::puts("PASS schedule_runtime_paused_coalescing_carries_latest_schedule_and_rebaselines");
    PanelObserverLoading();drain();logs.clear();capture();drain();CHECK(contains("baseline=load_epoch"));
    put(fixture.fleet,Address(0));logs.clear();capture();drain();
    CHECK(contains("status=not_unique_road_fleet_root") && !contains("[PANEL SCHEDULE ROW]"));
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
    std::puts("8 schedule runtime host-shim scenarios PASS (not Windows ABI/SEH or a DLL)");
}
