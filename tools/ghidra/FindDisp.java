import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.mem.Memory;
import ghidra.program.model.mem.MemoryAccessException;
import ghidra.program.model.mem.MemoryBlock;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;

/**
 * FindDisp - find every instruction that uses a given 32-bit displacement.
 *
 * A struct field is named by a displacement, not by an address, so Xrefs.java
 * cannot find its users. The sidebar caches its tab textures at
 * game+0xB650 (8-byte stride); the function that *draws* the tab bar is the one
 * that READS that field. Scanning for the displacement finds it.
 *
 * Searches blocks byte-wise and reports the instruction containing each hit.
 * Coincidental bytes within an instruction, immediates and other operands can
 * match; inspect the instruction, receiver and path before naming a field.
 *
 * Usage:  FindDisp.java <out_file> <disp_hex> [block_name_substring]
 *   disp_hex   the displacement, e.g. b650 for game+0xB650
 *   block      optional, default .text - limits the scan to one block
 *
 * Output addresses are project virtual addresses, not raw file offsets.
 * The input is a displacement, not a function address or RVA.
 */
public class FindDisp extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) {
            println("Usage: FindDisp.java <out_file> <disp_hex> [block]");
            return;
        }
        String hex = a[1].trim().toLowerCase().replace("0x", "");
        // The argument is a numeric displacement; a disp32 is stored
        // LITTLE-endian, so 0xB650 appears as 50 B6 00 00.
        long val = Long.parseLong(hex, 16);
        if (val < 0 || val > 0xFFFFFFFFL) {
            println("displacement must fit in 32 bits");
            return;
        }
        byte[] pat = new byte[4];
        pat[0] = (byte) (val & 0xFF);
        pat[1] = (byte) ((val >> 8) & 0xFF);
        pat[2] = (byte) ((val >> 16) & 0xFF);
        pat[3] = (byte) ((val >> 24) & 0xFF);
        String wantBlock = (a.length >= 3) ? a[2] : ".text";
        println("FindDisp: displacement bytes "
            + String.format("%02X %02X %02X %02X", pat[0], pat[1], pat[2], pat[3])
            + " in " + wantBlock);

        Memory mem = currentProgram.getMemory();
        List<MemoryBlock> blocks = new ArrayList<>();
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            if (!b.getName().contains(wantBlock)) continue;
            blocks.add(b);
        }

        int hits = 0, funcs = 0;
        try (PrintWriter w = new PrintWriter(new FileWriter(a[0]))) {
            w.printf("; displacement %s in %s%n", hex, wantBlock);
            for (MemoryBlock b : blocks) {
                long base = b.getStart().getOffset();
                long size = b.getSize();
                if (size > 256L * 1024 * 1024) continue;
                byte[] buf = new byte[(int) size];
                try {
                    mem.getBytes(b.getStart(), buf);
                } catch (MemoryAccessException e) {
                    continue;
                }
                for (int i = 0; i + 4 <= buf.length; i++) {
                    if (buf[i] != pat[0] || buf[i + 1] != pat[1]
                        || buf[i + 2] != pat[2] || buf[i + 3] != pat[3]) continue;
                    long addrVal = base + i;
                    Address addr = currentProgram.getAddressFactory()
                        .getDefaultAddressSpace().getAddress(addrVal);
                    // The displacement bytes sit INSIDE an instruction
                    // (after the opcode/ModRM), so ask for the containing
                    // instruction, not one starting exactly there.
                    Instruction ins = currentProgram.getListing()
                        .getInstructionContaining(addr);
                    if (ins == null) continue;   // padding or data
                    Function f = currentProgram.getFunctionManager()
                        .getFunctionContaining(ins.getAddress());
                    String fn = (f != null)
                        ? String.format("%s@0x%X", f.getName(),
                            f.getEntryPoint().getOffset())
                        : "NON_FUNCTION";
                    w.printf("0x%X  in %-28s  %s%n", ins.getAddress().getOffset(),
                        fn, ins);
                    hits++;
                }
            }
            w.printf("; %d hit(s)%n", hits);
        }
        println("FindDisp: " + hits + " hit(s) written to " + a[0]);
    }
}
