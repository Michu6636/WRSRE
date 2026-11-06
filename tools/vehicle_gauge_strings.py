"""Offline PE string-anchor inventory; no game execution or external dependencies.

Run on the SAME executable used by Ghidra:
  python tools/vehicle_gauge_strings.py SOVIET64.exe --output gauge-search.md

All candidates are leads, not identification of a vehicle instrument. The
Markdown includes full JSON results and this source. Import the listed VAs into
Ghidra and inspect references to string starts AND the reported interior anchors.
"""
from __future__ import annotations

import argparse
import hashlib
import json
import re
import struct
from pathlib import Path

# Tokenize escaped percent signs too, rather than treating %%f as a conversion.
FORMAT = re.compile(
    r"%%|%(?:\d+\$)?[-+ #0]*(?:\d+|\*)?(?:\.(?:\d+|\*))?"
    r"(?:I64|I32|hh|ll|[hlLjzt])?[diuoxXfFeEgGaAcCsSpn]"
)
PATTERNS = {
    "fuel_word": re.compile(r"fuel", re.I),
    "litre_word": re.compile(r"lit(?:re|er)s?", re.I),
    "speed_unit": re.compile(r"km\s*/\s*h|\bmph\b", re.I),
    "rpm": re.compile(r"\brpm\b", re.I),
    "distance": re.compile(r"odometer|distance|\bkm\b|\bmi\b", re.I),
}


def anchors(text: str) -> tuple[list[str], list[int]]:
    reasons, offsets = [], set()
    formats = [m for m in FORMAT.finditer(text)
               if m.group() != "%%" and m.group()[-1] in "fFeEgGaA"]
    if formats:
        reasons.append("float_format")
        offsets.update(m.start() for m in formats)
    for label, pattern in PATTERNS.items():
        matches = list(pattern.finditer(text))
        if matches:
            reasons.append(label)
            offsets.update(m.start() for m in matches)
    # Separately assembled units need not share a literal with the number.
    if text.strip() in {"l", "L", "л", "Л"}:
        reasons.append("standalone_litre_candidate")
        offsets.add(text.index(text.strip()))
    return reasons, sorted(offsets)


def decoded_runs(data: bytes):
    """Yield printable ASCII and BMP UTF-16LE runs, including embedded needles.

    UTF-16 is checked at both byte alignments. Nonterminated runs are reported
    as such, not silently assumed to be genuine string literals. No hit caps.
    Non-ASCII BMP text is retained so localized suffixes are not discarded.
    """
    for match in re.finditer(rb"[\x20-\x7e\t\r\n]+", data):
        yield (match.start(), match.end(), match.group().decode("ascii"),
               "ASCII", match.end() < len(data) and data[match.end()] == 0)
    for parity in (0, 1):
        start, chars = 0, []
        for offset in range(parity, len(data) - 1, 2):
            unit = data[offset] | data[offset + 1] << 8
            char = chr(unit)
            if char.isprintable() or char in "\t\r\n":
                if not chars:
                    start = offset
                chars.append(char)
            else:
                if chars:
                    yield start, offset, "".join(chars), "UTF-16LE", unit == 0
                    chars = []
        if chars:
            yield start, offset + 2, "".join(chars), "UTF-16LE", False


def scan_data(data: bytes, base_va: int = 0, file_base: int = 0) -> list[dict]:
    results = []
    for start, end, text, encoding, terminated in decoded_runs(data):
        reasons, positions = anchors(text)
        if not reasons:
            continue
        width = 1 if encoding == "ASCII" else 2
        results.append({
            "string_va": hex(base_va + start),
            "file_offset": hex(file_base + start),
            "encoding": encoding,
            "terminated": terminated,
            "text": text,
            "bytes_hex": data[start:end].hex(),
            "reasons": reasons,
            "interior_anchor_vas": [hex(base_va + start + p * width)
                                    for p in positions],
        })
    return results


