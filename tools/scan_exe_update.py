#!/usr/bin/env python3
"""
scan_exe_update.py - Automated SOVIET64.exe update / DLC relocation scanner
                     and `addresses.ini` generator for WRSRE.

Pure Python 3 standard library (zero external dependencies).

Usage:
  python3 tools/scan_exe_update.py path/to/SOVIET64.exe
  python3 tools/scan_exe_update.py path/to/SOVIET64.exe --write-ini addresses.ini
  python3 tools/scan_exe_update.py path/to/SOVIET64.exe --json report.json
  python3 tools/scan_exe_update.py selftest

What it does:
  1. Parses PE32+ headers, TimeDateStamp, sections, IAT (`C3DDLL64.dll` imports),
     and `.pdata` (`RUNTIME_FUNCTION` exception bounds) without executing the file.
  2. Extracts the game's real version tuple (`"v%d.%d.%d.%d (64 bit DX11.1)"`).
  3. Relocates WRSRE's globals, native helpers, and inline/call-site hook RVAs
     across recompiles using:
       - Dominant RIP-relative `RCX` singleton histogram (`game_object`)
       - Engine IAT cross-references (`sim_timer`, `daytick_power`, `fontmgr`,
         `font_default`, `font_alt_a`, `font_alt_b`, `panel2d` cluster, `input` cluster)
       - `.rdata` IEEE-754 constant-cluster fingerprinting (`day_length`,
         `map_expander` 6-float world bounds, `ui_panels` layout constants)
       - String cross-references (`menu_version_site`, `play_gui_sound` via `"gui_click"`)
       - `.pdata`-bounded dispatcher & prologue signature matching (`window_dispatch`,
         `warehouse_guard`, `panel_warehouse`, `storage_td5_guard`, `panel_storage_td5`,
         `panel_shop`, `panel_residence`, `panel_pub`, `panel_factory`, `panel_cityhall`,
         `panel_citizen`, `panel_storage`, `panel_do`, `panel_construction`,
         `labor_mobility_hook`, `retail_checkout_hook`, `logistics_transfer`)
  4. Audits `src/game_world.h` struct offsets (`+0x360`, `+0x12D0`, `+0x121C`,
     `+0x11420`, `+0x580/+0x588`, etc.) in `.text` displacement encodings to warn
     immediately if 3Division inserted fields inside `CBuilding` or `CPerson`.
  5. Emits a ready-to-use `addresses.ini` matching `src/game_addresses.h`
     (any unresolved optional RVA is emitted as `0x0` so only that single
     sub-feature disables cleanly instead of crashing the game).
"""

from __future__ import annotations

import argparse
import bisect
import collections
import datetime
import json
import struct
import sys
from pathlib import Path
from typing import Dict, List, Optional, Tuple

# ---------------------------------------------------------------------------
# Baseline v1.1.1.9 reference table (must match src/game_addresses.h)
# ---------------------------------------------------------------------------

BASELINE_STAMP = 0x6A3EB6AD
BASELINE_VERSION = "1.1.1.9"

BASELINE_RVAS: Dict[str, int] = {
    # Core globals
    "game_object":          0x9D4F10,
    "sim_timer":            0x9D4EE0,
    "day_length":           0x90AA78,
    "daytick_power":        0x334AA0,
    "menu_version_site":    0x28B5CC,
    # UI / Font / Layout globals
    "dpi":                  0x992088,
    "fontmgr":              0x996FB0,
    "font_default":         0x994200,
    "font_alt_a":           0x995220,
    "font_alt_b":           0x995228,
    "font_size":            0x99528C,
    "row_step":             0x90A9CC,
    "x_indent":             0x90AB98,
    "x_back":               0x90A8CC,
    "check_size":           0x90A928,
    "tab_btn_size":         0x90AAB8,
    "tab_row_dy":           0x90AB84,
    "icon_btn_size":        0x90AAA8,
    # C3D_PANEL2D cluster
    "panel2d":              0x9BE060,
    "p2d_rect_w":           0x9BE2E8,
    "p2d_rect_h":           0x9BE2EC,
    "p2d_rect_x":           0x9BE2F0,
    "p2d_rect_y":           0x9BE2F4,
    "p2d_rot":              0x9BE2F8,
    "p2d_tint":             0x9BE30C,
    "tint_hover":           0x90C4C0,
    "tint_normal":          0x90C100,
    "ninep_border":         0x90A9A0,
    "ninep_uv_a":           0x909D04,
    "ninep_uv_b":           0x909E78,
    "uv_full":              0x909F58,
    "panel_state":          0x9EAD08,
    # C3D_INPUT cluster
    "input":                0xA54B90,
    "esc_down":             0xA54B91,
    "esc_edge":             0xA54C91,
    "mouse_down":           0xA54E90,
    "click_latch":          0xA54E91,
    # Map bounds (`map_expander`)
    "world_size":           0x90B1A4,
    "half_world":           0x90B180,
    "gate_hi":              0x90B178,
    "gate_lo":              0x90BD2C,
    "margin_hi":            0x90B164,
    "margin_lo":            0x90BD20,
    # Direct-callable native helpers
    "relocate_person":      0x83CA50,
    "abandon_building":     0x13FE70,
    "eject_worker":         0x822FD0,
    "ui_slider":            0x2727F0,
    "ui_checkbox":          0x3956F0,
    "play_gui_sound":       0x3FFAD0,
    "clip_push":            0x4461F0,
    "vec_push_back":        0x08E050,
    "taskcfg_ctor":         0x1512A0,
    # Hook sites & panel builders
    "labor_mobility_hook":  0x1D3797,
    "labor_mobility_jbe":   0x1D37A9,
    "retail_checkout_hook": 0x1725DE,
    "retail_checkout_allow":0x1725F0,
    "retail_checkout_abort":0x17293A,
    "window_dispatch":      0x6E9B80,
    "panel_shop":           0x7272F0,
    "panel_residence":      0x761300,
    "panel_pub":            0x723950,
    "panel_factory":        0x799630,
    "panel_cityhall":       0x72DBF0,
    "panel_citizen":        0x7CEEB0,
    "panel_do":             0x741170,
    "panel_storage":        0x4CABD0,
    "storage_callsite":     0x6EA80C,
    "panel_warehouse":      0x7FC880,
    "warehouse_guard":      0x6EA5E3,
    "panel_storage_td5":    0x71D290,
    "storage_td5_guard":    0x6EAE07,
    "panel_construction":   0x7838E0,
    "logistics_transfer":   0x420350,
}

# Exact C3DDLL64.dll mangled imports used as relocation anchors
SYM_POWERTIME        = "?PowerTime@C3D_TIMER@@QEAAMM_N0@Z"
SYM_PRINT_VAR        = "?PrintLeftUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ"
SYM_PRINT_NOARG      = "?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z"
SYM_PRINT_CENTER     = "?PrintCenterUnicode@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_WZZ"
SYM_DRAW_PLAIN       = "?Draw@C3D_PANEL2D@@QEAAXMMMMM_N@Z"
SYM_DRAW_9PATCH      = "?DrawFrom9Patch@C3D_PANEL2D@@QEAAXUC3DRECT@@MMMMMM@Z"
SYM_COLLISION        = "?Collision@C3D_PANEL2D@@QEAA_NVC3DVECTOR3@@MM@Z"
SYM_MOUSE_SOLID      = "?GetMouseSolid@C3D_INPUT@@QEAA?AVC3DVECTOR3@@XZ"

# Verified function prologues (from ui_panels.cpp & private_construction.ini)
PROLOGUE_SHOP = bytes.fromhex(
    "48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 58 F5 FF FF"
)
PROLOGUE_RESIDENCE = bytes.fromhex(
    "48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 78 EB FF FF"
)
PROLOGUE_PUB = bytes.fromhex(
    "48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 78 F8 FF FF"
)
PROLOGUE_FACTORY = bytes.fromhex(
    "48 8B C4 55 56 57 41 54 41 55 41 56 41 57 48 8D A8 C8 EA FF FF"
)
PROLOGUE_CITYHALL = bytes.fromhex(
    "4C 8B DC 55 56 57 41 54 41 55 41 56 41 57"[:0] +
    "4C 8B DC 55 56 57 41 54 41 57 49 8D AB 98 F5 FF FF"
)
PROLOGUE_CITIZEN = bytes.fromhex(
    "48 8B C4 53 48 83 EC 40 80 7A 01 00 48 8B DA"
)
PROLOGUE_STORAGE = bytes.fromhex(
    "40 55 57 41 54 41 56 41 57 48 8D AC 24 B0 C9 FF FF B8 50 37 00 00"
)
PROLOGUE_CONSTRUCT = bytes.fromhex(
    "48 8B C4 55 56 57 41 54 41 55 41 56 41 57"
)


def f32_bytes(val: float) -> bytes:
    return struct.pack("<f", val)


def parse_masked_hex(pattern: str) -> Tuple[bytes, bytes]:
    """Converts 'F3 0F 10 ?? ?? 00' into (value_bytes, mask_bytes)."""
    vals = bytearray()
    mask = bytearray()
    for tok in pattern.split():
        if tok in ("??", "?"):
            vals.append(0)
            mask.append(0)
        else:
            vals.append(int(tok, 16))
            mask.append(0xFF)
    return bytes(vals), bytes(mask)


