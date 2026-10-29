import importlib.util
import struct
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
SPEC = importlib.util.spec_from_file_location("scanner", ROOT / "tools/vehicle_gauge_strings.py")
scanner = importlib.util.module_from_spec(SPEC)
SPEC.loader.exec_module(scanner)


def fake_pe(payload=b"Fuel: %.2f l remaining\0", pe64=True):
    """Small synthetic PE: one initialized data and one executable section."""
    data = bytearray(0x600)
    data[:2] = b"MZ"
    struct.pack_into("<I", data, 0x3C, 0x80)
    data[0x80:0x84] = b"PE\0\0"
    opt_size = 0xF0 if pe64 else 0xE0
    struct.pack_into("<HHI", data, 0x84, 0x8664 if pe64 else 0x14C, 2, 0x6A3EB6AD)
    struct.pack_into("<H", data, 0x94, opt_size)
    struct.pack_into("<H", data, 0x98, 0x20B if pe64 else 0x10B)
    struct.pack_into("<Q" if pe64 else "<I", data, 0x98 + (24 if pe64 else 28),
                     0x140000000 if pe64 else 0x400000)
    table = 0x98 + opt_size
    for i, (name, flags, off, rva) in enumerate([
        (b".rdata", 0x40000040, 0x200, 0x2000),
        (b".text", 0x60000020, 0x400, 0x1000),
    ]):
        pos = table + i * 40
        data[pos:pos + 8] = name.ljust(8, b"\0")
        struct.pack_into("<IIII", data, pos + 8, 0x200, rva, 0x200, off)
        struct.pack_into("<I", data, pos + 36, flags)
    data[0x200:0x200 + len(payload)] = payload
    data[0x400:0x40A] = b"RPM %.2f\0\0"
    return bytes(data)


class ScannerTests(unittest.TestCase):
    def ascii_candidates(self, data):
        return [x for x in scanner.scan_data(data) if x["encoding"] == "ASCII"]

    def test_embedded_match_with_suffix(self):
        text = b"Fuel: %.2f l remaining\0"
        self.assertEqual(self.ascii_candidates(text)[0]["text"], text[:-1].decode())

    def test_original_suffix_filter_would_miss(self):
        text = b"Fuel: %.2f l remaining\0"
        for needle in (b"Fuel", b"%.2f", b"%.2f l"):
            pos = text.index(needle)
            self.assertNotEqual(text[pos + len(needle)], 0)
        self.assertEqual(len(self.ascii_candidates(text)), 1)

    def test_more_than_twelve_hits(self):
        data = b"\0".join(f"candidate {i}: %.2f l".encode() for i in range(25)) + b"\0"
        self.assertEqual(len(self.ascii_candidates(data)), 25)

    def test_prefix_preserved_and_interior_address(self):
        result = scanner.scan_data(b"\0prefix %.2f l\0", 0x140002000, 0x200)[0]
        self.assertEqual(result["string_va"], "0x140002001")
        self.assertIn("0x140002008", result["interior_anchor_vas"])
        self.assertEqual(result["file_offset"], "0x201")

    def test_utf16_localized_suffix(self):
        text = "Паливо: %.2f л"
        data = b"\0\0" + text.encode("utf-16le") + b"\0\0"
        results = scanner.scan_data(data, 0x140000000)
        result = next(x for x in results if x["text"] == text)
        self.assertEqual(result["encoding"], "UTF-16LE")
        self.assertEqual(result["string_va"], "0x140000002")
        self.assertTrue(result["terminated"])
        self.assertIn(hex(0x140000002 + text.index("%") * 2), result["interior_anchor_vas"])

    def test_utf16_odd_alignment(self):
        text = "Fuel %.3f %ls"
        data = b"\0" + text.encode("utf-16le") + b"\0\0"
        self.assertTrue(any(x["text"] == text for x in scanner.scan_data(data)))

    def test_precision_and_separate_unit_formats(self):
        for text in ("%.*f %ls", "%.3f", "%8.2f litres", "%g", "%.2lf l"):
            self.assertIn("float_format", scanner.anchors(text)[0])

    def test_escaped_percent_is_not_float(self):
        self.assertNotIn("float_format", scanner.anchors("%%f")[0])
        self.assertIn("float_format", scanner.anchors("%%%f")[0])

    def test_standalone_unit(self):
        for unit in ("l", "L", "л"):
            self.assertIn("standalone_litre_candidate", scanner.anchors(unit)[0])

    def test_nonterminated_reported(self):
        result = self.ascii_candidates(b"Fuel %.2f l")[0]
        self.assertFalse(result["terminated"])

    def test_pe64_coverage_and_va(self):
        result = scanner.inventory(fake_pe())
        self.assertEqual(result["image_base"], "0x140000000")
        self.assertEqual(result["pe_timestamp"], "0x6a3eb6ad")
        self.assertEqual([s["scanned"] for s in result["sections"]], [True, False])
        self.assertEqual(result["candidates"][0]["string_va"], "0x140002000")
        self.assertFalse(any(x["text"].startswith("RPM") for x in result["candidates"]))

    def test_pe32(self):
        result = scanner.inventory(fake_pe(pe64=False))
        self.assertEqual(result["image_base"], "0x400000")
        self.assertEqual(result["candidates"][0]["string_va"], "0x402000")

    def test_invalid_and_truncated_rejected(self):
        for data in (b"", b"MZ", b"x" * 128, fake_pe()[:0x250]):
            with self.assertRaises(ValueError):
                scanner.inventory(data)

    def test_cli_embeds_results_and_source(self):
        with tempfile.TemporaryDirectory() as temp:
            source, output = Path(temp) / "synthetic.exe", Path(temp) / "report.md"
            source.write_bytes(fake_pe())
            result = subprocess.run([sys.executable, str(ROOT / "tools/vehicle_gauge_strings.py"),
                                     str(source), "--output", str(output)], capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            report = output.read_text()
            self.assertIn('"string_va": "0x140002000"', report)
            self.assertIn('Fuel: %.2f l remaining', report)
            self.assertIn('def inventory(data: bytes)', report)


if __name__ == "__main__":
    unittest.main()
