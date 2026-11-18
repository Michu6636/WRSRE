import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.ReferenceIterator;
import java.io.PrintWriter;
import java.io.FileWriter;

/**
 * Xrefs - enumerate cross-references to one or more addresses.
 *
 * Usage:  Xrefs.java <out_file> <addr1> [addr2 ...]
 *
 * Restored 2026-11-18 from the verbatim source quoted in
 * docs/private-logistics-runtime/RAW_EXPORTS_34D590.md section 10.4.
 */
public class Xrefs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        if (args.length < 2) {
            println("Usage: Xrefs.java <out_file> <addr1> [addr2 ...]");
            return;
        }
        String outFile = args[0];
        try (PrintWriter writer = new PrintWriter(new FileWriter(outFile))) {
            for (int i = 1; i < args.length; i++) {
                String addrStr = args[i];
                long addrVal = addrStr.toLowerCase().startsWith("0x")
                    ? Long.parseLong(addrStr.substring(2), 16)
                    : Long.parseLong(addrStr);
                Address addr = currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(addrVal);
                writer.println("XREFs to " + addrStr + ":");
                ReferenceIterator refs = currentProgram.getReferenceManager().getReferencesTo(addr);
                int count = 0;
                while (refs.hasNext()) {
                    Reference ref = refs.next();
                    Address fromAddr = ref.getFromAddress();
                    Function func = currentProgram.getFunctionManager().getFunctionContaining(fromAddr);
                    String funcName = (func != null) ? func.getName() : "NON_FUNCTION";
                    writer.println("  [" + ref.getReferenceType() + "] from " + fromAddr + " in " + funcName);
                    count++;
                }
                if (count == 0) {
                    writer.println("  (no references found)");
                }
                writer.println();
            }
        }
        println("XREFs written to " + outFile);
    }
}
