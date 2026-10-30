#!/usr/bin/env python3
"""Bounded local reaching-definition audit of the supplied renderer export.
Requires capstone==5.0.7. Run from repo root. No executable, hook, simulation
writer or native lifecycle is validated. Calls are Windows x64 ABI-preserving;
only direct RBP/RSP slot writes are tracked (not arbitrary escaped aliases).
"""
from pathlib import Path
import re,collections,hashlib
from capstone import *
from capstone.x86 import *
p=Path('docs/private-logistics-runtime/RAW_EXPORTS_VEHICLE_DETAIL_RENDERER.md');raw=p.read_bytes();text=raw.decode('utf-16')
rows={}
for l in text.splitlines():
 m=re.match(r'^(140[0-9a-f]+):\s+((?:[0-9A-F]{2} )*[0-9A-F]{2})\s+(\S.*)$',l)
 if m:
  a=int(m[1],16)
  if 0x1407d5460<=a<=0x1407ebb7b:
   assert a not in rows
   rows[a]=(bytes.fromhex(m[2]),m[3])
md=Cs(CS_ARCH_X86,CS_MODE_64);md.detail=True
inst={}
for a,(b,t) in rows.items():
 ds=list(md.disasm(b,a));assert len(ds)==1 and ds[0].size==len(b),(hex(a),t)
 inst[a]=ds[0]
addresses=sorted(inst)
assert all(a+inst[a].size==b for a,b in zip(addresses,addresses[1:]))
print('SOURCE_SHA256',hashlib.sha256(raw).hexdigest(),'instructions',len(inst),'contiguous_decode=PASS')
edges={};indirect=[];outside=[]
for a,i in inst.items():
 nxt=a+i.size;succ=[]
 if i.group(CS_GRP_JUMP):
  if i.operands[0].type==X86_OP_IMM:succ.append(i.operands[0].imm)
  else:indirect.append(a)
  if i.mnemonic!='jmp' and nxt in inst:succ.append(nxt)
 elif not i.group(CS_GRP_RET) and nxt in inst:succ.append(nxt)
 edges[a]=[v for v in succ if v in inst]
 outside.extend((a,v) for v in succ if v not in inst)
print('INDIRECT_JUMPS',[hex(x) for x in indirect],'OUTSIDE_BRANCHES',outside)
# Must-provenance lattice, only for exported instructions: ROOT=1, OTHER/unknown=2.
# Stack slot RBP+40 was used to save R15; current RSP+150 aliases it (RBP-RSP=110).
# Prologue: LEA RBP,[RSP-2EB0]; allocation 2FC0; no body RSP adjustment.
# Windows x64 direct/indirect calls preserve R15/RBP; arbitrary callees and escaped
# stack aliases are not analyzed, so this is a local register-reaching-def audit.
families=[{X86_REG_R15,X86_REG_R15D,X86_REG_R15W,X86_REG_R15B},
          {X86_REG_R12,X86_REG_R12D,X86_REG_R12W,X86_REG_R12B}]
# State: (R15, slot40, R12, slot08); 1=selected root/window, 2=other/unknown.
def xfer(i,state):
 result=list(state)
 written=i.regs_access()[1]
 for regidx,(family,slot,restore) in enumerate(zip(families,[0x40,8],[X86_REG_R15,X86_REG_R12])):
  pos=regidx*2
  if any(r in family for r in written):
   result[pos]=2
   if i.mnemonic=='mov' and i.operands[0].type==X86_OP_REG and i.operands[0].reg==restore:
    o=i.operands[1]
    if i.address in (0x1407d5550,0x1407d54fd):result[pos]=1
    elif o.type==X86_OP_MEM and o.mem.base==X86_REG_RBP and o.mem.disp==slot:result[pos]=state[pos+1]
 for o in i.operands:
  if o.type!=X86_OP_MEM or not o.access&CS_AC_WRITE:continue
  disp=o.mem.disp-(0x110 if o.mem.base==X86_REG_RSP else 0)
  for pos,slot,reg in [(0,0x40,X86_REG_R15),(2,8,X86_REG_R12)]:
   if o.mem.base in (X86_REG_RBP,X86_REG_RSP) and disp<slot+8 and disp+o.size>slot:
    result[pos+1]=2
    if disp==slot and o.size==8 and i.mnemonic=='mov':
     if i.operands[-1].type==X86_OP_REG and i.operands[-1].reg==reg:result[pos+1]=state[pos]
     if i.address==0x1407d5500 and slot==8:result[pos+1]=1
 return tuple(result)
states={addresses[0]:(2,2,2,2)};todo=collections.deque([addresses[0]])
while todo:
 a=todo.popleft();out=xfer(inst[a],states[a])
 for v in edges[a]:
  old=states.get(v,(0,0,0,0));merged=tuple(x|y for x,y in zip(old,out))
  if merged!=old:states[v]=merged;todo.append(v)
points=[0x1407d9ff3,0x1407da049,0x1407e6904,0x1407e8143,0x1407e815d,0x1407e8173,0x1407e8438,0x1407e8ed2,0x1407e9273]
for a in points:
 print('LOCAL_R15_PROVENANCE',hex(a),states.get(a),'ROOT_ONLY' if states.get(a,(0,))[0]==1 else 'UNRESOLVED')
 assert states[a][0]==1
for a in [0x1407d9feb,0x1407da036]:
 print('LOCAL_R12_PROVENANCE',hex(a),states[a],'WINDOW_ONLY' if states[a][2]==1 else 'UNRESOLVED')
 assert states[a][2]==1
for i in inst.values():
 if any(o.type==X86_OP_MEM and o.mem.base==X86_REG_R15 and o.mem.disp in (0x610,0x638,0x650,0x680,0x688,0x698) for o in i.operands):
  if i.address>=0x1407e6904:
   print('FIELD',hex(i.address),i.bytes.hex(' '),i.mnemonic,i.op_str)
fields={0x610,0x638,0x650,0x680,0x688,0x698}
writes=[hex(i.address) for i in inst.values() if any(
 o.type==X86_OP_MEM and o.mem.base==X86_REG_R15 and
 o.mem.disp in fields and o.access&CS_AC_WRITE for o in i.operands)]
print('DIRECT_R15_FIELD_WRITES',writes,'(callee/alias writers not analyzed)')
