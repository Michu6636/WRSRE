import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * DecompAt - decompile one or more functions by address.
 *
 * Usage:  DecompAt.java <out_file.c> <preferred_VA1> [preferred_VA2...]
 * Addresses are preferred-image virtual addresses, not file offsets or RVAs.
 * For the historical project base 0x140000000, add the RVA numerically:
 * 0x140000000 + 0x4461F0 = 0x1404461F0. Verify the actual project base.
 *
 * Uses a 120s per-function decompiler budget (same as the UI).
 * Writes C pseudo-code to <out_file>, overwriting.
 */
public class DecompAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) {
            println("Usage: DecompAt.java <out_file.c> <preferred_VA1> [preferred_VA2...]");
            return;
        }
        String outFile = a[0];
        DecompInterface decompiler = new DecompInterface();
        decompiler.openProgram(currentProgram);
        // timeout is passed to decompileFunction below
        try (PrintWriter w = new PrintWriter(new FileWriter(outFile))) {
            for (int i = 1; i < a.length; i++) {
                long addrVal = parseHex(a[i]);
                Function f = currentProgram.getFunctionManager()
                        .getFunctionAt(toAddr(addrVal));
                if (f == null) {
                    // try containing function
                    f = currentProgram.getFunctionManager()
                            .getFunctionContaining(toAddr(addrVal));
                }
                if (f == null) {
                    w.printf("// No function found at 0x%X%n", addrVal);
                    println("DecompAt: no function at 0x" + Long.toHexString(addrVal));
                    continue;
                }
                DecompileResults results = decompiler
                        .decompileFunction(f, 120, new ConsoleTaskMonitor());
                if (results.decompileCompleted()) {
                    w.printf("// ============================================================%n");
                    w.printf("// Function: %s at 0x%X%n", f.getName(), f.getEntryPoint().getOffset());
                    w.printf("// Target address checked: 0x%X%n", addrVal);
                    w.printf("// ============================================================ %n%n");
                    w.println(results.getDecompiledFunction().getC());
                    w.printf("%n%n");
                    println("DecompAt: decompiled " + f.getName() + " ("
                            + f.getBody().getNumAddresses() + " addr)");
                } else {
                    w.printf("// Decompilation failed/timed out at 0x%X%n", addrVal);
                    println("DecompAt: FAILED for 0x" + Long.toHexString(addrVal));
                }
            }
        }
        println("DecompAt: written to " + outFile);
    }

    private static long parseHex(String s) {
        s = s.trim().toLowerCase();
        if (s.startsWith("0x")) s = s.substring(2);
        return Long.parseLong(s, 16);
    }
}
