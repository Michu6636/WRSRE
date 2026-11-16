#!/usr/bin/env python3
"""Host-test actual extracted provider/publication/consumer bodies, not copies."""
import os
from pathlib import Path
import subprocess
import sys

root = Path(__file__).resolve().parents[2]
build = root / "build" / "tax_policy_host"
build.mkdir(parents=True, exist_ok=True)
source = (root / "plugins/taxes/taxes.cpp").read_text()

def between(text, start, end):
    assert text.count(start) == 1, start
    return text.split(start, 1)[1].split(end, 1)[0]

provider = "static int g_policySnapshotReady" + between(
    source, "static int g_policySnapshotReady", "// ---------------------------------------------------------------- settings")
publish = "    static TsmTaxesApi kApiV1;" + between(
    source, "    static TsmTaxesApi kApiV1;", "    return 0;")
parts = [provider, "static void PublishExtracted() {\n" + publish + "}\n"]
for name, path in [("Enterprise", "plugins/enterprise_base/enterprise_base.cpp"),
                   ("Shops", "plugins/private_shops/private_shops.cpp")]:
    text = (root / path).read_text()
    pos = text.index("H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION);")
    start = text.rfind("    g_taxes =", 0, pos)
    end_marker = "H->consume(TSM_SERVICE_TAXES, TSM_TAXES_VERSION_V1);"
    end = text.index(end_marker, pos) + len(end_marker)
    parts.append("static void Consume" + name + "Extracted() {\n" + text[start:end] + "\n}\n")
(build / "tax_policy_provider.inc").write_text("\n".join(parts))
flags = ["-std=c++17", "-Wall", "-Wextra", "-Werror", "-pedantic"]
if "--sanitize" in sys.argv:
    flags += ["-O1", "-g", "-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
else:
    flags += ["-O2"]
exe = build / ("policy_san" if "--sanitize" in sys.argv else "policy")
subprocess.run([os.environ.get("CXX", "g++"), *flags, "-I" + str(root / "src"),
                "-I" + str(build), str(root / "tests/almanac_button/tax_policy_test.cpp"),
                "-o", str(exe)], check=True)
subprocess.run([str(exe)], check=True)