def find_masked_all(buf: bytes, pattern: str, max_hits: int = 32) -> List[int]:
    """Fast masked byte-pattern search over a section buffer."""
    vals, mask = parse_masked_hex(pattern)
    plen = len(vals)
    if plen == 0 or len(buf) < plen:
        return []

    # Pick longest contiguous fixed run as Boyer-Moore/bytes.find anchor
    best_start, best_len = 0, 0
    cur_start, cur_len = 0, 0
    for i, m in enumerate(mask):
        if m == 0xFF:
            if cur_len == 0:
                cur_start = i
            cur_len += 1
            if cur_len > best_len:
                best_start, best_len = cur_start, cur_len
        else:
            cur_len = 0

    hits: List[int] = []
    if best_len >= 2:
        anchor = vals[best_start : best_start + best_len]
        pos = buf.find(anchor, best_start)
        while pos != -1:
            cand = pos - best_start
            if cand >= 0 and cand + plen <= len(buf):
                ok = True
                for j in range(plen):
                    if mask[j] and buf[cand + j] != vals[j]:
                        ok = False
                        break
                if ok:
                    hits.append(cand)
                    if len(hits) >= max_hits:
                        return hits
            pos = buf.find(anchor, pos + 1)
    else:
        for cand in range(len(buf) - plen + 1):
            if all((buf[cand + j] & mask[j]) == vals[j] for j in range(plen)):
                hits.append(cand)
                if len(hits) >= max_hits:
                    return hits
    return hits


# ---------------------------------------------------------------------------
# Lightweight PE32+ image analyzer
# ---------------------------------------------------------------------------

class Section:
    def __init__(self, name: str, vaddr: int, vsize: int, raw_off: int, raw_size: int, flags: int):
        self.name = name
        self.vaddr = vaddr
        self.vsize = vsize
        self.raw_off = raw_off
        self.raw_size = raw_size
        self.flags = flags

    @property
    def span(self) -> int:
        return max(self.vsize, self.raw_size)


class PE64Image:
    def __init__(self, raw: bytes):
        self.raw = raw
        if len(raw) < 0x200 or raw[:2] != b"MZ":
            raise ValueError("Not a valid MZ executable")
        lfanew = struct.unpack_from("<I", raw, 0x3C)[0]
        if lfanew + 0x108 > len(raw) or raw[lfanew : lfanew + 4] != b"PE\x00\x00":
            raise ValueError("Missing PE\\0\\0 signature")

        machine, num_sec, self.stamp, _, _, opt_size, _ = struct.unpack_from(
            "<HHIIIHH", raw, lfanew + 4
        )
        magic = struct.unpack_from("<H", raw, lfanew + 24)[0]
        if machine != 0x8664 or magic != 0x20B:
            raise ValueError(f"Expected PE32+ x86-64 (got machine=0x{machine:X}, magic=0x{magic:X})")

        self.image_base = struct.unpack_from("<Q", raw, lfanew + 24 + 24)[0]
        self.size_of_image = struct.unpack_from("<I", raw, lfanew + 24 + 56)[0]
        num_rva = struct.unpack_from("<I", raw, lfanew + 24 + 108)[0]

        self.dirs: List[Tuple[int, int]] = []
        dd_off = lfanew + 24 + 112
        for i in range(min(num_rva, 16)):
            self.dirs.append(struct.unpack_from("<II", raw, dd_off + i * 8))

        self.sections: List[Section] = []
        sec_off = lfanew + 24 + opt_size
        for i in range(num_sec):
            name_b, vsize, vaddr, raw_size, raw_off, _, _, _, _, flags = struct.unpack_from(
                "<8sIIIIIIHHI", raw, sec_off + i * 40
            )
            name = name_b.split(b"\x00", 1)[0].decode("ascii", "replace")
            self.sections.append(Section(name, vaddr, vsize, raw_off, raw_size, flags))

        self.iat_by_symbol: Dict[str, int] = {}
        self._parse_imports()
        self.funcs: List[Tuple[int, int]] = []
        self.func_starts: List[int] = []
        self._parse_pdata()

    def rva_to_off(self, rva: int) -> Optional[int]:
        for s in self.sections:
            if s.vaddr <= rva < s.vaddr + s.span:
                off = rva - s.vaddr
                if off < s.raw_size and s.raw_off + off < len(self.raw):
                    return s.raw_off + off
                return None
        return None

    def read_rva(self, rva: int, length: int) -> Optional[bytes]:
        off = self.rva_to_off(rva)
        if off is None or off + length > len(self.raw):
            return None
        return self.raw[off : off + length]

    def read_cstr(self, rva: int, limit: int = 256) -> Optional[str]:
        off = self.rva_to_off(rva)
        if off is None:
            return None
        end = self.raw.find(b"\x00", off, min(len(self.raw), off + limit))
        if end == -1:
            return None
        return self.raw[off:end].decode("ascii", "replace")

    def section_by_name(self, name: str) -> Optional[Section]:
        for s in self.sections:
            if s.name == name:
                return s
        return None

    def section_bytes(self, s: Section) -> bytes:
        return self.raw[s.raw_off : s.raw_off + s.raw_size]

    def exec_sections(self) -> List[Section]:
        ex = [s for s in self.sections if s.flags & 0x20000000]
        return ex if ex else ([self.section_by_name(".text")] if self.section_by_name(".text") else [])

    def rdata_sections(self) -> List[Section]:
        rd = [s for s in self.sections if s.name in (".rdata", ".rodata")]
        if rd:
            return rd
        return [s for s in self.sections if (s.flags & 0x40000000) and not (s.flags & 0xA0000000)]

    def data_range(self) -> Tuple[int, int]:
        ds = [s for s in self.sections if (s.flags & 0x80000000) and not (s.flags & 0x20000000)]
        if not ds:
            return (0x900000, self.size_of_image)
        return (min(s.vaddr for s in ds), max(s.vaddr + max(s.vsize, s.raw_size) for s in ds))

    def _parse_imports(self) -> None:
        if len(self.dirs) <= 1 or self.dirs[1][0] == 0:
            return
        imp_rva, _ = self.dirs[1]
        pos = imp_rva
        for _ in range(256):
            desc = self.read_rva(pos, 20)
            if not desc or desc == b"\x00" * 20:
                break
            ilt_rva, _, _, name_rva, iat_rva = struct.unpack("<IIIII", desc)
            pos += 20
            thunk_src = ilt_rva if ilt_rva else iat_rva
            idx = 0
            while idx < 8192:
                tb = self.read_rva(thunk_src + idx * 8, 8)
                if not tb:
                    break
                val = struct.unpack("<Q", tb)[0]
                if val == 0:
                    break
                if (val & (1 << 63)) == 0:
                    sym = self.read_cstr((val & 0x7FFFFFFF) + 2, 512)
                    if sym:
                        self.iat_by_symbol[sym] = iat_rva + idx * 8
                idx += 1

    def _parse_pdata(self) -> None:
        if len(self.dirs) <= 3 or self.dirs[3][0] == 0:
            return
        pdata_rva, pdata_sz = self.dirs[3]
        blob = self.read_rva(pdata_rva, pdata_sz)
        if not blob:
            return
        funcs: List[Tuple[int, int]] = []
        for off in range(0, len(blob) - 11, 12):
            b_rva, e_rva, _ = struct.unpack_from("<III", blob, off)
            if b_rva == 0 and e_rva == 0:
                break
            if b_rva < e_rva <= self.size_of_image:
                funcs.append((b_rva, e_rva))
        funcs.sort()
        self.funcs = funcs
        self.func_starts = [f[0] for f in funcs]

    def containing_function(self, rva: int) -> Optional[Tuple[int, int]]:
        if not self.func_starts:
            return None
        i = bisect.bisect_right(self.func_starts, rva) - 1
        if 0 <= i < len(self.funcs):
            b_rva, e_rva = self.funcs[i]
            if b_rva <= rva < e_rva:
                return (b_rva, e_rva)
        return None


# ---------------------------------------------------------------------------
# Relocation & Struct-Drift Scanner
# ---------------------------------------------------------------------------

class ScanResult:
    def __init__(self, pe: PE64Image):
        self.pe = pe
        self.version_str = "unknown"
        self.found: Dict[str, int] = {}
        self.confidence: Dict[str, str] = {}
        self.notes: Dict[str, str] = {}
        self.warnings: List[str] = []
        self.struct_checks: Dict[str, bool] = {}

    def record(self, key: str, rva: int, conf: str, note: str = "") -> None:
        if rva > 0:
            self.found[key] = rva
            self.confidence[key] = conf
            if note:
                self.notes[key] = note


def find_bytes_rva(pe: PE64Image, sections: List[Section], needle: bytes) -> List[int]:
    out: List[int] = []
    for s in sections:
        data = pe.section_bytes(s)
        pos = data.find(needle)
        while pos != -1:
            out.append(s.vaddr + pos)
            pos = data.find(needle, pos + 1)
    return out


def find_iat_callsites(pe: PE64Image, sym: str) -> List[int]:
    """Returns RVAs of `FF 15 <disp32>` (`CALL [RIP+disp32]`) targeting `sym`."""
    slot = pe.iat_by_symbol.get(sym)
    if not slot:
        return []
    sites: List[int] = []
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        pos = data.find(b"\xFF\x15")
        while pos != -1:
            if pos + 6 <= len(data):
                disp = struct.unpack_from("<i", data, pos + 2)[0]
                target = s.vaddr + pos + 6 + disp
                if target == slot:
                    sites.append(s.vaddr + pos)
            pos = data.find(b"\xFF\x15", pos + 1)
    return sites


