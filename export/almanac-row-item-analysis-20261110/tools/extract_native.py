import sys,re
R='/home/user/WRSRE'
E=R+'/export/almanac-row-item-analysis-20261110'
def ext(path,lo,hi):
    out=[]
    lo=int(lo,16); hi=int(hi,16)
    for line in open(R+'/'+path,encoding='utf-8',errors='replace'):
        m=re.match(r'([0-9a-fA-F]{9}):',line)
        if m and lo<=int(m.group(1),16)<=hi:
            out.append(line.rstrip('\n'))
    return out
jobs=[
("N01_host_dispatch_trade_0x14031edaf.txt","N01 host section dispatch: normal vs alternate row handler, AE58 write","export/140317db0_full_disasm.asm","14031edaf","14031ee18","CMP [R14+0x10398],R15B; JZ -> 0x14031EE10 -> CALL 0x140361BB0; else CALL 0x140373EA0 then AE58 = Y - AE50"),
("N02_normal_row_mapping_0x1403741a2.txt","N02 normal row handler: index remap and skip conditions","export/140373ea0_1403829fd_disasm.asm","1403741a2","1403741f6","i==2 -> count-1; i>2 -> i-1; skips on game+0x578 and game+0x5e0"),
("N03_normal_row_loop_bound_0x1403743b6.txt","N03 normal row handler: loop increment and bound","export/140373ea0_1403829fd_disasm.asm","1403743a8","1403743e0","loop bound EDI < count (all entries drawn)"),
("N04_normal_row_click_write_0x140374347.txt","N04 normal row handler: click write of selected index","export/140373ea0_1403829fd_disasm.asm","140374347","140374366","MOV [RSI+0xC65C],EBX (mapped vector index)"),
("N05_alt_row_count_skip_0x140361f3a.txt","N05 alternate row handler: count, loop start, skips, direct index","export/ch_tab23_full.asm","140361f3a","140361fe9","ESI used directly as index; bound count-1 (see N06)"),
("N06_alt_row_loop_bound_0x140362208.txt","N06 alternate row handler: X advance, loop bound count-1","export/ch_tab23_full.asm","14036220a","140362245","INC ESI; DEC EAX; CMP ESI,EAX; JL -> entries 0..count-2 only"),
("N07_alt_row_click_write_0x140362193.txt","N07 alternate row handler: click write of selected index","export/ch_tab23_full.asm","140362193","1403621a5","MOV [R14+0xC65C],ESI"),
("N08_alt_row_ae58_0x140361f12.txt","N08 alternate row handler: AE58 write","export/ch_tab23_full.asm","140361f12","140361f12","MOVSS [R14+0xAE58],XMM15 (before skip tests)"),
]
for out,title,src,lo,hi,purpose in jobs:
    lines=ext(src,lo,hi)
    hdr=[f"# {title}",f"# Source: {src} (preserved repo listing; game binary not in workspace, hash unverified)",f"# Range: {lo}-{hi}","# Static listing excerpt, copied verbatim by address. Nothing executed.",f"# Purpose: {purpose}",""]
    open(E+'/'+out,'w',encoding='utf-8').write('\n'.join(hdr+lines)+'\n')
    print(out,len(lines),'lines')
