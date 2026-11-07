import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.mem.Memory;
import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * DisasmMany - disassemble (instruction-by-instruction) a byte range.
 *
 * Usage:  DisasmMany.java <out_file.asm> <start_VA:end_VA>
 * Addresses are preferred-image virtual addresses, not file offsets or RVAs.
 * Numerically add the RVA to the actual project image base. The range is
 * inclusive and may be given as start:end (colon-separated) or start end.
 *
 * Writes: one line per instruction: address + raw bytes + mnemonic.
 */
public class DisasmMany extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) {
            println("Usage: DisasmMany.java <out_file.asm> <start_VA:end_VA>");
            return;
        }
        String outFile = a[0];
        String startS, endS;
        if (a[1].contains(":")) {
            String[] p = a[1].split(":");
            startS = p[0];
            endS = p[1];
        } else {
            startS = a[1];
            endS = (a.length >= 3) ? a[2] : a[1];
        }
        long start = parseHex(startS);
        long end = parseHex(endS);
        Address sa = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(start);
        Address ea = currentProgram.getAddressFactory()
                .getDefaultAddressSpace().getAddress(end);

        Function fn = currentProgram.getFunctionManager()
                .getFunctionContaining(sa);
        String fnName = (fn != null)
                ? String.format("%s@0x%X", fn.getName(), fn.getEntryPoint().getOffset())
                : "NON_FUNCTION";

        int n = 0;
        Memory mem = currentProgram.getMemory();
        try (PrintWriter w = new PrintWriter(new FileWriter(outFile))) {
            w.printf("; Range: 0x%X to 0x%X  (containing fn: %s)%n", start, end, fnName);
            InstructionIterator it = currentProgram.getListing()
                    .getInstructions(sa, true);
            while (it.hasNext()) {
                Instruction ins = it.next();
                if (ins.getAddress().compareTo(ea) > 0) break;
                // format: address:  bytes  mnemonic
                byte[] b = ins.getBytes();
                StringBuilder sb = new StringBuilder();
                for (int j = 0; j < b.length; j++) {
                    if (j > 0) sb.append(" ");
                    sb.append(String.format("%02X", b[j] & 0xFF));
                }
                w.printf("%s:  %-40s  %s%n",
                        ins.getAddress(), sb, ins);
                n++;
            }
            w.printf("; %d instruction(s)%n", n);
        }
        println("DisasmMany: " + n + " instr(s) -> " + outFile);
    }

    private static long parseHex(String s) {
        s = s.trim().toLowerCase();
        if (s.startsWith("0x")) s = s.substring(2);
        return Long.parseLong(s, 16);
    }
}
