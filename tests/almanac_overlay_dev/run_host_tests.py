#!/usr/bin/env python3
"""Host checks for the Almanac ImGui panel plugin. Never builds or deploys a DLL.

Two things are checked here, and only these two:

  1. the toggle policy, compiled from the plugin's own header, and
  2. the drawing source, compiled against a real Dear ImGui checkout and run
     headlessly to count the geometry it emits.

The Windows DLL, D3D11, Present interception, the game and input capture are NOT
exercised. A pass here means the panel's logic and drawing are sound; it does not
mean the plugin appears in the game.

    python3 tests/almanac_overlay_dev/run_host_tests.py
    python3 tests/almanac_overlay_dev/run_host_tests.py --imgui-dir PATH

Without --imgui-dir the drawing checks are reported as skipped, because the
dependency is never fetched automatically.
"""
import argparse
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[2]
IMGUI_FILES = ("imgui.cpp", "imgui_draw.cpp", "imgui_tables.cpp", "imgui_widgets.cpp")


def run(command):
    print("+", " ".join(map(str, command)), flush=True)
    subprocess.run(list(map(str, command)), cwd=ROOT, check=True, timeout=600)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sanitize", action="store_true",
                        help="build/run with AddressSanitizer and UBSan")
    parser.add_argument("--cxx", default=os.environ.get("CXX", "g++"),
                        help="compiler executable, not a shell command")
    parser.add_argument("--imgui-dir", type=Path,
                        help="an existing Dear ImGui checkout (v1.91.9b) for the drawing checks")
    args = parser.parse_args()

    compiler = shutil.which(args.cxx)
    if not compiler:
        parser.error(f"compiler not found: {args.cxx}")

    mode = "sanitized" if args.sanitize else "normal"
    build = ROOT / "build" / "almanac_overlay_dev" / mode
    build.mkdir(parents=True, exist_ok=True)
    summary = {"mode": mode, "compiler": compiler,
               "scope": "host-only; no Windows build, GPU, game or input test",
               "suites": []}
    flags = ["-std=c++17", "-O1", "-Wall", "-Wextra", "-Werror", "-pedantic"]
    if args.sanitize:
        flags += ["-g", "-fsanitize=address,undefined", "-fno-omit-frame-pointer"]

    try:
        summary["compiler_version"] = subprocess.check_output(
            [compiler, "--version"], text=True).splitlines()[0]

        # 1. Toggle policy. No dependency beyond the header itself.
        entry = {"name": "toggle_policy", "status": "running"}
        summary["suites"].append(entry)
        exe = build / ("toggle_policy.exe" if os.name == "nt" else "toggle_policy")
        run([compiler, *flags,
             "-I" + str(ROOT / "plugins" / "almanac_overlay_dev"),
             ROOT / "tests" / "almanac_overlay_dev" / "toggle_policy_test.cpp",
             "-o", exe])
        run([exe])
        entry["status"] = "passed"

        # 2. Drawing. Needs the real ImGui sources; skipped without them rather
        #    than fetched, so a plain run is always offline.
        entry = {"name": "panel_drawing", "status": "skipped",
                 "reason": "no --imgui-dir; dependencies are never fetched here"}
        summary["suites"].append(entry)
        if args.imgui_dir:
            imgui = args.imgui_dir.resolve()
            missing = [n for n in IMGUI_FILES if not (imgui / n).is_file()]
            if missing:
                raise ValueError(f"ImGui checkout lacks {missing}: {imgui}")
            entry["status"] = "running"
            entry.pop("reason", None)
            entry["imgui_dir"] = str(imgui)
            exe = build / ("panel_drawing.exe" if os.name == "nt" else "panel_drawing")
            run([compiler, *flags, "-isystem", imgui,
                 "-I" + str(ROOT / "plugins" / "almanac_overlay_dev"),
                 ROOT / "tests" / "almanac_overlay_dev" / "panel_drawing_test.cpp",
                 ROOT / "plugins" / "almanac_overlay_dev" / "almanac_panel_view.cpp",
                 *(imgui / n for n in IMGUI_FILES), "-o", exe])
            run([exe])
            entry["status"] = "passed"

        summary["status"] = "passed"
        return 0
    except (OSError, ValueError, subprocess.SubprocessError) as error:
        for entry in summary["suites"]:
            if entry["status"] == "running":
                entry["status"] = "failed"
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
