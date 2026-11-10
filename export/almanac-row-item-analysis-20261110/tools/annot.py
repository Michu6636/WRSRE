import sys,re,pefile
pe=pefile.PE('reputation.dll',fast_load=True)
IB=pe.OPTIONAL_HEADER.ImageBase
imp={}
pe.parse_data_directories(directories=[pefile.DIRECTORY_ENTRY['IMAGE_DIRECTORY_ENTRY_IMPORT']])
for e in pe.DIRECTORY_ENTRY_IMPORT:
    for i in e.imports:
        if i.name: imp[i.address]=f"{e.dll.decode()}!{i.name.decode()}"
def cstr(va):
    try:
        b=pe.get_data(va-IB,200)
    except Exception: return None
    j=b.find(b'\0')
    if j<3: return None
    s=b[:j]
    if all(32<=c<127 for c in s): return s.decode()
    return None
lo=int(sys.argv[1],16); hi=int(sys.argv[2],16)
for line in open('disasm.txt'):
    m=re.match(r'\s*([0-9a-f]+):\t(.*)',line)
    if not m: continue
    a=int(m.group(1),16)
    if a<lo or a>hi: continue
    txt=m.group(2).rstrip()
    ann=''
    t=re.search(r'# 0x([0-9a-f]+)',txt)
    if t:
        va=int(t.group(1),16)
        if va in imp: ann=f' ;; [{imp[va]}]'
        else:
            s=cstr(va)
            if s: ann=f' ;; "{s[:90]}"'
            elif 0x18008d000<=va<0x1800c3000 and 'call' in txt: ann=' ;; [import slot]'
    print(f"{a:x}:\t{txt}{ann}")
