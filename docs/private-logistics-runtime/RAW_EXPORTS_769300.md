# Raw Ghidra Exports — FUN_140769300

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON  
**All addresses are RVAs** (file-based = 0x140 + RVA)  
**Generated:** 2026-10-29 via scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21)

---

## 1. Full Decompilation of FUN_140769300 (0x140769300)

```c
// ============================================================
// Function: FUN_140769300 at 140769300
// Target address checked: 0x140769300
// ============================================================

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float FUN_140769300(longlong param_1,float param_2,float param_3,longlong param_4)

{
  longlong lVar1;
  longlong lVar2;
  longlong lVar3;
  longlong lVar4;
  int iVar5;
  bool bVar6;
  float fVar7;
  float fVar8;
  bool bVar9;
  undefined4 *puVar10;
  undefined8 *puVar11;
  wchar_t *pwVar12;
  undefined8 uVar13;
  char *pcVar14;
  int iVar15;
  ulonglong uVar16;
  ulonglong uVar17;
  uint uVar18;
  ulonglong uVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  undefined1 auStackY_178 [32];
  float local_138 [4];
  undefined4 local_128;
  float local_120;
  float local_11c;
  float local_118;
  float local_114;
  undefined8 local_110;
  undefined4 local_100;
  int iStack_fc;
  int iStack_f8;
  int iStack_f4;
  undefined4 local_f0;
  int iStack_ec;
  int iStack_e8;
  int iStack_e4;
  ulonglong local_e0;
  
  local_e0 = DAT_140992048 ^ (ulonglong)auStackY_178;
  lVar1 = *(longlong *)(param_4 + 0x240);
  if ((*(longlong *)(lVar1 + 0x318) == 0) ||
     (*(int *)(*(longlong *)(lVar1 + 0x318) + 0x360) != 0x2d)) {
    bVar6 = false;
  }
  else {
    bVar6 = true;
  }
  local_11c = param_2;
  local_114 = param_3;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5bd);
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,param_2,param_3,0xff000000,
             L"%s");
  fVar7 = DAT_14090a9a0;
  fVar20 = DAT_140992088 * DAT_14090a968;
  lVar2 = *DAT_140994208;
  param_3 = param_3 + DAT_140992088 * DAT_14090a9a0;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b8);
  puVar10 = (undefined4 *)(**(code **)(lVar2 + 0x28))(DAT_140994208,&local_f0,0,param_3);
  local_100 = *puVar10;
  iStack_fc = puVar10[1];
  iVar5 = puVar10[2];
  iStack_f4 = puVar10[3];
  lVar2 = *DAT_140994208;
  iStack_f8 = iVar5;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b9);
  puVar10 = (undefined4 *)(**(code **)(lVar2 + 0x28))(DAT_140994208,&local_f0,0,0);
  local_f0 = *puVar10;
  iStack_ec = puVar10[1];
  iStack_e8 = puVar10[2];
  iStack_e4 = puVar10[3];
  iVar15 = iStack_e8;
  if (iStack_e8 < iVar5) {
    iVar15 = iVar5;
  }
  fVar23 = (float)iVar15;
  local_118 = fVar23;
  puVar11 = (undefined8 *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  local_110 = *puVar11;
  FUN_14076a3a0(local_110,lVar1,&local_120,local_138);
  fVar25 = param_2 + fVar20 + fVar23;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b8);
  C3D_FONTMANAGER::PrintRightUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,
             fVar25 - (float)DAT_14099528c,param_3,0xff000000,L"%s");
  fVar20 = local_110._4_4_;
  if (((((fVar25 - fVar23 < (float)local_110) && ((float)local_110 < fVar25)) &&
       (DAT_1409c47a0 <= local_110._4_4_)) &&
      ((local_110._4_4_ <= DAT_1409c47a4 &&
       (fVar23 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060),
       (float)iStack_fc < fVar20 - fVar23)))) &&
     (fVar23 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060),
     fVar20 - fVar23 < (float)iStack_f4)) {
    pwVar12 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5bb);
    FUN_140015900(param_1 + 0xd5a0,pwVar12);
  }
  fVar23 = DAT_14090a828;
  fVar25 = fVar25 + DAT_140992088 * DAT_14090a828;
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  lVar2 = param_1 + 0xadb0;
  local_128 = 0;
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * fVar7;
  _DAT_1409be2f0 = CONCAT44(param_3 + DAT_140992088 * fVar23,fVar25 + DAT_140992088 * fVar23);
  _DAT_1409be2ec = _DAT_1409be2e8;
  if ((*(int *)(param_1 + 0xada8) == -1) ||
     (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
    FUN_1404461f0(lVar2);
  }
  uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar9 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13,0,0);
  fVar8 = DAT_140909f58;
  uVar17 = 0;
  fVar24 = local_120;
  if (bVar9) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if ((DAT_140a54e91 != '\0') && (*(char *)(param_4 + 0x20) != '\0')) {
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar9 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar9)) {
        pcVar14 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar14,true,fVar8,(char *)0x0);
      }
      fVar24 = local_120 + DAT_14090b288;
      if (fVar24 < 0.0) {
        fVar24 = 0.0;
      }
      uVar16 = uVar17;
      uVar19 = uVar17;
      if (*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3 != 0) {
        do {
          lVar3 = *(longlong *)(uVar16 + *(longlong *)(lVar1 + 0xd38));
          lVar4 = *(longlong *)(*(longlong *)(lVar3 + 8) + 0x318);
          if ((lVar4 == 0) || (*(int *)(lVar4 + 0x360) != 0x14)) {
            *(float *)(*(longlong *)(lVar3 + 0x10) + 4) = fVar24;
          }
          uVar18 = (int)uVar19 + 1;
          uVar16 = uVar16 + 8;
          uVar19 = (ulonglong)uVar18;
        } while ((ulonglong)(longlong)(int)uVar18 <
                 (ulonglong)(*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3));
      }
    }
  }
  else {
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar8,fVar8,0.0,true);
  fVar22 = DAT_14090a6a8;
  fVar25 = fVar25 + DAT_140992088 * DAT_14090a9cc;
  fVar21 = fVar25 - DAT_140992088 * DAT_14090a6a8;
  if (fVar24 < 0.0) {
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,fVar21,param_3,0xff000000
               ,L"> ? %%");
  }
  else {
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,fVar21,param_3,0xff000000
               ,L"> %d %%");
  }
  fVar21 = DAT_140992088 * DAT_14090aa0c;
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  DAT_1409be2f8 = 0;
  local_128 = 0;
  _DAT_1409be2e8 = DAT_140992088 * fVar7;
  _DAT_1409be2f0 =
       CONCAT44(DAT_140992088 * fVar23 + param_3,DAT_140992088 * DAT_14090a928 + fVar25 + fVar21);
  _DAT_1409be2ec = _DAT_1409be2e8;
  if ((*(int *)(param_1 + 0xada8) == -1) ||
     (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
    FUN_1404461f0(lVar2);
  }
  uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar9 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13,0,0);
  fVar7 = DAT_140909c78;
  if (bVar9) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if ((DAT_140a54e91 != '\0') && (*(char *)(param_4 + 0x20) != '\0')) {
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar9 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar9)) {
        pcVar14 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar14,true,fVar8,(char *)0x0);
      }
      if (fVar24 < 0.0) {
        fVar24 = 0.0;
      }
      fVar25 = fVar24 + fVar7;
      if (DAT_140909efc < fVar24 + fVar7) {
        fVar25 = DAT_140909efc;
      }
      uVar16 = uVar17;
      uVar19 = uVar17;
      if (*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3 != 0) {
        do {
          lVar3 = *(longlong *)(uVar16 + *(longlong *)(lVar1 + 0xd38));
          lVar4 = *(longlong *)(*(longlong *)(lVar3 + 8) + 0x318);
          if ((lVar4 == 0) || (*(int *)(lVar4 + 0x360) != 0x14)) {
            *(float *)(*(longlong *)(lVar3 + 0x10) + 4) = fVar25;
          }
          uVar18 = (int)uVar19 + 1;
          uVar16 = uVar16 + 8;
          uVar19 = (ulonglong)uVar18;
        } while ((ulonglong)(longlong)(int)uVar18 <
                 (ulonglong)(*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3));
      }
    }
  }
  else {
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar8,fVar8,0.0,true);
  param_3 = param_3 + DAT_140992088 * DAT_14090a9cc;
  fVar25 = DAT_140992088 * DAT_14090a968 + local_11c + local_118;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b9);
  C3D_FONTMANAGER::PrintRightUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,
             fVar25 - (float)DAT_14099528c,param_3,0xff000000,L"%s");
  if ((((fVar25 - local_118 < (float)local_110) && ((float)local_110 < fVar25)) &&
      (DAT_1409c47a0 <= fVar20)) &&
     (((fVar20 <= DAT_1409c47a4 &&
       (fVar24 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060),
       (float)iStack_ec + param_3 < fVar20 - fVar24)) &&
      (fVar24 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060),
      fVar20 - fVar24 < (float)iStack_e4 + param_3)))) {
    pwVar12 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5bc);
    FUN_140015900(param_1 + 0xd5a0,pwVar12);
  }
  fVar25 = fVar25 + DAT_140992088 * fVar23;
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9a0;
  local_128 = 0;
  _DAT_1409be2f0 = CONCAT44(param_3 + DAT_140992088 * fVar23,fVar25 + DAT_140992088 * fVar23);
  _DAT_1409be2ec = _DAT_1409be2e8;
  if ((*(int *)(param_1 + 0xada8) == -1) ||
     (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
    FUN_1404461f0(lVar2);
  }
  uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar9 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13,0,0);
  fVar20 = DAT_140909d28;
  fVar24 = local_138[0];
  if (bVar9) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if ((DAT_140a54e91 != '\0') && (*(char *)(param_4 + 0x20) != '\0')) {
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar9 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar9)) {
        pcVar14 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar14,true,fVar8,(char *)0x0);
      }
      if (bVar6) {
        if (local_138[0] < 0.0) {
          local_138[0] = fVar8;
        }
      }
      else if (local_138[0] < 0.0) {
        local_138[0] = fVar20;
      }
      fVar24 = local_138[0] + DAT_14090b288;
      if (local_138[0] + DAT_14090b288 < fVar7) {
        fVar24 = fVar7;
      }
      uVar16 = uVar17;
      uVar19 = uVar17;
      if (*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3 != 0) {
        do {
          lVar3 = *(longlong *)(uVar16 + *(longlong *)(lVar1 + 0xd38));
          lVar4 = *(longlong *)(*(longlong *)(lVar3 + 8) + 0x318);
          if ((lVar4 == 0) || (*(int *)(lVar4 + 0x360) != 0x14)) {
            *(float *)(*(longlong *)(lVar3 + 0x10) + 0xcc) = fVar24;
          }
          uVar18 = (int)uVar19 + 1;
          uVar16 = uVar16 + 8;
          uVar19 = (ulonglong)uVar18;
        } while ((ulonglong)(longlong)(int)uVar18 <
                 (ulonglong)(*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3));
      }
    }
  }
  else {
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar8,fVar8,0.0,true);
  fVar25 = fVar25 + DAT_140992088 * DAT_14090a9cc;
  fVar22 = fVar25 - DAT_140992088 * fVar22;
  if (fVar24 < 0.0) {
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,fVar22,param_3,0xff000000
               ,L"< ? %%");
  }
  else {
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,fVar22,param_3,0xff000000
               ,L"< %d %%");
  }
  fVar22 = DAT_140992088 * DAT_14090aa0c;
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9a0;
  local_128 = 0;
  _DAT_1409be2f0 =
       CONCAT44(DAT_140992088 * fVar23 + param_3,DAT_140992088 * DAT_14090a928 + fVar25 + fVar22);
  _DAT_1409be2ec = _DAT_1409be2e8;
  if ((*(int *)(param_1 + 0xada8) == -1) ||
     (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
    FUN_1404461f0(lVar2);
  }
  uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar9 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13,0,0);
  if (bVar9) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if ((DAT_140a54e91 != '\0') && (*(char *)(param_4 + 0x20) != '\0')) {
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar9 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar9)) {
        pcVar14 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar14,true,fVar8,(char *)0x0);
      }
      if (bVar6) {
        if (fVar24 < 0.0) {
          fVar24 = fVar8;
        }
      }
      else if (fVar24 < 0.0) {
        fVar24 = fVar20;
      }
      fVar20 = fVar24 + fVar7;
      if (fVar8 < fVar24 + fVar7) {
        fVar20 = fVar8;
      }
      uVar16 = uVar17;
      if (*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3 != 0) {
        do {
          lVar2 = *(longlong *)(uVar17 + *(longlong *)(lVar1 + 0xd38));
          lVar3 = *(longlong *)(*(longlong *)(lVar2 + 8) + 0x318);
          if ((lVar3 == 0) || (*(int *)(lVar3 + 0x360) != 0x14)) {
            *(float *)(*(longlong *)(lVar2 + 0x10) + 0xcc) = fVar20;
          }
          uVar18 = (int)uVar16 + 1;
          uVar17 = uVar17 + 8;
          uVar16 = (ulonglong)uVar18;
        } while ((ulonglong)(longlong)(int)uVar18 <
                 (ulonglong)(*(longlong *)(lVar1 + 0xd40) - *(longlong *)(lVar1 + 0xd38) >> 3));
      }
    }
  }
  else {
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar8,fVar8,0.0,true);
  return (DAT_140992088 * DAT_14090a9f0 + param_3) - local_114;
}



```

