#!/usr/bin/env python3
"""
peinfo.py - read a Windows PE (.dll or .exe) without running it.

SIMPLEST USE: put this script in the same folder as reputation.dll and run

    python peinfo.py

It finds every .dll and .exe beside itself, reads each one, prints a digest,
and writes the full dumps to a `peinfo-out` folder next to the script.
No arguments, no paths, no install. Standard library only.

Written for "reputation 2.0 ships no source, so how do we find out what it
does". A compiled plugin still has to declare, in the clear:

  * every function it imports from the game and from Windows
  * every function it exports
  * the build it was compiled against
  * its own function names, if it was built with symbols
  * every ini key, log message and file path it uses

`imports` in particular answers "which UI methods does it call", because
engine methods are imported by decorated C++ name and MSVC leaves those names
intact. A decorated name IS a signature.

NOTE ON THE FILENAME: do not rename this to inspect.py, types.py, string.py or
any other standard-library module name. Python searches the script's own
folder first, so the script would shadow the real module and argparse would
fail on import.

Individual commands, if you want them:
    python peinfo.py summary FILE
    python peinfo.py imports FILE [--dll C3DDLL64] [--grep Print]
    python peinfo.py exports FILE
    python peinfo.py symbols FILE [--grep Panel]
    python peinfo.py strings FILE [--min 8] [--grep panel_]
    python peinfo.py selftest
"""

import argparse
import datetime
import glob
import os
import re
import struct
import sys

# ----------------------------------------------------------------- PE reading

IMAGE_DOS_SIGNATURE = 0x5A4D
IMAGE_NT_SIGNATURE = 0x00004550
MACHINE = {0x8664: "x64", 0x014C: "x86", 0xAA64: "arm64"}


class PEError(Exception):
    pass


