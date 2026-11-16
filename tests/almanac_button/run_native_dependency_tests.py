#!/usr/bin/env python3
"""Offline CMake pin-guard regressions. No Windows compilation or execution."""
import argparse
import os
from pathlib import Path
import shutil
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[2]
SCRIPT = ROOT / "prototypes/almanac_overlay/native/verify_imgui.cmake"


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--imgui-dir", type=Path, required=True)
    parser.add_argument("--cmake", default="cmake")
    args = parser.parse_args()
    cmake = shutil.which(args.cmake)
    if not cmake:
        parser.error("CMake executable not found")
    source = args.imgui_dir.resolve()
    git_dir = subprocess.check_output(
        ["git", "-C", str(source), "rev-parse", "--absolute-git-dir"], text=True).strip()
    build = ROOT / "build"
    build.mkdir(exist_ok=True)
    count = 0

    def check(directory, expected_error=None):
        nonlocal count
        command = [cmake]
        if directory is not None:
            command.append(f"-DALMANAC_IMGUI_DIR={directory}")
        command += ["-P", str(SCRIPT)]
        result = subprocess.run(command, cwd=ROOT, capture_output=True, text=True,
                                timeout=60, env=dict(os.environ))
        output = result.stdout + result.stderr
        if expected_error is None:
            if result.returncode != 0:
                raise AssertionError(output)
        elif result.returncode == 0 or expected_error not in output:
            raise AssertionError(output)
        count += 1

    # Validate user-supplied source first, but mutate ONLY a disposable file copy.
    check(source)
    with tempfile.TemporaryDirectory(prefix="almanac-pin-", dir=build) as temporary:
        fixture = Path(temporary) / "imgui with spaces"
        shutil.copytree(source, fixture, ignore=shutil.ignore_patterns(".git"))
        # Read-only Git metadata reference: verifier runs rev-parse/hash-object,
        # never checkout, index updates or writes into the source dependency.
        metadata = fixture / ".git"
        metadata.write_text(f"gitdir: {git_dir}\n", encoding="utf-8")
        check(fixture)
        check(None, "Supply -DALMANAC_IMGUI_DIR")
        check(fixture / "does-not-exist", "Supply -DALMANAC_IMGUI_DIR")
        for name in ("imgui.cpp", "LICENSE.txt", "backends/imgui_impl_dx11.cpp",
                     "backends/imgui_impl_dx11.h"):
            path = fixture / name
            original = path.read_bytes()
            try:
                path.write_bytes(original + b"\n// deliberate pin-guard regression\n")
                check(fixture, "Modified ImGui input")
            finally:
                path.write_bytes(original)
        missing = fixture / "imgui.h"
        original = missing.read_bytes()
        try:
            missing.unlink()
            check(fixture, "Missing pinned ImGui input")
        finally:
            missing.write_bytes(original)
        original_metadata = metadata.read_text(encoding="utf-8")
        try:
            wrong_git = subprocess.check_output(
                ["git", "-C", str(ROOT), "rev-parse", "--absolute-git-dir"], text=True).strip()
            metadata.write_text(f"gitdir: {wrong_git}\n", encoding="utf-8")
            check(fixture, "ImGui HEAD must be")
        finally:
            metadata.write_text(original_metadata, encoding="utf-8")
        check(fixture)
    print(f"native dependency guards: {count} checks passed (no native build/run)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
