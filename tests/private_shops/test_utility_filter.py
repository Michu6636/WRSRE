import os
import subprocess
import tempfile
import unittest

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", ".."))
HEADER_DIR = os.path.join(REPO_ROOT, "plugins", "private_shops")

PROBE_CPP = r"""
#include <cassert>
#include <cstdio>
#include "utility_filter.h"

int main() {
    using wrsre_shops::IsUtilityResourceName;

    // Utility resources must be skipped (case-insensitive)
    assert(IsUtilityResourceName("eletric"));
    assert(IsUtilityResourceName("ELETRIC"));
    assert(IsUtilityResourceName("electric"));
    assert(IsUtilityResourceName("Electric"));
    assert(IsUtilityResourceName("water"));
    assert(IsUtilityResourceName("usagewater"));
    assert(IsUtilityResourceName("UsageWater"));
    assert(IsUtilityResourceName("heating"));
    assert(IsUtilityResourceName("heat"));
    assert(IsUtilityResourceName("hotwater"));

    // Freight resources must NOT be skipped
    assert(!IsUtilityResourceName("plants"));
    assert(!IsUtilityResourceName("chemicals"));
    assert(!IsUtilityResourceName("fabric"));
    assert(!IsUtilityResourceName("clothes"));
    assert(!IsUtilityResourceName("food"));
    assert(!IsUtilityResourceName("meat"));
    assert(!IsUtilityResourceName("fuel"));
    assert(!IsUtilityResourceName("eletronics"));
    assert(!IsUtilityResourceName("electronics"));

    // Null / empty safety
    assert(!IsUtilityResourceName(nullptr));
    assert(!IsUtilityResourceName(""));

    std::puts("utility_filter_ok");
    return 0;
}
"""


class UtilityFilterTests(unittest.TestCase):
    def test_utility_resource_name_predicate(self):
        with tempfile.TemporaryDirectory() as tmp:
            src = os.path.join(tmp, "probe.cpp")
            exe = os.path.join(tmp, "probe")
            with open(src, "w", encoding="utf-8") as f:
                f.write(PROBE_CPP)
            res = subprocess.run(
                ["g++", "-std=c++17", "-Wall", "-Wextra", "-Werror",
                 "-I", HEADER_DIR, src, "-o", exe],
                capture_output=True, text=True
            )
            self.assertEqual(res.returncode, 0, msg=res.stderr)
            run = subprocess.run([exe], capture_output=True, text=True)
            self.assertEqual(run.returncode, 0, msg=run.stderr)
            self.assertIn("utility_filter_ok", run.stdout)


if __name__ == "__main__":
    unittest.main()
