# Independent decode of the supplied C3DDLL64 excerpts

The two `*_objdump.asm` files decode the 512-byte hex excerpts preserved, unchanged, in [`../c3ddll64-exports-20261109/`](../c3ddll64-exports-20261109/) from commit `1ba3585`. The full DLL, its export table and its claimed hash are not available in this checkout. These files therefore demonstrate only how the supplied bytes decode, **assuming** the report correctly associates each excerpt with its claimed RVA and image base.

The saved objdump listings have trailing whitespace removed; the supplied hex remains unchanged. Using GNU objdump 2.40, reconstruct bytes from the whitespace-separated hex in each source file and disassemble as x86-64 binary, adjusting the VMA to the report's base `0x180000000` plus RVA:

```sh
python3 - <<'PY'
from pathlib import Path
p = Path('export/almanac-evidence/c3ddll64-exports-20261109')
for name in ('Collision', 'Draw'):
    raw = bytes.fromhex((p / f'{name}_raw_hex.txt').read_text())
    assert len(raw) == 512
    Path(f'/tmp/wrsre_{name}.bin').write_bytes(raw)
PY
objdump -D -b binary -m i386:x86-64 -M intel --adjust-vma=0x1800bc690 /tmp/wrsre_Collision.bin
objdump -D -b binary -m i386:x86-64 -M intel --adjust-vma=0x1800baf40 /tmp/wrsre_Draw.bin
```

The later `Draw_tail_objdump.asm` decodes the 1,280 bytes from [`../c3ddll64-draw-complete-20261109/draw_tail3_raw.asm`](../c3ddll64-draw-complete-20261109/draw_tail3_raw.asm), shared in commit `39f7452`. Each line contains a 16-byte hexadecimal dump starting at `0x1800bb400`. Parse its first sixteen hex pairs per line, concatenate in address order, and run `objdump -D -b binary -m i386:x86-64 -M intel --adjust-vma=0x1800bb400` on the resulting bytes. Their SHA-256 is `acef5fc3c2edf8d2b473b05fa4cb04b99ab0a987453c95b5e74ec9b6e2e82de5`. This decode assumes the reported starting address and a valid instruction boundary; the first instruction and bytes at that address match the earlier Ghidra output. It includes code beyond Draw's return.

Reconstructed byte SHA-256: Collision `5a322952be1a8398becd58e1858e7af22c35406e3619c3d8d415458137545779`; Draw `bef18c14674d3709da7527c45b628c9d3cb2a08ddfac97a25434bbd0260e8ae0`. Objdump decodes linearly and cannot distinguish all control flow or data. The last output line of each file is a truncated instruction at the 512-byte boundary, not a complete function or a valid standalone instruction. Branch targets and helper bodies outside the excerpts are not present.
