import ghidra.program.model.mem.Memory;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Program;
import java.io.FileWriter;
import java.io.PrintWriter;

public class dump_wide_strings extends ghidra.app.script.GhidraScript {
    public void run() throws Exception {
        Address start = currentProgram.getAddressFactory().getAddress("0x1408B51A0");
        Address end = currentProgram.getAddressFactory().getAddress("0x1408B5320");
        Memory mem = currentProgram.getMemory();
        
        try (FileWriter fw = new FileWriter("scratch/wide_strings_decoded.txt");
             PrintWriter pw = new PrintWriter(fw)) {
            
            Address addr = start;
            while (addr.compareTo(end) < 0) {
                try {
                    // Read raw bytes and decode as UTF-16LE
                    byte[] buf = new byte[2];
                    int read = mem.getBytes(addr, buf);
                    if (read == 2) {
                        char c = (char)((buf[1] & 0xFF) << 8 | (buf[0] & 0xFF));
                        if (c >= 32 && c <= 126) {
                            // Found printable char, try to read full string
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
                            if (sb.length() > 2) {
                                pw.println(String.format("0x%X : L\"%s\"", addr.getOffset(), sb.toString()));
                                addr = a.add(2);
                                continue;
                            }
                        }
                    }
                } catch (Exception e) {
                }
                addr = addr.add(2);
            }
        }
        println("Done");
    }
}