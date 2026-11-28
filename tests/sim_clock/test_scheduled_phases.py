import os
import shutil
import subprocess
import tempfile
import unittest

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
SRC_DIR = os.path.join(REPO_ROOT, "src")

PROBE_CPP = r"""
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include "sim_clock_api.h"

struct Sub {
    int phase;
    TsmDayCallback cb;
    void* ud;
};

static std::vector<Sub> g_subs;
static std::vector<int> g_log;
static int g_legacyCalls = 0;

static void RecordCb(void* ud, int, int, int) {
    g_log.push_back((int)(intptr_t)ud);
}

static int FakeRegisterScheduled(TsmScheduleFrequency, TsmDayCallback cb, void* ud) {
    g_legacyCalls++;
    g_subs.push_back({TSM_PHASE_DEFAULT, cb, ud});
    return (int)g_subs.size();
}

static int FakeRegisterScheduledPhase(TsmScheduleFrequency, int phase, TsmDayCallback cb, void* ud) {
    g_subs.push_back({phase, cb, ud});
    return (int)g_subs.size();
}

static void DispatchSorted() {
    std::vector<int> order;
    for (int i = 0; i < (int)g_subs.size(); i++) order.push_back(i);
    for (int i = 1; i < (int)order.size(); i++) {
        int idx = order[i];
        int keyPhase = g_subs[idx].phase;
        int j = i - 1;
        while (j >= 0 && g_subs[order[j]].phase > keyPhase) {
            order[j + 1] = order[j];
            j--;
        }
        order[j + 1] = idx;
    }
    for (int k : order) {
        g_subs[k].cb(g_subs[k].ud, 1, 2, 1960);
    }
}

int main() {
    // 1. Canonical phase ordering
    assert(TSM_PHASE_PRE_FISCAL < TSM_PHASE_FISCAL);
    assert(TSM_PHASE_FISCAL < TSM_PHASE_DEFAULT);
    assert(TSM_PHASE_DEFAULT < TSM_PHASE_ENTERPRISE_CLOSE);
    assert(TSM_PHASE_ENTERPRISE_CLOSE < TSM_PHASE_POST_CLOSE);

    // 2. Full API structSize uses registerScheduledPhase
    TsmSimClockApi api{};
    api.structSize = sizeof(TsmSimClockApi);
    api.registerScheduled = FakeRegisterScheduled;
    api.registerScheduledPhase = FakeRegisterScheduledPhase;

    // Register out of order: enterprise(30), currency(35), legacy1(25), taxes(20), bank(20), labor(10), legacy2(25)
    TsmSimClockRegisterScheduledPhase(&api, TSM_FREQ_MONTHLY, TSM_PHASE_ENTERPRISE_CLOSE, RecordCb, (void*)30);
    TsmSimClockRegisterScheduledPhase(&api, TSM_FREQ_MONTHLY, TSM_PHASE_POST_CLOSE, RecordCb, (void*)35);
    api.registerScheduled(TSM_FREQ_MONTHLY, RecordCb, (void*)251);
    TsmSimClockRegisterScheduledPhase(&api, TSM_FREQ_MONTHLY, TSM_PHASE_FISCAL, RecordCb, (void*)201);
    TsmSimClockRegisterScheduledPhase(&api, TSM_FREQ_MONTHLY, TSM_PHASE_FISCAL, RecordCb, (void*)202);
    TsmSimClockRegisterScheduledPhase(&api, TSM_FREQ_MONTHLY, TSM_PHASE_PRE_FISCAL, RecordCb, (void*)10);
    api.registerScheduled(TSM_FREQ_MONTHLY, RecordCb, (void*)252);

    DispatchSorted();
    std::vector<int> expected = {10, 201, 202, 251, 252, 30, 35};
    assert(g_log == expected);

    // 3. Older host structSize falls back cleanly to registerScheduled
    TsmSimClockApi oldApi{};
    oldApi.structSize = (unsigned)offsetof(TsmSimClockApi, registerScheduledPhase);
    oldApi.registerScheduled = FakeRegisterScheduled;
    oldApi.registerScheduledPhase = FakeRegisterScheduledPhase;
    int before = g_legacyCalls;
    TsmSimClockRegisterScheduledPhase(&oldApi, TSM_FREQ_MONTHLY, TSM_PHASE_FISCAL, RecordCb, (void*)99);
    assert(g_legacyCalls == before + 1);

    std::puts("sim_clock_phases_ok");
    return 0;
}
"""


class SimClockPhaseTests(unittest.TestCase):
    def test_phase_ordering_and_structsize_fallback(self):
        compiler = shutil.which("g++") or shutil.which("clang++")
        if not compiler:
            self.skipTest("C++ compiler required (g++ or clang++)")
        with tempfile.TemporaryDirectory() as tmp:
            src = os.path.join(tmp, "probe.cpp")
            exe = os.path.join(tmp, "probe.exe" if os.name == "nt" else "probe")
            with open(src, "w", encoding="utf-8") as f:
                f.write(PROBE_CPP)
            res = subprocess.run(
                [compiler, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 "-I", SRC_DIR, src, "-o", exe],
                capture_output=True, text=True
            )
            self.assertEqual(res.returncode, 0, msg=res.stderr)
            run = subprocess.run([exe], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, msg=run.stderr)
            self.assertIn("sim_clock_phases_ok", run.stdout)


if __name__ == "__main__":
    unittest.main()