class PE(object):
    """Just enough PE64/PE32 to read headers, sections, imports and exports."""

    def __init__(self, data):
        self.d = data
        if len(data) < 0x40:
            raise PEError("file is too small to be a PE")
        if struct.unpack_from("<H", data, 0)[0] != IMAGE_DOS_SIGNATURE:
            raise PEError("no MZ signature - not a PE file")

        self.e_lfanew = struct.unpack_from("<I", data, 0x3C)[0]
        if self.e_lfanew + 24 > len(data):
            raise PEError("PE header offset points past the end of the file")
        if struct.unpack_from("<I", data, self.e_lfanew)[0] != IMAGE_NT_SIGNATURE:
            raise PEError("no PE signature")

        fh = self.e_lfanew + 4
        (self.machine, self.n_sections, self.timestamp,
         _sym, _nsym, self.opt_size, self.characteristics
         ) = struct.unpack_from("<HHIIIHH", data, fh)

        opt = fh + 20
        self.magic = struct.unpack_from("<H", data, opt)[0]
        self.is64 = self.magic == 0x20B
        if self.magic not in (0x10B, 0x20B):
            raise PEError("unrecognised optional header magic 0x%04X" % self.magic)

        if self.is64:
            self.image_base = struct.unpack_from("<Q", data, opt + 24)[0]
            dd = opt + 112
        else:
            self.image_base = struct.unpack_from("<I", data, opt + 28)[0]
            dd = opt + 96
        self.image_size = struct.unpack_from("<I", data, opt + 56)[0]
        self.n_dd = struct.unpack_from("<I", data, dd - 4)[0]

        self.dirs = []
        for i in range(min(self.n_dd, 16)):
            rva, size = struct.unpack_from("<II", data, dd + i * 8)
            self.dirs.append((rva, size))

        self.sections = []
        sec = opt + self.opt_size
        for i in range(self.n_sections):
            o = sec + i * 40
            if o + 40 > len(data):
                break
            name = data[o:o + 8].rstrip(b"\0").decode("latin-1")
            vsize, vaddr, rsize, raddr = struct.unpack_from("<IIII", data, o + 8)
            flags = struct.unpack_from("<I", data, o + 36)[0]
            self.sections.append(
                {"name": name, "vsize": vsize, "vaddr": vaddr,
                 "rsize": rsize, "raddr": raddr, "flags": flags})

    # --- address translation

    def off(self, rva):
        """RVA -> file offset, or None when it lands outside every section."""
        for s in self.sections:
            size = max(s["vsize"], s["rsize"])
            if s["vaddr"] <= rva < s["vaddr"] + size:
                delta = rva - s["vaddr"]
                if delta < s["rsize"]:
                    return s["raddr"] + delta
                return None
        return None

    def cstr(self, rva, limit=512):
        o = self.off(rva)
        if o is None:
            return None
        end = self.d.find(b"\0", o, o + limit)
        if end < 0:
            return None
        return self.d[o:end].decode("latin-1")

    def u16(self, o):
        return struct.unpack_from("<H", self.d, o)[0]

    def u32(self, o):
        return struct.unpack_from("<I", self.d, o)[0]

    def u64(self, o):
        return struct.unpack_from("<Q", self.d, o)[0]

    # --- directories

    def imports(self):
        """[(dll, [(name_or_None, ordinal_or_None), ...]), ...]"""
        if len(self.dirs) < 2:
            return []
        rva, _size = self.dirs[1]
        if not rva:
            return []
        base = self.off(rva)
        if base is None:
            return []

        out = []
        i = 0
        while True:
            o = base + i * 20
            if o + 20 > len(self.d):
                break
            oft, _t, _f, name_rva, first = struct.unpack_from("<IIIII", self.d, o)
            if not (oft or name_rva or first):
                break
            dll = self.cstr(name_rva) or "<unreadable>"
            thunks = self._thunks(oft or first)
            out.append((dll, thunks))
            i += 1
            if i > 4096:
                break
        return out

    def _thunks(self, rva):
        res = []
        o = self.off(rva)
        if o is None:
            return res
        step = 8 if self.is64 else 4
        flag = (1 << 63) if self.is64 else (1 << 31)
        n = 0
        while o + step <= len(self.d) and n < 65536:
            v = self.u64(o) if self.is64 else self.u32(o)
            if v == 0:
                break
            if v & flag:
                res.append((None, v & 0xFFFF))
            else:
                # IMAGE_IMPORT_BY_NAME: WORD hint, then the name
                so = self.off(v)
                name = self.cstr(v + 2) if so is not None else None
                res.append((name, None))
            o += step
            n += 1
        return res

    def exports(self):
        """[(name_or_None, ordinal, rva), ...] plus the module's own name."""
        if len(self.dirs) < 1:
            return None, []
        rva, _size = self.dirs[0]
        if not rva:
            return None, []
        o = self.off(rva)
        if o is None:
            return None, []

        name_rva = self.u32(o + 12)
        ordinal_base = self.u32(o + 16)
        n_funcs = self.u32(o + 20)
        n_names = self.u32(o + 24)
        funcs_rva = self.u32(o + 28)
        names_rva = self.u32(o + 32)
        ords_rva = self.u32(o + 36)

        modname = self.cstr(name_rva)
        fo = self.off(funcs_rva)
        no = self.off(names_rva)
        oo = self.off(ords_rva)

        by_ord = {}
        if fo is not None:
            for i in range(min(n_funcs, 65536)):
                if fo + i * 4 + 4 > len(self.d):
                    break
                by_ord[i] = self.u32(fo + i * 4)

        named = {}
        if no is not None and oo is not None:
            for i in range(min(n_names, 65536)):
                if no + i * 4 + 4 > len(self.d) or oo + i * 2 + 2 > len(self.d):
                    break
                nm = self.cstr(self.u32(no + i * 4))
                named[self.u16(oo + i * 2)] = nm

        out = []
        for idx in sorted(by_ord):
            out.append((named.get(idx), idx + ordinal_base, by_ord[idx]))
        return modname, out


# ------------------------------------------------------------------- scanning

