import ghidra.program.model.mem.Memory;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Program;
import java.io.FileWriter;
import java.io.PrintWriter;

public class dump_all_strings extends ghidra.app.script.GhidraScript {
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        Address start = mem.getMinAddress();
        Address end = mem.getMaxAddress();
        int count = 0;
        
        try (FileWriter fw = new FileWriter("C:\\WRSRE\\scratch\\all_strings_decoded.txt");
             PrintWriter pw = new PrintWriter(fw)) {
            
            Address addr = start;
            while (addr.compareTo(end) < 0) {
                try {
                    byte[] buf = new byte[2];
                    int read = mem.getBytes(addr, buf);
                    if (read == 2) {
                        char c = (char)((buf[1] & 0xFF) << 8 | (buf[0] & 0xFF));
                        if (c >= 32 && c <= 126) {
                            StringBuilder sb = new StringBuilder();
                            Address a = addr;
                            while (a.compareTo(end) < 0) {
                                byte[] buf2 = new byte[2];
                                int r = mem.getBytes(a, buf2);
                                if (r == 2) {
                                    char c2 = (char)((buf2[1] & 0xFF) << 8 | (buf2[0] & 0xFF));
                                    if (c2 == 0) break;
                                    if (c2 >= 32 && c2 <= 126) {
                                        sb.append(c2);
                                    } else {
                                        break;
                                    }
                                } else break;
                                a = a.add(2);
                            }
                            String ws = sb.toString();
                            if (ws.length() > 3) {
                                String lower = ws.toLowerCase();
                                if (lower.contains("skd") || lower.contains("rttn") || 
                                    lower.contains("road") || lower.contains("vehicle") ||
                                    lower.contains("fuel") || lower.contains("gauge") ||
                                    lower.contains("speed") || lower.contains("rpm") ||
                                    lower.contains("odometer") || lower.contains("litre") ||
                                    lower.contains("liter") || lower.contains("kmh") ||
                                    lower.contains("mph") || lower.contains("tank") ||
                                    lower.contains("truck") || lower.contains("open")) {
                                    pw.println(String.format("0x%X : L\"%s\"", addr.getOffset(), ws.replace("\n", "\\n").replace("\r", "\\r").replace("\t", "\\t")));
                                    count++;
                                }
                            }
                            addr = a.add(2);
                            continue;
                        }
                    }
                } catch (Exception e) {
                }
                addr = addr.add(2);
            }
            pw.println("Total matches: " + count);
        }
        println("Done, found " + count);
    }
}