def extract_preceding_rip_reg(pe: PE64Image, call_rva: int, opcode3: bytes, window: int = 40) -> List[int]:
    """Scans backwards up to `window` bytes before `call_rva` (stopping at any
    earlier CALL boundary) and returns the closest (last) 7-byte RIP-relative
    operand matching `opcode3` (e.g., `48 8D 0D` LEA RCX,[rip+d32])."""
    start = max(0x1000, call_rva - window)
    blob = pe.read_rva(start, call_rva - start)
    if not blob:
        return []
    # Clip after any prior `FF 15 <d32>` indirect call so back-to-back calls
    # never bleed registers across call boundaries.
    cut = 0
    pos_ff = blob.find(b"\xFF\x15")
    while pos_ff != -1:
        if pos_ff + 6 <= len(blob):
            cut = pos_ff + 6
        pos_ff = blob.find(b"\xFF\x15", pos_ff + 1)
    sub = blob[cut:]
    sub_base = start + cut

    targets: List[int] = []
    pos = sub.find(opcode3)
    while pos != -1:
        if pos + 7 <= len(sub):
            disp = struct.unpack_from("<i", sub, pos + 3)[0]
            targets.append(sub_base + pos + 7 + disp)
        pos = sub.find(opcode3, pos + 1)
    return targets[-1:] if targets else []


def scan_version_and_menu(res: ScanResult) -> None:
    pe = res.pe
    # 1. Narrow version string ("v%d.%d.%d.%d (64 bit DX11.1)") -> version numbers
    narrow_fmt = b"v%d.%d.%d.%d (64 bit DX11.1)\x00"
    fmt_rvas = find_bytes_rva(pe, pe.rdata_sections(), narrow_fmt)
    if fmt_rvas:
        fmt_rva = fmt_rvas[0]
        for s in pe.exec_sections():
            data = pe.section_bytes(s)
            pos = data.find(b"\x48\x8D\x0D")
            while pos != -1:
                if pos + 7 <= len(data):
                    disp = struct.unpack_from("<i", data, pos + 3)[0]
                    if s.vaddr + pos + 7 + disp == fmt_rva:
                        win = data[max(0, pos - 64) : pos]
                        nums = _decode_version_setup(win)
                        if nums:
                            res.version_str = f"{nums[0]}.{nums[1]}.{nums[2]}.{nums[3]}"
                        break
                pos = data.find(b"\x48\x8D\x0D", pos + 1)

    # 2. Wide menu version format -> `menu_version_site` (LEA RAX, [RIP+disp32])
    wide_fmt = "v%d.%d.%d.%d (64 bit DX11.1 - GPU: %ls)\x00".encode("utf-16-le")
    w_rvas = find_bytes_rva(pe, pe.rdata_sections(), wide_fmt)
    if w_rvas:
        w_rva = w_rvas[0]
        for s in pe.exec_sections():
            data = pe.section_bytes(s)
            pos = data.find(b"\x48\x8D\x05")
            while pos != -1:
                if pos + 7 <= len(data):
                    disp = struct.unpack_from("<i", data, pos + 3)[0]
                    if s.vaddr + pos + 7 + disp == w_rva:
                        res.record("menu_version_site", s.vaddr + pos, "HIGH",
                                   f"LEA RAX -> wide version fmt 0x{w_rva:X}")
                        break
                pos = data.find(b"\x48\x8D\x05", pos + 1)


def _decode_version_setup(win: bytes) -> Optional[Tuple[int, int, int, int]]:
    reg = [None] * 16
    stk = None
    i = 0
    n = len(win)
    while i < n:
        b = win[i]
        if i + 8 <= n and win[i : i + 4] == b"\xC7\x44\x24\x20":
            v = struct.unpack_from("<I", win, i + 4)[0]
            stk = v if v < 1000 else None
            i += 8
            continue
        if b == 0x41 and i + 2 <= n:
            op = win[i + 1]
            if 0xB8 <= op <= 0xBF and i + 6 <= n:
                v = struct.unpack_from("<I", win, i + 2)[0]
                reg[8 + (op - 0xB8)] = v if v < 1000 else None
                i += 6
                continue
            if op == 0x33 and i + 3 <= n:
                m = win[i + 2]
                if (m & 0xC0) == 0xC0 and ((m >> 3) & 7) == (m & 7):
                    reg[8 + (m & 7)] = 0
                    i += 3
                    continue
        if b == 0x45 and i + 3 <= n and win[i + 1] == 0x33:
            m = win[i + 2]
            if (m & 0xC0) == 0xC0 and ((m >> 3) & 7) == (m & 7):
                reg[8 + (m & 7)] = 0
                i += 3
                continue
        if b == 0x44 and i + 3 <= n and win[i + 1] == 0x8B:
            m = win[i + 2]
            if (m & 0xC0) == 0xC0:
                reg[8 + ((m >> 3) & 7)] = reg[m & 7]
                i += 3
                continue
        if 0xB8 <= b <= 0xBF and i + 5 <= n:
            v = struct.unpack_from("<I", win, i + 1)[0]
            reg[b - 0xB8] = v if v < 1000 else None
            i += 5
            continue
        if b == 0x33 and i + 2 <= n:
            m = win[i + 1]
            if (m & 0xC0) == 0xC0 and ((m >> 3) & 7) == (m & 7):
                reg[m & 7] = 0
                i += 2
                continue
        i += 1
    if reg[2] is not None and reg[8] is not None and reg[9] is not None and stk is not None:
        return (reg[2], reg[8], reg[9], stk)
    return None


def scan_game_object(res: ScanResult) -> None:
    pe = res.pe
    d_lo, d_hi = pe.data_range()
    # Exclude already-identified C3D engine singletons so `CGame` stands alone.
    engine_singletons = {
        res.found.get(k, 0)
        for k in ("sim_timer", "fontmgr", "panel2d", "input")
    } - {0}
    counts: collections.Counter[int] = collections.Counter()
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        pos = data.find(b"\x48\x8D\x0D")
        while pos != -1:
            if pos + 7 <= len(data):
                disp = struct.unpack_from("<i", data, pos + 3)[0]
                target = s.vaddr + pos + 7 + disp
                if d_lo <= target < d_hi and target not in engine_singletons:
                    counts[target] += 1
            pos = data.find(b"\x48\x8D\x0D", pos + 1)
    if counts:
        top = counts.most_common(2)
        best_rva, best_n = top[0]
        second_n = top[1][1] if len(top) > 1 else 0
        if best_n >= 3 and (second_n == 0 or best_n >= second_n * 2):
            res.record("game_object", best_rva, "HIGH",
                       f"dominant LEA RCX singleton ({best_n} xrefs vs #{2}: {second_n})")


