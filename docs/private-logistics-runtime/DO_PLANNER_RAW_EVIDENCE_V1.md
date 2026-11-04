# DO Planner Raw Evidence

This file contains raw decompiled code for the DO planner, rebuild function, percentage increment, and stock/capacity helpers.

---

## 1. planner_424430.c (FUN_140424430)

```c
// ============================================================
// Function: FUN_140424430 at 140424430
// Target address checked: 0x140424430
// ============================================================

void FUN_140424430(longlong param_1,longlong param_2,longlong param_3,float param_4,char param_5,
                  char param_6,char param_7)

{
  float fVar1;
  int iVar2;
  longlong lVar3;
  longlong *plVar4;
  bool bVar5;
  bool bVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  longlong lVar12;
  longlong lVar13;
  ulonglong uVar14;
  ulonglong uVar15;
  uint uVar16;
  uint *puVar17;
  longlong lVar18;
  longlong *plVar19;
  ulonglong *puVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float local_res20 [2];
  uint local_198;
  ulonglong local_190;
  uint local_188;
  longlong local_180;
  ulonglong local_178;
  uint *local_170;
  longlong local_168;
  longlong local_160;
  longlong local_158;
  float *pfStack_150;
  float *local_148;
  float *local_140;
  longlong local_138;
  longlong local_130 [7];
  longlong *local_f8;
  ulonglong local_f0;
  ulonglong local_e8;
  ulonglong local_e0;
  ulonglong *local_d8;
  undefined8 local_d0;
  
  local_d0 = 0xfffffffffffffffe;
  uVar14 = 0;
  puVar20 = (ulonglong *)(param_1 + 0x144a8);
  lVar18 = *(longlong *)(param_1 + 0x144b0) - *puVar20;
  lVar13 = lVar18 >> 0x3f;
  if (lVar18 / 0x68 + lVar13 != lVar13) {
    do {
      lVar13 = uVar14 * 0x68;
      *(undefined8 *)(lVar13 + 8 + *puVar20) = *(undefined8 *)(lVar13 + *puVar20);
      *(undefined8 *)(lVar13 + 0x20 + *puVar20) = *(undefined8 *)(lVar13 + 0x18 + *puVar20);
      uVar14 = (ulonglong)((int)uVar14 + 1);
    } while (uVar14 < (ulonglong)((longlong)(*(longlong *)(param_1 + 0x144b0) - *puVar20) / 0x68));
  }
  uVar14 = *(ulonglong *)(param_1 + 0x144b0);
  local_d8 = puVar20;
  for (uVar15 = *puVar20; uVar15 != uVar14; uVar15 = uVar15 + 0x68) {
    FUN_140108190(uVar15);
  }
  *(ulonglong *)(param_1 + 0x144b0) = *puVar20;
  local_f0 = 0xffffffffffffffff;
  FUN_140422c30(param_1,param_2,0xffffffff);
  fVar11 = DAT_14090a280;
  fVar10 = DAT_140909f58;
  fVar9 = DAT_140909ddc;
  fVar8 = DAT_140909c78;
  fVar7 = DAT_140909a8c;
  if (-1 < (int)(*(longlong *)(param_1 + 0x14480) - *(longlong *)(param_1 + 0x14478) >> 3)) {
    local_e8 = 0xffffffffffffffff;
    do {
      uVar15 = 0;
      local_170 = (uint *)0x0;
      uVar14 = local_e8;
      if (local_e8 == 0xffffffffffffffff) {
        local_180 = param_2;
        if (param_5 == '\0') {
LAB_140424694:
          if (*(char *)(local_180 + 0xea8) == '\0') goto LAB_14042469e;
LAB_1404246ac:
          local_168 = 0;
          local_158 = 0;
          pfStack_150 = (float *)0x0;
          local_140 = (float *)0x0;
          local_130[0] = 0;
          local_130[1] = 0;
          local_130[2] = 0;
          local_130[3] = 0;
          local_130[4] = 0;
          local_130[5] = 0;
          local_160 = 0;
          local_148 = (float *)0x0;
          local_188 = 0;
          lVar12 = *(longlong *)(local_180 + 0x978) - *(longlong *)(local_180 + 0x970);
          lVar13 = lVar12 >> 0x3f;
          puVar17 = local_170;
          lVar18 = local_180;
          local_138 = local_180;
          if (lVar12 / 0xe0 + lVar13 != lVar13) {
            do {
              uVar16 = (uint)uVar15;
              plVar19 = (longlong *)(uVar15 * 0xe0 + *(longlong *)(lVar18 + 0x970));
              local_f8 = plVar19;
              if (((puVar17 == (uint *)0x0) ||
                  ((((((iVar2 = (int)plVar19[0x12], 1 < iVar2 - 0xcU &&
                       ((iVar2 != 1 || (*puVar17 == 3)))) && ((iVar2 != 0 || (*puVar17 == 3)))) &&
                     ((((iVar2 != 0xb || (*puVar17 == 3)) && ((iVar2 != 6 || (*puVar17 == 3)))) &&
                      ((iVar2 != 5 || (*puVar17 == 3)))))) && ((iVar2 != 3 || (*puVar17 == 4)))) &&
                   (((((iVar2 != 0xf || (*puVar17 == 0x13)) &&
                      ((iVar2 != 0x10 || (*puVar17 == 0x14)))) &&
                     ((iVar2 != 0xe || (*puVar17 - 0xb < 2)))) &&
                    (((iVar2 != 2 || (*puVar17 == 6)) &&
                     ((((iVar2 != 4 || (*puVar17 == 0x10)) || (*puVar17 == 3)) &&
                      ((*(longlong *)(puVar17 + 10) == 0 ||
                       (*(char *)(*(longlong *)(puVar17 + 10) + 0x141) != '\0')))))))))))) &&
                  (((int)plVar19[0x12] != 0x11 && (local_198 = 0, plVar19[1] - *plVar19 >> 4 != 0))))
              {
                local_178 = 0;
                do {
                  uVar14 = 0;
                  bVar5 = false;
                  local_190 = *(ulonglong *)(param_2 + 0x318);
                  if (local_190 != 0) {
                    uVar16 = *(uint *)(local_190 + 0x360);
                    if (((uVar16 < 0x29) &&
                        ((0x100000202c0U >> ((longlong)(int)uVar16 & 0x3fU) & 1) != 0)) ||
                       (uVar16 == 0x46)) {
                      bVar5 = true;
                      if (*(longlong *)(local_190 + 0x690) - *(longlong *)(local_190 + 0x688) >> 4
                          != 0) {
                        bVar6 = bVar5;
                        do {
                          bVar5 = false;
                          if (*(longlong *)(*(longlong *)(local_190 + 0x688) + uVar14 * 0x10) !=
                              *(longlong *)(*plVar19 + local_178 * 0x10)) {
                            bVar5 = bVar6;
                          }
                          uVar14 = (ulonglong)((int)uVar14 + 1);
                          lVar18 = local_180;
                          bVar6 = bVar5;
                        } while (uVar14 < (ulonglong)
                                              (*(longlong *)(local_190 + 0x690) -
                                               *(longlong *)(local_190 + 0x688) >> 4));
                      }
                      uVar14 = 0;
                      if (*(longlong *)(local_190 + 0x6c0) - *(longlong *)(local_190 + 0x6b8) >> 4
                          != 0) {
                        lVar13 = *(longlong *)(*(longlong *)(param_2 + 0x318) + 0x6b8);
                        bVar6 = bVar5;
                        do {
                          bVar5 = false;
                          if (*(longlong *)(lVar13 + uVar14 * 0x10) !=
                              *(longlong *)(*plVar19 + local_178 * 0x10)) {
                            bVar5 = bVar6;
                          }
                          uVar14 = (ulonglong)((int)uVar14 + 1);
                          lVar18 = local_180;
                          bVar6 = bVar5;
                        } while (uVar14 < (ulonglong)
                                              (*(longlong *)(*(longlong *)(param_2 + 0x318) + 0x6c0) -
                                               lVar13 >> 4));
                      }
                      if ((((*(longlong *)(lVar18 + 0x318) == 0) ||
                           (iVar2 = *(int *)(*(longlong *)(lVar18 + 0x318) + 0x3b8), iVar2 < 0)) ||
                         ((ulonglong)
                         ((*(longlong *)(lVar18 + 0x978) - *(longlong *)(lVar18 + 0x970)) / 0xe0)
                         <= (ulonglong)(longlong)iVar2)) ||
                        (plVar19 !=
                        (longlong *)((longlong)iVar2 * 0xe0 + *(longlong *)(lVar18 + 0x970)))) {
                        if (((bVar5) && (uVar16 == 9)) &&
                           (*(longlong *)(*plVar19 + local_178 * 0x10) ==
                           *(longlong *)(param_1 + 0xc330))) {
                          bVar5 = false;
                        }
                      }
                      else {
                        bVar5 = false;
                      }
                    }
                    uVar14 = 0;
                    if ((uVar16 == 5) &&
                       (bVar5 = true,
                       lVar12 = *(longlong *)(param_2 + 0x978) - *(longlong *)(param_2 + 0x970),
                       lVar13 = lVar12 >> 0x3f, lVar18 = local_180, lVar12 / 0xe0 + lVar13 != lVar13
                       )) {
                      lVar13 = *(longlong *)(param_2 + 0x970);
                      do {
                        uVar15 = 0;
                        lVar18 = uVar14 * 0xe0;
                        if (*(longlong *)(lVar18 + 8 + lVar13) - *(longlong *)(lVar18 + lVar13) >> 4
                            != 0) {
                          bVar6 = bVar5;
                          do {
                            bVar5 = false;
                            if (*(longlong *)(*(longlong *)(lVar18 + lVar13) + uVar15 * 0x10) !=
                                *(longlong *)(*plVar19 + local_178 * 0x10)) {
                              bVar5 = bVar6;
                            }
                            uVar15 = (ulonglong)((int)uVar15 + 1);
                            bVar6 = bVar5;
                          } while (uVar15 < (ulonglong)
                                                (*(longlong *)(lVar18 + 8 + lVar13) -
                                                 *(longlong *)(lVar18 + lVar13) >> 4));
                        }
                        uVar14 = (ulonglong)((int)uVar14 + 1);
                        lVar18 = local_180;
                      } while (uVar14 < (ulonglong)
                                            ((*(longlong *)(param_2 + 0x978) -
                                             *(longlong *)(param_2 + 0x970)) / 0xe0));
                    }
                  }
                  uVar14 = 0;
                  lVar13 = *(longlong *)(lVar18 + 0x318);
                  if ((((lVar13 != 0) && (*(int *)(lVar13 + 0x360) == 0)) && (param_4 < 0.0)) &&
                     ((*(char *)((longlong)plVar19 + 0x84) == '\x01' &&
                      (*(char *)((longlong)plVar19 + 0x85) == '\0')))) {
                    bVar5 = true;
                  }
                  if (((0.0 < param_4) && (param_2 != lVar18)) &&
                     ((local_190 != 0 &&
                      (*(longlong *)(local_190 + 0x690) - *(longlong *)(local_190 + 0x688) >> 4 != 0
                      )))) {
                    lVar12 = *(longlong *)(*(longlong *)(param_2 + 0x318) + 0x688);
                    uVar15 = uVar14;
                    do {
                      if (*(longlong *)(lVar12 + uVar15 * 0x10) ==
                          *(longlong *)(*plVar19 + local_178 * 0x10)) {
                        bVar5 = true;
                      }
                      uVar15 = (ulonglong)((int)uVar15 + 1);
                    } while (uVar15 < (ulonglong)
                                          (*(longlong *)(*(longlong *)(param_2 + 0x318) + 0x690) -
                                           lVar12 >> 4));
                  }
                  if ((((param_4 < 0.0) && (param_2 != lVar18)) && (lVar13 != 0)) &&
                     (*(longlong *)(lVar13 + 0x690) - *(longlong *)(lVar13 + 0x688) >> 4 != 0)) {
                    do {
                      if (*(longlong *)(*(longlong *)(lVar13 + 0x688) + uVar14 * 0x10) ==
                          *(longlong *)(*plVar19 + local_178 * 0x10)) {
                        bVar5 = true;
                      }
                      uVar14 = (ulonglong)((int)uVar14 + 1);
                    } while (uVar14 < (ulonglong)
                                          (*(longlong *)(lVar13 + 0x690) - *(longlong *)(lVar13 + 0x688)
                                          >> 4));
                  }
                  if (param_3 == 0) {
LAB_140424e29:
                    uVar14 = 0;
                    if ((0.0 < param_4) && (lVar12 = *plVar19, plVar19[1] - lVar12 >> 4 != 0)) {
                      do {
                        if ((((int)plVar19[0x12] - 2U < 3) &&
                            ((0.0 < *(float *)(lVar12 + 8 + uVar14 * 0x10) ||
                             ((fVar9 < *(float *)(plVar19[3] + 0xc + uVar14 * 0x10) &&
                               (fVar9 < *(float *)(plVar19[3] + 8 + uVar14 * 0x10))))))) &&
                             (((uint)uVar14 != local_198 &&
                              ((((!bVar5 && (bVar5 = true, lVar13 != 0)) &&
                                (*(uint *)(lVar13 + 0x360) < 0x15)) &&
                               ((0x100801U >> (*(uint *)(lVar13 + 0x360) & 0x1f) & 1) != 0)))))) {
                          bVar5 = false;
                        }
                        uVar14 = (ulonglong)((uint)uVar14 + 1);
                      } while (uVar14 < (ulonglong)(plVar19[1] - lVar12 >> 4));
                    }
                  }
                  else {
                    lVar12 = *plVar19;
                    lVar3 = *(longlong *)(lVar12 + local_178 * 0x10);
                    if (param_3 != lVar3) {
                      bVar5 = true;
                      goto LAB_140424e29;
                    }
                    if (0.0 < param_4) {
                      fVar23 = 0.0;
                      local_res20[0] = 0.0;
                      uVar14 = 0;
                      if (plVar19[1] - lVar12 >> 4 != 0) {
                        do {
                          fVar21 = *(float *)((longlong)plVar19 + 0x8c) *
                                   *(float *)((longlong)(int)plVar19[0x12] * 0x20 + 0xcc +
                                             *(longlong *)(lVar12 + uVar14 * 0x10));
                          if (fVar21 != 0.0) {
                            fVar23 = fVar23 + *(float *)(lVar12 + 8 + uVar14 * 0x10) / fVar21;
                            local_res20[0] = fVar23;
                          }
                          uVar14 = (ulonglong)((int)uVar14 + 1);
                        } while (uVar14 < (ulonglong)(plVar19[1] - lVar12 >> 4));
                        if (0.0 <= fVar23) {
                          if (fVar10 < fVar23) {
                            fVar23 = fVar10;
                          }
                        }
                        else {
                          fVar23 = 0.0;
                        }
                      }
                      fVar21 = *(float *)((longlong)(int)plVar19[0x12] * 0x20 + 0xcc + lVar3);
                      fVar1 = *(float *)((longlong)plVar19 + 0x8c);
                      fVar22 = fVar1 * fVar21;
                      fVar23 = fVar22 * (fVar10 - fVar23);
                      if ((param_7 != '\0') &&
                         (fVar9 < *(float *)(plVar19[3] + 0xc + local_178 * 0x10))) {
                        fVar22 = fVar1 * *(float *)(plVar19[3] + 8 + local_178 * 0x10) * fVar21;
                        fVar23 = fVar22 - *(float *)(lVar12 + 8 + local_178 * 0x10);
                      }
                      if ((lVar13 != 0) &&
                         ((((iVar2 = *(int *)(lVar13 + 0x360), iVar2 == 0xb ||
                           ((iVar2 == 0 && (param_6 == '\0')))) || (iVar2 == 0x14)) ||
                         ((iVar2 == 0x1c && (param_6 == '\0')))))) {
                        fVar23 = (fVar22 - *(float *)(lVar12 + 8 + local_178 * 0x10)) + fVar11;
                      }
                      if (param_7 == '\0') {
                        if (((fVar9 < *(float *)(plVar19[3] + 0xc + local_178 * 0x10)) &&
                            (fVar1 * *(float *)(plVar19[3] + 8 + local_178 * 0x10) * fVar21 <
                             *(float *)(lVar12 + 8 + local_178 * 0x10) + fVar7)) || (fVar23 <= 0.0))
                        {
                          bVar5 = true;
                        }
                      }
                      else if (fVar23 < param_4) {
                        bVar5 = true;
                      }
                      goto LAB_140424e29;
                    }
                  }
                  if ((int)plVar19[0x12] == 9) {
                    if (*(float *)(*plVar19 + 0xc + local_178 * 0x10) <= fVar8) {
                      bVar5 = true;
                    }
                    if (lVar18 == param_2) goto LAB_140424f11;
                  }
                  else {
LAB_140424f11:
                    if (!bVar5) {
                      lVar13 = local_178 * 0x10;
                      local_190 = *plVar19 + lVar13;
                      FUN_14008e050(&local_168,&local_190);
                      FUN_14008e050(local_130,&local_f8);
                      FUN_14008e050(local_130 + 3,&local_170);
                      plVar19 = local_f8;
                      fVar23 = *(float *)(*(longlong *)(lVar13 + *local_f8) + 0xcc +
                                         (longlong)(int)local_f8[0x12] * 0x20) *
                                *(float *)((longlong)local_f8 + 0x8c);
                      local_res20[0] = fVar23;
                      if ((local_res20 < local_148) && (pfStack_150 <= local_res20)) {
                        lVar13 = (longlong)local_res20 - (longlong)pfStack_150;
                        if (local_148 == local_140) {
                          FUN_14001e440(&pfStack_150,1);
                        }
                        if (local_148 != (float *)0x0) {
                          *local_148 = pfStack_150[lVar13 >> 2];
                        }
                      }
                      else {
                        if (local_148 == local_140) {
                          FUN_14001e440(&pfStack_150,1);
                        }
                        if (local_148 != (float *)0x0) {
                          *local_148 = fVar23;
                        }
                      }
                      local_148 = local_148 + 1;
                    }
                  }
                  local_198 = local_198 + 1;
                  local_178 = (ulonglong)local_198;
                  puVar17 = local_170;
                  uVar16 = local_188;
                } while (local_178 < (ulonglong)(plVar19[1] - *plVar19 >> 4));
              }
              local_188 = uVar16 + 1;
              uVar15 = (ulonglong)local_188;
              puVar20 = local_d8;
            } while ((ulonglong)local_188 <
                     (ulonglong)
                     ((*(longlong *)(lVar18 + 0x978) - *(longlong *)(lVar18 + 0x970)) / 0xe0));
          }
          uVar14 = local_e8;
          lVar18 = local_130[0];
          lVar13 = local_168;
          if (local_160 - local_168 >> 3 != 0) {
            plVar19 = (longlong *)puVar20[1];
            if ((&local_168 < plVar19) && (plVar4 = (longlong *)*puVar20, plVar4 <= &local_168)) {
              if (plVar19 == (longlong *)puVar20[2]) {
                FUN_140202d10(puVar20,1);
              }
              local_190 = puVar20[1];
              local_e0 = local_190;
              if (local_190 != 0) {
                FUN_1402108a0(local_190,
                              (((longlong)&local_168 - (longlong)plVar4) / 0x68) * 0x68 + *puVar20);
                lVar13 = local_168;
                lVar18 = local_130[0];
              }
            }
            else {
              if (plVar19 == (longlong *)puVar20[2]) {
                FUN_140202d10(puVar20,1);
              }
              local_190 = puVar20[1];
              local_e0 = local_190;
              if (local_190 != 0) {
                FUN_1402108a0(local_190,&local_168);
                lVar13 = local_168;
                lVar18 = local_130[0];
              }
            }
            puVar20[1] = puVar20[1] + 0x68;
          }
          if (local_130[3] != 0) {
            FUN_140001a50(local_130[3],local_130[5] - local_130[3] >> 3,8);
          }
          if (lVar18 != 0) {
            FUN_140001a50(lVar18,local_130[2] - lVar18 >> 3,8);
          }
          if (pfStack_150 != (float *)0x0) {
            FUN_140001a50(pfStack_150,(longlong)local_140 - (longlong)pfStack_150 >> 2,4);
          }
          if (lVar13 != 0) {
            FUN_140001a50(lVar13,local_158 - lVar13 >> 3,8);
          }
        }
      }
      else {
        local_180 = *(longlong *)(*(longlong *)(param_1 + 0x14478) + local_e8 * 8);
        local_170 = (uint *)((longlong)*(int *)(*(longlong *)(param_1 + 0x14490) + local_e8 * 4) *
                             0x60 + *(longlong *)(param_2 + 0xa10));
        if ((((((param_4 <= 0.0) || (0x14 < *local_170)) ||
              ((0x191850U >> (*local_170 & 0x1f) & 1) == 0)) ||
             (*(char *)((longlong)local_170 + 5) != '\0')) &&
           (((0.0 < param_4 || (0x14 < *local_170)) ||
            (((0x191850U >> (*local_170 & 0x1f) & 1) == 0 || ((char)local_170[1] != '\0')))))) &&
          (((0x14 < *local_170 || ((0x191850U >> (*local_170 & 0x1f) & 1) == 0)) ||
           ((char)local_170[0x10] != '\0')))) {
          if (0x7fffffffffffffff < local_e8) goto LAB_140424694;
LAB_14042469e:
          if (fVar10 < *(float *)(local_180 + 0x604) || fVar10 == *(float *)(local_180 + 0x604))
          goto LAB_1404246ac;
        }
      }
      uVar16 = (int)local_f0 + 1;
      local_f0 = (ulonglong)uVar16;
      local_e8 = uVar14 + 1;
    } while ((int)uVar16 <
             (int)(*(longlong *)(param_1 + 0x14480) - *(longlong *)(param_1 + 0x14478) >> 3));
  }
  return;
}
```

