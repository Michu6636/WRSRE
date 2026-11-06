"""Exercise production provenance logic and extracted ReadTank/FuelSlotTons.
These are host tests, never proof of native classification, offsets or hooks.
"""
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
PLUGIN = ROOT / 'plugins/private_logistics'


class FuelDiagnosticsTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.compiler = shutil.which('g++') or shutil.which('clang++')
        if not cls.compiler:
            raise unittest.SkipTest('C++ compiler required')
        cls.directory = tempfile.TemporaryDirectory()
        cls.addClassCleanup(cls.directory.cleanup)
        cls.cpp = (PLUGIN / 'private_logistics.cpp').read_text()

    def compile_and_run(self, path):
        exe = Path(self.directory.name) / 'test'
        result = subprocess.run([self.compiler, '-std=c++17', '-O2', '-Wall', '-Wextra',
                                 '-Werror', '-pedantic', '-I', str(PLUGIN), str(path),
                                 '-o', str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        result = subprocess.run([str(exe)], capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        return result.stdout

    def test_candidate_provenance_core(self):
        output = self.compile_and_run(Path(__file__).with_name('fuel_diagnostics_test.cpp'))
        self.assertIn('8 candidate-provenance scenarios PASS', output)

    def test_actual_readtank_and_slot_reader_with_host_memory(self):
        # Extract the actual unchanged production functions, not a test rewrite.
        slot = self.cpp.split('static double FuelSlotTons(void* v, int* found)', 1)[1]
        slot = 'static double FuelSlotTons(void* v, int* found)' + slot.split(
            '// The panel backing quantity', 1)[0]
        read = self.cpp.split('static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)', 1)[1]
        read = 'static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)' + read.split(
            'static void ScanVehicle', 1)[0]
        source = r'''
#include "fuel_diagnostics.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdarg>
using BYTE=unsigned char; using LONG=long;
constexpr int VEH_MAX=1;
static private_logistics_fuel_diagnostics::Samples g_vehFuelSamples[VEH_MAX];
static volatile LONG g_fuelDiagnosticEpoch=1;
static bool g_vehFuelValid[VEH_MAX]{};
static double g_vehFuel[VEH_MAX]{};
static int g_tankCrossLog=100;
static double displayRaw=0.09;
static std::string lastLog;
static int resource;
static bool ReadablePtr(const void* p, size_t) {return p!=nullptr;}
static void* FuelResourcePtr() {return &resource;}
static LONG InterlockedCompareExchange(volatile LONG* p,LONG,LONG) {return *p;}
static double MirrorTons(void*) {return displayRaw;}
static void Logf(const char*,...) __attribute__((format(printf,1,2)));
static void Logf(const char* fmt,...) {
    char line[2048]; va_list args; va_start(args,fmt);
    std::vsnprintf(line,sizeof(line),fmt,args);va_end(args);lastLog=line;
}
''' + slot + read + r'''
int main() {
    alignas(16) BYTE vehicle[0x600]{}, entry[16]{};
    BYTE* begin=entry; BYTE* end=entry+16;
    std::memcpy(vehicle+0x3B8,&begin,sizeof(begin));
    std::memcpy(vehicle+0x3C0,&end,sizeof(end));
    double decrease=0.0;unsigned increases=0;
    ReadTank(0,vehicle,1,&decrease,&increases);
    assert(g_vehFuelValid[0] && decrease==0.0);
    assert(lastLog.find("selected_source=display_5f0_fallback")!=std::string::npos);
    assert(lastLog.find("drift_valid=0")!=std::string::npos);
    displayRaw=0.07;ReadTank(0,vehicle,2,&decrease,&increases);
    assert(std::fabs(decrease-0.02)<1e-12 && increases==0);
    void* fuel=&resource;std::memcpy(entry,&fuel,sizeof(fuel));
    float value=0.01f;std::memcpy(entry+8,&value,4);
    decrease=0.0;ReadTank(0,vehicle,3,&decrease,&increases);
    assert(decrease==0.0 && lastLog.find("source_change_baseline")!=std::string::npos);
    value=-0.03f;std::memcpy(entry+8,&value,4);
    int found=0;assert(FuelSlotTons(vehicle,&found)<0.0 && found==1);
    ReadTank(0,vehicle,4,&decrease,&increases);
    assert(!g_vehFuelValid[0] && decrease==0.0);
    assert(lastLog.find("slot_found=1")!=std::string::npos);
    assert(lastLog.find("source_valid=0")!=std::string::npos);
    value=0.005f;std::memcpy(entry+8,&value,4);
    ReadTank(0,vehicle,5,&decrease,&increases);assert(decrease==0.0);
    value=0.004f;std::memcpy(entry+8,&value,4);++g_fuelDiagnosticEpoch;
    ReadTank(0,vehicle,6,&decrease,&increases);assert(decrease==0.0);
    value=0.01f;std::memcpy(entry+8,&value,4);
    ReadTank(0,vehicle,7,&decrease,&increases);assert(increases==1);
    assert(lastLog.find("verified_refill=0 billing_input=0")!=std::string::npos);
    std::puts("PASS extracted_production_readtank_and_slot_reader");
}
'''
        path = Path(self.directory.name) / 'readtank.cpp'
        path.write_text(source)
        self.assertIn('PASS extracted_production_readtank', self.compile_and_run(path))

    def test_fragment_tariff_and_its_logs_are_removed(self):
        for retired in ('priced_transfer_fragment(s)', 'payment_returned=',
                        'native journey settlement OFF', 'g_tripTariff',
                        'gTariffMargin', 'gWearLitres'):
            self.assertNotIn(retired, self.cpp)
        self.assertIn('observed_assignment_fuel=%.2f', self.cpp)
        self.assertIn('no wholesale, cargo-value freight, fragment tariff', self.cpp)

    def test_no_sampled_burn_calibration_or_tracker_feed(self):
        self.assertNotIn('SET litres_per_trip TO THE MEASURED FIGURE', self.cpp)
        self.assertNotIn('gLitresPerTrip', self.cpp)
        self.assertNotIn('litres_auto', self.cpp)
        self.assertNotIn('auto-tune', self.cpp)
        self.assertNotIn('private_logistics_journey::Tracker', self.cpp)
        self.assertNotIn('has NO fuel slot', self.cpp)
        read = self.cpp.split('static void ReadTank(int slot, void* v, int closedDay, double* burned, unsigned* refills)', 1)[1].split('static void ScanVehicle', 1)[0]
        self.assertNotIn('"agree"', read)
        self.assertIn('InterlockedCompareExchange(&g_fuelDiagnosticEpoch', read)


if __name__ == '__main__':
    unittest.main()