# MSVC decorates C++ symbols as ?name@class@@signature. That is what makes
# `?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@...` readable in a stripped binary.
DECORATED = re.compile(rb"\?[A-Za-z_][\w@?$]{3,}@@[A-Z0-9_@?$]{2,}")
PRINTABLE = rb"[\x20-\x7e]"


def ascii_strings(data, minlen):
    pat = re.compile(PRINTABLE + b"{%d,}" % minlen)
    for m in pat.finditer(data):
        yield m.start(), m.group().decode("latin-1")


def utf16_strings(data, minlen):
    pat = re.compile(b"(?:" + PRINTABLE + b"\x00){%d,}" % minlen)
    for m in pat.finditer(data):
        yield m.start(), m.group().decode("utf-16-le", "replace")


# -------------------------------------------------------------------- reports

def cmd_summary(pe, _a):
    print("machine        : %s (0x%04X)" % (MACHINE.get(pe.machine, "?"), pe.machine))
    print("bitness        : %s" % ("PE32+" if pe.is64 else "PE32"))
    print("timestamp      : 0x%08X" % pe.timestamp)
    print("image base     : 0x%X" % pe.image_base)
    print("image size     : 0x%08X" % pe.image_size)
    print("sections       : %d" % len(pe.sections))
    print()
    print("  %-10s %-10s %-10s %-10s %s" % ("name", "vaddr", "vsize", "raw", "flags"))
    for s in pe.sections:
        print("  %-10s 0x%08X 0x%08X 0x%08X 0x%08X"
              % (s["name"], s["vaddr"], s["vsize"], s["rsize"], s["flags"]))
    print()
    imps = pe.imports()
    print("imported DLLs  : %d" % len(imps))
    for dll, th in imps:
        print("  %-28s %d function(s)" % (dll, len(th)))
    modname, exps = pe.exports()
    print("exports        : %d%s"
          % (len(exps), (" (module name %s)" % modname) if modname else ""))


def cmd_imports(pe, a):
    total = 0
    for dll, thunks in pe.imports():
        if a.dll and a.dll.lower() not in dll.lower():
            continue
        rows = []
        for name, ordinal in thunks:
            label = name if name else "#%d" % ordinal
            if a.grep and a.grep.lower() not in label.lower():
                continue
            rows.append(label)
        if not rows:
            continue
        print("%s  (%d shown of %d)" % (dll, len(rows), len(thunks)))
        for r in sorted(rows):
            print("    %s" % r)
        print()
        total += len(rows)
    print("-- %d imported symbol(s) shown" % total)


def cmd_exports(pe, a):
    modname, exps = pe.exports()
    if modname:
        print("module name: %s" % modname)
    n = 0
    for name, ordinal, rva in exps:
        label = name or "<by ordinal>"
        if a.grep and a.grep.lower() not in label.lower():
            continue
        print("  %-6d 0x%08X  %s" % (ordinal, rva, label))
        n += 1
    print("-- %d export(s) shown" % n)


def cmd_symbols(pe, a):
    seen = set()
    for m in DECORATED.finditer(pe.d):
        s = m.group().decode("latin-1")
        if a.grep and a.grep.lower() not in s.lower():
            continue
        if s in seen:
            continue
        seen.add(s)
    for s in sorted(seen):
        print("  %s" % s)
    print("-- %d distinct decorated C++ name(s)" % len(seen))


def cmd_strings(pe, a):
    n = 0
    for _o, s in ascii_strings(pe.d, a.min):
        if a.grep and a.grep.lower() not in s.lower():
            continue
        print("  A  %s" % s)
        n += 1
    for _o, s in utf16_strings(pe.d, a.min):
        if a.grep and a.grep.lower() not in s.lower():
            continue
        print("  W  %s" % s)
        n += 1
    print("-- %d string(s) shown" % n)


# ------------------------------------------------------------------- selftest