---

## 2. rebuild_1E2380.c (FUN_1401E2380)

```c
// ============================================================
// Function: FUN_1401e2380 at 1401e2380
// Target address checked: 0x1401E2380
// ============================================================

void FUN_1401e2380(longlong param_1,longlong param_2)

{
  int iVar1;
  char *pcVar2;
  char *pcVar3;
  bool bVar4;
  bool bVar5;
  float fVar6;
  uint ***pppuVar7;
  char cVar8;
  char cVar9;
  ulonglong uVar10;
  longlong lVar11;
  ulonglong uVar12;
  longlong lVar13;
  longlong lVar14;
  uint uVar15;
  uint uVar16;
  uint ****ppppuVar17;
  uint ****ppppuVar18;
  longlong *plVar19;
  longlong lVar20;
  uint ****ppppuVar21;
  ulonglong uVar22;
  uint ****ppppuVar23;
  int iVar24;
  uint ****ppppuVar25;
  undefined4 extraout_XMM0_Da;
  undefined4 extraout_XMM0_Da_00;
  uint **local_res18;
  uint **local_res20;
  uint ***local_298;
  longlong local_290;
  uint ***local_288;
  uint ***local_280;
  uint ***local_278;
  uint ***local_270;
  uint ***local_268;
  uint ***pppuStack_260;
  uint ***local_258;
  uint ***local_250;
  uint ***pppuStack_248;
  uint ***local_240;
  uint ***local_238;
  uint ***local_220;
  uint ***pppuStack_218;
  uint ***local_210;
  longlong local_208;
  longlong local_200;
  uint local_1f8;
  uint local_1f4;
  longlong local_1f0;
  longlong lStack_1e8;
  longlong local_1e0;
  undefined8 local_1d8;
  undefined4 uStack_1d0;
  uint uStack_1cc;
  undefined8 local_1c8;
  undefined8 uStack_1c0;
  undefined4 local_1b8;
  uint uStack_1b4;
  undefined8 uStack_1b0;
  undefined8 local_198;
  undefined1 uStack_196;
  uint5 uStack_195;
  undefined8 uStack_190;
  undefined8 local_188;
  undefined4 uStack_180;
  uint uStack_17c;
  undefined8 local_178;
  undefined8 uStack_170;
  undefined8 uStack_168;
  uint uStack_160;
  undefined4 uStack_15c;
  ulonglong local_158;
  longlong local_150;
  longlong local_148;
  longlong lStack_140;
  longlong local_138;
  longlong local_128;
  longlong lStack_120;
  longlong local_118;
  undefined8 uStack_110;
  undefined8 local_108;
  undefined8 uStack_100;
  undefined8 local_f8;
  undefined8 uStack_f0;
  undefined8 local_e8;
  longlong lStack_e0;
  undefined8 local_d8;
  undefined8 uStack_d0;
  undefined8 local_c8;
  undefined4 uStack_c0;
  undefined4 uStack_bc;
  undefined4 local_b8;
  undefined4 uStack_b4;
  undefined4 uStack_b0;
  undefined4 uStack_ac;
  longlong local_a8;
  uint ***local_a0;
  longlong local_98;
  longlong local_90;
  ulonglong local_88;
  undefined8 local_80;
  
  local_80 = 0xfffffffffffffffe;
  local_268 = (uint ***)0x0;
  pppuStack_260 = (uint ***)0x0;
  ppppuVar23 = (uint ****)0x0;
  local_278 = (uint ***)0x0;
  local_258 = (uint ***)0x0;
  local_250 = (uint ***)0x0;
  pppuStack_248 = (uint ***)0x0;
  local_288 = (uint ***)0x0;
  local_240 = (uint ***)0x0;
  local_220 = (uint ***)0x0;
  pppuStack_218 = (uint ***)0x0;
  local_270 = (uint ***)0x0;
  local_210 = (uint ***)0x0;
  local_238 = (uint ***)0x0;
  pppuStack_230 = (uint ***)0x0;
  local_280 = (uint ***)0x0;
  local_228 = (uint ***)0x0;
  local_148 = 0;
  lStack_140 = 0;
  local_150 = 0;
  local_138 = 0;
  local_1f0 = 0;
  lStack_1e8 = 0;
  local_1e0 = 0;
  ppppuVar17 = ppppuVar23;
  ppppuVar18 = ppppuVar23;
  ppppuVar21 = ppppuVar23;
  if (param_2 != 0) {
    *(undefined1 *)(param_2 + 0xd50) = 0;
    *(undefined8 *)(param_2 + 0xd60) = *(undefined8 *)(param_2 + 0xd58);
    if (*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3 != 0) {
      do {
        uVar10 = 0;
        pcVar2 = *(char **)(*(longlong *)(param_2 + 0xd38) + (longlong)ppppuVar23 * 8);
        if ((**(char **)(pcVar2 + 0x10) != '\0') &&
           (*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3 != 0)) {
          do {
            pcVar3 = *(char **)(*(longlong *)(param_2 + 0xd38) + uVar10 * 8);
            if (((int)ppppuVar23 != (int)uVar10) &&
               (((*(char *)(*(longlong *)(pcVar3 + 0x10) + 200) != '\0' && (*pcVar2 != '\0')) &&
                 (*pcVar3 != '\0')))) {
              local_1c8 = 0;
              uStack_190 = 0;
              local_1d8 = 0;
              uStack_1c0 = 0;
              local_1b8 = 0;
              uStack_1b0 = 0;
              local_198 = 0;
              uStack_196 = 0;
              local_188 = 0;
              uStack_180 = 0;
              local_178 = 0;
              uStack_160 = uStack_160 & 0xff000000;
              uStack_170 = 0;
              uStack_168 = 0;
              uStack_1d0 = 0;
              lStack_120 = (ulonglong)uStack_1cc << 0x20;
              local_128 = 0;
              local_108 = (ulonglong)uStack_1b4 << 0x20;
              uStack_100 = 0;
              local_f8 = 0;
              uStack_f0 = 0;
              local_e8 = (ulonglong)uStack_195 << 0x18;
              uStack_d0 = (ulonglong)uStack_17c << 0x20;
              local_c8 = 0;
              uStack_c0 = 0;
              uStack_bc = 0;
              local_b8 = 0;
              uStack_b4 = 0;
              uStack_b0 = uStack_160;
              uStack_ac = uStack_15c;
              local_118 = *(longlong *)(pcVar2 + 8);
              lStack_e0 = *(longlong *)(pcVar3 + 8);
              uStack_110 = ppppuVar23;
              local_d8 = uVar10;
              FUN_140200280(&local_148,&local_128);
            }
            uVar10 = (ulonglong)((int)uVar10 + 1);
          } while (uVar10 < (ulonglong)
                            (*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3));
        }
        if ((*(longlong *)(*(longlong *)(pcVar2 + 8) + 0x318) != 0) &&
           (*(int *)(*(longlong *)(*(longlong *)(pcVar2 + 8) + 0x318) + 0x360) == 0x14)) {
          *(undefined4 *)(*(longlong *)(pcVar2 + 0x10) + 4) = 0;
          *(undefined4 *)(*(longlong *)(pcVar2 + 0x10) + 0xcc) = 0x3f7d70a4;
        }
        ppppuVar23 = (uint ****)(ulonglong)((int)ppppuVar23 + 1);
      } while (ppppuVar23 <
               (uint ****)(*(longlong *)(param_2 + 0xd40) - *(longlong *)(param_2 + 0xd38) >> 3));
    }
    uVar10 = 0;
    lVar13 = *(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0);
    lVar14 = lVar13 >> 0x3f;
    local_150 = local_138;
    if (lVar13 / 0x340 + lVar14 != lVar14) {
      do {
        lVar14 = uVar10 * 0x340 + *(longlong *)(param_1 + 0xc2b0);
        if (0.0 < *(float *)(lVar14 + 0x22c)) {
          FUN_1400f9a00(lVar14,&local_1f0);
        }
        uVar10 = (ulonglong)((int)uVar10 + 1);
      } while (uVar10 < (ulonglong)
                          ((*(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0)) / 0x340
                          ));
    }
    uVar10 = 0;
    lVar13 = *(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0);
    lVar14 = lVar13 >> 0x3f;
    if (lVar13 / 0x340 + lVar14 != lVar14) {
      do {
        lVar14 = uVar10 * 0x340 + *(longlong *)(param_1 + 0xc2b0);
        if (0.0 < *(float *)(lVar14 + 0x14c)) {
          FUN_1400f9a00(lVar14,&local_1f0);
        }
        uVar10 = (ulonglong)((int)uVar10 + 1);
      } while (uVar10 < (ulonglong)
                          ((*(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0)) / 0x340
                          ));
    }
    uVar10 = 0;
    lVar13 = *(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0);
    lVar14 = lVar13 >> 0x3f;
    if (lVar13 / 0x340 + lVar14 != lVar14) {
      do {
        lVar14 = uVar10 * 0x340 + *(longlong *)(param_1 + 0xc2b0);
        if (0.0 < *(float *)(lVar14 + 0x10c)) {
          FUN_1400f9a00(lVar14,&local_1f0);
        }
        uVar10 = (ulonglong)((int)uVar10 + 1);
      } while (uVar10 < (ulonglong)
                          ((*(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0)) / 0x340
                          ));
    }
    uVar10 = 0;
    lVar13 = *(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0);
    lVar14 = lVar13 >> 0x3f;
    if (lVar13 / 0x340 + lVar14 != lVar14) {
      do {
        lVar14 = uVar10 * 0x340 + *(longlong *)(param_1 + 0xc2b0);
        if (0.0 < *(float *)(lVar14 + 300)) {
          FUN_1400f9a00(lVar14,&local_1f0);
        }
        uVar10 = (ulonglong)((int)uVar10 + 1);
      } while (uVar10 < (ulonglong)
                          ((*(longlong *)(param_1 + 0xc2b8) - *(longlong *)(param_1 + 0xc2b0)) / 0x340
                          ));
    }
    FUN_14019c750(param_1);
    fVar6 = DAT_140909f58;
    local_1f4 = 0;
    uVar10 = lStack_1e8 - local_1f0 >> 3;
    local_158 = uVar10;
    if (uVar10 == 0) {
      ppppuVar23 = (uint ****)0x0;
      ppppuVar17 = (uint ****)0x0;
      ppppuVar18 = (uint ****)0x0;
      ppppuVar21 = (uint ****)0x0;
    }
    else {
      uVar22 = 0;
      lVar14 = local_1f0;
      do {
        lVar13 = *(longlong *)(lVar14 + uVar22 * 8);
        local_290 = lVar13;
        if (lVar13 != *(longlong *)(param_1 + 0xc2c8)) {
          local_1f8 = 0;
          local_88 = lStack_140 - local_148 >> 7;
          lVar14 = local_1f0;
          if (local_88 != 0) {
            uVar22 = 0;
            do {
              local_298 = local_268;
              pppuStack_260 = local_268;
              local_a0 = local_250;
              pppuStack_248 = local_250;
              pppuStack_218 = local_220;
              pppuStack_230 = local_238;
              local_90 = uVar22 * 0x80 + local_148;
              local_200 = *(longlong *)(local_90 + 0x10);
              local_208 = *(longlong *)(local_90 + 0x48);
              lVar14 = *(longlong *)
                        (*(longlong *)
                          (*(longlong *)(param_2 + 0xd38) + (longlong)*(int *)(local_90 + 0x18) * 8)
                        + 0x10);
              lVar11 = *(longlong *)(lVar14 + 0x10) - *(longlong *)(lVar14 + 8) >> 4;
              lVar20 = *(longlong *)
                        (*(longlong *)
                          (*(longlong *)(param_2 + 0xd38) + (longlong)*(int *)(local_90 + 0x50) * 8)
                        + 0x10);
              local_98 = *(longlong *)(lVar20 + 0xd8) - *(longlong *)(lVar20 + 0xd0) >> 4;
              local_a8 = lVar11;
              if (((*(longlong *)(local_200 + 0x318) == 0) ||
                  (*(longlong *)(local_208 + 0x318) == 0)) ||
                 ((*(int *)(*(longlong *)(local_200 + 0x318) + 0x360) != 0x14 ||
                  (*(int *)(*(longlong *)(local_208 + 0x318) + 0x360) != 0x14)))) {
                uVar10 = *(longlong *)(lVar14 + 0x10) - *(longlong *)(lVar14 + 8);
                if ((0xf < uVar10) &&
                   (uVar22 = 0, *(longlong *)(lVar14 + 0x10) - *(longlong *)(lVar14 + 8) >> 4 != 0))
                {
                  do {
                    if (lVar13 == *(longlong *)(*(longlong *)(lVar14 + 8) + uVar22 * 0x10))
                    goto LAB_1401e29f6;
                    uVar22 = (ulonglong)((int)uVar22 + 1);
                  } while (uVar22 < (ulonglong)
                                      (*(longlong *)(lVar14 + 0x10) - *(longlong *)(lVar14 + 8) >> 4))
                  ;
                }
                if (uVar10 < 0x10) {
LAB_1401e29f6:
                  uVar10 = *(longlong *)(lVar20 + 0xd8) - *(longlong *)(lVar20 + 0xd0);
                  local_res18._0_4_ = CONCAT31(local_res18._1_3_,uVar10 < 0x10);
                  if ((0xf < uVar10) &&
                     (uVar15 = 0,
                     *(longlong *)(lVar20 + 0xd8) - *(longlong *)(lVar20 + 0xd0) >> 4 != 0)) {
                    uVar22 = 0;
                    do {
                      if (local_290 == *(longlong *)(*(longlong *)(lVar20 + 0xd0) + uVar22 * 0x10))
                      {
                        local_res18._0_4_ = CONCAT31(local_res18._1_3_,1);
                        goto LAB_1401e2a74;
                      }
                      uVar15 = uVar15 + 1;
                      uVar22 = (ulonglong)uVar15;
                    } while (uVar22 < (ulonglong)
                                          (*(longlong *)(lVar20 + 0xd8) - *(longlong *)(lVar20 + 0xd0)
                                          >> 4));
                  }
                  lVar13 = local_290;
                  if (uVar10 < 0x10) {
LAB_1401e2a74:
                    local_res20._0_4_ = 0;
                    lVar13 = *(longlong *)(local_200 + 0x978) - *(longlong *)(local_200 + 0x970);
                    lVar14 = lVar13 >> 0x3f;
                    ppppuVar23 = (uint ****)local_220;
                    ppppuVar17 = (uint ****)local_268;
                    if (lVar13 / 0xe0 + lVar14 != lVar14) {
                      uVar10 = 0;
                      ppppuVar18 = (uint ****)local_268;
                      do {
                        plVar19 = (longlong *)(*(longlong *)(local_200 + 0x970) + uVar10 * 0xe0);
                        if ((int)plVar19[0x12] == 0x11) {
                          cVar8 = FUN_140428780(param_1,local_290);
                          uVar15 = (uint)local_res20;
                          if ((cVar8 != '\0') &&
                             ((plVar19 == (longlong *)0x0 ||
                              ((1 < (int)plVar19[0x11] - 5U &&
                               ((plVar19 == (longlong *)0x0 || (1 < (int)plVar19[0x11] - 7U))))))))
                          {
                            if ((*(longlong *)(local_200 + 0x318) != 0) &&
                               (*(int *)(*(longlong *)(local_200 + 0x318) + 0x360) != 5)) {
                              lVar14 = 0;
                              if (((plVar19 != (longlong *)0x0) && ((int)plVar19[0x12] == 0x11)) &&
                                 (uVar16 = 0, plVar19[4] - plVar19[3] >> 4 != 0)) {
                                uVar10 = 0;
                                lVar13 = plVar19[3];
                                do {
                                  if (fVar6 <= *(float *)(lVar13 + 8 + uVar10 * 0x10)) {
                                    lVar14 = *(longlong *)(lVar13 + (ulonglong)uVar16 * 0x10);
                                    break;
                                  }
                                  uVar16 = uVar16 + 1;
                                  uVar10 = (ulonglong)uVar16;
                                  lVar14 = 0;
                                } while (uVar10 < (ulonglong)(plVar19[4] - lVar13 >> 4));
                              }
                              if (lVar14 == *(longlong *)(param_1 + 0xc408)) {
                                lVar14 = *(longlong *)(param_1 + 0xc3b8);
                              }
                              if ((lVar14 != 0) && (local_290 != lVar14)) goto LAB_1401e2f23;
                            }
                            if (*(char *)((longlong)plVar19 + 0x84) == '\0') {
                              local_res18._0_4_ = CONCAT31(local_res18._1_3_,1);
                              local_298._0_4_ = (uint)local_res20;
                              if ((&local_298 < ppppuVar17) && (ppppuVar18 <= &local_298)) {
                                lVar14 = (longlong)&local_298 - (longlong)ppppuVar18;
                                if (ppppuVar17 == (uint ****)local_278) {
                                  FUN_14001e440(&local_268,1);
                                  local_278 = local_258;
                                  ppppuVar18 = (uint ****)local_268;
                                  ppppuVar17 = (uint ****)pppuStack_260;
                                }
                                if (ppppuVar17 != (uint ****)0x0) {
                                  *(undefined4 *)ppppuVar17 =
                                       *(undefined4 *)((longlong)ppppuVar18 + (lVar14 >> 2) * 4);
                                }
                              }
                              else {
                                if (ppppuVar17 == (uint ****)local_278) {
                                  FUN_14001e440(&local_268,1);
                                  local_278 = local_258;
                                  ppppuVar18 = (uint ****)local_268;
                                  ppppuVar17 = (uint ****)pppuStack_260;
                                }
                                if (ppppuVar17 != (uint ****)0x0) {
                                  *(uint *)ppppuVar17 = uVar15;
                                }
                              }
                              ppppuVar17 = (uint ****)((longlong)ppppuVar17 + 4);
                              local_298 = (uint ***)CONCAT44(local_298._4_4_,0xffffffff);
                              pppuStack_260 = (uint ***)ppppuVar17.
```

---

## 3. decomp_1406EE2A0.c (FUN_1406EE2A0 - Percentage Increment)

<!-- DECOMP_1406EE2A0_CONTENT -->

---

## 4. Stock/Capacity Helper Functions

### 4.1 clip_fn.c (FUN_1404461F0)

<!-- CLIP_FN_CONTENT -->

### 4.2 clip_push.c (FUN_1404461F0 duplicate)

<!-- CLIP_PUSH_CONTENT -->

### 4.3 decomp_1406EE2A0.c (Percentage Increment - already in section 3)

---

## 5. Percentage Clamping Block & Bounds Assignments

<!-- PERCENTAGE_CLAMPING_CONTENT -->