def scan_sim_clock_and_ui_iat(res: ScanResult) -> None:
    pe = res.pe

    # 1. PowerTime -> daytick_power + sim_timer + day_length
    pt_sites = find_iat_callsites(pe, SYM_POWERTIME)
    for call_rva in pt_sites:
        # VerifyDayTickCall in sim_clock.cpp expects:
        #   p+00: 45 32 C9          xor r9b, r9b
        #   p+03: 48 8D 0D <disp32> lea rcx, [sim_timer]
        #   p+10: 45 32 C0          xor r8b, r8b
        #   p+13: FF 15 <disp32>    call [PowerTime]
        p = call_rva - 13
        seq = pe.read_rva(p, 19)
        if seq and seq[0:3] == b"\x45\x32\xC9" and seq[3:6] == b"\x48\x8D\x0D" and seq[10:13] == b"\x45\x32\xC0":
            disp = struct.unpack_from("<i", seq, 6)[0]
            sim_timer = p + 10 + disp
            res.record("daytick_power", p, "HIGH", "exact 19B PowerTime day-tick callsite")
            res.record("sim_timer", sim_timer, "HIGH", "RIP-relative RCX from daytick_power")
            # Scan nearby instructions (+-128B) for RIP-relative float 60.0f (`day_length`)
            near_start = max(0x1000, p - 96)
            near = pe.read_rva(near_start, 224) or b""
            for off in range(len(near) - 8):
                # F3 0F 10 / F3 0F 5E / 0F 2F with ModRM [RIP+disp32] ((modrm & 0xC7) == 0x05)
                if near[off] in (0x10, 0x5E, 0x2F) and off >= 1 and near[off - 1] == 0x0F:
                    modrm = near[off + 1]
                    if (modrm & 0xC7) == 0x05:
                        d32 = struct.unpack_from("<i", near, off + 2)[0]
                        cand = near_start + off + 6 + d32
                        fb = pe.read_rva(cand, 4)
                        if fb == f32_bytes(60.0):
                            res.record("day_length", cand, "HIGH", "60.0f constant at day-tick")
                            break
            break

    # 2. C3D_FONTMANAGER & fonts
    fm_counts: collections.Counter[int] = collections.Counter()
    rdx_var_counts: collections.Counter[int] = collections.Counter()
    rdx_noarg_counts: collections.Counter[int] = collections.Counter()
    rdx_center_counts: collections.Counter[int] = collections.Counter()

    for sym, font_counter in (
        (SYM_PRINT_VAR, rdx_var_counts),
        (SYM_PRINT_NOARG, rdx_noarg_counts),
        (SYM_PRINT_CENTER, rdx_center_counts),
    ):
        for site in find_iat_callsites(pe, sym):
            for t in extract_preceding_rip_reg(pe, site, b"\x48\x8D\x0D", 44):
                fm_counts[t] += 1
            for t in extract_preceding_rip_reg(pe, site, b"\x48\x8B\x15", 44):
                font_counter[t] += 1

    if fm_counts:
        res.record("fontmgr", fm_counts.most_common(1)[0][0], "HIGH",
                   f"LEA RCX before Print*Unicode ({fm_counts.most_common(1)[0][1]} calls)")
    if rdx_var_counts:
        top_fonts = [rva for rva, _ in rdx_var_counts.most_common(3)]
        res.record("font_default", top_fonts[0], "HIGH", "primary MOV RDX,[font] before PrintLeftUnicode")
        if len(top_fonts) >= 2:
            res.record("font_alt_a", top_fonts[1], "MEDIUM", "secondary MOV RDX,[font] before PrintLeftUnicode")
    if "font_alt_a" not in res.found and rdx_noarg_counts:
        res.record("font_alt_a", rdx_noarg_counts.most_common(1)[0][0], "HIGH",
                   "MOV RDX,[font] before PrintLeftUnicodeNoArg")
    if rdx_center_counts:
        res.record("font_alt_b", rdx_center_counts.most_common(1)[0][0], "HIGH",
                   "MOV RDX,[font] before PrintCenterUnicode")

    # 3. C3D_PANEL2D singleton + derived rect/tint cluster
    p2d_counts: collections.Counter[int] = collections.Counter()
    for sym in (SYM_DRAW_PLAIN, SYM_DRAW_9PATCH, SYM_COLLISION):
        for site in find_iat_callsites(pe, sym):
            for t in extract_preceding_rip_reg(pe, site, b"\x48\x8D\x0D", 48):
                p2d_counts[t] += 1
    if p2d_counts:
        p2d = p2d_counts.most_common(1)[0][0]
        res.record("panel2d", p2d, "HIGH", f"LEA RCX before C3D_PANEL2D calls ({p2d_counts[p2d]}x)")
        res.record("p2d_rect_w", p2d + 0x288, "HIGH", "panel2d + 0x288")
        res.record("p2d_rect_h", p2d + 0x28C, "HIGH", "panel2d + 0x28C")
        res.record("p2d_rect_x", p2d + 0x290, "HIGH", "panel2d + 0x290")
        res.record("p2d_rect_y", p2d + 0x294, "HIGH", "panel2d + 0x294")
        res.record("p2d_rot",    p2d + 0x298, "HIGH", "panel2d + 0x298")
        res.record("p2d_tint",   p2d + 0x2AC, "HIGH", "panel2d + 0x2AC")

    # 4. C3D_INPUT singleton + derived key/mouse edge bytes
    inp_counts: collections.Counter[int] = collections.Counter()
    for site in find_iat_callsites(pe, SYM_MOUSE_SOLID):
        for t in extract_preceding_rip_reg(pe, site, b"\x48\x8D\x0D", 36):
            inp_counts[t] += 1
    if inp_counts:
        inp = inp_counts.most_common(1)[0][0]
        res.record("input",       inp,         "HIGH", f"LEA RCX before C3D_INPUT::GetMouseSolid ({inp_counts[inp]}x)")
        res.record("esc_down",    inp + 0x001, "HIGH", "input + 0x001")
        res.record("esc_edge",    inp + 0x101, "HIGH", "input + 0x101")
        res.record("mouse_down",  inp + 0x300, "HIGH", "input + 0x300")
        res.record("click_latch", inp + 0x301, "HIGH", "input + 0x301")


def scan_rdata_float_clusters(res: ScanResult) -> None:
    pe = res.pe
    b20000 = f32_bytes(20000.0)
    b10000 = f32_bytes(10000.0)
    b9900  = f32_bytes(9900.0)
    b8000  = f32_bytes(8000.0)

    # 1. `map_expander` 20km/10km/9.9km/8km cluster in .rdata
    for s in pe.rdata_sections():
        data = pe.section_bytes(s)
        pos = data.find(b20000)
        while pos != -1:
            if pos % 4 == 0:
                win_lo = max(0, pos - 0x80)
                win_hi = min(len(data), pos + 0x80)
                win = data[win_lo:win_hi]
                p_half = win.find(b10000)
                p_gate = win.find(b9900)
                p_marg = win.find(b8000)
                if p_half != -1 and p_gate != -1 and p_marg != -1:
                    res.record("world_size", s.vaddr + pos, "HIGH", ".rdata 20000.0f cluster")
                    res.record("half_world", s.vaddr + win_lo + p_half, "HIGH", ".rdata 10000.0f cluster")
                    res.record("gate_hi",    s.vaddr + win_lo + p_gate, "HIGH", ".rdata 9900.0f (primary)")
                    res.record("margin_hi",  s.vaddr + win_lo + p_marg, "HIGH", ".rdata 8000.0f (primary)")
                    # Secondary gate_lo / margin_lo pair ~0xB00..0x1000 higher in .rdata
                    hi2_lo = min(len(data), pos + 0x400)
                    hi2_hi = min(len(data), pos + 0x1800)
                    sub = data[hi2_lo:hi2_hi]
                    g2 = sub.find(b9900)
                    m2 = sub.find(b8000)
                    if g2 != -1 and m2 != -1 and abs(g2 - m2) <= 0x40:
                        res.record("gate_lo",   s.vaddr + hi2_lo + g2, "HIGH", ".rdata 9900.0f (secondary)")
                        res.record("margin_lo", s.vaddr + hi2_lo + m2, "HIGH", ".rdata 8000.0f (secondary)")
                    break
            pos = data.find(b20000, pos + 4)

    # 2. `ui_panels` layout float cluster near `day_length` (60.0f)
    anchor_rva = res.found.get("day_length")
    if anchor_rva:
        for s in pe.rdata_sections():
            if not (s.vaddr <= anchor_rva < s.vaddr + s.span):
                continue
            data = pe.section_bytes(s)
            center = anchor_rva - s.vaddr
            lo = max(0, center - 0x1200)
            hi = min(len(data), center + 0x400)
            win = data[lo:hi]

            def pick_nearest(val: float, rel_hint: int) -> Optional[int]:
                needle = f32_bytes(val)
                cands: List[int] = []
                p = win.find(needle)
                while p != -1:
                    if (lo + p) % 4 == 0:
                        cands.append(s.vaddr + lo + p)
                    p = win.find(needle, p + 4)
                if not cands:
                    return None
                target_est = anchor_rva + rel_hint
                return min(cands, key=lambda r: abs(r - target_est))

            layout_specs = [
                ("row_step",      35.0,      0x90A9CC - 0x90AA78),
                ("x_indent",      110.0,     0x90AB98 - 0x90AA78),
                ("x_back",        15.0,      0x90A8CC - 0x90AA78),
                ("check_size",    20.0,      0x90A928 - 0x90AA78),
                ("ninep_border",  30.0,      0x90A9A0 - 0x90AA78),
                ("icon_btn_size", 65.0,      0x90AAA8 - 0x90AA78),
                ("tab_btn_size",  70.0,      0x90AAB8 - 0x90AA78),
                ("tab_row_dy",    105.0,     0x90AB84 - 0x90AA78),
                ("ninep_uv_a",    0.4375,    0x909D04 - 0x90AA78),
                ("ninep_uv_b",    0.546875,  0x909E78 - 0x90AA78),
                ("uv_full",       1.0,       0x909F58 - 0x90AA78),
            ]
            for key, fval, hint in layout_specs:
                got = pick_nearest(fval, hint)
                if got:
                    res.record(key, got, "HIGH", f".rdata layout float {fval}")