def build_test_pe():
    """A minimal but structurally valid PE64 with one import and one export."""
    SEC = 0x1000
    data = bytearray()

    dos = bytearray(0x40)
    struct.pack_into("<H", dos, 0, IMAGE_DOS_SIGNATURE)
    struct.pack_into("<I", dos, 0x3C, 0x40)
    data += dos

    # --- build the .rdata payload first, at rva 0x1000
    r = bytearray()

    def at(off):
        return SEC + off

    dllname = len(r); r += b"C3DDLL64.dll\0"
    impname = len(r); r += struct.pack("<H", 0) + b"?GetSize@C3D_FONT@@QEAAMXZ\0"
    expmod = len(r); r += b"testplug.dll\0"
    expname = len(r); r += b"TsmPluginInit\0"
    while len(r) % 8:
        r += b"\0"

    oft = len(r)
    r += struct.pack("<Q", at(impname)) + struct.pack("<Q", 0)
    iat = len(r)
    r += struct.pack("<Q", at(impname)) + struct.pack("<Q", 0)

    impdesc = len(r)
    r += struct.pack("<IIIII", at(oft), 0, 0, at(dllname), at(iat))
    r += struct.pack("<IIIII", 0, 0, 0, 0, 0)

    expfuncs = len(r); r += struct.pack("<I", 0x2000)
    expnames = len(r); r += struct.pack("<I", at(expname))
    expords = len(r); r += struct.pack("<H", 0)
    while len(r) % 4:
        r += b"\0"
    expdir = len(r)
    # Characteristics, TimeDateStamp, MajorVersion, MinorVersion = 12 bytes.
    # Getting this wrong by four shifts every field after it, which is exactly
    # the "anchor the frame pointer" trap in docs/03-reverse-engineering.md.
    r += struct.pack("<IIHH", 0, 0, 0, 0)
    r += struct.pack("<I", at(expmod)) + struct.pack("<I", 1)
    r += struct.pack("<II", 1, 1)
    r += struct.pack("<III", at(expfuncs), at(expnames), at(expords))

    r += b"panel_hook is a config key\0"
    r += b"?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z\0"

    opt_size = 240
    nt = bytearray(4 + 20 + opt_size + 40)
    struct.pack_into("<I", nt, 0, IMAGE_NT_SIGNATURE)
    struct.pack_into("<HHIIIHH", nt, 4, 0x8664, 1, 0x6A3EB6AD, 0, 0, opt_size, 0x2022)
    o = 24
    struct.pack_into("<H", nt, o, 0x20B)
    struct.pack_into("<Q", nt, o + 24, 0x180000000)
    struct.pack_into("<I", nt, o + 56, 0x3000)
    struct.pack_into("<I", nt, o + 108, 16)          # NumberOfRvaAndSizes
    dd = o + 112
    struct.pack_into("<II", nt, dd + 0, at(expdir), 40)
    struct.pack_into("<II", nt, dd + 8, at(impdesc), 40)
    s = o + opt_size
    struct.pack_into("<8s", nt, s, b".rdata")
    struct.pack_into("<IIII", nt, s + 8, len(r), SEC, len(r), 0x400)
    struct.pack_into("<I", nt, s + 36, 0x40000040)
    data += nt

    data += b"\0" * (0x400 - len(data))
    data += r
    return bytes(data)