def pe_sections(data: bytes):
    """Read PE image base and section table, rejecting truncated input."""
    def need(offset, length):
        if offset < 0 or offset + length > len(data):
            raise ValueError(f"Truncated PE range {offset:#x}+{length:#x}")

    need(0, 64)
    if data[:2] != b"MZ":
        raise ValueError("Not a DOS/PE image")
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    need(pe, 24)
    if data[pe:pe + 4] != b"PE\0\0":
        raise ValueError("Missing PE signature")
    machine, count, timestamp = struct.unpack_from("<HHI", data, pe + 4)
    opt_size = struct.unpack_from("<H", data, pe + 20)[0]
    opt = pe + 24
    need(opt, opt_size)
    if opt_size < 32:
        raise ValueError("Optional header too short")
    magic = struct.unpack_from("<H", data, opt)[0]
    if magic == 0x20B:
        image_base = struct.unpack_from("<Q", data, opt + 24)[0]
    elif magic == 0x10B:
        image_base = struct.unpack_from("<I", data, opt + 28)[0]
    else:
        raise ValueError(f"Unsupported optional-header magic {magic:#x}")
    table = opt + opt_size
    need(table, count * 40)
    sections = []
    for index in range(count):
        pos = table + index * 40
        name = data[pos:pos + 8].split(b"\0", 1)[0].decode("ascii", "replace")
        virtual_size, rva, raw_size, raw_offset = struct.unpack_from("<IIII", data, pos + 8)
        flags = struct.unpack_from("<I", data, pos + 36)[0]
        # Scan initialized non-executable data; explicitly report other sections.
        scan = bool(flags & 0x40) and not bool(flags & 0x20000000) and raw_size > 0
        span = min(raw_size, virtual_size) if virtual_size else raw_size
        if scan:
            need(raw_offset, raw_size)
        sections.append({"name": name, "rva": hex(rva),
                         "raw_offset": raw_offset, "raw_size": raw_size,
                         "scan_bytes": span if scan else 0,
                         "flags": hex(flags), "scanned": scan})
    return {"image_base": hex(image_base), "machine": hex(machine),
            "pe_timestamp": hex(timestamp)}, sections


def inventory(data: bytes) -> dict:
    identity, sections = pe_sections(data)
    results = []
    for section in sections:
        if not section["scanned"]:
            continue
        offset, size = section["raw_offset"], section["scan_bytes"]
        candidates = scan_data(data[offset:offset + size],
                               int(identity["image_base"], 16) + int(section["rva"], 16),
                               offset)
        for candidate in candidates:
            candidate["section"] = section["name"]
        results.extend(candidates)
        section["candidate_count"] = len(candidates)
    return {"sha256": hashlib.sha256(data).hexdigest(), **identity,
            "scope": "All file-backed initialized non-executable PE sections; "
                     "ASCII and BMP UTF-16LE runs; no candidate cap. "
                     "Not runtime/localization-resource analysis. "
                     "Surrogate-pair Unicode strings may be split.",
            "sections": sections, "candidate_count": len(results),
            "candidates": results}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("executable", type=Path)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    try:
        result = inventory(args.executable.read_bytes())
    except (OSError, ValueError) as error:
        parser.exit(2, f"Search failed: {error}\n")
    markdown = (
        "# Vehicle gauge string candidates — offline PE inventory\n\n"
        "These are candidates, NOT confirmed vehicle UI or fuel semantics.\n"
        "VAs use the PE preferred image base, not runtime ASLR addresses.\n"
        "Inspect Ghidra references to string_va AND interior_anchor_vas.\n"
        "No references found at one address does not establish no callers.\n\n"
        "## Complete results and section coverage\n\n```json\n"
        + json.dumps(result, ensure_ascii=False, indent=2)
        + "\n```\n\n## Search source used\n\n```python\n"
        + Path(__file__).read_text(encoding="utf-8") + "\n```\n"
    )
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(markdown, encoding="utf-8")
    print(f"Wrote {result['candidate_count']} candidates to {args.output}")


if __name__ == "__main__":
    main()
