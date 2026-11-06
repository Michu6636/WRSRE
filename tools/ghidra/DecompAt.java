import ghidra.app.script.GhidraScript;
import ghidra.program.model.listing.Function;
import ghidra.app.decompiler.DecompileInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.util.task.ConsoleTaskMonitor;
import java.io.FileWriter;
import java.io.PrintWriter;

/**
 * DecompAt - decompile one or more functions by address.
 *
 * Usage:  DecompAt.java <out_file.c> <0x140RVA1> [0x140RVA2...]
 * Addresses are FILE-BASED (the project loads SOVIET64.exe at 0x140000000),
 * so pass 0x140 + RVA, e.g. 0x1404461f0 for RVA 0x4461F0.
 *
 * Uses a 120s per-function decompiler budget (same as the UI).
 * Writes C pseudo-code to <out_file>, overwriting.
 */
public class DecompAt extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        if (a.length < 2) {
            println("Usage: DecompAt.java <out_file.c> <0x140RVA1> [0x140RVA2...]");
            return;
        }
        String outFile = a[0];
        DecompileInterface decompiler = new DecompileInterface();
        decompiler.openProgram(currentProgram);
        // set decompiler timeout to match UI budget
        decompiler.setDecompileTimeoutSeconds(120);
        try (PrintWriter w = new PrintWriter(new FileWriter(outFile))) {
            for (int i = 1; i < a.length; i++) {
                long addrVal = parseHex(a[i]);
                Function f = currentProgram.getFunctionManager()
                        .getFunctionAt(addr(addrVal));
                if (f == null) {
                    // try containing function
                    f = currentProgram.getFunctionManager()
                            .getFunctionContaining(addr(addrVal));
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
                    println("DecompAt: FAILED for 0x" + Long.toHexString(addrVal)
                            + " (" + results.getDecompileStatus() + ")");
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