def cmd_selftest(_pe, _a):
    pe = PE(build_test_pe())
    ok = True

    def check(label, got, want):
        nonlocal ok
        good = got == want
        ok = ok and good
        print("  %-34s %-34r %s" % (label, got, "OK" if good else "FAIL want %r" % (want,)))

    print("parsing a synthetic PE64:")
    check("machine", MACHINE.get(pe.machine), "x64")
    check("is64", pe.is64, True)
    check("timestamp", "0x%08X" % pe.timestamp, "0x6A3EB6AD")
    check("image size", "0x%08X" % pe.image_size, "0x00003000")
    check("section name", pe.sections[0]["name"], ".rdata")

    imps = pe.imports()
    check("imported DLL", imps[0][0] if imps else None, "C3DDLL64.dll")
    check("imported symbol",
          imps[0][1][0][0] if imps and imps[0][1] else None,
          "?GetSize@C3D_FONT@@QEAAMXZ")

    modname, exps = pe.exports()
    check("export module", modname, "testplug.dll")
    check("export name", exps[0][0] if exps else None, "TsmPluginInit")
    check("export ordinal", exps[0][1] if exps else None, 1)

    names = sorted(set(m.group().decode("latin-1") for m in DECORATED.finditer(pe.d)))
    check("decorated names found", len(names), 2)
    check("longest decorated name", max(names, key=len),
          "?PrintLeftUnicodeNoArg@C3D_FONTMANAGER@@QEAAXPEAVC3D_FONT@@MMKPEB_W@Z")

    found = [s for _o, s in ascii_strings(pe.d, 6) if "panel_hook" in s]
    check("ascii string scan", bool(found), True)

    print()
    print("SELFTEST %s" % ("PASSED" if ok else "FAILED"))
    return 0 if ok else 1



# --------------------------------------------------------------- the easy path

# Names worth pulling out of a long import list without being asked.
UI_HINTS = ("print", "font", "draw", "panel", "text", "sprite", "texture",
            "render", "window", "string", "gui", "mouse", "icon")


def stamp_to_date(ts):
    try:
        d = datetime.datetime.fromtimestamp(ts, datetime.timezone.utc)
        return d.strftime("%Y-%m-%d %H:%M UTC")
    except (OverflowError, OSError, ValueError):
        return "unreadable"


def write(path, lines):
    with open(path, "w", encoding="utf-8") as f:
        f.write("\n".join(lines) + "\n")


class Collect(object):
    """Captures what the report functions print."""

    def __init__(self):
        self.lines = []

    def __enter__(self):
        self._old = sys.stdout
        sys.stdout = self
        return self

    def __exit__(self, *a):
        sys.stdout = self._old

    def write(self, s):
        self.lines.append(s)

    def flush(self):
        pass

    def text(self):
        return "".join(self.lines).splitlines()


class Args(object):
    def __init__(self, **kw):
        self.grep = None
        self.dll = None
        self.min = 8
        self.__dict__.update(kw)


def run_one(path, outdir):
    name = os.path.basename(path)
    print("=" * 72)
    print(name)
    print("=" * 72)

    try:
        with open(path, "rb") as f:
            data = f.read()
        pe = PE(data)
    except (OSError, PEError) as e:
        print("  skipped: %s" % e)
        print()
        return False

    print("  build timestamp : 0x%08X   (%s)" % (pe.timestamp, stamp_to_date(pe.timestamp)))
    print("  image size      : 0x%08X" % pe.image_size)
    print("  machine         : %s" % MACHINE.get(pe.machine, "0x%04X" % pe.machine))
    if pe.timestamp == 0x6A3EB6AD:
        print("                    ^ this is game build 1.1.1.9")

    imps = pe.imports()
    total = sum(len(t) for _d, t in imps)
    print("  imports         : %d function(s) from %d DLL(s)" % (total, len(imps)))
    for dll, th in sorted(imps, key=lambda x: -len(x[1])):
        print("      %-30s %d" % (dll, len(th)))

    modname, exps = pe.exports()
    named = [e for e in exps if e[0]]
    print("  exports         : %d (%d named)" % (len(exps), len(named)))
    for nm, ordinal, _rva in named[:12]:
        print("      %s" % nm)

    decorated = sorted(set(m.group().decode("latin-1")
                           for m in DECORATED.finditer(data)))
    print("  decorated C++ names found in the image: %d" % len(decorated))

    # the part that answers the question
    hits = []
    for dll, th in imps:
        for nm, _o in th:
            if nm and any(h in nm.lower() for h in UI_HINTS):
                hits.append((dll, nm))
    print()
    print("  LIKELY UI / ENGINE CALLS (%d):" % len(hits))
    if hits:
        for dll, nm in sorted(hits)[:40]:
            print("      %s" % nm)
        if len(hits) > 40:
            print("      ... %d more, see the imports file" % (len(hits) - 40))
    else:
        print("      none matched the hint list - read the full imports file,")
        print("      the plugin may resolve engine functions by address instead")

    base = os.path.join(outdir, name.replace(".", "_"))
    for label, fn, args in (
            ("summary", cmd_summary, Args()),
            ("imports", cmd_imports, Args()),
            ("exports", cmd_exports, Args()),
            ("symbols", cmd_symbols, Args()),
            ("strings", cmd_strings, Args(min=8))):
        with Collect() as c:
            try:
                fn(pe, args)
            except Exception as e:                      # noqa: BLE001
                print("error producing %s: %s" % (label, e))
        write("%s.%s.txt" % (base, label), c.text())

    print()
    print("  written: %s.{summary,imports,exports,symbols,strings}.txt" % os.path.basename(base))
    print()
    return True