def scan_hooks_and_panels(res: ScanResult) -> None:
    pe = res.pe

    # 1. `citizen_labor` mobility hook (exact 18B, then offset-tolerant fallback)
    exact_labor = "F3 0F 10 84 8A 1C 12 00 00 41 0F 2F 84 8E 1C 12 00 00"
    drift_labor = "F3 0F 10 84 8A ?? ?? 00 00 41 0F 2F 84 8E ?? ?? 00 00"
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        hits = find_masked_all(data, exact_labor, 4)
        if len(hits) == 1:
            rva = s.vaddr + hits[0]
            res.record("labor_mobility_hook", rva, "HIGH", "exact 18B workplace wage COMISS")
            res.record("labor_mobility_jbe",  rva + 18, "HIGH", "labor_mobility_hook + 18")
            break
        dhits = find_masked_all(data, drift_labor, 4)
        if len(dhits) == 1:
            off = dhits[0]
            d1 = struct.unpack_from("<I", data, off + 5)[0]
            d2 = struct.unpack_from("<I", data, off + 14)[0]
            if d1 == d2:
                res.warnings.append(
                    f"STRUCT OFFSET DRIFT: workplace wage table moved from +0x121C to +0x{d1:X} "
                    f"(at RVA 0x{s.vaddr + off:X})! Update citizen_labor stub before enabling hook."
                )

    # 2. `citizen_retail` checkout hook + abort target
    checkout_pat = "F3 0F 10 44 1A 08 4D 8D 85 ?? ?? ?? ?? F3 41 0F 59 D7"
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        hits = find_masked_all(data, checkout_pat, 4)
        if len(hits) == 1:
            off = hits[0]
            rva = s.vaddr + off
            disp = struct.unpack_from("<I", data, off + 9)[0]
            if disp != 0x11420:
                res.warnings.append(
                    f"STRUCT OFFSET DRIFT: retail price table LEA moved from +0x11420 to +0x{disp:X}!"
                )
            else:
                res.record("retail_checkout_hook",  rva,      "HIGH", "exact 18B retail checkout site")
                res.record("retail_checkout_allow", rva + 18, "HIGH", "retail_checkout_hook + 18")
                # Locate abort jump target: JBE/JA/JMP rel32 shortly after +18
                tail = data[off + 18 : min(len(data), off + 80)]
                for j in range(len(tail) - 6):
                    if tail[j] == 0x0F and tail[j + 1] in (0x82, 0x86, 0x84, 0x85, 0x87):
                        rel = struct.unpack_from("<i", tail, j + 2)[0]
                        abort_rva = rva + 18 + j + 6 + rel
                        res.record("retail_checkout_abort", abort_rva, "HIGH",
                                   "conditional branch skip-demand target after checkout hook")
                        break
            break

    # 3. `window_dispatch` + `warehouse_guard` + `panel_warehouse`
    wh_pat = "83 7E 34 03 75 ?? 48 8B D6 48 8B CF E8 ?? ?? ?? ??"
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        hits = find_masked_all(data, wh_pat, 4)
        if len(hits) == 1:
            off = hits[0]
            guard_rva = s.vaddr + off
            rel32 = struct.unpack_from("<i", data, off + 13)[0]
            wh_builder = guard_rva + 17 + rel32
            res.record("warehouse_guard", guard_rva, "HIGH", "CMP [RSI+0x34], 3 guard")
            res.record("panel_warehouse", wh_builder, "HIGH", "CALL target from warehouse_guard")
            fn = pe.containing_function(guard_rva)
            if fn:
                res.record("window_dispatch", fn[0], "HIGH",
                           f".pdata function containing warehouse_guard (0x{fn[0]:X}..0x{fn[1]:X})")
            break

    # 4. Inside `window_dispatch`: `storage_td5`, `storage`, `do`, and panel builders!
    disp_rva = res.found.get("window_dispatch")
    if disp_rva:
        fn = pe.containing_function(disp_rva)
        disp_len = (fn[1] - fn[0]) if fn else 0x8000
        disp_blob = pe.read_rva(disp_rva, min(disp_len, 0x10000)) or b""

        # 4a. `storage_td5_guard` + `panel_storage_td5` (exact 20B kStorageTD5Expect shape)
        td5_hits = find_masked_all(
            disp_blob, "83 BA 60 03 00 00 05 75 ?? 48 8B D6 48 8B CF E8 ?? ?? ?? ??", 4
        )
        if len(td5_hits) == 1:
            off = td5_hits[0]
            g_rva = disp_rva + off
            rel = struct.unpack_from("<i", disp_blob, off + 16)[0]
            res.record("storage_td5_guard", g_rva, "HIGH", "CMP [RDX+0x360], 5 in dispatcher")
            res.record("panel_storage_td5", g_rva + 20 + rel, "HIGH", "CALL target of storage_td5_guard")

        # 4b. Collect all `E8 <rel32>` callees inside `window_dispatch` and match their prologues!
        callees_with_sites: List[Tuple[int, int]] = []
        for off in range(len(disp_blob) - 5):
            if disp_blob[off] == 0xE8:
                rel = struct.unpack_from("<i", disp_blob, off + 1)[0]
                target = disp_rva + off + 5 + rel
                if 0x1000 <= target < pe.size_of_image:
                    callees_with_sites.append((disp_rva + off, target))

        unique_callees = list(dict.fromkeys(t for _, t in callees_with_sites))
        prologue_map = [
            ("panel_shop",         PROLOGUE_SHOP),
            ("panel_residence",    PROLOGUE_RESIDENCE),
            ("panel_pub",          PROLOGUE_PUB),
            ("panel_factory",      PROLOGUE_FACTORY),
            ("panel_cityhall",     PROLOGUE_CITYHALL),
            ("panel_citizen",      PROLOGUE_CITIZEN),
            ("panel_storage",      PROLOGUE_STORAGE),
        ]
        for key, proto in prologue_map:
            matches = [t for t in unique_callees if pe.read_rva(t, len(proto)) == proto]
            if len(matches) == 1:
                res.record(key, matches[0], "HIGH", f"dispatcher callee with exact {len(proto)}B prologue")

        # `storage_callsite` = unique `E8` in dispatcher targeting `panel_storage`
        if "panel_storage" in res.found:
            st_sites = [cs for cs, t in callees_with_sites if t == res.found["panel_storage"]]
            if len(st_sites) == 1:
                res.record("storage_callsite", st_sites[0], "HIGH", "dispatcher CALL to panel_storage")

        # Building-type guard scan inside dispatcher:
        # Look for `CMP reg, imm8` (`83 F8..FF imm8` or `83 ba 60 03 00 00 imm8`) shortly before `E8`
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x27, "panel_cityhall", PROLOGUE_CITYHALL)
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x2B, "panel_do", None)
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x21, "panel_construction", PROLOGUE_CONSTRUCT)
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x03, "panel_shop", PROLOGUE_SHOP[:14])
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x02, "panel_residence", PROLOGUE_RESIDENCE[:14])
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x17, "panel_pub", PROLOGUE_PUB[:14])
        _match_dispatcher_case(pe, res, disp_blob, disp_rva, 0x06, "panel_factory", PROLOGUE_FACTORY[:14])

    # 5. `"gui_click"` string xref -> `play_gui_sound`
    gc_rvas = find_bytes_rva(pe, pe.rdata_sections(), b"gui_click\x00")
    if gc_rvas:
        gc_rva = gc_rvas[0]
        sound_callees: collections.Counter[int] = collections.Counter()
        for s in pe.exec_sections():
            data = pe.section_bytes(s)
            pos = data.find(b"\x48\x8D\x15")
            while pos != -1:
                if pos + 7 <= len(data):
                    disp = struct.unpack_from("<i", data, pos + 3)[0]
                    if s.vaddr + pos + 7 + disp == gc_rva:
                        # Scan next 24 bytes for `E8 <rel32>`
                        sub = data[pos + 7 : min(len(data), pos + 31)]
                        for k in range(len(sub) - 4):
                            if sub[k] == 0xE8:
                                rel = struct.unpack_from("<i", sub, k + 1)[0]
                                callee = s.vaddr + pos + 7 + k + 5 + rel
                                if 0x1000 <= callee < pe.size_of_image:
                                    sound_callees[callee] += 1
                                break
                pos = data.find(b"\x48\x8D\x15", pos + 1)
        if sound_callees:
            top_fn, cnt = sound_callees.most_common(1)[0]
            res.record("play_gui_sound", top_fn, "HIGH", f'called with "gui_click" ({cnt}x)')

    # 6. `dpi` global: multiplied with `tab_row_dy` or `row_step` inside panel builders
    dpi_counts: collections.Counter[int] = collections.Counter()
    for pkey in ("panel_shop", "panel_residence", "panel_pub", "panel_factory", "panel_cityhall"):
        prva = res.found.get(pkey)
        if not prva:
            continue
        fn = pe.containing_function(prva)
        flen = min((fn[1] - fn[0]) if fn else 0x3000, 0x6000)
        fblob = pe.read_rva(prva, flen) or b""
        # `F3 0F 59 05 <disp32>` / `F3 0F 59 0D <disp32>` (`MULSS XMM0/1, [RIP+disp32]`)
        d_lo, d_hi = pe.data_range()
        for off in range(len(fblob) - 8):
            if fblob[off] == 0xF3 and fblob[off + 1] == 0x0F and fblob[off + 2] == 0x59:
                modrm = fblob[off + 3]
                if (modrm & 0xC7) == 0x05:
                    d32 = struct.unpack_from("<i", fblob, off + 4)[0]
                    cand = prva + off + 8 + d32
                    if d_lo <= cand < d_hi:
                        dpi_counts[cand] += 1
    if dpi_counts:
        top_dpi, cnt = dpi_counts.most_common(1)[0]
        res.record("dpi", top_dpi, "HIGH", f"dominant .data MULSS scale in panel builders ({cnt}x)")

    # 7. `logistics_transfer` (cataloged for `private_logistics`): function iterating storage `+0x12D0`
    xfer_pat = "48 8B C4 55 53 56 57 41 54 41 55 41 56 41 57 48 8D A8 ?? ?? FF FF 48 81 EC"
    for s in pe.exec_sections():
        data = pe.section_bytes(s)
        for off in find_masked_all(data, xfer_pat, 64):
            rva = s.vaddr + off
            fn = pe.containing_function(rva)
            span = min((fn[1] - fn[0]) if fn else 0x1800, 0x2800)
            body = data[off : min(len(data), off + span)]
            # Must touch storage slots (+0x12D0 = D0 12 00 00) and typedef (+0x360 = 60 03 00 00)
            if b"\xD0\x12\x00\x00" in body and b"\x60\x03\x00\x00" in body:
                res.record("logistics_transfer", rva, "MEDIUM",
                           "function with matching prologue + 0x12D0/0x360 storage transfer refs")
                break


