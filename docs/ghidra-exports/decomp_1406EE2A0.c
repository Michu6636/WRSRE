// ============================================================
// Function: FUN_1406ee2a0 at 1406ee2a0
// Target address checked: 0x1406EE2A0
// ============================================================

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined2 *
FUN_1406ee2a0(longlong param_1,uint param_2,longlong param_3,int param_4,uint param_5,int param_6,
             int param_7,short *param_8,longlong param_9)

{
  longlong *plVar1;
  undefined4 uVar2;
  short sVar3;
  undefined1 *puVar4;
  longlong lVar5;
  undefined1 auVar6 [16];
  float fVar7;
  uint uVar8;
  undefined8 *puVar9;
  bool bVar10;
  char cVar11;
  int iVar12;
  char *pcVar13;
  void *pvVar14;
  undefined2 *puVar15;
  __uint64 _Var16;
  undefined8 uVar17;
  undefined8 *puVar18;
  uint uVar19;
  ulonglong uVar20;
  longlong lVar21;
  longlong *plVar22;
  longlong lVar23;
  uint uVar24;
  uint uVar25;
  ulonglong uVar26;
  ulonglong uVar27;
  longlong *plVar28;
  int iVar29;
  short *psVar30;
  uint uVar31;
  ulonglong uVar32;
  ulonglong uVar33;
  longlong lVar34;
  undefined1 uVar35;
  ulonglong *puVar36;
  float fVar37;
  float fVar38;
  float fVar39;
  float fVar40;
  float fVar41;
  float fVar42;
  longlong local_res8;
  uint local_res10;
  int local_res20 [2];
  longlong local_c8 [18];
  
  lVar21 = *(longlong *)(param_1 + 0xc650);
  uVar27 = 0;
  local_res8 = param_1;
  local_res10 = param_2;
  local_res20[0] = param_4;
  if (lVar21 == 0) {
    if (param_4 == 0) goto LAB_1406ee375;
  }
  else if (param_4 == 0) {
    if (((param_3 != 0) && (*(longlong *)(param_3 + 0x318) != 0)) &&
       (*(int *)(*(longlong *)(param_3 + 0x318) + 0x360) == 0x27)) {
      iVar12 = strcmp((char *)(lVar21 + 1),"population");
      uVar32 = uVar27;
      if (iVar12 != 0) {
        do {
          uVar20 = uVar32 + 1;
          if (*(char *)(lVar21 + 1 + uVar32) != "trade"[uVar32]) goto LAB_1406ee375;
          uVar32 = uVar20;
        } while (uVar20 != 6);
      }
      *(undefined8 *)(param_1 + 0xc650) = 0;
    }
LAB_1406ee375:
    if (((param_2 == 4) && (param_3 != 0)) &&
       ((*(longlong *)(param_3 + 0x318) != 0 &&
        ((*(int *)(*(longlong *)(param_3 + 0x318) + 0x360) == 0xf &&
         (uVar32 = uVar27,
         *(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3 != 0)))))) {
      do {
        puVar4 = *(undefined1 **)(*(longlong *)(param_1 + 0xcac0) + uVar32 * 8);
        if ((*(int *)(puVar4 + 0x248) == 0) &&
           ((((*(int *)(puVar4 + 0x24c) == 4 && (*(longlong *)(puVar4 + 0x240) != param_3)) &&
             (lVar21 = *(longlong *)(*(longlong *)(puVar4 + 0x240) + 0x318), lVar21 != 0)) &&
            (*(int *)(lVar21 + 0x360) == 0xf)))) {
          *puVar4 = 1;
        }
        uVar32 = (ulonglong)((int)uVar32 + 1);
      } while (uVar32 < (ulonglong)
                        (*(longlong *)(param_1 + 0xcac8) - *(longlong *)(param_1 + 0xcac0) >> 3));
    }
  }
  iVar12 = local_res20[0];
  bVar10 = false;
  plVar1 = (longlong *)(param_1 + 0xcac0);
  if (*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3 != 0) {
    do {
      puVar4 = *(undefined1 **)(*plVar1 + uVar27 * 8);
      if (((*(longlong *)(puVar4 + 0x240) == param_3) ||
          (uVar24 = *(uint *)(puVar4 + 0x23c), uVar24 - 0xc < 2)) &&
         (((uVar24 = *(uint *)(puVar4 + 0x23c), uVar24 == param_5 &&
           ((*(int *)(puVar4 + 0x34) == param_6 && (*(int *)(puVar4 + 0x38) == param_7)))) &&
          (param_7 != 0x28)))) {
        puVar4[1] = 0;
        *(undefined4 *)(puVar4 + 0x14) = 0x3a83126f;
        FUN_1406ed120(param_1,uVar27);
        uVar24 = *(uint *)(puVar4 + 0x23c);
        if (uVar24 - 0xb < 3) {
          *(undefined4 *)(puVar4 + 0x264) = 0xffffffff;
          *(undefined4 *)(puVar4 + 0x268) = 0;
        }
LAB_1406ee4f9:
        bVar10 = true;
      }
      else if (((!bVar10) && (iVar12 == 7)) && (*(int *)(puVar4 + 0x248) == 7)) {
        *(longlong *)(puVar4 + 0x240) = param_3;
        goto LAB_1406ee4f9;
      }
      if ((param_5 < 0xe) && ((0x3ee6U >> (param_5 & 0x1f) & 1) != 0)) {
        if ((uVar24 == param_5) && (param_5 - 0xb < 3)) {
          *(undefined4 *)(puVar4 + 0x264) = 0xfffffffe;
        }
        else if ((uVar24 < 0xe) && ((0x3ee6U >> (uVar24 & 0x1f) & 1) != 0)) {
          *puVar4 = 1;
          *(undefined4 *)(param_1 + 0xfc8) = 0xbf000000;
        }
      }
      if ((param_5 - 3 < 2) && (*(int *)(puVar4 + 0x23c) - 3U < 2)) {
        *puVar4 = 1;
        *(undefined4 *)(param_1 + 0xfc8) = 0xbf000000;
      }
      uVar27 = (ulonglong)((int)uVar27 + 1);
    } while (uVar27 < (ulonglong)(*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3));
    param_2 = local_res10;
    if (bVar10) {
      return (undefined2 *)0x0;
    }
  }
  if (((param_2 < 2) && (*(char *)(param_1 + 0x14) == '\0')) &&
     (bVar10 = C3D_CONFIGURATOR::GetValueExist
                         ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_window_open"), bVar10)) {
    pcVar13 = C3D_CONFIGURATOR::GetValueString
                        ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_window_open");
    C3D_SOUNDMANAGER::PlaySound2D
              (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar13,true,DAT_140909f58,(char *)0x0);
  }
  pvVar14 = operator_new(0x5d8);
  puVar15 = (undefined2 *)FUN_14025c500(pvVar14);
  *(undefined4 *)(puVar15 + 0x2e0) = 0;
  *(undefined1 *)(puVar15 + 0x2e2) = 0;
  *(undefined8 *)(puVar15 + 0x292) = 0;
  *(longlong *)(puVar15 + 0x120) = param_3;
  *(int *)(puVar15 + 0x124) = local_res20[0];
  *(uint *)(puVar15 + 0x126) = param_2;
  *(undefined4 *)(puVar15 + 300) = 0;
  *(undefined4 *)(puVar15 + 0x12e) = 0xffffffff;
  *(int *)(puVar15 + 0x1a) = param_6;
  *(int *)(puVar15 + 0x1c) = param_7;
  puVar15[0x1e] = 0;
  if (param_8 != (short *)0x0) {
    if (param_6 == 0x23) {
      lVar21 = -1;
      do {
        lVar34 = lVar21;
        lVar21 = lVar34 + 1;
      } while (param_8[lVar21] != 0);
      auVar6._8_8_ = 0;
      auVar6._0_8_ = lVar34 + 2;
      _Var16 = SUB168(ZEXT816(2) * auVar6,0);
      if (SUB168(ZEXT816(2) * auVar6,8) != 0) {
        _Var16 = 0xffffffffffffffff;
      }
      pvVar14 = operator_new(_Var16);
      *(void **)(puVar15 + 0x2e8) = pvVar14;
      lVar21 = (longlong)pvVar14 - (longlong)param_8;
      do {
        sVar3 = *param_8;
        *(short *)(lVar21 + (longlong)param_8) = sVar3;
        param_8 = param_8 + 1;
      } while (sVar3 != 0);
    }
    else {
      psVar30 = param_8;
      do {
        sVar3 = *psVar30;
        *(short *)((longlong)puVar15 + (0x3c - (longlong)param_8) + (longlong)psVar30) = sVar3;
        psVar30 = psVar30 + 1;
      } while (sVar3 != 0);
    }
  }
  fVar7 = DAT_140909ddc;
  *(float *)(puVar15 + 2) = (float)(int)DAT_14099528c * DAT_140909ddc;
  *(float *)(puVar15 + 4) = (float)(int)DAT_140995274 * fVar7;
  *(undefined4 *)(puVar15 + 6) = 0;
  *(undefined4 *)(puVar15 + 0x130) = 0xffffffff;
  *(undefined4 *)(puVar15 + 0x132) = 0xffffffff;
  *(undefined8 *)(puVar15 + 0x138) = 0;
  *(undefined4 *)(puVar15 + 0x134) = 0;
  *(undefined4 *)(puVar15 + 0x13c) = 0xffffffff;
  *(undefined8 *)(puVar15 + 0x140) = 0;
  *(undefined4 *)(puVar15 + 0x144) = 0xffffffff;
  *(undefined8 *)(puVar15 + 0x148) = 0;
  *(undefined4 *)(puVar15 + 0x14c) = 0xffffffff;
  *(undefined8 *)(puVar15 + 0x150) = 0;
  *(undefined8 *)(puVar15 + 0x154) = 0;
  *(undefined8 *)(puVar15 + 0x158) = 0;
  *(undefined4 *)(puVar15 + 10) = 0;
  *puVar15 = 0;
  bVar10 = C3D_INPUT::CommonInterfaceGetButtonDown
                     ((C3D_INPUT *)&DAT_140a54b90,"BUILDING_SLOW_ROTATE");
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  if (iVar12 == 0) {
    *(undefined1 *)(puVar15 + 0x278) = 0;
  }
  else {
    if (((*(int *)(puVar15 + 0x126) == 1) || (bVar10)) || (*(int *)(puVar15 + 0x1a) == 0x1d)) {
      uVar35 = 0;
    }
    else {
      uVar35 = 1;
    }
    *(undefined1 *)(puVar15 + 0x278) = uVar35;
  }
  *(undefined4 *)(puVar15 + 0x27a) = 2;
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  if (iVar12 == 0) {
    *(undefined8 *)(puVar15 + 0x2e4) = 0;
  }
  else {
    *(longlong *)(puVar15 + 0x2e4) = param_9;
  }
  if (param_9 != 0) {
    *(undefined8 *)(puVar15 + 0x160) = *(undefined8 *)(param_9 + 0x2c0);
    *(undefined8 *)(puVar15 + 0x164) = *(undefined8 *)(param_9 + 0x2c8);
  }
  *(undefined8 *)(puVar15 + 0xc) = 0;
  *(undefined4 *)(puVar15 + 0x12) = 0;
  *(undefined8 *)(puVar15 + 0x128) = 0;
  *(uint *)(puVar15 + 0x11e) = param_5;
  *(undefined4 *)(puVar15 + 0x254) = 0;
  *(undefined4 *)(puVar15 + 0x256) = 0xffffffff;
  *(undefined8 *)(puVar15 + 600) = 0;
  *(undefined8 *)(puVar15 + 0x25c) = 0;
  fVar40 = DAT_140992088;
  fVar42 = DAT_14090b03c;
  if (param_5 != 0) {
    fVar37 = DAT_140992088 * DAT_14090b03c;
    *(float *)(puVar15 + 0x292) = fVar37;
    *(float *)(puVar15 + 0x294) = fVar37;
  }
  if (((local_res20[0] == 0) && (param_3 != 0)) &&
     ((*(longlong *)(param_3 + 0x318) != 0 &&
      (*(int *)(*(longlong *)(param_3 + 0x318) + 0x360) == 0x69)))) {
    *(float *)(puVar15 + 0x292) = fVar40 * fVar42;
    *(float *)(puVar15 + 0x294) = fVar40 * fVar42;
  }
  *(undefined4 *)(puVar15 + 0x252) = 0xf;
  *(undefined8 *)(puVar15 + 0x168) = 0;
  *(undefined8 *)(puVar15 + 0x16c) = 0;
  *(undefined8 *)(puVar15 + 0x170) = 0;
  *(undefined8 *)(puVar15 + 0x174) = 0;
  *(undefined8 *)(puVar15 + 0x178) = 0;
  *(undefined8 *)(puVar15 + 0x296) = 0;
  *(undefined8 *)(puVar15 + 0x29a) = 0;
  *(undefined8 *)(puVar15 + 0x29e) = 0;
  *(undefined8 *)(puVar15 + 0x2a2) = 0;
  *(undefined8 *)(puVar15 + 0x2a6) = 0;
  *(undefined8 *)(puVar15 + 0x2aa) = 0;
  *(undefined8 *)(puVar15 + 0x2ae) = 0;
  *(undefined8 *)(puVar15 + 0x2b2) = 0;
  *(undefined8 *)(puVar15 + 0x2b6) = 0;
  *(undefined8 *)(puVar15 + 0x2ba) = 0;
  fVar42 = DAT_14090aa78;
  fVar37 = 0.0;
  if ((local_res20[0] == 0) && (param_3 != 0)) {
    *(undefined4 *)(puVar15 + 0x2aa) = *(undefined4 *)(param_3 + 0x1310);
    fVar38 = 0.0;
    if ((*(float *)(param_3 + 0x1310) != 0.0) &&
       ((iVar12 = *(int *)(*(longlong *)(param_3 + 0x318) + 0x83c), -1 < iVar12 &&
        ((ulonglong)(longlong)iVar12 <
         (ulonglong)((*(longlong *)(param_3 + 0x978) - *(longlong *)(param_3 + 0x970)) / 0xe0))))) {
      plVar22 = (longlong *)((longlong)iVar12 * 0xe0 + *(longlong *)(param_3 + 0x970));
      if (((int)plVar22[0x12] - 0xfU < 2) &&
         (((plVar22[1] - *plVar22 >> 4 == 2 &&
           (plVar28 = (longlong *)*plVar22, *plVar28 == *(longlong *)(param_1 + 0xc388))) &&
          (plVar28[2] == *(longlong *)(param_1 + 0xc390))))) {
        fVar38 = (*(float *)(plVar28 + 3) + *(float *)(plVar28 + 1)) /
                 *(float *)((longlong)plVar22 + 0x8c);
      }
      *(float *)(puVar15 + 700) = fVar38;
    }
    if (((*(longlong *)(param_3 + 0x318) != 0) &&
        ((iVar12 = *(int *)(*(longlong *)(param_3 + 0x318) + 0x360), iVar12 == 0x62 ||
         (iVar12 - 0x5dU < 3)))) &&
       (uVar27 = (*(longlong *)(param_3 + 0x978) - *(longlong *)(param_3 + 0x970)) / 0xe0,
       (int)uVar27 != 0)) {
      do {
        uVar24 = (int)uVar27 - 1;
        uVar27 = (ulonglong)uVar24;
        if (*(int *)(uVar27 * 0xe0 + 0x90 + *(longlong *)(param_3 + 0x970)) == 0x10) {
          plVar22 = (longlong *)((ulonglong)uVar24 * 0xe0 + *(longlong *)(param_3 + 0x970));
          if ((((int)plVar22[0x12] - 0xfU < 2) && (plVar22[1] - *plVar22 >> 4 == 2)) &&
             ((plVar28 = (longlong *)*plVar22, *plVar28 == *(longlong *)(param_1 + 0xc388) &&
              (plVar28[2] == *(longlong *)(param_1 + 0xc390))))) {
            fVar37 = (*(float *)(plVar28 + 3) + *(float *)(plVar28 + 1)) /
                     *(float *)((longlong)plVar22 + 0x8c);
          }
          *(float *)(puVar15 + 700) = fVar37;
          break;
        }
      } while (uVar24 != 0);
    }
    if ((*(longlong *)(param_3 + 0x318) != 0) &&
       (*(int *)(*(longlong *)(param_3 + 0x318) + 0x360) == 0x5f)) {
      *(float *)(puVar15 + 0x2ac) = *(float *)(param_3 + 0x1318) * fVar42;
    }
  }
  if ((((*(int *)(puVar15 + 0x1a) - 7U & 0xfffffffa) == 0) && (*(int *)(puVar15 + 0x1a) != 0xb)) ||
     ((local_res20[0] == 0 && ((param_5 < 0xb && ((0x6e6U >> (param_5 & 0x1f) & 1) != 0)))))) {
    pvVar14 = operator_new(0x270);
    uVar17 = FUN_1400fd890(pvVar14,DAT_140995240,0xffaa0000);
    *(undefined8 *)(puVar15 + 0x148) = uVar17;
    fVar40 = DAT_140992088;
  }
  fVar41 = DAT_14090b840;
  fVar38 = DAT_14090b67c;
  fVar37 = DAT_140909ed4;
  if (*(int *)(puVar15 + 0x126) == 5) {
    *(float *)(puVar15 + 0x14) = fVar40 * DAT_14090b840 * DAT_140909ed4;
    *(float *)(puVar15 + 0x16) = fVar40 * fVar38;
  }
  if (*(int *)(puVar15 + 0x126) == 1) {
    *(float *)(puVar15 + 0x14) = fVar40 * fVar41;
    *(float *)(puVar15 + 0x16) = fVar40 * fVar38;
  }
  if (*(int *)(puVar15 + 0x126) == 0) {
    *(float *)(puVar15 + 0x14) = fVar40 * fVar41;
    *(float *)(puVar15 + 0x16) = fVar40 * fVar38;
  }
  if (*(int *)(puVar15 + 0x126) == 2) {
    *(float *)(puVar15 + 0x14) = fVar40 * _DAT_14090b8e0;
    *(float *)(puVar15 + 0x16) = fVar40 * _DAT_14090b864;
    *(undefined8 *)(param_1 + 0xc8e8) = 0;
    *(undefined8 *)(param_1 + 0xc8e0) = 0;
  }
  if (*(int *)(puVar15 + 0x126) == 3) {
    *(float *)(puVar15 + 0x14) = fVar40 * _DAT_14090b99c;
    *(float *)(puVar15 + 0x16) = fVar40 * DAT_14090b8ac;
    *(undefined8 *)(param_1 + 0xc8e8) = 0;
    *(undefined8 *)(param_1 + 0xc8e0) = 0;
  }
  if (*(int *)(puVar15 + 0x126) == 4) {
    *(float *)(puVar15 + 0x14) = fVar40 * DAT_14090bb14;
    *(float *)(puVar15 + 0x16) = fVar40 * _DAT_14090b96c;
    if ((*(int *)(puVar15 + 0x1a) - 0x27U & 0xfffffffb) == 0) {
      *(float *)(puVar15 + 0x16) = fVar40 * DAT_14090b778;
    }
    *(undefined8 *)(param_1 + 0xc8e8) = 0;
    *(undefined8 *)(param_1 + 0xc8e0) = 0;
  }
  FUN_1408177b0(puVar15);
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_OPEN_ON_LEFT");
  iVar29 = 1;
  if (iVar12 != 0) {
    iVar29 = -1;
  }
  if ((param_9 != 0) &&
     (iVar29 = 1,
     *(float *)(param_9 + 4) <= (float)(int)DAT_14099528c * fVar7 &&
     (float)(int)DAT_14099528c * fVar7 != *(float *)(param_9 + 4))) {
    iVar29 = -1;
  }
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  fVar40 = DAT_140992088;
  if (iVar12 == 0) {
    *(float *)(puVar15 + 2) = DAT_140992088 * DAT_14090ae1c;
    fVar38 = fVar40 * DAT_14090aca8;
    *(float *)(puVar15 + 4) = fVar38;
  }
  else if (param_9 == 0) {
    fVar38 = DAT_140992088 * _DAT_14090ae54 - *(float *)(puVar15 + 0x16);
    *(float *)(puVar15 + 4) = fVar38;
    iVar12 = C3D_CONFIGURATOR::GetValueInteger
                       ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_OPEN_ON_LEFT");
    fVar40 = DAT_140992088;
    if (iVar12 == 0) {
      if (*(int *)(param_1 + 0x13178) == -1) {
        *(float *)(puVar15 + 2) =
             ((float)(int)DAT_14099528c + *(float *)(puVar15 + 0x14)) -
             DAT_140992088 * DAT_14090a9f0;
      }
      else {
        *(float *)(puVar15 + 2) =
             ((float)(int)DAT_14099528c + *(float *)(puVar15 + 0x14)) -
             DAT_140992088 * _DAT_14090af0c;
      }
    }
    else if (*(longlong *)(param_1 + 0xc650) == 0) {
      *(float *)(puVar15 + 2) =
           (DAT_140992088 * DAT_14090ab2c - *(float *)(puVar15 + 0x14)) +
           DAT_140992088 * DAT_14090a9f0;
    }
    else {
      *(float *)(puVar15 + 2) =
           (DAT_140992088 * *(float *)(param_1 + 0xae54) - *(float *)(puVar15 + 0x14)) +
           DAT_140992088 * DAT_14090a9f0;
    }
  }
  else {
    *(float *)(puVar15 + 4) = DAT_140992088 * _DAT_14090ae54 - *(float *)(puVar15 + 0x16);
    *(float *)(puVar15 + 2) =
         ((*(float *)(puVar15 + 0x14) + *(float *)(param_9 + 0x28)) - fVar40 * fVar42) *
         (float)iVar29 + *(float *)(param_9 + 4);
    fVar38 = *(float *)(puVar15 + 4);
  }
  fVar41 = fVar40 * *(float *)(param_1 + 0xae54) + fVar40 * DAT_14090a928;
  if (*(longlong *)(param_1 + 0xc650) == 0) {
    fVar41 = fVar40 * DAT_14090a928;
  }
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  if ((iVar12 == 0) && (*(float *)(puVar15 + 2) <= fVar41 && fVar41 != *(float *)(puVar15 + 2))) {
    *(float *)(puVar15 + 2) = fVar41;
  }
  uVar2 = *(undefined4 *)(puVar15 + 2);
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  uVar27 = 0;
  uVar24 = 0;
  if ((((iVar12 != 0) && (bVar10 = false, *(int *)(puVar15 + 0x124) != 5)) && (param_9 == 0)) &&
     (uVar32 = *(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3, (int)uVar32 != 0)) {
    puVar36 = (ulonglong *)(*plVar1 + (uVar32 & 0xffffffff) * 8);
    uVar20 = uVar27;
    do {
      uVar31 = (int)uVar32 - 1;
      uVar32 = (ulonglong)uVar31;
      puVar36 = puVar36 + -1;
      uVar33 = *puVar36;
      uVar26 = uVar20;
      if ((*(char *)(uVar33 + 0x4f0) != '\0') && (uVar33 != 0)) {
        uVar26 = uVar33;
        if ((uVar20 != 0) &&
           (uVar26 = uVar20,
           *(float *)(uVar33 + 8) <= *(float *)(uVar20 + 8) &&
           *(float *)(uVar20 + 8) != *(float *)(uVar33 + 8))) {
          uVar26 = uVar33;
        }
        if (!bVar10) {
          if (*(float *)(uVar33 + 8) == fVar38) {
            bVar10 = true;
          }
          else {
            bVar10 = false;
          }
        }
      }
      uVar20 = uVar26;
    } while (uVar31 != 0);
    if ((uVar26 != 0) && (bVar10)) {
      *(undefined8 *)(puVar15 + 2) = *(undefined8 *)(uVar26 + 4);
      *(undefined4 *)(puVar15 + 6) = *(undefined4 *)(uVar26 + 0xc);
    }
  }
  if (((*(int *)(puVar15 + 0x126) != 1) && (1 < *(int *)(puVar15 + 0x1a) - 0x29U)) &&
     ((*(int *)(puVar15 + 0x124) - 5U & 0xfffffffd) != 0)) {
    iVar12 = C3D_CONFIGURATOR::GetValueInteger
                       ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
    fVar41 = DAT_140992088;
    fVar38 = DAT_14090aa28;
    fVar40 = DAT_14090a8cc;
    if (iVar12 == 0) {
      uVar32 = (ulonglong)DAT_140995274;
      uVar20 = (ulonglong)DAT_14099528c;
LAB_1406ef080:
      do {
        uVar17 = 1;
        uVar33 = uVar27;
        if (*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3 == 0) break;
        do {
          if ((*(longlong *)(*plVar1 + uVar33 * 8) != *(longlong *)(puVar15 + 0x2e4)) &&
             (lVar21 = *(longlong *)(*plVar1 + uVar33 * 8), *(char *)(lVar21 + 0x4f0) == '\0')) {
            fVar39 = fVar41 * fVar40;
            fVar42 = *(float *)(puVar15 + 2);
            cVar11 = FUN_1406f48c0(fVar39,lVar21,fVar42 + fVar39,fVar39 + *(float *)(puVar15 + 4));
            if (cVar11 != '\0') {
              uVar17 = 0;
              fVar42 = fVar42 + fVar41 * fVar38;
              *(float *)(puVar15 + 2) = fVar42;
              if ((float)(int)uVar20 * fVar37 < fVar42) {
                *(undefined4 *)(puVar15 + 2) = uVar2;
                *(float *)(puVar15 + 4) = fVar41 * fVar38 + *(float *)(puVar15 + 4);
              }
              if ((float)(int)uVar32 * fVar37 < *(float *)(puVar15 + 4)) goto LAB_1406ef080;
            }
          }
          uVar33 = (ulonglong)((int)uVar33 + 1);
        } while (uVar33 < (ulonglong)(*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3));
      } while ((char)uVar17 == '\0');
    }
    else if ((param_9 != 0) &&
            (uVar32 = *(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3, (int)uVar32 != 0)) {
      lVar21 = (uVar32 & 0xffffffff) << 3;
      do {
        uVar31 = (int)uVar32 - 1;
        uVar32 = (ulonglong)uVar31;
        lVar21 = lVar21 + -8;
        if ((*(longlong *)(*(longlong *)(lVar21 + *plVar1) + 0x5c8) == param_9) &&
           (*(int *)(*(longlong *)(lVar21 + *plVar1) + 0x23c) == 0)) {
          *(float *)(puVar15 + 2) = *(float *)(puVar15 + 2) - (float)iVar29 * fVar41 * fVar42;
          *(float *)(puVar15 + 4) = fVar41 * fVar42 + *(float *)(puVar15 + 4);
        }
      } while (uVar31 != 0);
    }
  }
  iVar12 = C3D_CONFIGURATOR::GetValueInteger
                     ((C3D_CONFIGURATOR *)&DAT_1409bdfc0,"WINDOW_AUTOCLOSING");
  if (iVar12 == 0) {
    *(float *)(puVar15 + 2) = *(float *)(puVar15 + 2) - *(float *)(puVar15 + 0x14);
    *(float *)(puVar15 + 4) = *(float *)(puVar15 + 4) - *(float *)(puVar15 + 0x16);
  }
  uVar8 = DAT_14099528c;
  uVar31 = DAT_140995274;
  fVar42 = DAT_140992088;
  if (*(int *)(puVar15 + 0x126) == 1) {
    fVar40 = (float)(int)DAT_14099528c;
    *(float *)(puVar15 + 2) = fVar40 * fVar7;
    *(float *)(puVar15 + 4) = (float)(int)uVar31 * fVar7;
    uVar32 = *(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3;
    uVar19 = uVar24;
    if ((int)uVar32 != 0) {
      plVar22 = (longlong *)(*plVar1 + (uVar32 & 0xffffffff) * 8);
      uVar20 = uVar27;
      do {
        plVar22 = plVar22 + -1;
        uVar19 = (uint)uVar20 + 1;
        if (*(int *)(*plVar22 + 0x24c) != 1) {
          uVar19 = (uint)uVar20;
        }
        uVar20 = (ulonglong)uVar19;
        uVar25 = (int)uVar32 - 1;
        uVar32 = (ulonglong)uVar25;
      } while (uVar25 != 0);
    }
    fVar37 = (float)(int)(uVar19 * 0x28) * fVar42;
    *(float *)(puVar15 + 2) = fVar40 * fVar7 - fVar37;
    *(float *)(puVar15 + 4) = fVar37 + *(float *)(puVar15 + 4);
  }
  if (((param_6 - 0x17U & 0xffffffef) == 0) || (*(int *)(puVar15 + 0x1a) - 0x29U < 3)) {
    *(float *)(puVar15 + 2) = (float)(int)uVar8 * fVar7;
    *(float *)(puVar15 + 4) = (float)(int)uVar31 * fVar7;
    if (*(int *)(puVar15 + 0x1a) == 0x2a) {
      *(float *)(puVar15 + 4) = (float)(int)uVar31 * fVar7 - fVar42 * DAT_14090ad18;
    }
    if ((param_6 == 0x27) || (*(int *)(puVar15 + 0x1a) - 0x29U < 2)) {
      *(undefined1 *)(puVar15 + 0x278) = 0;
    }
  }
  if (param_6 == 0x28) {
    *(undefined1 *)(puVar15 + 0x278) = 0;
    *(float *)(puVar15 + 2) = (float)(int)uVar8 * fVar7;
    *(float *)(puVar15 + 4) = (float)(int)uVar31 * fVar7;
    FUN_140803f30(param_1,puVar15,0);
  }
  *(undefined4 *)(puVar15 + 0x17c) = 0;
  *(undefined4 *)(puVar15 + 0x17e) = 0x1010100;
  *(undefined4 *)(puVar15 + 0x180) = 0x1010101;
  puVar15[0x182] = 0x101;
  lVar21 = 6;
  puVar9 = (undefined8 *)(puVar15 + 0x188);
  do {
    puVar18 = puVar9;
    *puVar18 = 0x200000002;
    puVar18[1] = 0x200000002;
    puVar18[2] = 0x200000002;
    puVar18[3] = 0x200000002;
    puVar18[4] = 0x200000002;
    puVar18[5] = 0x200000002;
    puVar18[6] = 0x200000002;
    puVar18[7] = 0x200000002;
    lVar21 = lVar21 + -1;
    puVar9 = puVar18 + 8;
  } while (lVar21 != 0);
  puVar18[8] = 0x200000002;
  puVar18[9] = 0x200000002;
  *(undefined4 *)(puVar15 + 0x183) = 0;
  *(undefined4 *)(puVar15 + 0x185) = 1;
  puVar15[0x187] = 0;
  *(undefined4 *)(puVar15 + 0x250) = 0x41200000;
  lVar21 = param_1;
  if (*(longlong *)(puVar15 + 0x120) != 0) {
    if (*(int *)(puVar15 + 0x124) != 0) goto LAB_1406ef602;
    lVar34 = *(longlong *)(puVar15 + 0x120);
    if ((*(longlong *)(lVar34 + 0x318) != 0) &&
       ((iVar12 = *(int *)(*(longlong *)(lVar34 + 0x318) + 0x360), iVar12 == 0xc || (iVar12 == 0x6a)
        ))) {
      uVar35 = 0;
      if (*(longlong *)(lVar34 + 0xf80) - *(longlong *)(lVar34 + 0xf78) >> 4 != 0) {
        uVar32 = uVar27;
        do {
          if (*(longlong *)(*(longlong *)(lVar34 + 0xf78) + uVar32 * 0x10) != 0) {
            uVar35 = 1;
            break;
          }
          uVar32 = (ulonglong)((int)uVar32 + 1);
        } while (uVar32 < (ulonglong)
                          (*(longlong *)(lVar34 + 0xf80) - *(longlong *)(lVar34 + 0xf78) >> 4));
      }
      *(undefined1 *)((longlong)puVar15 + 0x2f9) = uVar35;
    }
    if (*(longlong *)(lVar34 + 0x318) != 0) {
      if (*(longlong *)(lVar34 + 0xc78) - *(longlong *)(lVar34 + 0xc70) >> 3 != 0) {
        uVar32 = uVar27;
        uVar20 = uVar27;
        do {
          uVar24 = (uint)uVar32 + 1;
          if (*(longlong *)(*(longlong *)(*(longlong *)(lVar34 + 0xc70) + uVar20 * 8) + 0x398) != 0)
          {
            uVar24 = (uint)uVar32;
          }
          uVar32 = (ulonglong)uVar24;
          uVar20 = (ulonglong)((int)uVar20 + 1);
        } while (uVar20 < (ulonglong)
                          (*(longlong *)(lVar34 + 0xc78) - *(longlong *)(lVar34 + 0xc70) >> 3));
      }
      if (*(int *)(*(longlong *)(lVar34 + 0x318) + 0x820) <= (int)uVar24) {
        *(undefined1 *)(puVar15 + 0x17c) = 1;
      }
    }
  }
  if (((*(int *)(puVar15 + 0x124) == 0) && (lVar34 = *(longlong *)(puVar15 + 0x120), lVar34 != 0))
     && (lVar23 = *(longlong *)(lVar34 + 0x978) - *(longlong *)(lVar34 + 0x970),
        lVar5 = lVar23 >> 0x3f, lVar21 = local_res8, lVar23 / 0xe0 + lVar5 != lVar5)) {
    plVar22 = (longlong *)(puVar15 + 0x2d4);
    do {
      plVar28 = (longlong *)(uVar27 * 0xe0 + *(longlong *)(lVar34 + 0x970));
      local_c8[0] = 0;
      local_c8[1] = 0;
      local_c8[2] = 0;
      FUN_14081e210(plVar22,local_c8);
      if (local_c8[0] != 0) {
        FUN_140001a50(local_c8[0],local_c8[2] - local_c8[0] >> 2,4);
      }
      lVar21 = *(longlong *)(puVar15 + 0x2d8);
      lVar5 = *plVar22;
      lVar23 = *plVar22;
      uVar32 = 0;
      if (plVar28[1] - *plVar28 >> 4 != 0) {
        do {
          local_res20[0] = 0;
          FUN_140042de0(lVar23 + (ulonglong)((int)((lVar21 - lVar5) / 0x18) - 1) * 0x18,local_res20)
          ;
          uVar32 = (ulonglong)((int)uVar32 + 1);
        } while (uVar32 < (ulonglong)(plVar28[1] - *plVar28 >> 4));
      }
      uVar27 = (ulonglong)((int)uVar27 + 1);
      lVar21 = local_res8;
    } while (uVar27 < (ulonglong)
                      ((*(longlong *)(lVar34 + 0x978) - *(longlong *)(lVar34 + 0x970)) / 0xe0));
  }
LAB_1406ef602:
  *(undefined1 *)(puVar15 + 0x28a) = 0;
  uVar27 = 0;
  *(undefined4 *)(puVar15 + 0x28c) = 0;
  *(undefined4 *)(puVar15 + 0x284) = 0;
  *(undefined1 *)(puVar15 + 0x27c) = 0;
  if ((*(int *)(puVar15 + 0x1a) == 8) &&
     (uVar32 = uVar27, *(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3 != 0)) {
    do {
      if (*(int *)(*(undefined1 **)(uVar27 + *plVar1) + 0x34) == 8) {
        **(undefined1 **)(uVar27 + *plVar1) = 1;
      }
      uVar24 = (int)uVar32 + 1;
      uVar27 = uVar27 + 8;
      uVar32 = (ulonglong)uVar24;
    } while ((ulonglong)(longlong)(int)uVar24 <
             (ulonglong)(*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3));
  }
  if ((*(int *)(puVar15 + 0x1a) == 9) && (*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3 != 0)) {
    lVar34 = *plVar1 + 8;
  }
  else {
    lVar34 = *plVar1;
  }
  FUN_14001e140(plVar1,&local_res8,lVar34);
  iVar12 = 1;
  if (1 < (int)(*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3)) {
    lVar34 = 8;
    do {
      *(undefined4 *)(*(longlong *)(*plVar1 + lVar34) + 0x18) = 0;
      iVar12 = iVar12 + 1;
      lVar34 = lVar34 + 8;
    } while (iVar12 < (int)(*(longlong *)(param_1 + 0xcac8) - *plVar1 >> 3));
  }
  *(undefined8 *)(lVar21 + 0xcb48) = 0;
  if (puVar15 != (undefined2 *)0x0) {
    *(undefined4 *)(puVar15 + 0xc) = 0x3f19999a;
    *(undefined8 *)(lVar21 + 0xadc8) = 1;
    *(undefined4 *)(lVar21 + 0xadf0) = 5;
    bVar10 = C3D_INPUT::CommonInterfaceGetButtonDown((C3D_INPUT *)&DAT_140a54b90,"GAMEPAD_CONFIRM");
    if (bVar10) {
      *(undefined4 *)(lVar21 + 0xadf4) = 3;
    }
    *(undefined8 *)(lVar21 + 0xae34) = *(undefined8 *)(puVar15 + 2);
    *(undefined4 *)(lVar21 + 0xae3c) = *(undefined4 *)(puVar15 + 6);
    if (((*(int *)(lVar21 + 0xade8) != 0) && ((*(int *)(puVar15 + 0x1a) - 0x27U & 0xfffffffc) == 0))
       && (*(int *)(puVar15 + 0x1a) != 0x28)) {
      *(undefined4 *)(lVar21 + 0xadf4) = 3;
    }
    if ((*(uint *)(puVar15 + 0x11e) < 0xb) &&
       ((0x6e6U >> (*(uint *)(puVar15 + 0x11e) & 0x1f) & 1) != 0)) {
      *(undefined4 *)(lVar21 + 0xadc8) = 6;
    }
    if (*(int *)(puVar15 + 0x124) == 7) {
      *(undefined4 *)(lVar21 + 0xadc8) = 0;
    }
    if (*(int *)(puVar15 + 0x1a) == 0x27) {
      *(undefined4 *)(lVar21 + 0xadc8) = 1;
    }
  }
  return puVar15;
}



