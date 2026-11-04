// ============================================================
// Function: FUN_1401e5480 at 1401e5480
// Target address checked: 0x1401E5480
// ============================================================

ulonglong FUN_1401e5480(longlong param_1,longlong param_2,longlong param_3)

{
  longlong *plVar1;
  undefined4 *puVar2;
  undefined8 *puVar3;
  undefined8 *puVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  float fVar9;
  undefined8 uVar10;
  undefined8 uVar11;
  char cVar12;
  byte bVar13;
  ulonglong uVar14;
  longlong lVar15;
  longlong lVar16;
  ulonglong *puVar17;
  float *pfVar18;
  uint uVar19;
  undefined8 *puVar20;
  float fVar21;
  float extraout_XMM0_Da;
  float extraout_XMM0_Da_00;
  float extraout_XMM0_Da_01;
  double dVar22;
  undefined1 local_res18 [8];
  undefined8 local_68;
  undefined8 uStack_60;
  
  plVar1 = (longlong *)(param_3 + 0xd58);
  local_68 = 0;
  uStack_60 = 0;
  if ((1 < (ulonglong)(*(longlong *)(param_3 + 0xd60) - *plVar1 >> 7)) &&
     (plVar1 != (longlong *)0x0)) {
    lVar15 = *(longlong *)(param_3 + 0xd60);
    lVar16 = *plVar1;
    if (lVar15 - lVar16 >> 7 != 0) {
      local_res18[0] = 0;
      FUN_14020af10(lVar16,lVar15,lVar15 - lVar16 >> 7,local_res18);
    }
  }
  uVar19 = 0;
  uVar14 = 0;
  if (*(longlong *)(param_3 + 0xd60) - *plVar1 >> 7 != 0) {
    uVar14 = 0;
    do {
      uVar14 = FUN_1401de2b0(param_1,uVar14 * 0x80 + *plVar1,param_2,param_3);
      if ((char)uVar14 != '\0') {
        if (-1 < (int)uVar19) {
          puVar20 = (undefined8 *)((longlong)(int)uVar19 * 0x80 + *plVar1);
          *(undefined8 *)(param_2 + 0x688) = *(undefined8 *)(param_2 + 0x680);
          lVar15 = *(longlong *)(param_2 + 0x640);
          for (lVar16 = *(longlong *)(param_2 + 0x638); lVar16 != lVar15; lVar16 = lVar16 + 0x198) {
            FUN_1401513f0(lVar16 + 200);
            FUN_1401513f0(lVar16);
          }
          *(undefined8 *)(param_2 + 0x640) = *(undefined8 *)(param_2 + 0x638);
          local_68 = *puVar20;
          uStack_60 = 0;
          cVar12 = FUN_14067d5d0(param_2,puVar20[2],0xffffffff,0);
          uVar11 = uStack_60;
          uVar10 = local_68;
          fVar9 = DAT_140909f58;
          if (cVar12 != '\0') {
            lVar15 = *(longlong *)(**(longlong **)(param_3 + 0xd38) + 0x10);
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            lVar16 = ((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198 + *(longlong *)(param_2 + 0x638);
            FUN_1401e0fc0(lVar16 + -0x198,lVar15);
            FUN_1401e0fc0(lVar16 + -0xd0,lVar15 + 200);
            *(undefined8 *)(lVar16 + -8) = *(undefined8 *)(lVar15 + 400);
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar15 = *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            lVar16 = ((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198;
            puVar17 = (ulonglong *)(lVar15 + -400 + lVar16);
            puVar3 = (undefined8 *)*puVar17;
            puVar17[1] = (ulonglong)puVar3;
            if ((&local_68 < puVar3) && (puVar4 = (undefined8 *)*puVar17, puVar4 <= &local_68)) {
              if (puVar3 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar5 = (undefined4 *)puVar17[1];
              if (puVar5 != (undefined4 *)0x0) {
                puVar2 = (undefined4 *)
                         (((longlong)&local_68 - (longlong)puVar4 & 0xfffffffffffffff0U) + *puVar17)
                ;
                uVar6 = puVar2[1];
                uVar7 = puVar2[2];
                uVar8 = puVar2[3];
                *puVar5 = *puVar2;
                puVar5[1] = uVar6;
                puVar5[2] = uVar7;
                puVar5[3] = uVar8;
              }
            }
            else {
              if (puVar3 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar3 = (undefined8 *)puVar17[1];
              if (puVar3 != (undefined8 *)0x0) {
                *puVar3 = uVar10;
                puVar3[1] = uVar11;
              }
            }
            puVar17[1] = puVar17[1] + 0x10;
            *(undefined8 *)(lVar16 + -8 + lVar15) = 0;
            pfVar18 = (float *)(lVar15 + -0x194 + lVar16);
            *(undefined1 *)(lVar16 + -0x198 + lVar15) = 1;
            *(undefined1 *)(lVar16 + -0xd0 + lVar15) = 0;
            *(undefined4 *)(lVar16 + -0xcc + lVar15) = 0x3f800000;
            *pfVar18 = 1.0;
            if (*(char *)(puVar20 + 8) != '\0') {
              *(undefined1 *)(lVar16 + -0x170 + lVar15) = 0;
            }
            fVar21 = (float)FUN_1406bddb0(param_2,*puVar20,0,0);
            dVar22 = (double)fVar21;
            if ((dVar22 <= 0.0) ||
               ((*(longlong *)(puVar20[9] + 0x318) != 0 &&
                (*(int *)(*(longlong *)(puVar20[9] + 0x318) + 0x360) == 0x14)))) {
              *pfVar18 = 1.0;
            }
            else if ((*(longlong *)(puVar20[2] + 0x318) == 0) ||
                    (*(int *)(*(longlong *)(puVar20[2] + 0x318) + 0x360) != 0)) {
              *pfVar18 = (float)((((double)puVar20[0xe] - (double)puVar20[0xd]) - DAT_14090a008) /
                                dVar22);
            }
            else {
              *pfVar18 = (float)((((double)puVar20[0xe] - (double)puVar20[0xd]) + DAT_14090a008) /
                                dVar22);
            }
            if (pfVar18 != (float *)0x0) {
              if (0.0 <= *pfVar18) {
                if (fVar9 < *pfVar18) {
                  *pfVar18 = 1.0;
                }
              }
              else {
                *pfVar18 = 0.0;
              }
            }
          }
          cVar12 = FUN_14067d5d0(param_2,puVar20[9],0xffffffff,0);
          if (cVar12 != '\0') {
            lVar15 = *(longlong *)(**(longlong **)(param_3 + 0xd38) + 0x10);
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            lVar16 = ((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198 + *(longlong *)(param_2 + 0x638);
            FUN_1401e0fc0(lVar16 + -0x198,lVar15);
            FUN_1401e0fc0(lVar16 + -0xd0,lVar15 + 200);
            *(undefined8 *)(lVar16 + -8) = *(undefined8 *)(lVar15 + 400);
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar15 = *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            lVar16 = ((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198;
            puVar17 = (ulonglong *)(lVar15 + -200 + lVar16);
            puVar3 = (undefined8 *)*puVar17;
            puVar17[1] = (ulonglong)puVar3;
            if ((&local_68 < puVar3) && (puVar4 = (undefined8 *)*puVar17, puVar4 <= &local_68)) {
              if (puVar3 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar5 = (undefined4 *)puVar17[1];
              if (puVar5 != (undefined4 *)0x0) {
                puVar2 = (undefined4 *)
                         (((longlong)&local_68 - (longlong)puVar4 & 0xfffffffffffffff0U) + *puVar17)
                ;
                uVar6 = puVar2[1];
                uVar7 = puVar2[2];
                uVar8 = puVar2[3];
                *puVar5 = *puVar2;
                puVar5[1] = uVar6;
                puVar5[2] = uVar7;
                puVar5[3] = uVar8;
              }
            }
            else {
              if (puVar3 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar3 = (undefined8 *)puVar17[1];
              if (puVar3 != (undefined8 *)0x0) {
                *puVar3 = uVar10;
                puVar3[1] = uVar11;
              }
            }
            puVar17[1] = puVar17[1] + 0x10;
            *(undefined8 *)(lVar16 + -8 + lVar15) = 0;
            *(undefined1 *)(lVar16 + -0x198 + lVar15) = 0;
            *(undefined1 *)(lVar16 + -0xd0 + lVar15) = 1;
            *(undefined4 *)(lVar16 + -0x194 + lVar15) = 0x3f800000;
            *(undefined4 *)(lVar16 + -0xcc + lVar15) = 0x3f800000;
            if (*(char *)(puVar20 + 0xf) != '\0') {
              *(undefined1 *)(lVar16 + -0xa8 + lVar15) = 0;
            }
          }
          cVar12 = FUN_14067d5d0(param_2,param_3,0xffffffff,0);
          if (cVar12 != '\0') {
            lVar15 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar15 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar15),8) + lVar15;
            *(undefined1 *)
             (((lVar15 >> 8) - (lVar15 >> 0x3f)) * 0x198 + -0x198 + *(longlong *)(param_2 + 0x638))
                 = 0;
            lVar15 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar15 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar15),8) + lVar15;
            *(undefined1 *)
             (((lVar15 >> 8) - (lVar15 >> 0x3f)) * 0x198 + -0xd0 + *(longlong *)(param_2 + 0x638)) =
                 0;
          }
          *(undefined4 *)(param_2 + 0x698) = 0;
          if (*(longlong *)(param_2 + 0x4f0) == 0) {
            bVar13 = FUN_1406b8eb0(param_2,0,0,0);
            if (bVar13 == 0) {
              if (*(longlong *)(param_2 + 0x688) - *(longlong *)(param_2 + 0x680) >> 3 != 0) {
                *(undefined8 *)(param_2 + 0x518) = 0;
                *(undefined8 *)(param_2 + 0x520) = **(undefined8 **)(param_2 + 0x680);
              }
              goto LAB_1401e5b73;
            }
            *(undefined8 *)(param_2 + 0x518) = 0;
            *(undefined8 *)(param_2 + 0x520) = 0;
            fVar21 = extraout_XMM0_Da_00;
          }
          else {
            bVar13 = 1;
            fVar21 = extraout_XMM0_Da;
          }
          *(undefined4 *)(param_2 + 0x508) = 0x3f000000;
          if (((0 < *(int *)(param_1 + 0x5bc)) &&
              ((undefined4 *)(param_2 + 0x508) != (undefined4 *)0x0)) &&
             (fVar21 = (float)FUN_1401aa280(fVar21,param_3), fVar21 < fVar9)) {
            pfVar18 = (float *)FUN_140010260(fVar21,DAT_140909d88,fVar9,DAT_14090a8cc,DAT_140909ddc)
            ;
            *pfVar18 = extraout_XMM0_Da_01;
            fVar21 = extraout_XMM0_Da_01;
          }
          if ((*(longlong *)(param_2 + 0x1708) == 0) ||
             (*(int *)(*(longlong *)(param_2 + 0x1708) + 0x294) != 4)) {
            FUN_140429a80(param_1,param_2,0);
          }
          else {
            FUN_14042a0d0(fVar21,param_2);
          }
          FUN_1401e4b10(param_1,param_3,puVar20);
LAB_1401e5b73:
          return (ulonglong)bVar13;
        }
        break;
      }
      uVar19 = uVar19 + 1;
      uVar14 = (ulonglong)uVar19;
    } while (uVar14 < (ulonglong)(*(longlong *)(param_3 + 0xd60) - *plVar1 >> 7));
  }
  return uVar14 & 0xffffffffffffff00;
}



// ============================================================
// Function: FUN_1401e5ba0 at 1401e5ba0
// Target address checked: 0x1401E5BA0
// ============================================================

ulonglong FUN_1401e5ba0(longlong param_1,longlong param_2,longlong param_3,longlong param_4)

{
  undefined4 *puVar1;
  uint uVar2;
  uint uVar3;
  undefined8 *puVar4;
  undefined8 *puVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  char cVar10;
  byte bVar11;
  ulonglong uVar12;
  float *pfVar13;
  longlong lVar14;
  longlong *plVar15;
  longlong lVar16;
  ulonglong *puVar17;
  int iVar18;
  ulonglong uVar19;
  undefined8 *puVar20;
  bool bVar21;
  float extraout_XMM0_Da;
  float extraout_XMM0_Da_00;
  float fVar22;
  float extraout_XMM0_Da_01;
  undefined8 local_38;
  undefined8 uStack_30;
  
  uVar19 = 0;
  local_38 = 0;
  uStack_30 = 0;
  uVar12 = 0;
  if (*(longlong *)(param_3 + 0xd60) - *(longlong *)(param_3 + 0xd58) >> 7 != 0) {
    lVar16 = *(longlong *)(param_3 + 0xd58);
    do {
      plVar15 = (longlong *)(uVar19 * 0x80 + lVar16);
      lVar14 = *plVar15;
      iVar18 = (int)uVar19;
      if ((lVar14 == param_4) &&
         (uVar12 = *(ulonglong *)
                    (*(longlong *)(*(longlong *)(param_3 + 0xd38) + (longlong)(int)plVar15[10] * 8)
                    + 0x10),
         DAT_14090a1f0 - (double)*(float *)(uVar12 + 0xcc) < (double)plVar15[0xc])) {
        if (((lVar14 != 0) &&
            ((((lVar14 == *(longlong *)(param_1 + 0xc3c8) ||
               (lVar14 == *(longlong *)(param_1 + 0xc3d0))) ||
              (lVar14 == *(longlong *)(param_1 + 0xc3d8))) ||
             (lVar14 == *(longlong *)(param_1 + 0xc3e0))))) &&
           (lVar14 = *(longlong *)(param_2 + 0x1708), lVar14 != 0)) {
          uVar12 = (ulonglong)*(int *)((longlong)plVar15 + 0x54);
          if ((*(int *)((longlong)plVar15 + 0x54) < 0) || ((int)plVar15[0xb] < 0)) {
            bVar21 = *(int *)(lVar14 + 0x8600) == 0x11;
          }
          else {
            uVar2 = *(uint *)(uVar12 * 0xe0 + 0x90 + *(longlong *)(plVar15[9] + 0x970));
            uVar3 = *(uint *)(lVar14 + 0x8600);
            uVar12 = (ulonglong)uVar3;
            bVar21 = uVar2 == uVar3;
            if ((uVar2 != uVar3) && (uVar3 == 0x11)) goto LAB_1401e5cc8;
          }
          if (!bVar21) goto LAB_1401e5e8c;
        }
LAB_1401e5cc8:
        if (-1 < iVar18) {
          *(undefined8 *)(param_2 + 0x688) = *(undefined8 *)(param_2 + 0x680);
          lVar14 = *(longlong *)(param_2 + 0x640);
          puVar20 = (undefined8 *)((longlong)iVar18 * 0x80 + lVar16);
          for (lVar16 = *(longlong *)(param_2 + 0x638); lVar16 != lVar14; lVar16 = lVar16 + 0x198) {
            FUN_1401513f0(lVar16 + 200);
            FUN_1401513f0(lVar16);
          }
          *(undefined8 *)(param_2 + 0x640) = *(undefined8 *)(param_2 + 0x638);
          local_38 = *puVar20;
          uStack_30 = uStack_30 & 0xffffffff00000000;
          cVar10 = FUN_14067d5d0(param_2,puVar20[9],0xffffffff,0);
          if (cVar10 != '\0') {
            lVar16 = *(longlong *)(**(longlong **)(param_3 + 0xd38) + 0x10);
            lVar14 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar14 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar14),8) + lVar14;
            lVar14 = ((lVar14 >> 8) - (lVar14 >> 0x3f)) * 0x198 + *(longlong *)(param_2 + 0x638);
            FUN_1401e0fc0(lVar14 + -0x198,lVar16);
            FUN_1401e0fc0(lVar14 + -0xd0,lVar16 + 200);
            *(undefined8 *)(lVar14 + -8) = *(undefined8 *)(lVar16 + 400);
            lVar14 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar16 = *(longlong *)(param_2 + 0x638);
            lVar14 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar14),8) + lVar14;
            lVar14 = ((lVar14 >> 8) - (lVar14 >> 0x3f)) * 0x198;
            puVar17 = (ulonglong *)(lVar16 + -200 + lVar14);
            *(undefined8 *)(lVar14 + -8 + lVar16) = 0;
            puVar4 = (undefined8 *)*puVar17;
            puVar17[1] = (ulonglong)puVar4;
            if ((&local_38 < puVar4) && (puVar5 = (undefined8 *)*puVar17, puVar5 <= &local_38)) {
              if (puVar4 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar6 = (undefined4 *)puVar17[1];
              if (puVar6 != (undefined4 *)0x0) {
                puVar1 = (undefined4 *)
                         (((longlong)&local_38 - (longlong)puVar5 & 0xfffffffffffffff0U) + *puVar17)
                ;
                uVar7 = puVar1[1];
                uVar8 = puVar1[2];
                uVar9 = puVar1[3];
                *puVar6 = *puVar1;
                puVar6[1] = uVar7;
                puVar6[2] = uVar8;
                puVar6[3] = uVar9;
              }
            }
            else {
              if (puVar4 == (undefined8 *)puVar17[2]) {
                FUN_1400b1c20(puVar17,1);
              }
              puVar6 = (undefined4 *)puVar17[1];
              if (puVar6 != (undefined4 *)0x0) {
                *puVar6 = (undefined4)local_38;
                puVar6[1] = local_38._4_4_;
                puVar6[2] = (undefined4)uStack_30;
                puVar6[3] = uStack_30._4_4_;
              }
            }
            puVar17[1] = puVar17[1] + 0x10;
            *(undefined1 *)(lVar14 + -0x198 + lVar16) = 0;
            *(undefined1 *)(lVar14 + -0xd0 + lVar16) = 1;
            *(undefined4 *)(lVar14 + -0x194 + lVar16) = 0x3f800000;
            *(undefined4 *)(lVar14 + -0xcc + lVar16) = 0x3f800000;
            if (*(char *)(puVar20 + 0xf) != '\0') {
              *(undefined1 *)(lVar14 + -0xa8 + lVar16) = 0;
            }
          }
          cVar10 = FUN_14067d5d0(param_2,param_3,0xffffffff,0);
          if (cVar10 != '\0') {
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            *(undefined1 *)
             (((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198 + -0x198 + *(longlong *)(param_2 + 0x638))
                 = 0;
            lVar16 = *(longlong *)(param_2 + 0x640) - *(longlong *)(param_2 + 0x638);
            lVar16 = SUB168(SEXT816(-0x5f5f5f5f5f5f5f5f) * SEXT816(lVar16),8) + lVar16;
            *(undefined1 *)
             (((lVar16 >> 8) - (lVar16 >> 0x3f)) * 0x198 + -0xd0 + *(longlong *)(param_2 + 0x638)) =
                 0;
          }
          *(undefined4 *)(param_2 + 0x698) = 0;
          if (*(longlong *)(param_2 + 0x4f0) == 0) {
            bVar11 = FUN_1406b8eb0(param_2,0,0,0);
            if (bVar11 == 0) {
              if (*(longlong *)(param_2 + 0x688) - *(longlong *)(param_2 + 0x680) >> 3 != 0) {
                *(undefined8 *)(param_2 + 0x518) = 0;
                *(undefined8 *)(param_2 + 0x520) = **(undefined8 **)(param_2 + 0x680);
              }
              goto LAB_1401e6024;
            }
            *(undefined8 *)(param_2 + 0x518) = 0;
            *(undefined8 *)(param_2 + 0x520) = 0;
            fVar22 = extraout_XMM0_Da_00;
          }
          else {
            bVar11 = 1;
            fVar22 = extraout_XMM0_Da;
          }
          *(undefined4 *)(param_2 + 0x508) = 0x3f000000;
          if (((0 < *(int *)(param_1 + 0x5bc)) &&
              ((undefined4 *)(param_2 + 0x508) != (undefined4 *)0x0)) &&
             (fVar22 = (float)FUN_1401aa280(fVar22,param_3), fVar22 < DAT_140909f58)) {
            pfVar13 = (float *)FUN_140010260(fVar22,DAT_140909d88,DAT_140909f58,DAT_14090a8cc,
                                             DAT_140909ddc);
            *pfVar13 = extraout_XMM0_Da_01;
            fVar22 = extraout_XMM0_Da_01;
          }
          if ((*(longlong *)(param_2 + 0x1708) == 0) ||
             (*(int *)(*(longlong *)(param_2 + 0x1708) + 0x294) != 4)) {
            FUN_140429a80(param_1,param_2,0);
            FUN_1401e3bd0(param_1,puVar20);
          }
          else {
            FUN_14042a0d0(fVar22,param_2);
            FUN_1401e3bd0(param_1,puVar20);
          }
LAB_1401e6024:
          return (ulonglong)bVar11;
        }
        break;
      }
LAB_1401e5e8c:
      uVar19 = (ulonglong)(iVar18 + 1);
      uVar12 = *(longlong *)(param_3 + 0xd60) - lVar16 >> 7;
    } while (uVar19 < uVar12);
  }
  return uVar12 & 0xffffffffffffff00;
}