def _match_dispatcher_case(
    pe: PE64Image,
    res: ScanResult,
    disp_blob: bytes,
    disp_rva: int,
    imm_val: int,
    out_key: str,
    prologue_prefix: Optional[bytes],
) -> None:
    if out_key in res.found:
        return
    cands: List[int] = []
    ib = bytes([imm_val])
    for off in range(len(disp_blob) - 24):
        # Match `83 F8..FF imm8` or `83 7E 34 imm8` or `83 BA 60 03 00 00 imm8`
        matched_cmp = False
        if disp_blob[off] == 0x83 and 0xF8 <= disp_blob[off + 1] <= 0xFF and disp_blob[off + 2 : off + 3] == ib:
            matched_cmp = True
        elif disp_blob[off : off + 3] == b"\x83\x7E\x34" and disp_blob[off + 3 : off + 4] == ib:
            matched_cmp = True
        elif disp_blob[off : off + 6] == b"\x83\xBA\x60\x03\x00\x00" and disp_blob[off + 6 : off + 7] == ib:
            matched_cmp = True
        if not matched_cmp:
            continue
        # Look forward up to 28 bytes for `E8 <rel32>`
        for k in range(3, 28):
            if off + k + 5 <= len(disp_blob) and disp_blob[off + k] == 0xE8:
                rel = struct.unpack_from("<i", disp_blob, off + k + 1)[0]
                callee = disp_rva + off + k + 5 + rel
                if 0x1000 <= callee < pe.size_of_image:
                    if prologue_prefix is None or pe.read_rva(callee, len(prologue_prefix)) == prologue_prefix:
                        cands.append(callee)
                break
    uniq = list(dict.fromkeys(cands))
    if len(uniq) == 1:
        res.record(out_key, uniq[0], "HIGH", f"dispatcher case 0x{imm_val:02X} -> 0x{uniq[0]:X}")


def audit_struct_offsets(res: ScanResult) -> None:
    """Checks `.text` for critical `src/game_world.h` displacement immediates."""
    pe = res.pe
    text_blobs = [pe.section_bytes(s) for s in pe.exec_sections()]
    checks = {
        "typedef_building_type (+0x360)": b"\x60\x03\x00\x00",
        "building_storage_slots (+0x12D0)": b"\xD0\x12\x00\x00",
        "building_wage_table (+0x121C)": b"\x1C\x12\x00\x00",
        "building_price_table (+0x11420)": b"\x20\x14\x01\x00",
        "treasury_usd_rub (+0x580/+0x588)": b"\x80\x05\x00\x00",
        "person_workplace (+0x274)": b"\x74\x02\x00\x00",
    }
    for label, pat in checks.items():
        present = any(pat in tb for tb in text_blobs)
        res.struct_checks[label] = present
        if not present:
            res.warnings.append(f"STRUCT OFFSET MISSING IN .TEXT: {label}")


def run_scan(pe: PE64Image) -> ScanResult:
    res = ScanResult(pe)
    # Fast-path: if exact baseline build `0x6A3EB6AD`, seed all baseline RVAs first,
    # then run scanners to cross-verify every anchor!
    is_baseline = (pe.stamp == BASELINE_STAMP)
    scan_version_and_menu(res)
    scan_sim_clock_and_ui_iat(res)
    scan_game_object(res)
    scan_rdata_float_clusters(res)
    scan_hooks_and_panels(res)
    audit_struct_offsets(res)

    if is_baseline:
        for k, v in BASELINE_RVAS.items():
            if k not in res.found:
                res.record(k, v, "BASELINE", "exact v1.1.1.9 PE TimeDateStamp match")
    return res


# ---------------------------------------------------------------------------
# `addresses.ini` Emitter & Human Report
# ---------------------------------------------------------------------------

def render_addresses_ini(res: ScanResult) -> str:
    pe = res.pe
    ts_utc = datetime.datetime.fromtimestamp(pe.stamp, tz=datetime.timezone.utc).strftime(
        "%Y-%m-%d %H:%M:%S UTC"
    )
    lines = [
        "; addresses.ini - Auto-generated by tools/scan_exe_update.py",
        f"; Target SOVIET64.exe version : {res.version_str}",
        f"; Target PE TimeDateStamp     : 0x{pe.stamp:08X} ({ts_utc})",
        ";",
        "; Loaded automatically by tesmiolauncher.exe, tesmioloader.dll, and WRSRE",
        "; plugins when `target_stamp` matches the installed SOVIET64.exe.",
        "; Any unresolved optional RVA is set to 0x0 so only that single sub-feature",
        "; is skipped cleanly while the rest of WRSRE runs.",
        "",
        "[build]",
        f"target_stamp = 0x{pe.stamp:08X}",
        f"game_version = {res.version_str}",
        "",
        "[rva]",
    ]
    for key, base_val in BASELINE_RVAS.items():
        val = res.found.get(key, 0)
        conf = res.confidence.get(key, "UNRESOLVED -> disabled (0x0)")
        note = res.notes.get(key, "")
        delta = f"delta {val - base_val:+0x}" if val else f"was 0x{base_val:X}"
        comment = f"{conf} ({delta})" + (f" - {note}" if note else "")
        lines.append(f"{key:24s} = 0x{val:X} ; {comment}")
    lines.append("")
    return "\n".join(lines)


def print_report(res: ScanResult) -> None:
    pe = res.pe
    ts_utc = datetime.datetime.fromtimestamp(pe.stamp, tz=datetime.timezone.utc).strftime(
        "%Y-%m-%d %H:%M:%S UTC"
    )
    resolved = sum(1 for k in BASELINE_RVAS if res.found.get(k, 0) != 0)
    total = len(BASELINE_RVAS)
    print("=" * 76)
    print("WRSRE SOVIET64.exe Relocation & Struct-Drift Report")
    print("=" * 76)
    print(f"  Detected version   : {res.version_str}")
    print(f"  PE TimeDateStamp   : 0x{pe.stamp:08X} ({ts_utc})")
    print(f"  Baseline v1.1.1.9  : 0x{BASELINE_STAMP:08X} "
          f"({'MATCH' if pe.stamp == BASELINE_STAMP else 'NEW BUILD - RELOCATION REQUIRED'})")
    print(f"  .pdata functions   : {len(pe.funcs)}")
    print(f"  IAT symbols parsed : {len(pe.iat_by_symbol)}")
    print(f"  Relocated RVAs     : {resolved} / {total}")
    print("-" * 76)
    for key, base_val in BASELINE_RVAS.items():
        val = res.found.get(key, 0)
        conf = res.confidence.get(key, "MISSING")
        if val:
            print(f"  [OK]   {key:24s} 0x{base_val:06X} -> 0x{val:06X}  ({val - base_val:+08x})  [{conf}]")
        else:
            print(f"  [MISS] {key:24s} 0x{base_val:06X} -> 0x0       (disabled safely unless manually set)")
    print("-" * 76)
    print("Struct-Offset Drift Checks (src/game_world.h):")
    for label, ok in res.struct_checks.items():
        print(f"  [{'PASS' if ok else 'WARN'}] {label}")
    if res.warnings:
        print("-" * 76)
        print("WARNINGS:")
        for w in res.warnings:
            print(f"  ! {w}")
    print("=" * 76)


# ---------------------------------------------------------------------------
# Synthetic Shifted-DLC PE32+ Self-Test (`python3 tools/scan_exe_update.py selftest`)
# ---------------------------------------------------------------------------

