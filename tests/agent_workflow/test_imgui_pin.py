"""The Dear ImGui pin must stay consistent across every place that names it.

`build.bat` clones a pinned tag and refuses to trust a checkout whose
IMGUI_VERSION_NUM does not match. The panel header carries the same tag for its
version string. Before 2026-11-18 the tag was written out nine times in
build.bat and once in the header, so a bump could silently desynchronise the
clone from the check.

These tests are contracts about text, not proof that a build works.
"""

from pathlib import Path
import re
import unittest

ROOT = Path(__file__).resolve().parents[2]
BUILD = ROOT / "build.bat"
VERSION_H = ROOT / "plugins" / "almanac_overlay_dev" / "almanac_version.h"

# Dear ImGui encodes the version as XYYZZ, and ZZ counts *patch releases* of
# the minor: 1.91.0 -> 19100, 1.91.9 -> 19109, 1.91.9b -> 19191, 1.91.10 ->
# 19110. A trailing letter bumps ZZ by ten, so it is not simple concatenation.
# Verified against imgui.h at tag v1.91.9b (IMGUI_VERSION_NUM 19191).
TAG_RE = re.compile(r"^v(\d+)\.(\d+)\.(\d+)([a-z])?$")


def _build_text() -> str:
    return BUILD.read_text(encoding="utf-8")


def _defined(name: str):
    """Return the literal assigned to `set "NAME=value"` in build.bat."""
    match = re.search(r'^set "%s=([^"]*)"' % re.escape(name),
                      _build_text(), re.MULTILINE)
    return match.group(1) if match else None


def _header_tag():
    text = VERSION_H.read_text(encoding="utf-8")
    match = re.search(r'#define\s+ALMANAC_IMGUI_TAG\s+"([^"]+)"', text)
    return match.group(1) if match else None


def _expected_version_num(tag: str):
    """v1.91.9b -> 19191, matching Dear ImGui's XYYZZ encoding.

    ZZ is the patch *times ten* plus the letter index (a=0, b=1, ...), so
    1.90.9 -> 19090 and 1.91.9b -> 19191. Verified against imgui.h at tags
    v1.90.9 and v1.91.9b; that is the only evidence for the rule, so if the
    pin ever moves, re-check it against the real header before trusting this.
    """
    parsed = TAG_RE.match(tag)
    if not parsed:
        return None
    major, minor, patch, letter = parsed.groups()
    zz = int(patch) * 10 + (ord(letter) - ord("a") if letter else 0)
    return int(major) * 10000 + int(minor) * 100 + zz


class ImGuiPinTests(unittest.TestCase):
    def test_build_declares_the_pin_once(self):
        self.assertIsNotNone(_defined("IMGUI_TAG"),
                             'build.bat must define set "IMGUI_TAG=..."')
        self.assertIsNotNone(_defined("IMGUI_NUM"),
                             'build.bat must define set "IMGUI_NUM=..."')

    def test_tag_is_a_version_tag(self):
        tag = _defined("IMGUI_TAG")
        self.assertRegex(tag, TAG_RE, "IMGUI_TAG must look like v1.91.9b")

    def test_version_num_matches_the_tag(self):
        tag = _defined("IMGUI_TAG")
        declared = int(_defined("IMGUI_NUM"))
        expected = _expected_version_num(tag)
        self.assertEqual(
            expected, declared,
            "IMGUI_NUM %d does not match tag %r (expected %d)"
            % (declared, tag, expected))

    def test_header_tag_matches_build(self):
        self.assertEqual(
            _header_tag(), _defined("IMGUI_TAG"),
            "almanac_version.h ALMANAC_IMGUI_TAG and build.bat IMGUI_TAG differ")

    def test_no_hardcoded_tag_outside_the_definition(self):
        """Every mention must use !IMGUI_TAG!/!IMGUI_NUM!, not the literal."""
        tag = _defined("IMGUI_TAG")
        num = _defined("IMGUI_NUM")
        offenders = []
        for number, line in enumerate(_build_text().splitlines(), 1):
            stripped = line.strip()
            if stripped.startswith(('REM', '::')):
                continue
            if re.match(r'^set "(IMGUI_TAG|IMGUI_NUM)=', stripped):
                continue          # the definitions themselves
            if tag in line:
                offenders.append("%d: hardcoded tag in %r" % (number, stripped))
            elif re.search(r'(?<!\d)%s(?!\d)' % re.escape(num), line):
                offenders.append("%d: hardcoded IMGUI_VERSION_NUM in %r"
                                 % (number, stripped))
        self.assertEqual(offenders, [],
                         "use !IMGUI_TAG!/!IMGUI_NUM! instead of literals:\n"
                         + "\n".join(offenders))


if __name__ == "__main__":
    unittest.main(verbosity=2)