def cmd_auto(_pe, _a):
    here = os.path.dirname(os.path.abspath(__file__))
    outdir = os.path.join(here, "peinfo-out")
    os.makedirs(outdir, exist_ok=True)

    me = os.path.basename(__file__).lower()
    files = []
    for ext in ("*.dll", "*.exe"):
        for p in glob.glob(os.path.join(here, ext)):
            if os.path.basename(p).lower() != me:
                files.append(p)
    files.sort()

    print()
    print("peinfo - scanning %s" % here)
    print()
    if not files:
        print("No .dll or .exe found in this folder.")
        print()
        print("Put reputation.dll next to this script and run it again.")
        print("Or name a file directly:   python peinfo.py imports C:\\path\\to\\reputation.dll")
        return 1

    done = sum(1 for p in files if run_one(p, outdir))
    print("-" * 72)
    print("%d file(s) read. Full dumps are in:" % done)
    print("    %s" % outdir)
    print()
    print("The two worth sending back are *_imports.txt and *_symbols.txt")
    return 0


# ----------------------------------------------------------------------- main

def main(argv=None):
    argv = sys.argv[1:] if argv is None else argv
    interactive = not argv and sys.stdin is not None and sys.stdin.isatty()

    if not argv:
        rc = cmd_auto(None, None)
        if interactive:
            try:
                input("Press Enter to close...")
            except (EOFError, KeyboardInterrupt):
                pass
        return rc

    p = argparse.ArgumentParser(
        prog="peinfo.py",
        description="Read a Windows PE without running it. "
                    "Run with no arguments to scan this folder.")
    sub = p.add_subparsers(dest="cmd")

    def add(name, fn, needs_file=True):
        q = sub.add_parser(name)
        if needs_file:
            q.add_argument("file")
        q.add_argument("--grep", default=None,
                       help="case-insensitive substring filter")
        q.set_defaults(fn=fn, needs_file=needs_file)
        return q

    add("summary", cmd_summary)
    q = add("imports", cmd_imports)
    q.add_argument("--dll", default=None, help="only this imported DLL")
    add("exports", cmd_exports)
    add("symbols", cmd_symbols)
    q = add("strings", cmd_strings)
    q.add_argument("--min", type=int, default=8)
    add("selftest", cmd_selftest, needs_file=False)
    add("auto", cmd_auto, needs_file=False)

    a = p.parse_args(argv)
    if not a.cmd:
        p.print_help()
        return 2
    if not a.needs_file:
        return a.fn(None, a) or 0

    try:
        with open(a.file, "rb") as f:
            data = f.read()
    except OSError as e:
        print("cannot read %s: %s" % (a.file, e), file=sys.stderr)
        return 1
    try:
        pe = PE(data)
    except PEError as e:
        print("%s is not a readable PE: %s" % (a.file, e), file=sys.stderr)
        return 1
    return a.fn(pe, a) or 0


if __name__ == "__main__":
    sys.exit(main())