def _build_synthetic_dlc_pe() -> Tuple[bytes, Dict[str, int]]:
    """Builds a valid PE32+ image simulating a post-DLC SOVIET64.exe v1.1.2.0
    where .text, .rdata, and .data have all shifted to new RVAs."""
    STAMP = 0x6B00D1C0
    TEXT_RVA,  TEXT_SZ  = 0x01000, 0x8000
    RDATA_RVA, RDATA_SZ = 0x10000, 0x4000
    DATA_RVA,  DATA_SZ  = 0x18000, 0x4000
    PDATA_RVA, PDATA_SZ = 0x20000, 0x1000

    text  = bytearray(TEXT_SZ)
    rdata = bytearray(RDATA_SZ)
    data  = bytearray(DATA_SZ)
    pdata = bytearray(PDATA_SZ)

    expected: Dict[str, int] = {}

    # --- 1. Build minimal Import Directory + IAT in .rdata ---
    syms = [
        SYM_POWERTIME, SYM_PRINT_VAR, SYM_PRINT_NOARG, SYM_PRINT_CENTER,
        SYM_DRAW_PLAIN, SYM_DRAW_9PATCH, SYM_COLLISION, SYM_MOUSE_SOLID,
    ]
    imp_desc_off = 0x100
    ilt_off      = 0x140
    iat_off      = 0x1C0
    str_cursor   = 0x240

    dll_name = b"C3DDLL64.dll\x00"
    rdata[str_cursor : str_cursor + len(dll_name)] = dll_name
    dll_name_rva = RDATA_RVA + str_cursor
    str_cursor += 32

    iat_rva_of: Dict[str, int] = {}
    for i, sym in enumerate(syms):
        ibn_rva = RDATA_RVA + str_cursor
        sb = sym.encode("ascii") + b"\x00"
        struct.pack_into("<H", rdata, str_cursor, i + 1)
        rdata[str_cursor + 2 : str_cursor + 2 + len(sb)] = sb
        str_cursor = (str_cursor + 2 + len(sb) + 7) & ~7
        struct.pack_into("<Q", rdata, ilt_off + i * 8, ibn_rva)
        struct.pack_into("<Q", rdata, iat_off + i * 8, ibn_rva)
        iat_rva_of[sym] = RDATA_RVA + iat_off + i * 8

    struct.pack_into(
        "<IIIII", rdata, imp_desc_off,
        RDATA_RVA + ilt_off, 0, 0, dll_name_rva, RDATA_RVA + iat_off
    )

    # --- 2. Strings & Float Clusters in .rdata ---
    vfmt_narrow_rva = RDATA_RVA + 0x700
    vfmt_narrow = b"v%d.%d.%d.%d (64 bit DX11.1)\x00"
    rdata[0x700 : 0x700 + len(vfmt_narrow)] = vfmt_narrow

    vfmt_wide_rva = RDATA_RVA + 0x740
    vfmt_wide = "v%d.%d.%d.%d (64 bit DX11.1 - GPU: %ls)\x00".encode("utf-16-le")
    rdata[0x740 : 0x740 + len(vfmt_wide)] = vfmt_wide

    guiclick_rva = RDATA_RVA + 0x800
    rdata[0x800 : 0x80A] = b"gui_click\x00"

    # UI layout floats + day_length (60.0f) around RDATA + 0x1A78
    def put_f32(rva: int, val: float, key: Optional[str] = None) -> None:
        struct.pack_into("<f", rdata, rva - RDATA_RVA, val)
        if key:
            expected[key] = rva

    put_f32(RDATA_RVA + 0x0D04, 0.4375,   "ninep_uv_a")
    put_f32(RDATA_RVA + 0x0E78, 0.546875, "ninep_uv_b")
    put_f32(RDATA_RVA + 0x0F58, 1.0,      "uv_full")
    put_f32(RDATA_RVA + 0x18CC, 15.0,     "x_back")
    put_f32(RDATA_RVA + 0x1928, 20.0,     "check_size")
    put_f32(RDATA_RVA + 0x19A0, 30.0,     "ninep_border")
    put_f32(RDATA_RVA + 0x19CC, 35.0,     "row_step")
    put_f32(RDATA_RVA + 0x1A78, 60.0,     "day_length")
    put_f32(RDATA_RVA + 0x1AA8, 65.0,     "icon_btn_size")
    put_f32(RDATA_RVA + 0x1AB8, 70.0,     "tab_btn_size")
    put_f32(RDATA_RVA + 0x1B84, 105.0,    "tab_row_dy")
    put_f32(RDATA_RVA + 0x1B98, 110.0,    "x_indent")

    # Map-expander world bounds cluster
    put_f32(RDATA_RVA + 0x2164, 8000.0,   "margin_hi")
    put_f32(RDATA_RVA + 0x2178, 9900.0,   "gate_hi")
    put_f32(RDATA_RVA + 0x2180, 10000.0,  "half_world")
    put_f32(RDATA_RVA + 0x21A4, 20000.0,  "world_size")
    put_f32(RDATA_RVA + 0x2D20, 8000.0,   "margin_lo")
    put_f32(RDATA_RVA + 0x2D2C, 9900.0,   "gate_lo")

    # --- 3. Shifted .data singletons ---
    expected["game_object"]  = DATA_RVA + 0x1F10
    expected["sim_timer"]    = DATA_RVA + 0x1EE0
    expected["dpi"]          = DATA_RVA + 0x0588
    expected["fontmgr"]      = DATA_RVA + 0x06B0
    expected["font_default"] = DATA_RVA + 0x0400
    expected["font_alt_a"]   = DATA_RVA + 0x0420
    expected["font_alt_b"]   = DATA_RVA + 0x0428
    expected["panel2d"]      = DATA_RVA + 0x0800
    expected["p2d_rect_w"]   = expected["panel2d"] + 0x288
    expected["p2d_rect_h"]   = expected["panel2d"] + 0x28C
    expected["p2d_rect_x"]   = expected["panel2d"] + 0x290
    expected["p2d_rect_y"]   = expected["panel2d"] + 0x294
    expected["p2d_rot"]      = expected["panel2d"] + 0x298
    expected["p2d_tint"]     = expected["panel2d"] + 0x2AC
    expected["input"]        = DATA_RVA + 0x1200
    expected["esc_down"]     = expected["input"] + 0x001
    expected["esc_edge"]     = expected["input"] + 0x101
    expected["mouse_down"]   = expected["input"] + 0x300
    expected["click_latch"]  = expected["input"] + 0x301

    # --- 4. Helper emitters into `.text` + `.pdata` ---
    pdata_entries: List[Tuple[int, int]] = []

    def put_bytes(rva: int, b: bytes) -> int:
        off = rva - TEXT_RVA
        text[off : off + len(b)] = b
        return rva + len(b)

    def emit_rip(rva: int, prefix: bytes, target_rva: int) -> int:
        disp = target_rva - (rva + len(prefix) + 4)
        return put_bytes(rva, prefix + struct.pack("<i", disp))

    def emit_call(rva: int, target_rva: int) -> int:
        disp = target_rva - (rva + 5)
        return put_bytes(rva, b"\xE8" + struct.pack("<i", disp))

    # 4a. Version format callsite (v1.1.2.0) + wide menu_version_site
    cur = TEXT_RVA + 0x100
    cur = put_bytes(cur, b"\xC7\x44\x24\x20" + struct.pack("<I", 0))  # v[3] = 0
    cur = put_bytes(cur, b"\xBA" + struct.pack("<I", 1))              # v[0] = 1
    cur = put_bytes(cur, b"\x41\xB8" + struct.pack("<I", 1))          # v[1] = 1
    cur = put_bytes(cur, b"\x41\xB9" + struct.pack("<I", 2))          # v[2] = 2
    cur = emit_rip(cur, b"\x48\x8D\x0D", vfmt_narrow_rva)
    expected["menu_version_site"] = cur
    cur = emit_rip(cur, b"\x48\x8D\x05", vfmt_wide_rva)

    # 4b. Dominant `game_object` LEA RCX xrefs (8x) + struct-offset canary instructions
    for _ in range(8):
        cur = emit_rip(cur, b"\x48\x8D\x0D", expected["game_object"])
    # Struct offset instructions (`+0x360`, `+0x12D0`, `+0x121C`, `+0x11420`, `+0x580`, `+0x274`)
    cur = put_bytes(cur, b"\x8B\x81\x60\x03\x00\x00\x8B\x81\xD0\x12\x00\x00")
    cur = put_bytes(cur, b"\x8B\x81\x80\x05\x00\x00\x8B\x81\x74\x02\x00\x00")

    # 4c. `daytick_power` + `sim_timer` + `day_length`
    cur = (cur + 0x1F) & ~0xF
    cur = emit_rip(cur, b"\xF3\x0F\x10\x05", expected["day_length"])
    expected["daytick_power"] = cur
    cur = put_bytes(cur, b"\x45\x32\xC9")
    cur = emit_rip(cur, b"\x48\x8D\x0D", expected["sim_timer"])
    cur = put_bytes(cur, b"\x45\x32\xC0")
    cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_POWERTIME])

    # 4d. `fontmgr`, `font_default`, `font_alt_a`, `font_alt_b`, `panel2d`, `input`, `play_gui_sound`
    expected["play_gui_sound"] = TEXT_RVA + 0x600
    put_bytes(expected["play_gui_sound"], b"\x48\x89\x5C\x24\x08\xC3")
    pdata_entries.append((expected["play_gui_sound"], expected["play_gui_sound"] + 0x20))

    cur = TEXT_RVA + 0x640
    for _ in range(3):
        cur = emit_rip(cur, b"\x48\x8D\x0D", expected["fontmgr"])
        cur = emit_rip(cur, b"\x48\x8B\x15", expected["font_default"])
        cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_PRINT_VAR])
    cur = emit_rip(cur, b"\x48\x8D\x0D", expected["fontmgr"])
    cur = emit_rip(cur, b"\x48\x8B\x15", expected["font_alt_a"])
    cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_PRINT_NOARG])
    cur = emit_rip(cur, b"\x48\x8D\x0D", expected["fontmgr"])
    cur = emit_rip(cur, b"\x48\x8B\x15", expected["font_alt_b"])
    cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_PRINT_CENTER])

    cur = emit_rip(cur, b"\x48\x8D\x0D", expected["panel2d"])
    cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_DRAW_9PATCH])
    cur = emit_rip(cur, b"\x48\x8D\x0D", expected["input"])
    cur = emit_rip(cur, b"\xFF\x15", iat_rva_of[SYM_MOUSE_SOLID])
    cur = emit_rip(cur, b"\x48\x8D\x15", guiclick_rva)
    cur = emit_call(cur, expected["play_gui_sound"])

    # 4e. `labor_mobility_hook` & `retail_checkout_hook`
    expected["labor_mobility_hook"] = TEXT_RVA + 0x900
    expected["labor_mobility_jbe"]  = expected["labor_mobility_hook"] + 18
    put_bytes(expected["labor_mobility_hook"], bytes.fromhex(
        "F3 0F 10 84 8A 1C 12 00 00 41 0F 2F 84 8E 1C 12 00 00 76 10"
    ))

    expected["retail_checkout_hook"]  = TEXT_RVA + 0xA00
    expected["retail_checkout_allow"] = expected["retail_checkout_hook"] + 18
    expected["retail_checkout_abort"] = TEXT_RVA + 0xB80
    c_end = put_bytes(expected["retail_checkout_hook"], bytes.fromhex(
        "F3 0F 10 44 1A 08 4D 8D 85 20 14 01 00 F3 41 0F 59 D7"
    ))
    jbe_rel = expected["retail_checkout_abort"] - (c_end + 6)
    put_bytes(c_end, b"\x0F\x86" + struct.pack("<i", jbe_rel))

    # 4f. Panel builders + `window_dispatch`
    expected["panel_shop"]         = TEXT_RVA + 0x1000
    expected["panel_residence"]    = TEXT_RVA + 0x1200
    expected["panel_pub"]          = TEXT_RVA + 0x1400
    expected["panel_factory"]      = TEXT_RVA + 0x1600
    expected["panel_cityhall"]     = TEXT_RVA + 0x1800
    expected["panel_storage"]      = TEXT_RVA + 0x1A00
    expected["panel_do"]           = TEXT_RVA + 0x1C00
    expected["panel_warehouse"]    = TEXT_RVA + 0x1E00
    expected["panel_storage_td5"]  = TEXT_RVA + 0x2000
    expected["panel_construction"] = TEXT_RVA + 0x2200

    for k, proto in (
        ("panel_shop", PROLOGUE_SHOP),
        ("panel_residence", PROLOGUE_RESIDENCE),
        ("panel_pub", PROLOGUE_PUB),
        ("panel_factory", PROLOGUE_FACTORY),
        ("panel_cityhall", PROLOGUE_CITYHALL),
        ("panel_storage", PROLOGUE_STORAGE),
        ("panel_construction", PROLOGUE_CONSTRUCT),
    ):
        end_p = put_bytes(expected[k], proto)
        emit_rip(end_p, b"\xF3\x0F\x59\x05", expected["dpi"])
        pdata_entries.append((expected[k], expected[k] + 0x180))

    for k in ("panel_do", "panel_warehouse", "panel_storage_td5"):
        put_bytes(expected[k], b"\x48\x8B\xC4\x55\x56\x57\xC3")
        pdata_entries.append((expected[k], expected[k] + 0x100))

    # `window_dispatch` function containing warehouse_guard, storage_td5_guard, and case calls
    disp_start = TEXT_RVA + 0x3000
    expected["window_dispatch"] = disp_start
    dc = put_bytes(disp_start, b"\x40\x55\x56\x57")

    # Warehouse guard
    expected["warehouse_guard"] = dc
    dc = put_bytes(dc, b"\x83\x7E\x34\x03\x75\x20\x48\x8B\xD6\x48\x8B\xCF")
    dc = emit_call(dc, expected["panel_warehouse"])

    # Storage td==5 guard (exact 20B kStorageTD5Expect shape)
    expected["storage_td5_guard"] = dc
    dc = put_bytes(dc, b"\x83\xBA\x60\x03\x00\x00\x05\x75\x10\x48\x8B\xD6\x48\x8B\xCF")
    dc = emit_call(dc, expected["panel_storage_td5"])

    # Storage inline/callsite
    expected["storage_callsite"] = dc + 3
    dc = put_bytes(dc, b"\x83\xF8\x05")
    dc = emit_call(dc, expected["panel_storage"])

    # Shop, Residence, Pub, Factory, CityHall, DO, Construction cases
    for imm, k in (
        (0x03, "panel_shop"),
        (0x02, "panel_residence"),
        (0x17, "panel_pub"),
        (0x06, "panel_factory"),
        (0x27, "panel_cityhall"),
        (0x2B, "panel_do"),
        (0x21, "panel_construction"),
    ):
        dc = put_bytes(dc, bytes([0x83, 0xF8, imm, 0x75, 0x05]))
        dc = emit_call(dc, expected[k])

    pdata_entries.append((disp_start, dc + 0x40))

    # Write sorted `.pdata` entries
    pdata_entries.sort()
    for idx, (b_rva, e_rva) in enumerate(pdata_entries):
        struct.pack_into("<III", pdata, idx * 12, b_rva, e_rva, RDATA_RVA)

    # --- 5. Assemble PE32+ file ---
    hdr = bytearray(0x400)
    hdr[0:2] = b"MZ"
    lfanew = 0x80
    struct.pack_into("<I", hdr, 0x3C, lfanew)
    hdr[lfanew : lfanew + 4] = b"PE\x00\x00"
    num_sec = 4
    opt_size = 240
    struct.pack_into("<HHIIIHH", hdr, lfanew + 4, 0x8664, num_sec, STAMP, 0, 0, opt_size, 0x22)
    opt_off = lfanew + 24
    struct.pack_into("<H", hdr, opt_off, 0x20B)
    struct.pack_into("<Q", hdr, opt_off + 24, 0x140000000)
    struct.pack_into("<I", hdr, opt_off + 32, 0x1000)
    struct.pack_into("<I", hdr, opt_off + 36, 0x200)
    struct.pack_into("<I", hdr, opt_off + 56, 0x30000)
    struct.pack_into("<I", hdr, opt_off + 60, 0x400)
    struct.pack_into("<I", hdr, opt_off + 108, 16)
    # Data directory [1] Import, [3] Exception (.pdata)
    struct.pack_into("<II", hdr, opt_off + 112 + 1 * 8, RDATA_RVA + imp_desc_off, 40)
    struct.pack_into("<II", hdr, opt_off + 112 + 3 * 8, PDATA_RVA, len(pdata_entries) * 12)

    secs = [
        (b".text\x00\x00\x00",  TEXT_SZ,  TEXT_RVA,  text,  0x60000020),
        (b".rdata\x00\x00",     RDATA_SZ, RDATA_RVA, rdata, 0x40000040),
        (b".data\x00\x00\x00",  DATA_SZ,  DATA_RVA,  data,  0xC0000040),
        (b".pdata\x00\x00",     PDATA_SZ, PDATA_RVA, pdata, 0x40000040),
    ]
    raw_cursor = 0x400
    sec_hdr_off = opt_off + opt_size
    body = bytearray()
    for i, (sname, vsz, vrva, sbuf, sflags) in enumerate(secs):
        struct.pack_into(
            "<8sIIIIIIHHI", hdr, sec_hdr_off + i * 40,
            sname, vsz, vrva, len(sbuf), raw_cursor, 0, 0, 0, 0, sflags
        )
        body.extend(sbuf)
        raw_cursor += len(sbuf)

    return bytes(hdr + body), expected


