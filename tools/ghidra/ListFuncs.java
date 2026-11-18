import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.listing.FunctionIterator;
import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * ListFuncs - enumerate functions overlapping an address range.
 *
 * Usage:  ListFuncs.java <out_file> <start_addr> <end_addr>
 * Addresses are FILE-BASED (the project loads SOVIET64.exe at 0x140000000),
 * so pass 0x140 + RVA, e.g. 0x140780000..0x1407A0000 for the panel family.
 *
 * Writes: 0x<entry>  size <n>  <name>   - one line per function, ascending.
 *
 * Restored 2026-11-18 from the verbatim source quoted in
 * docs/private-logistics-runtime/RAW_EXPORTS_34D590.md section 10.1.
 */
public class ListFuncs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 3) {
            println("Usage: ListFuncs.java <out_file> <start_addr> <end_addr>");
            return;
        }
        long start = parseHex(a[1]);
        long end   = parseHex(a[2]);

        Address base = currentProgram.getAddressFactory()
                                   .getDefaultAddressSpace().getAddress(start);
        FunctionIterator it = currentProgram.getFunctionManager().getFunctions(base, true);

        int n = 0;
        try (PrintWriter w = new PrintWriter(new FileWriter(a[0]))) {
            w.printf("; functions from %s to 0x%X (file-based addresses)%n", a[1], end);
            while (it.hasNext()) {
                Function f = it.next();
                if (f.isExternal()) continue;
                Address e = f.getEntryPoint();
                if (e.getOffset() > end) break;
                long size = (f.getBody() != null) ? f.getBody().getNumAddresses() : -1;
                w.printf("0x%X  size %6d  %s%n", e.getOffset(), size, f.getName());
                n++;
            }
        }
        println("ListFuncs: " + n + " function(s) written to " + a[0]);
    }

    private static long parseHex(String s) {
        s = s.trim().toLowerCase();
        if (s.startsWith("0x")) s = s.substring(2);
        return Long.parseLong(s, 16);
    }
}
