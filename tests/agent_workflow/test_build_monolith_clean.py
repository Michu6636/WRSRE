"""Static contracts for build.bat intermediate isolation, clean.bat, and
monolith almanac_overlay_dev wiring.

These are text/structure contracts, not proof that MSVC compiles on Windows.
"""

from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
BUILD_BAT = ROOT / "build.bat"
CLEAN_BAT = ROOT / "clean.bat"
ALMANAC_CPP = ROOT / "plugins" / "almanac_overlay_dev" / "almanac_overlay_dev.cpp"
WRSRE_ENTRY = ROOT / "src" / "wrsre_entry.cpp"


class BuildAndMonolithTests(unittest.TestCase):
    def test_build_bat_is_crlf_and_paren_balanced(self):
        raw = BUILD_BAT.read_bytes()
        self.assertEqual(raw.count(b"\r\n"), raw.count(b"\n"),
                         "build.bat must remain CRLF-only")
        depth = 0
        for lineno, line in enumerate(raw.decode("utf-8").splitlines(), 1):
            stripped = line.strip()
            if stripped.startswith(("REM", "::")) or re.match(r"^echo", stripped, re.I):
                continue
            depth += len(re.findall(r"(?<!\^)\(", line)) - len(re.findall(r"(?<!\^)\)", line))
            self.assertGreaterEqual(depth, 0, f"negative paren depth at line {lineno}")
        self.assertEqual(depth, 0, "unbalanced parentheses in build.bat")

    def test_cl_and_link_redirect_intermediates_into_build_dirs(self):
        text = BUILD_BAT.read_text(encoding="utf-8")
        self.assertNotIn("if exists ", text.lower(), "cmd syntax requires 'if exist'")
        logical_lines = []
        acc = ""
        start_no = 1
        for lineno, line in enumerate(text.splitlines(), 1):
            if not acc:
                start_no = lineno
            s = line.rstrip()
            if s.endswith("^"):
                acc += " " + s[:-1].strip()
                continue
            full = (acc + " " + s).strip() if acc else s.strip()
            logical_lines.append((start_no, full))
            acc = ""
        for lineno, stripped in logical_lines:
            if stripped.startswith("cl "):
                self.assertRegex(stripped, r"/Fo\"?build\\",
                                 f"build.bat:{lineno} cl missing /Fo into build\\")
            if "/LD " in stripped or stripped.startswith("link "):
                self.assertRegex(stripped, r"/IMPLIB:\"?build\\",
                                 f"build.bat:{lineno} link missing /IMPLIB into build\\")

    def test_clean_bat_runs_on_all_exit_paths_and_cleans_obj_dirs(self):
        build_text = BUILD_BAT.read_text(encoding="utf-8")
        done_sec = build_text.split(":done", 1)[1].split(":no_vs", 1)[0]
        failed_sec = build_text.split(":failed", 1)[1]
        self.assertIn('call "%ROOT%clean.bat"', done_sec)
        self.assertIn('call "%ROOT%clean.bat"', failed_sec)

        clean_text = CLEAN_BAT.read_text(encoding="utf-8")
        for needle in ("*.obj", "*.exp", "*.lib", "*.ilk", "*.pdb", "build\\obj", "build\\wrsre_obj"):
            self.assertIn(needle, clean_text)

    def test_almanac_overlay_supports_monolith_entry_renaming(self):
        almanac = ALMANAC_CPP.read_text(encoding="utf-8")
        entry = WRSRE_ENTRY.read_text(encoding="utf-8")
        for sym in ("WRSRE_MONOLITHIC_MODULE", "WrsreInit_", "WrsreStart_"):
            self.assertIn(sym, almanac)
        self.assertIn("WRSRE_WITH_ALMANAC", entry)
        self.assertIn("WRSRE_MOD_ENTRY(almanac_overlay_dev)", entry)


if __name__ == "__main__":
    unittest.main()