def run_selftest() -> int:
    raw, expected = _build_synthetic_dlc_pe()
    pe = PE64Image(raw)
    res = run_scan(pe)

    assert res.version_str == "1.1.2.0", f"version mismatch: {res.version_str}"
    assert pe.stamp == 0x6B00D1C0

    mismatches: List[str] = []
    for k, exp_rva in expected.items():
        got = res.found.get(k, 0)
        if got != exp_rva:
            mismatches.append(f"{k}: expected 0x{exp_rva:X}, got 0x{got:X}")

    for label, ok in res.struct_checks.items():
        if not ok:
            mismatches.append(f"struct check failed: {label}")

    ini_text = render_addresses_ini(res)
    if "target_stamp = 0x6B00D1C0" not in ini_text:
        mismatches.append("INI missing target_stamp = 0x6B00D1C0")

    if mismatches:
        print("SELFTEST FAILED:")
        for m in mismatches:
            print("  -", m)
        return 1

    print(
        f"SELFTEST PASSED: synthetic post-DLC SOVIET64.exe v{res.version_str} "
        f"(stamp 0x{pe.stamp:08X}) -> {len(expected)}/{len(expected)} shifted RVAs "
        f"and {len(res.struct_checks)}/{len(res.struct_checks)} struct-offset checks verified."
    )
    return 0


# ---------------------------------------------------------------------------
# CLI Entrypoint
# ---------------------------------------------------------------------------

def main(argv: List[str]) -> int:
    if len(argv) >= 2 and argv[1] == "selftest":
        return run_selftest()

    ap = argparse.ArgumentParser(
        description="Relocate WRSRE SOVIET64.exe RVAs and emit addresses.ini after a game update/DLC."
    )
    ap.add_argument("exe", help="Path to SOVIET64.exe (or 'selftest')")
    ap.add_argument("--write-ini", metavar="PATH", help="Write relocated addresses.ini to PATH")
    ap.add_argument("--json", metavar="PATH", help="Write machine-readable JSON scan report to PATH")
    args = ap.parse_args(argv[1:])

    if args.exe == "selftest":
        return run_selftest()

    exe_path = Path(args.exe)
    if not exe_path.is_file():
        print(f"error: file not found: {exe_path}", file=sys.stderr)
        return 2

    pe = PE64Image(exe_path.read_bytes())
    res = run_scan(pe)
    print_report(res)

    if args.write_ini:
        out_ini = Path(args.write_ini)
        out_ini.write_text(render_addresses_ini(res), encoding="utf-8")
        print(f"Wrote {out_ini} (target_stamp = 0x{pe.stamp:08X})")

    if args.json:
        payload = {
            "version": res.version_str,
            "stamp": f"0x{pe.stamp:08X}",
            "stamp_int": pe.stamp,
            "rvas": {k: res.found.get(k, 0) for k in BASELINE_RVAS},
            "confidence": res.confidence,
            "struct_checks": res.struct_checks,
            "warnings": res.warnings,
        }
        Path(args.json).write_text(json.dumps(payload, indent=2), encoding="utf-8")
        print(f"Wrote JSON report to {args.json}")

    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