---

## 2. Full Disassembly of FUN_140769300 (0x140769300–0x14076A170)

```asm
; Range: 0x140769300 to 0x14076A16F
140769300:  4C 8B DC                 MOV R11,RSP
140769303:  55                       PUSH RBP
140769304:  53                       PUSH RBX
140769305:  56                       PUSH RSI
140769306:  57                       PUSH RDI
140769307:  41 54                    PUSH R12
140769309:  41 55                    PUSH R13
14076930b:  49 8D 6B 88              LEA RBP,[R11 + -0x78]
14076930f:  48 81 EC 48 01 00 00     SUB RSP,0x148
140769316:  41 0F 29 73 A8           MOVAPS xmmword ptr [R11 + -0x58],XMM6
14076931b:  41 0F 29 7B 98           MOVAPS xmmword ptr [R11 + -0x68],XMM7
140769320:  45 0F 29 8B 78 FF FF FF  MOVAPS xmmword ptr [R11 + -0x88],XMM9
140769328:  45 0F 29 93 68 FF FF FF  MOVAPS xmmword ptr [R11 + -0x98],XMM10
140769330:  45 0F 29 9B 58 FF FF FF  MOVAPS xmmword ptr [R11 + -0xa8],XMM11
140769338:  45 0F 29 AB 38 FF FF FF  MOVAPS xmmword ptr [R11 + -0xc8],XMM13
140769340:  48 8B 05 01 8D 22 00     MOV RAX,qword ptr [0x140992048]
140769347:  48 33 C4                 XOR RAX,RSP
14076934a:  48 89 45 98              MOV qword ptr [RBP + -0x68],RAX
14076934e:  49 8B B9 40 02 00 00     MOV RDI,qword ptr [R9 + 0x240]
140769355:  44 0F 28 D2              MOVAPS XMM10,XMM2
140769359:  4D 89 7B C0              MOV qword ptr [R11 + -0x40],R15
14076935d:  4D 8B E9                 MOV R13,R9
140769360:  45 0F 29 43 88           MOVAPS xmmword ptr [R11 + -0x78],XMM8
140769365:  48 8B F1                 MOV RSI,RCX
140769368:  44 0F 28 C1              MOVAPS XMM8,XMM1
14076936c:  F3 44 0F 11 54 24 64     MOVSS dword ptr [RSP + 0x64],XMM10
140769373:  48 8B 87 18 03 00 00     MOV RAX,qword ptr [RDI + 0x318]
14076937a:  F3 44 0F 11 44 24 5C     MOVSS dword ptr [RSP + 0x5c],XMM8
140769381:  45 0F 29 B3 28 FF FF FF  MOVAPS xmmword ptr [R11 + -0xd8],XMM14
140769389:  48 85 C0                 TEST RAX,RAX
14076938c:  74 0E                    JZ 0x14076939c
14076938e:  83 B8 60 03 00 00 2D     CMP dword ptr [RAX + 0x360],0x2d
140769395:  75 05                    JNZ 0x14076939c
140769397:  41 B4 01                 MOV R12B,0x1
14076939a:  EB 03                    JMP 0x14076939f
14076939c:  45 32 E4                 XOR R12B,R12B
14076939f:  BA BD 05 00 00           MOV EDX,0x5bd
1407693a4:  48 8D 0D E5 E1 22 00     LEA RCX,[0x140997590]
1407693ab:  FF 15 DF 34 10 00        CALL qword ptr [0x14086c890]
1407693b1:  48 8B 15 50 AE 22 00     MOV RDX,qword ptr [0x140994208]
1407693b8:  48 8D 0D F1 DB 22 00     LEA RCX,[0x140996fb0]
1407693bf:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1407693c4:  41 0F 28 DA              MOVAPS XMM3,XMM10
1407693c8:  41 0F 28 D0              MOVAPS XMM2,XMM8
1407693cc:  66 41 0F 7E D9           MOVD R9D,XMM3
1407693d1:  48 8D 05 C4 4B 19 00     LEA RAX,[0x1408fdf9c]
1407693d8:  66 41 0F 7E D0           MOVD R8D,XMM2
1407693dd:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407693e2:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1407693ea:  FF 15 90 34 10 00        CALL qword ptr [0x14086c880]
1407693f0:  F3 0F 10 0D 90 8C 22 00  MOVSS XMM1,dword ptr [0x140992088]
1407693f8:  48 8D 0D 91 E1 22 00     LEA RCX,[0x140997590]
1407693ff:  48 8B 05 02 AE 22 00     MOV RAX,qword ptr [0x140994208]
140769406:  0F 28 C1                 MOVAPS XMM0,XMM1
140769409:  F3 0F 59 05 57 15 1A 00  MULSS XMM0,dword ptr [0x14090a968]
140769411:  BA B8 05 00 00           MOV EDX,0x5b8
140769416:  F3 44 0F 10 2D 81 15 1A 00 MOVSS XMM13,dword ptr [0x14090a9a0]
14076941f:  48 8B 18                 MOV RBX,qword ptr [RAX]
140769422:  F3 41 0F 59 CD           MULSS XMM1,XMM13
140769427:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
14076942c:  F3 44 0F 58 D1           ADDSS XMM10,XMM1
140769431:  FF 15 59 34 10 00        CALL qword ptr [0x14086c890]
140769437:  48 8B 0D CA AD 22 00     MOV RCX,qword ptr [0x140994208]
14076943e:  48 8D 55 88              LEA RDX,[RBP + -0x78]
140769442:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140769447:  41 0F 28 DA              MOVAPS XMM3,XMM10
14076944b:  48 8D 05 56 4B 19 00     LEA RAX,[0x1408fdfa8]
140769452:  66 41 0F 7E D9           MOVD R9D,XMM3
140769457:  0F 57 FF                 XORPS XMM7,XMM7
14076945a:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14076945f:  0F 28 D7                 MOVAPS XMM2,XMM7
140769462:  66 41 0F 7E D0           MOVD R8D,XMM2
140769467:  FF 53 28                 CALL qword ptr [RBX + 0x28]
14076946a:  BA B9 05 00 00           MOV EDX,0x5b9
14076946f:  48 8D 0D 1A E1 22 00     LEA RCX,[0x140997590]
140769476:  0F 10 30                 MOVUPS XMM6,xmmword ptr [RAX]
140769479:  48 8B 05 88 AD 22 00     MOV RAX,qword ptr [0x140994208]
140769480:  0F 11 74 24 78           MOVUPS xmmword ptr [RSP + 0x78],XMM6
140769485:  48 8B 18                 MOV RBX,qword ptr [RAX]
140769488:  FF 15 02 34 10 00        CALL qword ptr [0x14086c890]
14076948e:  48 8B 0D 73 AD 22 00     MOV RCX,qword ptr [0x140994208]
140769495:  48 8D 55 88              LEA RDX,[RBP + -0x78]
140769499:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14076949e:  0F 28 DF                 MOVAPS XMM3,XMM7
1407694a1:  0F 28 D7                 MOVAPS XMM2,XMM7
1407694a4:  66 41 0F 7E D9           MOVD R9D,XMM3
1407694a9:  48 8D 05 08 4B 19 00     LEA RAX,[0x1408fdfb8]
1407694b0:  66 41 0F 7E D0           MOVD R8D,XMM2
1407694b5:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1407694ba:  FF 53 28                 CALL qword ptr [RBX + 0x28]
1407694bd:  66 0F 73 DE 08           PSRLDQ XMM6,0x8
1407694c2:  48 8D 54 24 48           LEA RDX,[RSP + 0x48]
1407694c7:  0F 10 08                 MOVUPS XMM1,xmmword ptr [RAX]
1407694ca:  66 0F 7E F0              MOVD EAX,XMM6
1407694ce:  0F 11 4D 88              MOVUPS xmmword ptr [RBP + -0x78],XMM1
1407694d2:  66 0F 73 D9 08           PSRLDQ XMM1,0x8
1407694d7:  66 0F 7E C9              MOVD ECX,XMM1
1407694db:  3B C1                    CMP EAX,ECX
1407694dd:  0F 4F C8                 CMOVG ECX,EAX
1407694e0:  66 0F 6E F1              MOVD XMM6,ECX
1407694e4:  48 8D 0D A5 B6 2E 00     LEA RCX,[0x140a54b90]
1407694eb:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
1407694ee:  F3 0F 11 74 24 60        MOVSS dword ptr [RSP + 0x60],XMM6
1407694f4:  FF 15 6E 33 10 00        CALL qword ptr [0x14086c868]
1407694fa:  4C 8D 4C 24 40           LEA R9,[RSP + 0x40]
1407694ff:  C6 44 24 20 01           MOV byte ptr [RSP + 0x20],0x1
140769504:  4C 8D 44 24 58           LEA R8,[RSP + 0x58]
140769509:  48 8B D7                 MOV RDX,RDI
14076950c:  F2 0F 10 00              MOVSD XMM0,qword ptr [RAX]
140769510:  F2 0F 11 44 24 68        MOVSD qword ptr [RSP + 0x68],XMM0
140769516:  E8 85 0E 00 00           CALL 0x14076a3a0
14076951b:  BA B8 05 00 00           MOV EDX,0x5b8
140769520:  48 8D 0D 69 E0 22 00     LEA RCX,[0x140997590]
140769527:  F3 44 0F 58 C6           ADDSS XMM8,XMM6
14076952c:  FF 15 5E 33 10 00        CALL qword ptr [0x14086c890]
140769532:  66 0F 6E 05 52 BD 22 00  MOVD XMM0,dword ptr [0x14099528c]
14076953a:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
14076953d:  48 8B 15 C4 AC 22 00     MOV RDX,qword ptr [0x140994208]
140769544:  48 8D 0D 65 DA 22 00     LEA RCX,[0x140996fb0]
14076954b:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140769550:  41 0F 28 D0              MOVAPS XMM2,XMM8
140769554:  F3 0F 5C D0              SUBSS XMM2,XMM0
140769558:  41 0F 28 DA              MOVAPS XMM3,XMM10
14076955c:  48 8D 05 65 4A 19 00     LEA RAX,[0x1408fdfc8]
140769563:  66 41 0F 7E D9           MOVD R9D,XMM3
140769568:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
14076956d:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140769575:  66 41 0F 7E D0           MOVD R8D,XMM2
14076957a:  FF 15 F8 32 10 00        CALL qword ptr [0x14086c878]
140769580:  F3 0F 10 4C 24 68        MOVSS XMM1,dword ptr [RSP + 0x68]
140769586:  41 0F 28 C0              MOVAPS XMM0,XMM8
14076958a:  F3 44 0F 10 5C 24 6C     MOVSS XMM11,dword ptr [RSP + 0x6c]
140769591:  F3 0F 5C C6              SUBSS XMM0,XMM6
140769595:  0F 2F C8                 COMISS XMM1,XMM0
140769598:  0F 86 84 00 00 00        JBE 0x140769622
14076959e:  44 0F 2F C1              COMISS XMM8,XMM1
1407695a2:  76 7E                    JBE 0x140769622
1407695a4:  44 0F 2F 1D F4 B1 25 00  COMISS XMM11,dword ptr [0x1409c47a0]
1407695ac:  72 74                    JC 0x140769622
1407695ae:  F3 0F 10 05 EE B1 25 00  MOVSS XMM0,dword ptr [0x1409c47a4]
1407695b6:  41 0F 2F C3              COMISS XMM0,XMM11
1407695ba:  72 66                    JC 0x140769622
1407695bc:  48 8D 0D 9D 4A 25 00     LEA RCX,[0x1409be060]
1407695c3:  FF 15 97 36 10 00        CALL qword ptr [0x14086cc60]
1407695c9:  41 0F 28 CB              MOVAPS XMM1,XMM11
1407695cd:  F3 0F 5C C8              SUBSS XMM1,XMM0
1407695d1:  66 0F 6E 44 24 7C        MOVD XMM0,dword ptr [RSP + 0x7c]
1407695d7:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1407695da:  0F 2F C8                 COMISS XMM1,XMM0
1407695dd:  76 43                    JBE 0x140769622
1407695df:  48 8D 0D 7A 4A 25 00     LEA RCX,[0x1409be060]
1407695e6:  FF 15 74 36 10 00        CALL qword ptr [0x14086cc60]
1407695ec:  41 0F 28 CB              MOVAPS XMM1,XMM11
1407695f0:  F3 0F 5C C8              SUBSS XMM1,XMM0
1407695f4:  66 0F 6E 45 84           MOVD XMM0,dword ptr [RBP + -0x7c]
1407695f9:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1407695fc:  0F 2F C1                 COMISS XMM0,XMM1
1407695ff:  76 21                    JBE 0x140769622
140769601:  BA BB 05 00 00           MOV EDX,0x5bb
140769606:  48 8D 0D 83 DF 22 00     LEA RCX,[0x140997590]
14076960d:  FF 15 7D 32 10 00        CALL qword ptr [0x14086c890]
140769613:  48 8B D0                 MOV RDX,RAX
140769616:  48 8D 8E A0 D5 00 00     LEA RCX,[RSI + 0xd5a0]
14076961d:  E8 DE C2 8A FF           CALL 0x140015900
140769622:  48 8B 8E A0 B4 00 00     MOV RCX,qword ptr [RSI + 0xb4a0]
140769629:  33 D2                    XOR EDX,EDX
14076962b:  F3 0F 10 05 55 8A 22 00  MOVSS XMM0,dword ptr [0x140992088]
140769633:  F3 44 0F 10 35 EC 11 1A 00 MOVSS XMM14,dword ptr [0x14090a828]
14076963c:  4C 8B 05 C5 16 28 00     MOV R8,qword ptr [0x1409ead08]
140769643:  48 8B 01                 MOV RAX,qword ptr [RCX]
140769646:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14076964b:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769650:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140769653:  F3 0F 10 1D 2D 8A 22 00  MOVSS XMM3,dword ptr [0x140992088]
14076965b:  4C 8D BE B0 AD 00 00     LEA R15,[RSI + 0xadb0]
140769662:  0F 28 C3                 MOVAPS XMM0,XMM3
140769665:  C7 44 24 50 00 00 00 00  MOV dword ptr [RSP + 0x50],0x0
14076966d:  8B 44 24 50              MOV EAX,dword ptr [RSP + 0x50]
140769671:  41 0F 28 C8              MOVAPS XMM1,XMM8
140769675:  F3 41 0F 59 C6           MULSS XMM0,XMM14
14076967a:  41 0F 28 D2              MOVAPS XMM2,XMM10
14076967e:  89 05 74 4C 25 00        MOV dword ptr [0x1409be2f8],EAX
140769684:  F3 41 0F 59 DD           MULSS XMM3,XMM13
140769689:  F3 0F 58 C8              ADDSS XMM1,XMM0
14076968d:  F3 0F 58 D0              ADDSS XMM2,XMM0
140769691:  F3 0F 11 1D 4F 4C 25 00  MOVSS dword ptr [0x1409be2e8],XMM3
140769699:  F3 0F 11 1D 4B 4C 25 00  MOVSS dword ptr [0x1409be2ec],XMM3
1407696a1:  0F 28 C1                 MOVAPS XMM0,XMM1
1407696a4:  0F 14 C2                 UNPCKLPS XMM0,XMM2
1407696a7:  F2 0F 11 05 41 4C 25 00  MOVSD qword ptr [0x1409be2f0],XMM0
1407696af:  44 8B 8E A8 AD 00 00     MOV R9D,dword ptr [RSI + 0xada8]
1407696b6:  41 83 F9 FF              CMP R9D,-0x1
1407696ba:  74 06                    JZ 0x1407696c2
1407696bc:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
1407696c0:  75 08                    JNZ 0x1407696ca
1407696c2:  49 8B CF                 MOV RCX,R15
1407696c5:  E8 26 CB CD FF           CALL 0x1404461f0
1407696ca:  48 8D 54 24 48           LEA RDX,[RSP + 0x48]
1407696cf:  4C 89 B4 24 40 01 00 00  MOV qword ptr [RSP + 0x140],R14
1407696d7:  48 8D 0D B2 B4 2E 00     LEA RCX,[0x140a54b90]
1407696de:  FF 15 84 31 10 00        CALL qword ptr [0x14086c868]
1407696e4:  0F 28 DF                 MOVAPS XMM3,XMM7
1407696e7:  48 8D 0D 72 49 25 00     LEA RCX,[0x1409be060]
1407696ee:  48 8B D0                 MOV RDX,RAX
1407696f1:  0F 28 D7                 MOVAPS XMM2,XMM7
1407696f4:  FF 15 56 31 10 00        CALL qword ptr [0x14086c850]
1407696fa:  F3 44 0F 10 0D 55 08 1A 00 MOVSS XMM9,dword ptr [0x140909f58]
140769703:  33 DB                    XOR EBX,EBX
140769705:  84 C0                    TEST AL,AL
140769707:  0F 84 F4 00 00 00        JZ 0x140769801
14076970d:  38 1D 7E B7 2E 00        CMP byte ptr [0x140a54e91],BL
140769713:  0F 28 05 A6 2D 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
14076971a:  0F 11 05 EB 4B 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140769721:  0F 84 E8 00 00 00        JZ 0x14076980f
140769727:  41 38 5D 20              CMP byte ptr [R13 + 0x20],BL
14076972b:  0F 84 DE 00 00 00        JZ 0x14076980f
140769731:  38 5E 14                 CMP byte ptr [RSI + 0x14],BL
140769734:  75 48                    JNZ 0x14076977e
140769736:  48 8D 15 23 48 19 00     LEA RDX,[0x1408fdf60]
14076973d:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140769744:  FF 15 C6 35 10 00        CALL qword ptr [0x14086cd10]
14076974a:  84 C0                    TEST AL,AL
14076974c:  74 30                    JZ 0x14076977e
14076974e:  48 8D 15 0B 48 19 00     LEA RDX,[0x1408fdf60]
140769755:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14076975c:  FF 15 B6 35 10 00        CALL qword ptr [0x14086cd18]
140769762:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
140769769:  41 0F 28 D9              MOVAPS XMM3,XMM9
14076976d:  48 8B D0                 MOV RDX,RAX
140769770:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140769775:  41 B0 01                 MOV R8B,0x1
140769778:  FF 15 2A 3B 10 00        CALL qword ptr [0x14086d2a8]
14076977e:  F3 0F 10 74 24 58        MOVSS XMM6,dword ptr [RSP + 0x58]
140769784:  F3 0F 58 35 FC 1A 1A 00  ADDSS XMM6,dword ptr [0x14090b288]
14076978c:  0F 2F FE                 COMISS XMM7,XMM6
14076978f:  76 03                    JBE 0x140769794
140769791:  0F 28 F7                 MOVAPS XMM6,XMM7
140769794:  48 8B 87 40 0D 00 00     MOV RAX,qword ptr [RDI + 0xd40]
14076979b:  44 8B C3                 MOV R8D,EBX
14076979e:  48 2B 87 38 0D 00 00     SUB RAX,qword ptr [RDI + 0xd38]
1407697a5:  48 C1 F8 03              SAR RAX,0x3
1407697a9:  48 85 C0                 TEST RAX,RAX
1407697ac:  74 67                    JZ 0x140769815
1407697ae:  48 8B D3                 MOV RDX,RBX
1407697b1:  48 8B 87 38 0D 00 00     MOV RAX,qword ptr [RDI + 0xd38]
1407697b8:  4C 8B 0C 02              MOV R9,qword ptr [RDX + RAX*0x1]
1407697bc:  49 8B 41 08              MOV RAX,qword ptr [R9 + 0x8]
1407697c0:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
1407697c7:  48 85 C9                 TEST RCX,RCX
1407697ca:  74 09                    JZ 0x1407697d5
1407697cc:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
1407697d3:  74 09                    JZ 0x1407697de
1407697d5:  49 8B 41 10              MOV RAX,qword ptr [R9 + 0x10]
1407697d9:  F3 0F 11 70 04           MOVSS dword ptr [RAX + 0x4],XMM6
1407697de:  48 8B 8F 40 0D 00 00     MOV RCX,qword ptr [RDI + 0xd40]
1407697e5:  41 FF C0                 INC R8D
1407697e8:  48 2B 8F 38 0D 00 00     SUB RCX,qword ptr [RDI + 0xd38]
1407697ef:  48 83 C2 08              ADD RDX,0x8
1407697f3:  48 C1 F9 03              SAR RCX,0x3
1407697f7:  49 63 C0                 MOVSXD RAX,R8D
1407697fa:  48 3B C1                 CMP RAX,RCX
1407697fd:  72 B2                    JC 0x1407697b1
1407697ff:  EB 14                    JMP 0x140769815
140769801:  0F 28 05 F8 28 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
140769808:  0F 11 05 FD 4A 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14076980f:  F3 0F 10 74 24 58        MOVSS XMM6,dword ptr [RSP + 0x58]
140769815:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14076981a:  48 8D 0D 3F 48 25 00     LEA RCX,[0x1409be060]
140769821:  F3 0F 11 7C 24 28        MOVSS dword ptr [RSP + 0x28],XMM7
140769827:  41 0F 28 D9              MOVAPS XMM3,XMM9
14076982b:  0F 28 D7                 MOVAPS XMM2,XMM7
14076982e:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140769835:  0F 28 CF                 MOVAPS XMM1,XMM7
140769838:  44 0F 29 A4 24 C0 00 00 00 MOVAPS xmmword ptr [RSP + 0xc0],XMM12
140769841:  FF 15 19 30 10 00        CALL qword ptr [0x14086c860]
140769847:  0F 2F F7                 COMISS XMM6,XMM7
14076984a:  48 8D 0D 5F D7 22 00     LEA RCX,[0x140996fb0]
140769851:  48 8B 15 B0 A9 22 00     MOV RDX,qword ptr [0x140994208]
140769858:  F3 0F 10 0D 28 88 22 00  MOVSS XMM1,dword ptr [0x140992088]
140769860:  41 0F 28 DA              MOVAPS XMM3,XMM10
140769864:  F3 44 0F 10 25 3B 0E 1A 00 MOVSS XMM12,dword ptr [0x14090a6a8]
14076986d:  0F 28 C1                 MOVAPS XMM0,XMM1
140769870:  F3 0F 59 05 54 11 1A 00  MULSS XMM0,dword ptr [0x14090a9cc]
140769878:  F3 41 0F 59 CC           MULSS XMM1,XMM12
14076987d:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769882:  66 41 0F 7E D9           MOVD R9D,XMM3
140769887:  41 0F 28 D0              MOVAPS XMM2,XMM8
14076988b:  F3 0F 5C D1              SUBSS XMM2,XMM1
14076988f:  66 41 0F 7E D0           MOVD R8D,XMM2
140769894:  72 2F                    JC 0x1407698c5
140769896:  0F 28 C6                 MOVAPS XMM0,XMM6
140769899:  F3 0F 59 05 CF 12 1A 00  MULSS XMM0,dword ptr [0x14090ab70]
1407698a1:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
1407698a5:  89 44 24 30              MOV dword ptr [RSP + 0x30],EAX
1407698a9:  48 8D 05 C0 46 19 00     LEA RAX,[0x1408fdf70]
1407698b0:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407698b5:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1407698bd:  FF 15 BD 2F 10 00        CALL qword ptr [0x14086c880]
1407698c3:  EB 1A                    JMP 0x1407698df
1407698c5:  48 8D 05 B4 46 19 00     LEA RAX,[0x1408fdf80]
1407698cc:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407698d1:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1407698d9:  FF 15 A1 2F 10 00        CALL qword ptr [0x14086c880]
1407698df:  48 8B 8E 98 B4 00 00     MOV RCX,qword ptr [RSI + 0xb498]
1407698e6:  33 D2                    XOR EDX,EDX
1407698e8:  F3 0F 10 05 98 87 22 00  MOVSS XMM0,dword ptr [0x140992088]
1407698f0:  F3 0F 59 05 14 11 1A 00  MULSS XMM0,dword ptr [0x14090aa0c]
1407698f8:  4C 8B 05 09 14 28 00     MOV R8,qword ptr [0x1409ead08]
1407698ff:  48 8B 01                 MOV RAX,qword ptr [RCX]
140769902:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769907:  FF 50 70                 CALL qword ptr [RAX + 0x70]
14076990a:  F3 0F 10 15 76 87 22 00  MOVSS XMM2,dword ptr [0x140992088]
140769912:  8B C3                    MOV EAX,EBX
140769914:  0F 28 CA                 MOVAPS XMM1,XMM2
140769917:  89 1D DB 49 25 00        MOV dword ptr [0x1409be2f8],EBX
14076991d:  F3 0F 59 0D 03 10 1A 00  MULSS XMM1,dword ptr [0x14090a928]
140769925:  0F 28 C2                 MOVAPS XMM0,XMM2
140769928:  89 5C 24 50              MOV dword ptr [RSP + 0x50],EBX
14076992c:  F3 41 0F 59 C5           MULSS XMM0,XMM13
140769931:  F3 41 0F 59 D6           MULSS XMM2,XMM14
140769936:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
14076993b:  F3 0F 11 05 A5 49 25 00  MOVSS dword ptr [0x1409be2e8],XMM0
140769943:  F3 0F 11 05 A1 49 25 00  MOVSS dword ptr [0x1409be2ec],XMM0
14076994b:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140769950:  0F 28 C1                 MOVAPS XMM0,XMM1
140769953:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140769956:  F2 0F 11 05 92 49 25 00  MOVSD qword ptr [0x1409be2f0],XMM0
14076995e:  44 8B 8E A8 AD 00 00     MOV R9D,dword ptr [RSI + 0xada8]
140769965:  41 83 F9 FF              CMP R9D,-0x1
140769969:  74 06                    JZ 0x140769971
14076996b:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
14076996f:  75 08                    JNZ 0x140769979
140769971:  49 8B CF                 MOV RCX,R15
140769974:  E8 77 C8 CD FF           CALL 0x1404461f0
140769979:  48 8D 54 24 48           LEA RDX,[RSP + 0x48]
14076997e:  48 8D 0D 0B B2 2E 00     LEA RCX,[0x140a54b90]
140769985:  FF 15 DD 2E 10 00        CALL qword ptr [0x14086c868]
14076998b:  0F 28 DF                 MOVAPS XMM3,XMM7
14076998e:  48 8D 0D CB 46 25 00     LEA RCX,[0x1409be060]
140769995:  48 8B D0                 MOV RDX,RAX
140769998:  0F 28 D7                 MOVAPS XMM2,XMM7
14076999b:  FF 15 AF 2E 10 00        CALL qword ptr [0x14086c850]
1407699a1:  F3 44 0F 10 2D CE 02 1A 00 MOVSS XMM13,dword ptr [0x140909c78]
1407699aa:  84 C0                    TEST AL,AL
1407699ac:  0F 84 FE 00 00 00        JZ 0x140769ab0
1407699b2:  38 1D D9 B4 2E 00        CMP byte ptr [0x140a54e91],BL
1407699b8:  0F 28 05 01 2B 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1407699bf:  0F 11 05 46 49 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407699c6:  0F 84 F2 00 00 00        JZ 0x140769abe
1407699cc:  41 38 5D 20              CMP byte ptr [R13 + 0x20],BL
1407699d0:  0F 84 E8 00 00 00        JZ 0x140769abe
1407699d6:  38 5E 14                 CMP byte ptr [RSI + 0x14],BL
1407699d9:  75 48                    JNZ 0x140769a23
1407699db:  48 8D 15 AE 45 19 00     LEA RDX,[0x1408fdf90]
1407699e2:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
1407699e9:  FF 15 21 33 10 00        CALL qword ptr [0x14086cd10]
1407699ef:  84 C0                    TEST AL,AL
1407699f1:  74 30                    JZ 0x140769a23
1407699f3:  48 8D 15 96 45 19 00     LEA RDX,[0x1408fdf90]
1407699fa:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140769a01:  FF 15 11 33 10 00        CALL qword ptr [0x14086cd18]
140769a07:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
140769a0e:  41 0F 28 D9              MOVAPS XMM3,XMM9
140769a12:  48 8B D0                 MOV RDX,RAX
140769a15:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140769a1a:  41 B0 01                 MOV R8B,0x1
140769a1d:  FF 15 85 38 10 00        CALL qword ptr [0x14086d2a8]
140769a23:  0F 2F FE                 COMISS XMM7,XMM6
140769a26:  76 03                    JBE 0x140769a2b
140769a28:  0F 28 F7                 MOVAPS XMM6,XMM7
140769a2b:  F3 0F 10 05 C9 04 1A 00  MOVSS XMM0,dword ptr [0x140909efc]
140769a33:  F3 41 0F 58 F5           ADDSS XMM6,XMM13
140769a38:  0F 2F F0                 COMISS XMM6,XMM0
140769a3b:  76 03                    JBE 0x140769a40
140769a3d:  0F 28 F0                 MOVAPS XMM6,XMM0
140769a40:  48 8B 87 40 0D 00 00     MOV RAX,qword ptr [RDI + 0xd40]
140769a47:  44 8B CB                 MOV R9D,EBX
140769a4a:  48 2B 87 38 0D 00 00     SUB RAX,qword ptr [RDI + 0xd38]
140769a51:  48 C1 F8 03              SAR RAX,0x3
140769a55:  48 85 C0                 TEST RAX,RAX
140769a58:  74 64                    JZ 0x140769abe
140769a5a:  48 8B D3                 MOV RDX,RBX
140769a5d:  0F 1F 00                 NOP dword ptr [RAX]
140769a60:  48 8B 87 38 0D 00 00     MOV RAX,qword ptr [RDI + 0xd38]
140769a67:  4C 8B 04 02              MOV R8,qword ptr [RDX + RAX*0x1]
140769a6b:  49 8B 40 08              MOV RAX,qword ptr [R8 + 0x8]
140769a6f:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
140769a76:  48 85 C9                 TEST RCX,RCX
140769a79:  74 09                    JZ 0x140769a84
140769a7b:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
140769a82:  74 09                    JZ 0x140769a8d
140769a84:  49 8B 40 10              MOV RAX,qword ptr [R8 + 0x10]
140769a88:  F3 0F 11 70 04           MOVSS dword ptr [RAX + 0x4],XMM6
140769a8d:  48 8B 8F 40 0D 00 00     MOV RCX,qword ptr [RDI + 0xd40]
140769a94:  41 FF C1                 INC R9D
140769a97:  48 2B 8F 38 0D 00 00     SUB RCX,qword ptr [RDI + 0xd38]
140769a9e:  48 83 C2 08              ADD RDX,0x8
140769aa2:  48 C1 F9 03              SAR RCX,0x3
140769aa6:  49 63 C1                 MOVSXD RAX,R9D
140769aa9:  48 3B C1                 CMP RAX,RCX
140769aac:  72 B2                    JC 0x140769a60
140769aae:  EB 0E                    JMP 0x140769abe
140769ab0:  0F 28 05 49 26 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
140769ab7:  0F 11 05 4E 48 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140769abe:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140769ac3:  48 8D 0D 96 45 25 00     LEA RCX,[0x1409be060]
140769aca:  F3 0F 11 7C 24 28        MOVSS dword ptr [RSP + 0x28],XMM7
140769ad0:  41 0F 28 D9              MOVAPS XMM3,XMM9
140769ad4:  0F 28 D7                 MOVAPS XMM2,XMM7
140769ad7:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140769ade:  0F 28 CF                 MOVAPS XMM1,XMM7
140769ae1:  FF 15 79 2D 10 00        CALL qword ptr [0x14086c860]
140769ae7:  F3 0F 10 05 99 85 22 00  MOVSS XMM0,dword ptr [0x140992088]
140769aef:  48 8D 0D 9A DA 22 00     LEA RCX,[0x140997590]
140769af6:  44 0F 28 C0              MOVAPS XMM8,XMM0
140769afa:  BA B9 05 00 00           MOV EDX,0x5b9
140769aff:  F3 44 0F 59 05 60 0E 1A 00 MULSS XMM8,dword ptr [0x14090a968]
140769b08:  F3 0F 59 05 BC 0E 1A 00  MULSS XMM0,dword ptr [0x14090a9cc]
140769b10:  F3 44 0F 58 44 24 5C     ADDSS XMM8,dword ptr [RSP + 0x5c]
140769b17:  F3 44 0F 58 D0           ADDSS XMM10,XMM0
140769b1c:  F3 44 0F 58 44 24 60     ADDSS XMM8,dword ptr [RSP + 0x60]
140769b23:  FF 15 67 2D 10 00        CALL qword ptr [0x14086c890]
140769b29:  66 0F 6E 05 5B B7 22 00  MOVD XMM0,dword ptr [0x14099528c]
140769b31:  48 8D 0D 78 D4 22 00     LEA RCX,[0x140996fb0]
140769b38:  48 8B 15 C9 A6 22 00     MOV RDX,qword ptr [0x140994208]
140769b3f:  41 0F 28 D0              MOVAPS XMM2,XMM8
140769b43:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140769b48:  41 0F 28 DA              MOVAPS XMM3,XMM10
140769b4c:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
140769b4f:  48 8D 05 8E 44 19 00     LEA RAX,[0x1408fdfe4]
140769b56:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140769b5b:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140769b63:  66 41 0F 7E D9           MOVD R9D,XMM3
140769b68:  F3 0F 5C D0              SUBSS XMM2,XMM0
140769b6c:  66 41 0F 7E D0           MOVD R8D,XMM2
140769b71:  FF 15 01 2D 10 00        CALL qword ptr [0x14086c878]
140769b77:  F3 0F 10 4C 24 68        MOVSS XMM1,dword ptr [RSP + 0x68]
140769b7d:  41 0F 28 C0              MOVAPS XMM0,XMM8
140769b81:  F3 0F 5C 44 24 60        SUBSS XMM0,dword ptr [RSP + 0x60]
140769b87:  0F 2F C8                 COMISS XMM1,XMM0
140769b8a:  0F 86 8F 00 00 00        JBE 0x140769c1f
140769b90:  44 0F 2F C1              COMISS XMM8,XMM1
140769b94:  0F 86 85 00 00 00        JBE 0x140769c1f
140769b9a:  44 0F 2F 1D FE AB 25 00  COMISS XMM11,dword ptr [0x1409c47a0]
140769ba2:  72 7B                    JC 0x140769c1f
140769ba4:  F3 0F 10 05 F8 AB 25 00  MOVSS XMM0,dword ptr [0x1409c47a4]
140769bac:  41 0F 2F C3              COMISS XMM0,XMM11
140769bb0:  72 6D                    JC 0x140769c1f
140769bb2:  48 8D 0D A7 44 25 00     LEA RCX,[0x1409be060]
140769bb9:  FF 15 A1 30 10 00        CALL qword ptr [0x14086cc60]
140769bbf:  66 0F 6E 4D 8C           MOVD XMM1,dword ptr [RBP + -0x74]
140769bc4:  41 0F 28 D3              MOVAPS XMM2,XMM11
140769bc8:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140769bcb:  F3 0F 5C D0              SUBSS XMM2,XMM0
140769bcf:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140769bd4:  0F 2F D1                 COMISS XMM2,XMM1
140769bd7:  76 46                    JBE 0x140769c1f
140769bd9:  48 8D 0D 80 44 25 00     LEA RCX,[0x1409be060]
140769be0:  FF 15 7A 30 10 00        CALL qword ptr [0x14086cc60]
140769be6:  66 0F 6E 4D 94           MOVD XMM1,dword ptr [RBP + -0x6c]
140769beb:  F3 44 0F 5C D8           SUBSS XMM11,XMM0
140769bf0:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
140769bf3:  F3 41 0F 58 CA           ADDSS XMM1,XMM10
140769bf8:  41 0F 2F CB              COMISS XMM1,XMM11
140769bfc:  76 21                    JBE 0x140769c1f
140769bfe:  BA BC 05 00 00           MOV EDX,0x5bc
140769c03:  48 8D 0D 86 D9 22 00     LEA RCX,[0x140997590]
140769c0a:  FF 15 80 2C 10 00        CALL qword ptr [0x14086c890]
140769c10:  48 8B D0                 MOV RDX,RAX
140769c13:  48 8D 8E A0 D5 00 00     LEA RCX,[RSI + 0xd5a0]
140769c1a:  E8 E1 BC 8A FF           CALL 0x140015900
140769c1f:  48 8B 8E A0 B4 00 00     MOV RCX,qword ptr [RSI + 0xb4a0]
140769c26:  33 D2                    XOR EDX,EDX
140769c28:  F3 0F 10 05 58 84 22 00  MOVSS XMM0,dword ptr [0x140992088]
140769c30:  4C 8B 05 D1 10 28 00     MOV R8,qword ptr [0x1409ead08]
140769c37:  F3 41 0F 59 C6           MULSS XMM0,XMM14
140769c3c:  48 8B 01                 MOV RAX,qword ptr [RCX]
140769c3f:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769c44:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140769c47:  F3 0F 10 1D 39 84 22 00  MOVSS XMM3,dword ptr [0x140992088]
140769c4f:  41 0F 28 C8              MOVAPS XMM1,XMM8
140769c53:  0F 28 C3                 MOVAPS XMM0,XMM3
140769c56:  89 1D 9C 46 25 00        MOV dword ptr [0x1409be2f8],EBX
140769c5c:  F3 0F 59 1D 3C 0D 1A 00  MULSS XMM3,dword ptr [0x14090a9a0]
140769c64:  41 0F 28 D2              MOVAPS XMM2,XMM10
140769c68:  8B C3                    MOV EAX,EBX
140769c6a:  89 5C 24 50              MOV dword ptr [RSP + 0x50],EBX
140769c6e:  F3 41 0F 59 C6           MULSS XMM0,XMM14
140769c73:  F3 0F 11 1D 6D 46 25 00  MOVSS dword ptr [0x1409be2e8],XMM3
140769c7b:  F3 0F 11 1D 69 46 25 00  MOVSS dword ptr [0x1409be2ec],XMM3
140769c83:  F3 0F 58 C8              ADDSS XMM1,XMM0
140769c87:  F3 0F 58 D0              ADDSS XMM2,XMM0
140769c8b:  0F 28 C1                 MOVAPS XMM0,XMM1
140769c8e:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140769c91:  F2 0F 11 05 57 46 25 00  MOVSD qword ptr [0x1409be2f0],XMM0
140769c99:  44 8B 8E A8 AD 00 00     MOV R9D,dword ptr [RSI + 0xada8]
140769ca0:  41 83 F9 FF              CMP R9D,-0x1
140769ca4:  74 06                    JZ 0x140769cac
140769ca6:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
140769caa:  75 08                    JNZ 0x140769cb4
140769cac:  49 8B CF                 MOV RCX,R15
140769caf:  E8 3C C5 CD FF           CALL 0x1404461f0
140769cb4:  48 8D 54 24 48           LEA RDX,[RSP + 0x48]
140769cb9:  48 8D 0D D0 AE 2E 00     LEA RCX,[0x140a54b90]
140769cc0:  FF 15 A2 2B 10 00        CALL qword ptr [0x14086c868]
140769cc6:  0F 28 DF                 MOVAPS XMM3,XMM7
140769cc9:  48 8D 0D 90 43 25 00     LEA RCX,[0x1409be060]
140769cd0:  48 8B D0                 MOV RDX,RAX
140769cd3:  0F 28 D7                 MOVAPS XMM2,XMM7
140769cd6:  FF 15 74 2B 10 00        CALL qword ptr [0x14086c850]
140769cdc:  F3 44 0F 10 1D 43 00 1A 00 MOVSS XMM11,dword ptr [0x140909d28]
140769ce5:  84 C0                    TEST AL,AL
140769ce7:  0F 84 16 01 00 00        JZ 0x140769e03
140769ced:  38 1D 9E B1 2E 00        CMP byte ptr [0x140a54e91],BL
140769cf3:  0F 28 05 C6 27 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140769cfa:  0F 11 05 0B 46 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140769d01:  0F 84 0A 01 00 00        JZ 0x140769e11
140769d07:  41 38 5D 20              CMP byte ptr [R13 + 0x20],BL
140769d0b:  0F 84 00 01 00 00        JZ 0x140769e11
140769d11:  38 5E 14                 CMP byte ptr [RSI + 0x14],BL
140769d14:  75 48                    JNZ 0x140769d5e
140769d16:  48 8D 15 D3 42 19 00     LEA RDX,[0x1408fdff0]
140769d1d:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140769d24:  FF 15 E6 2F 10 00        CALL qword ptr [0x14086cd10]
140769d2a:  84 C0                    TEST AL,AL
140769d2c:  74 30                    JZ 0x140769d5e
140769d2e:  48 8D 15 BB 42 19 00     LEA RDX,[0x1408fdff0]
140769d35:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140769d3c:  FF 15 D6 2F 10 00        CALL qword ptr [0x14086cd18]
140769d42:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
140769d49:  41 0F 28 D9              MOVAPS XMM3,XMM9
140769d4d:  48 8B D0                 MOV RDX,RAX
140769d50:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140769d55:  41 B0 01                 MOV R8B,0x1
140769d58:  FF 15 4A 35 10 00        CALL qword ptr [0x14086d2a8]
140769d5e:  F3 0F 10 74 24 40        MOVSS XMM6,dword ptr [RSP + 0x40]
140769d64:  45 84 E4                 TEST R12B,R12B
140769d67:  74 0B                    JZ 0x140769d74
140769d69:  0F 2F FE                 COMISS XMM7,XMM6
140769d6c:  76 0F                    JBE 0x140769d7d
140769d6e:  41 0F 28 F1              MOVAPS XMM6,XMM9
140769d72:  EB 09                    JMP 0x140769d7d
140769d74:  0F 2F FE                 COMISS XMM7,XMM6
140769d77:  76 04                    JBE 0x140769d7d
140769d79:  41 0F 28 F3              MOVAPS XMM6,XMM11
140769d7d:  F3 0F 58 35 03 15 1A 00  ADDSS XMM6,dword ptr [0x14090b288]
140769d85:  44 0F 2F EE              COMISS XMM13,XMM6
140769d89:  76 04                    JBE 0x140769d8f
140769d8b:  41 0F 28 F5              MOVAPS XMM6,XMM13
140769d8f:  48 8B 87 40 0D 00 00     MOV RAX,qword ptr [RDI + 0xd40]
140769d96:  44 8B C3                 MOV R8D,EBX
140769d99:  48 2B 87 38 0D 00 00     SUB RAX,qword ptr [RDI + 0xd38]
140769da0:  48 C1 F8 03              SAR RAX,0x3
140769da4:  48 85 C0                 TEST RAX,RAX
140769da7:  74 6E                    JZ 0x140769e17
140769da9:  48 8B D3                 MOV RDX,RBX
140769dac:  0F 1F 40 00              NOP dword ptr [RAX]
140769db0:  48 8B 87 38 0D 00 00     MOV RAX,qword ptr [RDI + 0xd38]
140769db7:  4C 8B 0C 02              MOV R9,qword ptr [RDX + RAX*0x1]
140769dbb:  49 8B 41 08              MOV RAX,qword ptr [R9 + 0x8]
140769dbf:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
140769dc6:  48 85 C9                 TEST RCX,RCX
140769dc9:  74 09                    JZ 0x140769dd4
140769dcb:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
140769dd2:  74 0C                    JZ 0x140769de0
140769dd4:  49 8B 41 10              MOV RAX,qword ptr [R9 + 0x10]
140769dd8:  F3 0F 11 B0 CC 00 00 00  MOVSS dword ptr [RAX + 0xcc],XMM6
140769de0:  48 8B 8F 40 0D 00 00     MOV RCX,qword ptr [RDI + 0xd40]
140769de7:  41 FF C0                 INC R8D
140769dea:  48 2B 8F 38 0D 00 00     SUB RCX,qword ptr [RDI + 0xd38]
140769df1:  48 83 C2 08              ADD RDX,0x8
140769df5:  48 C1 F9 03              SAR RCX,0x3
140769df9:  49 63 C0                 MOVSXD RAX,R8D
140769dfc:  48 3B C1                 CMP RAX,RCX
140769dff:  72 AF                    JC 0x140769db0
140769e01:  EB 14                    JMP 0x140769e17
140769e03:  0F 28 05 F6 22 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
140769e0a:  0F 11 05 FB 44 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140769e11:  F3 0F 10 74 24 40        MOVSS XMM6,dword ptr [RSP + 0x40]
140769e17:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140769e1c:  48 8D 0D 3D 42 25 00     LEA RCX,[0x1409be060]
140769e23:  F3 0F 11 7C 24 28        MOVSS dword ptr [RSP + 0x28],XMM7
140769e29:  41 0F 28 D9              MOVAPS XMM3,XMM9
140769e2d:  0F 28 D7                 MOVAPS XMM2,XMM7
140769e30:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
140769e37:  0F 28 CF                 MOVAPS XMM1,XMM7
140769e3a:  FF 15 20 2A 10 00        CALL qword ptr [0x14086c860]
140769e40:  0F 2F F7                 COMISS XMM6,XMM7
140769e43:  48 8D 0D 66 D1 22 00     LEA RCX,[0x140996fb0]
140769e4a:  48 8B 15 B7 A3 22 00     MOV RDX,qword ptr [0x140994208]
140769e51:  F3 0F 10 0D 2F 82 22 00  MOVSS XMM1,dword ptr [0x140992088]
140769e59:  41 0F 28 DA              MOVAPS XMM3,XMM10
140769e5d:  0F 28 C1                 MOVAPS XMM0,XMM1
140769e60:  66 41 0F 7E D9           MOVD R9D,XMM3
140769e65:  F3 0F 59 05 5F 0B 1A 00  MULSS XMM0,dword ptr [0x14090a9cc]
140769e6d:  F3 41 0F 59 CC           MULSS XMM1,XMM12
140769e72:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769e77:  41 0F 28 D0              MOVAPS XMM2,XMM8
140769e7b:  F3 0F 5C D1              SUBSS XMM2,XMM1
140769e7f:  66 41 0F 7E D0           MOVD R8D,XMM2
140769e84:  72 2F                    JC 0x140769eb5
140769e86:  0F 28 C6                 MOVAPS XMM0,XMM6
140769e89:  F3 0F 59 05 DF 0C 1A 00  MULSS XMM0,dword ptr [0x14090ab70]
140769e91:  F3 0F 2C C0              CVTTSS2SI EAX,XMM0
140769e95:  89 44 24 30              MOV dword ptr [RSP + 0x30],EAX
140769e99:  48 8D 05 60 41 19 00     LEA RAX,[0x1408fe000]
140769ea0:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140769ea5:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140769ead:  FF 15 CD 29 10 00        CALL qword ptr [0x14086c880]
140769eb3:  EB 1A                    JMP 0x140769ecf
140769eb5:  48 8D 05 54 41 19 00     LEA RAX,[0x1408fe010]
140769ebc:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140769ec1:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
140769ec9:  FF 15 B1 29 10 00        CALL qword ptr [0x14086c880]
140769ecf:  48 8B 8E 98 B4 00 00     MOV RCX,qword ptr [RSI + 0xb498]
140769ed6:  33 D2                    XOR EDX,EDX
140769ed8:  F3 0F 10 05 A8 81 22 00  MOVSS XMM0,dword ptr [0x140992088]
140769ee0:  F3 0F 59 05 24 0B 1A 00  MULSS XMM0,dword ptr [0x14090aa0c]
140769ee8:  4C 8B 05 19 0E 28 00     MOV R8,qword ptr [0x1409ead08]
140769eef:  48 8B 01                 MOV RAX,qword ptr [RCX]
140769ef2:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140769ef7:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140769efa:  F3 0F 10 15 86 81 22 00  MOVSS XMM2,dword ptr [0x140992088]
140769f02:  8B C3                    MOV EAX,EBX
140769f04:  44 0F 28 A4 24 C0 00 00 00 MOVAPS XMM12,xmmword ptr [RSP + 0xc0]
140769f0d:  0F 28 CA                 MOVAPS XMM1,XMM2
140769f10:  F3 0F 59 0D 10 0A 1A 00  MULSS XMM1,dword ptr [0x14090a928]
140769f18:  0F 28 C2                 MOVAPS XMM0,XMM2
140769f1b:  89 1D D7 43 25 00        MOV dword ptr [0x1409be2f8],EBX
140769f21:  F3 0F 59 05 77 0A 1A 00  MULSS XMM0,dword ptr [0x14090a9a0]
140769f29:  89 5C 24 50              MOV dword ptr [RSP + 0x50],EBX
140769f2d:  F3 41 0F 59 D6           MULSS XMM2,XMM14
140769f32:  F3 41 0F 58 C8           ADDSS XMM1,XMM8
140769f37:  44 0F 28 B4 24 A0 00 00 00 MOVAPS XMM14,xmmword ptr [RSP + 0xa0]
140769f40:  44 0F 28 84 24 00 01 00 00 MOVAPS XMM8,xmmword ptr [RSP + 0x100]
140769f49:  F3 0F 11 05 97 43 25 00  MOVSS dword ptr [0x1409be2e8],XMM0
140769f51:  F3 41 0F 58 D2           ADDSS XMM2,XMM10
140769f56:  F3 0F 11 05 8E 43 25 00  MOVSS dword ptr [0x1409be2ec],XMM0
140769f5e:  0F 28 C1                 MOVAPS XMM0,XMM1
140769f61:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140769f64:  F2 0F 11 05 84 43 25 00  MOVSD qword ptr [0x1409be2f0],XMM0
140769f6c:  44 8B 8E A8 AD 00 00     MOV R9D,dword ptr [RSI + 0xada8]
140769f73:  41 83 F9 FF              CMP R9D,-0x1
140769f77:  74 06                    JZ 0x140769f7f
140769f79:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
140769f7d:  75 08                    JNZ 0x140769f87
140769f7f:  49 8B CF                 MOV RCX,R15
140769f82:  E8 69 C2 CD FF           CALL 0x1404461f0
140769f87:  48 8D 54 24 48           LEA RDX,[RSP + 0x48]
140769f8c:  48 8D 0D FD AB 2E 00     LEA RCX,[0x140a54b90]
140769f93:  FF 15 CF 28 10 00        CALL qword ptr [0x14086c868]
140769f99:  0F 28 DF                 MOVAPS XMM3,XMM7
140769f9c:  48 8D 0D BD 40 25 00     LEA RCX,[0x1409be060]
140769fa3:  48 8B D0                 MOV RDX,RAX
140769fa6:  0F 28 D7                 MOVAPS XMM2,XMM7
140769fa9:  FF 15 A1 28 10 00        CALL qword ptr [0x14086c850]
140769faf:  4C 8B BC 24 38 01 00 00  MOV R15,qword ptr [RSP + 0x138]
140769fb7:  84 C0                    TEST AL,AL
140769fb9:  0F 84 13 01 00 00        JZ 0x14076a0d2
140769fbf:  38 1D CC AE 2E 00        CMP byte ptr [0x140a54e91],BL
140769fc5:  0F 28 05 F4 24 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140769fcc:  0F 11 05 39 43 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140769fd3:  0F 84 07 01 00 00        JZ 0x14076a0e0
140769fd9:  41 38 5D 20              CMP byte ptr [R13 + 0x20],BL
140769fdd:  0F 84 FD 00 00 00        JZ 0x14076a0e0
140769fe3:  38 5E 14                 CMP byte ptr [RSI + 0x14],BL
140769fe6:  75 48                    JNZ 0x14076a030
140769fe8:  48 8D 15 E1 3F 19 00     LEA RDX,[0x1408fdfd0]
140769fef:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
140769ff6:  FF 15 14 2D 10 00        CALL qword ptr [0x14086cd10]
140769ffc:  84 C0                    TEST AL,AL
140769ffe:  74 30                    JZ 0x14076a030
14076a000:  48 8D 15 C9 3F 19 00     LEA RDX,[0x1408fdfd0]
14076a007:  48 8D 8E 10 FD 00 00     LEA RCX,[RSI + 0xfd10]
14076a00e:  FF 15 04 2D 10 00        CALL qword ptr [0x14086cd18]
14076a014:  48 8B 8E 08 FD 00 00     MOV RCX,qword ptr [RSI + 0xfd08]
14076a01b:  41 0F 28 D9              MOVAPS XMM3,XMM9
14076a01f:  48 8B D0                 MOV RDX,RAX
14076a022:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
14076a027:  41 B0 01                 MOV R8B,0x1
14076a02a:  FF 15 78 32 10 00        CALL qword ptr [0x14086d2a8]
14076a030:  45 84 E4                 TEST R12B,R12B
14076a033:  74 0B                    JZ 0x14076a040
14076a035:  0F 2F FE                 COMISS XMM7,XMM6
14076a038:  76 0F                    JBE 0x14076a049
14076a03a:  41 0F 28 F1              MOVAPS XMM6,XMM9
14076a03e:  EB 09                    JMP 0x14076a049
14076a040:  0F 2F FE                 COMISS XMM7,XMM6
14076a043:  76 04                    JBE 0x14076a049
14076a045:  41 0F 28 F3              MOVAPS XMM6,XMM11
14076a049:  F3 41 0F 58 F5           ADDSS XMM6,XMM13
14076a04e:  41 0F 2F F1              COMISS XMM6,XMM9
14076a052:  76 04                    JBE 0x14076a058
14076a054:  41 0F 28 F1              MOVAPS XMM6,XMM9
14076a058:  48 8B 87 40 0D 00 00     MOV RAX,qword ptr [RDI + 0xd40]
14076a05f:  48 2B 87 38 0D 00 00     SUB RAX,qword ptr [RDI + 0xd38]
14076a066:  48 C1 F8 03              SAR RAX,0x3
14076a06a:  48 85 C0                 TEST RAX,RAX
14076a06d:  74 71                    JZ 0x14076a0e0
14076a06f:  48 8B D3                 MOV RDX,RBX
14076a072:  0F 1F 40 00              NOP dword ptr [RAX]
14076a076:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
14076a080:  48 8B 87 38 0D 00 00     MOV RAX,qword ptr [RDI + 0xd38]
14076a087:  4C 8B 04 02              MOV R8,qword ptr [RDX + RAX*0x1]
14076a08b:  49 8B 40 08              MOV RAX,qword ptr [R8 + 0x8]
14076a08f:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
14076a096:  48 85 C9                 TEST RCX,RCX
14076a099:  74 09                    JZ 0x14076a0a4
14076a09b:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
14076a0a2:  74 0C                    JZ 0x14076a0b0
14076a0a4:  49 8B 40 10              MOV RAX,qword ptr [R8 + 0x10]
14076a0a8:  F3 0F 11 B0 CC 00 00 00  MOVSS dword ptr [RAX + 0xcc],XMM6
14076a0b0:  48 8B 8F 40 0D 00 00     MOV RCX,qword ptr [RDI + 0xd40]
14076a0b7:  FF C3                    INC EBX
14076a0b9:  48 2B 8F 38 0D 00 00     SUB RCX,qword ptr [RDI + 0xd38]
14076a0c0:  48 83 C2 08              ADD RDX,0x8
14076a0c4:  48 C1 F9 03              SAR RCX,0x3
14076a0c8:  48 63 C3                 MOVSXD RAX,EBX
14076a0cb:  48 3B C1                 CMP RAX,RCX
14076a0ce:  72 B0                    JC 0x14076a080
14076a0d0:  EB 0E                    JMP 0x14076a0e0
14076a0d2:  0F 28 05 27 20 1A 00     MOVAPS XMM0,xmmword ptr [0x14090c100]
14076a0d9:  0F 11 05 2C 42 25 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14076a0e0:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14076a0e5:  48 8D 0D 74 3F 25 00     LEA RCX,[0x1409be060]
14076a0ec:  F3 0F 11 7C 24 28        MOVSS dword ptr [RSP + 0x28],XMM7
14076a0f2:  41 0F 28 D9              MOVAPS XMM3,XMM9
14076a0f6:  0F 28 D7                 MOVAPS XMM2,XMM7
14076a0f9:  F3 44 0F 11 4C 24 20     MOVSS dword ptr [RSP + 0x20],XMM9
14076a100:  0F 28 CF                 MOVAPS XMM1,XMM7
14076a103:  FF 15 57 27 10 00        CALL qword ptr [0x14086c860]
14076a109:  F3 0F 10 05 77 7F 22 00  MOVSS XMM0,dword ptr [0x140992088]
14076a111:  F3 0F 59 05 D7 08 1A 00  MULSS XMM0,dword ptr [0x14090a9f0]
14076a119:  4C 8B B4 24 40 01 00 00  MOV R14,qword ptr [RSP + 0x140]
14076a121:  F3 41 0F 58 C2           ADDSS XMM0,XMM10
14076a126:  F3 0F 5C 44 24 64        SUBSS XMM0,dword ptr [RSP + 0x64]
14076a12c:  48 8B 4D 98              MOV RCX,qword ptr [RBP + -0x68]
14076a130:  48 33 CC                 XOR RCX,RSP
14076a133:  E8 68 3D 0E 00           CALL 0x14084dea0
14076a138:  4C 8D 9C 24 48 01 00 00  LEA R11,[RSP + 0x148]
14076a140:  41 0F 28 73 D8           MOVAPS XMM6,xmmword ptr [R11 + -0x28]
14076a145:  41 0F 28 7B C8           MOVAPS XMM7,xmmword ptr [R11 + -0x38]
14076a14a:  45 0F 28 4B A8           MOVAPS XMM9,xmmword ptr [R11 + -0x58]
14076a14f:  45 0F 28 53 98           MOVAPS XMM10,xmmword ptr [R11 + -0x68]
14076a154:  45 0F 28 5B 88           MOVAPS XMM11,xmmword ptr [R11 + -0x78]
14076a159:  45 0F 28 AB 68 FF FF FF  MOVAPS XMM13,xmmword ptr [R11 + -0x98]
14076a161:  49 8B E3                 MOV RSP,R11
14076a164:  41 5D                    POP R13
14076a166:  41 5C                    POP R12
14076a168:  5F                       POP RDI
14076a169:  5E                       POP RSI
14076a16a:  5B                       POP RBX
14076a16b:  5D                       POP RBP
14076a16c:  C3                       RET

```
