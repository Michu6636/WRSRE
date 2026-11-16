#!/usr/bin/env python3
"""Offline-by-default Almanac host tests. Never builds/deploys Windows plugins."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
SUITES = (
    "ui_button_core", "almanac_button_geometry", "almanac_open_state",
    "almanac_panel_state", "almanac_finance", "almanac_finance_editor",
    "almanac_overlay_bridge", "almanac_overlay_controller",
    "almanac_finance_session", "almanac_finance_executor", "almanac_debug_toggle",
)
IMGUI_COMMIT = "f5befd2d29e66809cd1110a152e375a7f1981f06"
IMGUI_FILES = (
    "imgui.cpp", "imgui_draw.cpp", "imgui_tables.cpp", "imgui_widgets.cpp",
    "imgui.h", "imconfig.h", "imgui_internal.h", "imstb_rectpack.h",
    "imstb_textedit.h", "imstb_truetype.h", "LICENSE.txt",
)


def run(command, *, env=None):
    print("+", " ".join(map(str, command)), flush=True)
    subprocess.run(list(map(str, command)), cwd=ROOT, env=env,
                   check=True, timeout=180)


def verify_imgui(path):
    """Require the reviewed commit AND an unmodified set of test input files."""
    actual = subprocess.check_output(
        ["git", "-C", str(path), "rev-parse", "HEAD"], text=True).strip()
    if actual != IMGUI_COMMIT:
        raise ValueError(f"ImGui HEAD must be {IMGUI_COMMIT}, got {actual}")
    for name in IMGUI_FILES:
        expected = subprocess.check_output(
            ["git", "-C", str(path), "show", f"{IMGUI_COMMIT}:{name}"])
        if (path / name).read_bytes() != expected:
            raise ValueError(f"Modified ImGui test input: {name}")
    return {"commit": actual, "imgui_h_sha256": hashlib.sha256(
        (path / "imgui.h").read_bytes()).hexdigest()}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sanitize", action="store_true",
                        help="Build/run with AddressSanitizer and UBSan (not TSan)")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"),
                        help="Compiler executable/path, not a shell command")
    parser.add_argument("--imgui-dir", type=Path,
                        help="Optional local, unmodified checkout at the reviewed pin")
    args = parser.parse_args()
    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f"Compiler executable not found: {args.cxx}")
    mode = "sanitized" if args.sanitize else "normal"
    build = ROOT / "build" / "almanac_host" / mode
    build.mkdir(parents=True, exist_ok=True)
    summary = {"mode": mode, "compiler": compiler, "suites": [],
               "scope": "host-only; no native/GPU/input acceptance"}
    flags = ["-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic", "-pthread"]
    if args.sanitize:
        flags += ["-g", "-fsanitize=address,undefined", "-fno-omit-frame-pointer"]
    includes = ["-I" + str(ROOT / "src"),
                "-I" + str(ROOT / "plugins" / "almanac_button")]
    active = None
    try:
        summary["compiler_version"] = subprocess.check_output(
            [compiler, "--version"], text=True).splitlines()[0]
        for name in SUITES:
            active = {"name": name, "status": "running"}
            summary["suites"].append(active)
            exe = build / (name + (".exe" if os.name == "nt" else ""))
            run([compiler, *flags, *includes,
                 ROOT / "tests" / "almanac_button" / f"{name}_test.cpp", "-o", exe])
            run([exe])
            active["status"] = "passed"
        active = {"name": "tax_policy_extracted", "status": "running"}
        summary["suites"].append(active)
        env = dict(os.environ, CXX=compiler)
        run([sys.executable, ROOT / "tests/almanac_button/run_tax_policy_tests.py",
             *(["--sanitize"] if args.sanitize else [])], env=env)
        active["status"] = "passed"
        active = {"name": "imgui_headless", "status": "skipped",
                  "reason": "No --imgui-dir; no dependencies fetched automatically"}
        summary["suites"].append(active)
        if args.imgui_dir:
            active["status"] = "running"
            active.pop("reason", None)
            imgui = args.imgui_dir.resolve()
            summary["imgui"] = verify_imgui(imgui)
            exe = build / ("imgui_headless.exe" if os.name == "nt" else "imgui_headless")
            run([compiler, *flags, *includes,
                 "-I" + str(ROOT / "prototypes/almanac_overlay"),
                 "-isystem", imgui,
                 ROOT / "tests/almanac_button/almanac_overlay_view_test.cpp",
                 ROOT / "prototypes/almanac_overlay/almanac_overlay_view.cpp",
                 ROOT / "prototypes/almanac_overlay/earthworks/panel_style.cpp",
                 *(imgui / name for name in IMGUI_FILES[:4]), "-o", exe])
            run([exe])
            active["status"] = "passed"
        summary["status"] = "passed"
        return 0
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        if active is not None and active["status"] == "running":
            active["status"] = "failed"
        summary["status"] = "failed"
        summary["error"] = str(error)
        print(f"FAIL: {error}", file=sys.stderr)
        return 1
    finally:
        report = build / "summary.json"
        report.write_text(json.dumps(summary, indent=2) + "\n", encoding="utf-8")
        print(f"Results: {report}", flush=True)


if __name__ == "__main__":
    sys.exit(main())
