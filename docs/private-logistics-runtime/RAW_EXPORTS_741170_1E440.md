# Raw Ghidra Exports — FUN_140741170 and FUN_14001E440

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON  
**All addresses are RVAs** (file-based = 0x140 + RVA)  
**Generated:** 2026-10-29 via scratch/ghidra_run.bat (Ghidra 12.1.4, JDK 21)

---

## 1. Full Decompilation of FUN_140741170 (0x140741170)

```c
// ============================================================
// Function: FUN_140741170 at 140741170
// Target address checked: 0x140741170
// ============================================================

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140741170(longlong param_1,ulonglong param_2)

{
  longlong *plVar1;
  char cVar2;
  wchar_t wVar3;
  uint uVar4;
  longlong lVar5;
  undefined4 *puVar6;
  longlong lVar7;
  longlong lVar8;
  bool bVar9;
  float fVar10;
  double dVar11;
  double dVar12;
  bool bVar13;
  bool bVar14;
  bool bVar15;
  char cVar16;
  bool bVar17;
  undefined1 uVar18;
  short sVar19;
  int iVar20;
  wchar_t *pwVar21;
  longlong *plVar22;
  char *pcVar23;
  longlong lVar24;
  undefined8 uVar25;
  undefined4 *puVar26;
  float *pfVar27;
  undefined8 uVar28;
  longlong *plVar29;
  longlong lVar30;
  short *psVar31;
  short *psVar32;
  int iVar33;
  undefined1 *puVar34;
  ulonglong uVar35;
  void *_Memory;
  undefined8 *puVar36;
  ulonglong uVar37;
  longlong lVar38;
  void *pvVar39;
  uint uVar40;
  ulonglong uVar41;
  int *piVar42;
  ulonglong uVar43;
  ulonglong uVar44;
  int *piVar45;
  char *pcVar46;
  float fVar47;
  float fVar48;
  float fVar49;
  float fVar50;
  double dVar51;
  float fVar52;
  float fVar53;
  float fVar54;
  undefined4 uVar55;
  undefined4 uVar56;
  undefined4 uVar57;
  undefined4 uVar58;
  float fVar59;
  undefined4 uVar60;
  undefined4 uVar61;
  undefined4 uVar62;
  undefined4 uVar63;
  float fVar64;
  undefined4 uVar65;
  undefined4 uVar66;
  undefined4 uVar67;
  undefined4 uVar68;
  undefined1 auStackY_f58 [32];
  float local_ec8;
  float local_ec4;
  int local_ec0;
  char local_ebc;
  undefined1 local_ebb;
  uint local_eb8 [2];
  ulonglong local_eb0;
  undefined4 local_ea0;
  uint local_e98;
  int *local_e90;
  double local_e88;
  ulonglong local_e80;
  ulonglong local_e78;
  int *local_e70;
  int *local_e68;
  int *piStack_e60;
  int *local_e58;
  undefined4 local_e48;
  longlong *local_e40;
  longlong *plStack_e38;
  longlong *local_e30;
  longlong local_e28;
  longlong local_e18;
  ulonglong uStack_e10;
  longlong local_e08;
  longlong lStack_e00;
  ulonglong local_df8;
  longlong local_df0;
  longlong lStack_de8;
  longlong local_de0;
  longlong local_dd8 [5];
  undefined4 local_db0;
  undefined4 local_da4;
  undefined4 local_d98;
  undefined8 local_d90;
  undefined1 local_d88 [16];
  undefined1 local_d78 [96];
  undefined8 local_d18;
  uint uStack_d10;
  undefined4 uStack_d0c;
  longlong local_d08;
  undefined8 uStack_d00;
  undefined1 local_cf8 [14];
  short sStack_cea;
  wchar_t local_ce8 [2];
  short local_ce4 [1022];
  undefined1 local_4e8 [512];
  undefined1 local_2e8 [512];
  ulonglong local_e8;
  
  local_d90 = 0xfffffffffffffffe;
  local_e8 = DAT_140992048 ^ (ulonglong)auStackY_f58;
  uVar43 = **(ulonglong **)(param_1 + 0xcac0);
  if (*(char *)(param_2 + 1) != '\0') {
    return;
  }
  local_df0 = 0;
  lStack_de8 = 0;
  local_de0 = 0;
  local_e68 = (int *)0x0;
  piStack_e60 = (int *)0x0;
  local_e70 = (int *)0x0;
  local_e58 = (int *)0x0;
  local_eb8[0] = 0;
  local_dd8[0] = 0;
  local_dd8[1] = 0;
  local_dd8[2] = 0;
  plVar29 = local_dd8;
  local_eb0 = param_2;
  local_e28 = param_1;
  FUN_1400cce00(plVar29,0,local_eb8);
  fVar50 = DAT_14090a9f0;
  fVar49 = DAT_14090a9cc;
  uVar44 = 0;
  local_e78 = 0;
  local_dd8[3] = 0;
  fVar64 = (*(float *)(param_2 + 4) - DAT_140992088 * DAT_14090a6a8) + *(float *)(param_2 + 0x28) +
           DAT_140992088 * DAT_14090a9cc;
  fVar52 = *(float *)(param_2 + 8) + *(float *)(param_2 + 0x2c);
  local_ec8 = DAT_140992088 * DAT_14090a9f0 + fVar52;
  fVar52 = DAT_140992088 * DAT_14090ab84 + fVar52;
  lVar38 = *(longlong *)(param_2 + 0x240);
  local_ebc = '\0';
  local_ec4 = fVar64;
  if ((param_2 == uVar43) &&
     (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0)) {
    param_2 = uVar44;
    uVar43 = uVar44;
    do {
      lVar24 = *(longlong *)(param_2 + *(longlong *)(lVar38 + 0xd38));
      lVar30 = *(longlong *)(lVar24 + 8);
      if (((longlong *)(param_1 + 0x11dc0) != (longlong *)0x0) &&
         (lVar5 = *(longlong *)(param_1 + 0x11dc0),
         *(longlong *)(param_1 + 0x11dc8) - lVar5 >> 3 != 0)) {
        uVar41 = uVar44;
        do {
          if (lVar30 == *(longlong *)(lVar5 + uVar41 * 8)) {
            pcVar46 = *(char **)(lVar24 + 0x10);
            if (((*pcVar46 == '\0') ||
                ((local_d08 = _DAT_140908e68, uStack_d00 = _UNK_140908e70, pcVar46[200] == '\0' &&
                 (local_d08 = _DAT_140908e08, uStack_d00 = _UNK_140908e10, *pcVar46 == '\0')))) &&
               (local_d08 = _DAT_140908de8, uStack_d00 = _DAT_140908df0, pcVar46[200] != '\0')) {
              local_d08 = _DAT_140908df8;
              uStack_d00 = _UNK_140908e00;
            }
            FUN_14040c550(param_1,lVar30,0);
            break;
          }
          uVar41 = (ulonglong)((int)uVar41 + 1);
        } while (uVar41 < (ulonglong)(*(longlong *)(param_1 + 0x11dc8) - lVar5 >> 3));
      }
      uVar40 = (int)uVar43 + 1;
      uVar43 = (ulonglong)uVar40;
      param_2 = param_2 + 8;
      plVar29 = (longlong *)(*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3);
    } while ((longlong *)(longlong)(int)uVar40 < plVar29);
  }
  uVar43 = local_e78;
  uVar41 = 0;
  uVar44 = uVar41;
  if (*(longlong *)(lVar38 + 0xc78) - *(longlong *)(lVar38 + 0xc70) >> 3 != 0) {
    do {
      uVar35 = 0;
      lVar24 = *(longlong *)(uVar41 + *(longlong *)(lVar38 + 0xc70));
      if ((*(longlong *)(lVar24 + 0x1708) == 0) ||
         (*(int *)(*(longlong *)(lVar24 + 0x1708) + 0x8600) != 0x11)) {
        if (*(longlong *)(lVar24 + 0x3c0) - *(longlong *)(lVar24 + 0x3b8) >> 4 != 0) {
          do {
            lVar30 = *(longlong *)(*(longlong *)(lVar24 + 0x3b8) + uVar35 * 0x10);
            if ((-1 < *(int *)(lVar30 + 0x44)) && (*(int *)(lVar24 + 0x1794) == 0)) {
              FUN_1400f9a00(lVar30,&local_df0);
            }
            uVar35 = (ulonglong)((int)uVar35 + 1);
          } while (uVar35 < (ulonglong)
                            (*(longlong *)(lVar24 + 0x3c0) - *(longlong *)(lVar24 + 0x3b8) >> 4));
        }
      }
      else {
        FUN_14019c750(&DAT_1409d4f10,&local_df0);
      }
      param_2 = 0;
      uVar35 = param_2;
      if (*(longlong *)(lVar24 + 0x3a8) - *(longlong *)(lVar24 + 0x3a0) >> 3 != 0) {
        do {
          FUN_1406bed90(*(undefined8 *)(*(longlong *)(lVar24 + 0x3a0) + uVar35 * 8),&local_df0,1);
          uVar40 = (int)uVar35 + 1;
          param_2 = (ulonglong)uVar40;
          uVar35 = (ulonglong)uVar40;
        } while ((ulonglong)uVar40 <
                 (ulonglong)(*(longlong *)(lVar24 + 0x3a8) - *(longlong *)(lVar24 + 0x3a0) >> 3));
      }
      if ((local_ebc == '\0') &&
         ((*(longlong *)(lVar24 + 0x520) != 0 || (*(longlong *)(lVar24 + 0x518) != 0)))) {
        local_ebc = '\x01';
      }
      uVar40 = (int)uVar44 + 1;
      uVar44 = (ulonglong)uVar40;
      uVar41 = uVar41 + 8;
      plVar29 = (longlong *)(*(longlong *)(lVar38 + 0xc78) - *(longlong *)(lVar38 + 0xc70) >> 3);
    } while ((longlong *)(longlong)(int)uVar40 < plVar29);
  }
  uVar55 = DAT_140909b90;
  uVar35 = 0;
  local_eb8[0] = local_eb8[0] & 0xffffff00;
  param_2 = param_2 & 0xffffffffffffff00;
  local_e98 = (uint)param_2;
  uVar41 = (ulonglong)plVar29 & 0xffffffffffffff00;
  local_e88 = (double)CONCAT44(local_e88._4_4_,(int)uVar41);
  bVar17 = false;
  uVar44 = uVar44 & 0xffffffffffffff00;
  local_e80 = CONCAT44(local_e80._4_4_,(int)uVar44);
  local_ec0 = 0;
  local_e90 = piStack_e60;
  bVar13 = false;
  if (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0) {
    do {
      bVar17 = bVar13;
      uVar41 = 0;
      local_e78 = *(ulonglong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8);
      if (**(char **)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) != '\0') {
        bVar13 = false;
        bVar9 = false;
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8);
        lVar30 = *(longlong *)(lVar24 + 0x978) - *(longlong *)(lVar24 + 0x970);
        lVar24 = lVar30 >> 0x3f;
        bVar14 = false;
        bVar15 = false;
        if (lVar30 / 0xe0 + lVar24 != lVar24) {
          do {
            bVar9 = bVar15;
            bVar13 = bVar14;
            if (*(int *)(uVar41 * 0xe0 + 0x90 +
                        *(longlong *)
                         (*(longlong *)
                           (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8) + 0x970))
                == 2) {
              bVar13 = true;
            }
            if (*(int *)(uVar41 * 0xe0 + 0x90 +
                        *(longlong *)
                         (*(longlong *)
                           (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8) + 0x970))
                == 1) {
              bVar9 = true;
            }
            lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8);
            uVar41 = (ulonglong)((int)uVar41 + 1);
            bVar14 = bVar13;
            bVar15 = bVar9;
          } while (uVar41 < (ulonglong)
                            ((*(longlong *)(lVar24 + 0x978) - *(longlong *)(lVar24 + 0x970)) / 0xe0)
                  );
          param_2 = (ulonglong)local_e98;
          param_1 = local_e28;
        }
        uVar41 = 0;
        uVar40 = (uint)param_2;
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10);
        if (*(longlong *)(lVar24 + 0x10) - *(longlong *)(lVar24 + 8) >> 4 != 0) {
          do {
            if ((bVar13) &&
               (((*(longlong *)
                   (*(longlong *)
                     (*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) + 8) +
                   uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3d0) ||
                 (*(longlong *)
                   (*(longlong *)
                     (*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) + 8) +
                   uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3d8))) ||
                (*(longlong *)
                  (*(longlong *)
                    (*(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
                    + 8) + uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3c8))))) {
              param_2 = 1;
            }
            uVar40 = (uint)param_2;
            if ((bVar9) &&
               (*(longlong *)
                 (*(longlong *)
                   (*(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
                   + 8) + uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3e0))) {
              bVar17 = true;
            }
            uVar41 = (ulonglong)((int)uVar41 + 1);
            lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
            ;
          } while (uVar41 < (ulonglong)
                            (*(longlong *)(lVar24 + 0x10) - *(longlong *)(lVar24 + 8) >> 4));
          uVar44 = local_e80 & 0xffffffff;
          local_e98 = uVar40;
        }
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10);
        if ((ulonglong)(*(longlong *)(lVar24 + 0x10) - *(longlong *)(lVar24 + 8)) < 0x10) {
          local_e98 = uVar40 & 0xff;
          if (bVar13) {
            local_e98 = 1;
          }
          if (bVar9) {
            bVar17 = true;
          }
        }
      }
      uVar41 = 0;
      if (*(char *)(*(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
                   + 200) != '\0') {
        bVar13 = false;
        bVar9 = false;
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8);
        lVar30 = *(longlong *)(lVar24 + 0x978) - *(longlong *)(lVar24 + 0x970);
        lVar24 = lVar30 >> 0x3f;
        bVar14 = false;
        bVar15 = false;
        if (lVar30 / 0xe0 + lVar24 != lVar24) {
          do {
            bVar9 = bVar15;
            bVar13 = bVar14;
            if (*(int *)(*(longlong *)
                          (*(longlong *)
                            (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8) + 0x970)
                         + 0x90 + uVar41 * 0xe0) == 2) {
              bVar13 = true;
            }
            if (*(int *)(*(longlong *)
                          (*(longlong *)
                            (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8) + 0x970)
                         + 0x90 + uVar41 * 0xe0) == 1) {
              bVar9 = true;
            }
            lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8);
            uVar41 = (ulonglong)((int)uVar41 + 1);
            param_1 = local_e28;
            bVar14 = bVar13;
            bVar15 = bVar9;
          } while (uVar41 < (ulonglong)
                            ((*(longlong *)(lVar24 + 0x978) - *(longlong *)(lVar24 + 0x970)) / 0xe0)
                  );
        }
        uVar41 = 0;
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10);
        if (*(longlong *)(lVar24 + 0xd8) - *(longlong *)(lVar24 + 0xd0) >> 4 != 0) {
          do {
            if ((bVar13) &&
               (((*(longlong *)
                   (*(longlong *)
                     (*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) + 0xd0) +
                   uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3d0) ||
                 (*(longlong *)
                   (*(longlong *)
                     (*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) + 0xd0) +
                   uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3d8))) ||
                (*(longlong *)
                  (*(longlong *)
                    (*(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
                    + 0xd0) + uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3c8))))) {
              local_e88 = (double)CONCAT71(local_e88._1_7_,1);
            }
            if ((bVar9) &&
               (uVar44 = uVar44 & 0xff,
               *(longlong *)
                (*(longlong *)
                  (*(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) +
                  0xd0) + uVar41 * 0x10) == *(longlong *)(param_1 + 0xc3e0))) {
              uVar44 = 1;
            }
            uVar41 = (ulonglong)((int)uVar41 + 1);
            lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
            ;
          } while (uVar41 < (ulonglong)
                            (*(longlong *)(lVar24 + 0xd8) - *(longlong *)(lVar24 + 0xd0) >> 4));
          local_e80 = CONCAT44(local_e80._4_4_,(int)uVar44);
        }
        lVar24 = *(longlong *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10);
        if ((ulonglong)(*(longlong *)(lVar24 + 0xd8) - *(longlong *)(lVar24 + 0xd0)) < 0x10) {
          uVar40 = local_e88._0_4_ & 0xff;
          if (bVar13) {
            uVar40 = 1;
          }
          local_e88 = (double)CONCAT44(local_e88._4_4_,uVar40);
          uVar44 = uVar44 & 0xff;
          if (bVar9) {
            uVar44 = 1;
          }
          local_e80 = CONCAT44(local_e80._4_4_,(int)uVar44);
        }
      }
      if ((*(longlong *)(local_e78 + 0x318) != 0) &&
         (*(int *)(*(longlong *)(local_e78 + 0x318) + 0x360) == 0)) {
        if ((**(char **)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) != '\0')
           && (0.0 < *(float *)(*(longlong *)
                                 (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10)
                               + 4))) {
          FUN_140424430(param_1,local_e78,0,0);
          uVar37 = 0;
          lVar30 = *(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8);
          lVar24 = lVar30 >> 0x3f;
          uVar41 = uVar37;
          if (lVar30 / 0x68 + lVar24 != lVar24) {
            do {
              if (*(int *)(*(longlong *)
                            (*(longlong *)(uVar37 + 0x30 + *(longlong *)(param_1 + 0x144a8)) + 0x318
                            ) + 0x360) == 0xb) {
                if ((&local_ec0 < local_e90) && (local_e68 <= &local_ec0)) {
                  lVar24 = (longlong)&local_ec0 - (longlong)local_e68;
                  if (local_e90 == local_e70) {
                    FUN_14001e440(&local_e68,1);
                    local_e70 = local_e58;
                    local_e90 = piStack_e60;
                  }
                  if (local_e90 != (int *)0x0) {
                    iVar20 = local_e68[lVar24 >> 2];
LAB_140741c07:
                    *local_e90 = iVar20;
                  }
                }
                else {
                  if (local_e90 == local_e70) {
                    FUN_14001e440(&local_e68,1);
                    local_e70 = local_e58;
                    local_e90 = piStack_e60;
                  }
                  iVar20 = local_ec0;
                  if (local_e90 != (int *)0x0) goto LAB_140741c07;
                }
                local_ebb = 1;
                local_d18._0_2_ = (undefined2)local_dd8[0];
                local_d18._2_2_ = (undefined2)((ulonglong)local_dd8[0] >> 0x10);
                local_d18._4_4_ = (undefined4)((ulonglong)local_dd8[0] >> 0x20);
                uStack_d10 = 0;
                if (uVar43 != 0) {
                  if (((longlong)uVar43 < 0) && (uVar43 != 0)) {
                    lVar24 = local_dd8[0] - ((-uVar43 - 1 >> 5) * 4 + 4);
                    local_d18._0_2_ = (undefined2)lVar24;
                    local_d18._2_2_ = (undefined2)((ulonglong)lVar24 >> 0x10);
                    local_d18._4_4_ = (undefined4)((ulonglong)lVar24 >> 0x20);
                  }
                  else {
                    lVar24 = local_dd8[0] + (uVar43 >> 5) * 4;
                    local_d18._0_2_ = (undefined2)lVar24;
                    local_d18._2_2_ = (undefined2)((ulonglong)lVar24 >> 0x10);
                    local_d18._4_4_ = (undefined4)((ulonglong)lVar24 >> 0x20);
                  }
                  uStack_d10 = (uint)uVar43 & 0x1f;
                }
                uStack_d0c = 0;
                local_d08 = CONCAT44(local_d18._4_4_,CONCAT22(local_d18._2_2_,(undefined2)local_d18)
                                    );
                uStack_d00 = (ulonglong)uStack_d10;
                puVar34 = local_d78;
                goto LAB_140741e3e;
              }
              uVar40 = (int)uVar41 + 1;
              uVar37 = uVar37 + 0x68;
              uVar41 = (ulonglong)uVar40;
            } while ((ulonglong)(longlong)(int)uVar40 <
                     (ulonglong)
                     ((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68))
            ;
          }
        }
        if (*(char *)(*(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 0x10) + 200) !=
            '\0') {
          FUN_140424430(param_1,local_e78,0,uVar55);
          uVar37 = 0;
          lVar30 = *(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8);
          lVar24 = lVar30 >> 0x3f;
          uVar41 = uVar37;
          if (lVar30 / 0x68 + lVar24 != lVar24) {
LAB_140741d00:
            if (*(int *)(*(longlong *)
                          (*(longlong *)(uVar37 + 0x30 + *(longlong *)(param_1 + 0x144a8)) + 0x318)
                        + 0x360) != 0xb) goto code_r0x000140741d1c;
            if ((&local_ec0 < local_e90) && (local_e68 <= &local_ec0)) {
              lVar24 = (longlong)&local_ec0 - (longlong)local_e68;
              if (local_e90 == local_e70) {
                FUN_14001e440(&local_e68,1);
                local_e70 = local_e58;
                local_e90 = piStack_e60;
              }
              if (local_e90 != (int *)0x0) {
                iVar20 = local_e68[lVar24 >> 2];
LAB_140741dd3:
                *local_e90 = iVar20;
              }
            }
            else {
              if (local_e90 == local_e70) {
                FUN_14001e440(&local_e68,1);
                local_e70 = local_e58;
                local_e90 = piStack_e60;
              }
              iVar20 = local_ec0;
              if (local_e90 != (int *)0x0) goto LAB_140741dd3;
            }
            local_ebb = 0;
            local_e18 = local_dd8[0];
            uStack_e10 = 0;
            if (uVar43 != 0) {
              if (((longlong)uVar43 < 0) && (uVar43 != 0)) {
                local_e18 = -((-uVar43 - 1 >> 5) * 4 + 4);
              }
              else {
                local_e18 = (uVar43 >> 5) * 4;
              }
              local_e18 = local_dd8[0] + local_e18;
              uStack_e10 = (ulonglong)((uint)uVar43 & 0x1f);
            }
            puVar34 = local_d88;
            local_d08 = local_e18;
            uStack_d00 = uStack_e10;
LAB_140741e3e:
            piStack_e60 = local_e90 + 1;
            local_e90 = piStack_e60;
            FUN_140204c70(local_dd8,puVar34,&local_d08);
            uVar43 = local_dd8[3];
          }
        }
      }
LAB_140741e6f:
      local_eb8[0] = local_eb8[0] & 0xff;
      if (**(char **)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) == '\0') {
        local_eb8[0] = 1;
      }
      local_ec0 = local_ec0 + 1;
      uVar35 = (ulonglong)local_ec0;
      param_2 = (ulonglong)local_e98;
      bVar13 = bVar17;
    } while (uVar35 < (ulonglong)
                      (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3));
    uVar41 = (ulonglong)local_e88 & 0xffffffff;
    fVar49 = DAT_14090a9cc;
  }
  uVar43 = 0;
  bVar13 = false;
  plVar29 = (longlong *)(lVar38 + 0xc70);
  if ((((char)param_2 != '\0') && ((char)uVar41 != '\0')) &&
     (bVar13 = true, uVar41 = uVar43, *(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3 != 0)) {
    do {
      lVar24 = *(longlong *)(*plVar29 + uVar41 * 8);
      if ((*(longlong *)(lVar24 + 0x1708) != 0) &&
         (*(int *)(*(longlong *)(lVar24 + 0x1708) + 0x8600) == 2)) {
        bVar13 = false;
        break;
      }
      uVar41 = (ulonglong)((int)uVar41 + 1);
    } while (uVar41 < (ulonglong)(*(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3));
  }
  if ((bVar17) && ((char)uVar44 != '\0')) {
    bVar17 = true;
    if (*(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3 != 0) {
      do {
        lVar24 = *(longlong *)(*plVar29 + uVar43 * 8);
        if ((*(longlong *)(lVar24 + 0x1708) != 0) &&
           (*(int *)(*(longlong *)(lVar24 + 0x1708) + 0x8600) == 1)) goto LAB_140741f99;
        uVar43 = (ulonglong)((int)uVar43 + 1);
      } while (uVar43 < (ulonglong)(*(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3));
    }
  }
  else {
LAB_140741f99:
    bVar17 = false;
  }
  FUN_1406f36b0(param_1,lVar38,local_ec4,local_ec8);
  local_ec4 = DAT_140992088 * DAT_14090ab98 + fVar64;
  local_ce8[0] = L'\0';
  bVar9 = false;
  cVar16 = FUN_1401bbfe0(param_1,lVar38);
  if (cVar16 == '\0') {
    cVar16 = FUN_1401bbd50();
    if (cVar16 != '\0') {
      psVar31 = &sStack_cea;
      do {
        psVar31 = psVar31 + 1;
      } while (*psVar31 != 0);
      *(undefined4 *)psVar31 = DAT_140886354;
      pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x954);
      psVar31 = &sStack_cea;
      do {
        psVar31 = psVar31 + 1;
      } while (*psVar31 != 0);
      lVar24 = 0;
      do {
        wVar3 = pwVar21[lVar24];
        psVar31[lVar24] = wVar3;
        lVar24 = lVar24 + 1;
      } while (wVar3 != L'\0');
      psVar31 = &sStack_cea;
      do {
        psVar31 = psVar31 + 1;
      } while (*psVar31 != 0);
      *(undefined4 *)psVar31 = DAT_1408fc154;
      sVar19 = DAT_1408fc158;
      goto LAB_140742119;
    }
  }
  else {
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc174;
    psVar32[3] = DAT_1408fc178;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x95a);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc17c;
    sVar19 = DAT_1408fc180;
LAB_140742119:
    bVar9 = true;
    psVar31[2] = sVar19;
  }
  cVar16 = FUN_14076a8d0(param_1,lVar38,local_ce8);
  if (cVar16 != '\0') {
    bVar9 = true;
  }
  cVar16 = FUN_1401bbe70(param_1,lVar38);
  if ((cVar16 != '\0') || (*(char *)(param_1 + 0xcbd8) != '\0')) {
    bVar9 = true;
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc15c;
    psVar32[3] = DAT_1408fc160;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x597);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc164;
    psVar32[3] = DAT_1408fc168;
  }
  if ((bVar13) || (*(char *)(param_1 + 0xcbd8) != '\0')) {
    bVar9 = true;
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc16c;
    psVar32[3] = DAT_1408fc170;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110d);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc060;
    psVar32[3] = DAT_1408fc064;
  }
  if ((bVar17) || (*(char *)(param_1 + 0xcbd8) != '\0')) {
    bVar9 = true;
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc068;
    psVar32[3] = DAT_1408fc06c;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110e);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc070;
    psVar32[3] = DAT_1408fc074;
  }
  if (((ulonglong)(*(longlong *)(lVar38 + 0xc78) - *plVar29) < 8) ||
     (*(char *)(param_1 + 0xcbd8) != '\0')) {
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc078;
    psVar32[3] = DAT_1408fc07c;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b0);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc040;
    sVar19 = DAT_1408fc044;
LAB_1407424b9:
    bVar9 = true;
    psVar31[2] = sVar19;
  }
  else if ((ulonglong)(*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38)) < 8) {
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc048;
    psVar32[3] = DAT_1408fc04c;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x59a);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc050;
    sVar19 = DAT_1408fc054;
    goto LAB_1407424b9;
  }
  if ((char)local_eb8[0] == '\0') {
    if (local_ebc != '\0') {
      psVar31 = &sStack_cea;
      do {
        psVar32 = psVar31;
        psVar31 = psVar32 + 1;
      } while (*psVar31 != 0);
      *(undefined4 *)psVar31 = DAT_1408fc0b8;
      psVar32[3] = DAT_1408fc0bc;
      pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0xfe2);
      psVar31 = &sStack_cea;
      do {
        psVar31 = psVar31 + 1;
      } while (*psVar31 != 0);
      lVar24 = 0;
      do {
        wVar3 = pwVar21[lVar24];
        psVar31[lVar24] = wVar3;
        lVar24 = lVar24 + 1;
      } while (wVar3 != L'\0');
      psVar31 = &sStack_cea;
      do {
        psVar31 = psVar31 + 1;
      } while (*psVar31 != 0);
      *(undefined4 *)psVar31 = DAT_1408fc0c0;
      sVar19 = DAT_1408fc0c4;
      goto LAB_1407425f9;
    }
  }
  else {
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc058;
    psVar32[3] = DAT_1408fc05c;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b6);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc0b0;
    sVar19 = DAT_1408fc0b4;
LAB_1407425f9:
    bVar9 = true;
    psVar31[2] = sVar19;
  }
  piVar45 = local_e68;
  piVar42 = (int *)((longlong)local_e90 - (longlong)local_e68 >> 2);
  local_e90 = piVar42;
  if (piVar42 == (int *)0x0) {
    if (!bVar9) {
      plVar22 = *(longlong **)(param_1 + 0xb518);
      goto LAB_1407426c3;
    }
  }
  else {
    bVar9 = true;
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc0c8;
    psVar32[3] = DAT_1408fc0cc;
    pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b3);
    psVar31 = &sStack_cea;
    do {
      psVar31 = psVar31 + 1;
    } while (*psVar31 != 0);
    lVar24 = 0;
    do {
      wVar3 = pwVar21[lVar24];
      psVar31[lVar24] = wVar3;
      lVar24 = lVar24 + 1;
    } while (wVar3 != L'\0');
    psVar31 = &sStack_cea;
    do {
      psVar32 = psVar31;
      psVar31 = psVar32 + 1;
    } while (*psVar31 != 0);
    *(undefined4 *)psVar31 = DAT_1408fc080;
    psVar32[3] = DAT_1408fc084;
  }
  plVar22 = *(longlong **)(param_1 + 0xb500);
LAB_1407426c3:
  (**(code **)(*plVar22 + 0x70))(plVar22,0,DAT_1409ead08);
  fVar10 = DAT_140909f58;
  _DAT_1409be30c = _DAT_14090c4c0;
  uRam00000001409be314 = _UNK_14090c4c8;
  local_ea0 = 0;
  _DAT_1409be2f0 = CONCAT44(fVar52 - DAT_140992088 * fVar49,DAT_140992088 * _DAT_14090aa90 + fVar64)
  ;
  DAT_1409be2f8 = 0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090aab8;
  _DAT_1409be2ec = _DAT_1409be2e8;
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,DAT_140909f58,DAT_140909f58,0.0,true);
  fVar49 = DAT_14090a8cc;
  fVar54 = 0.0;
  if (bVar9) {
    (**(code **)(*DAT_140994208 + 0x28))
              (DAT_140994208,&local_d08,local_ec4 - DAT_140992088 * 0.0,
               DAT_140992088 * 0.0 + local_ec8);
    if (DAT_140992088 * DAT_14090a828 + fVar52 < (float)uStack_d00._4_4_) {
      fVar54 = ((float)uStack_d00._4_4_ - fVar52) + DAT_140992088 * fVar49;
    }
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140994208,
               DAT_140992088 * 0.0 + local_ec4,DAT_140992088 * 0.0 + local_ec8,0xff990000,local_ce8)
    ;
  }
  local_ec8 = fVar54 + fVar52;
  local_ec4 = fVar64;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x214);
  fVar54 = DAT_14090a7b0;
  fVar52 = DAT_14090a444;
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             local_ec4 - DAT_140992088 * DAT_14090a7b0,local_ec8 - DAT_140992088 * DAT_14090a444,
             0xff990000,L"%ls:");
  lVar24 = *DAT_140995220;
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x214);
  plVar22 = (longlong *)(**(code **)(lVar24 + 0x28))(DAT_140995220,&local_d18,0,0);
  local_d08 = *plVar22;
  uStack_d00 = plVar22[1];
  uVar43 = 0;
  if ((lVar38 != 0) && (uVar44 = uVar43, *(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3 != 0)) {
    do {
      uVar44 = (ulonglong)((int)uVar44 + 1);
    } while (uVar44 < (ulonglong)(*(longlong *)(lVar38 + 0xc78) - *plVar29 >> 3));
  }
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             (local_ec4 - DAT_140992088 * fVar54) + (float)(int)uStack_d00,
             local_ec8 - DAT_140992088 * fVar52,0xff000000,L"%d");
  uVar44 = local_eb0;
  local_ec4 = fVar64 - DAT_140992088 * fVar49;
  local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a9b4;
  FUN_1407a4990(param_1,&local_ec4,&local_ec8,local_eb0);
  fVar54 = DAT_14090a9a0;
  local_ec4 = fVar64;
  cVar16 = FUN_1406f49d0(param_1,uVar44,fVar64 - DAT_140992088 * DAT_14090a9a0,
                         DAT_140992088 * fVar49 + local_ec8);
  if (*(char *)(uVar44 + 0x30c) != cVar16) {
    DAT_1409e183b = cVar16;
  }
  *(char *)(uVar44 + 0x30c) = cVar16;
  pcVar46 = &DAT_1409e81c4;
  if (((cVar16 != '\0') && (DAT_1409e8088 != -1)) &&
     ((iVar20 = strcmp(&DAT_1409e8258,"SetDistOffSource"), iVar20 == 0 ||
      (iVar20 = strcmp(&DAT_1409e8258,"SetDistOffDestination"), iVar20 == 0)))) {
    pcVar23 = &DAT_1409e81c4;
    do {
      cVar16 = *pcVar23;
      cVar2 = pcVar23[*(longlong *)(lVar38 + 0x318) + -0x1409e81c4];
      if (cVar16 != cVar2) break;
      pcVar23 = pcVar23 + 1;
    } while (cVar2 != '\0');
    if ((cVar16 == cVar2) && (DAT_1409e82ec < 2)) {
      DAT_1409e82ec = 2;
      _DAT_1409e82f8 = local_ec4 - DAT_140992088 * fVar54;
      _DAT_1409e82fc = DAT_140992088 * fVar49 + local_ec8;
    }
  }
  if (*(char *)(uVar44 + 0x30c) == '\0') {
    local_e40 = (longlong *)0x0;
    plStack_e38 = (longlong *)0x0;
    local_e30 = (longlong *)0x0;
    local_e08 = 0;
    lStack_e00 = 0;
    local_e78 = 0;
    local_df8 = 0;
    FUN_14076a3a0();
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x814);
    fVar54 = DAT_14090a730;
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
               local_ec4 - DAT_140992088 * DAT_14090a730,DAT_140992088 * fVar52 + local_ec8,
               0xff990000,L"%ls: %d / 49");
    lVar24 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x814);
    lVar24 = (**(code **)(lVar24 + 0x28))(DAT_140995220,&local_d08,0,0);
    iVar20 = *(int *)(lVar24 + 8);
    local_ec8 = local_ec8 + DAT_140992088 * fVar49;
    (**(code **)(**(longlong **)(param_1 + 0xb330) + 0x70))
              (*(longlong **)(param_1 + 0xb330),0,DAT_1409ead08);
    fVar59 = (float)iVar20;
    local_ea0 = 0;
    _DAT_1409be2f0 = CONCAT44(local_ec8,fVar59 + local_ec4 + DAT_140992088 * fVar49);
    DAT_1409be2f8 = 0;
    _DAT_1409be2e8 = DAT_140992088 * fVar50;
    _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
    uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
    _DAT_1409be2ec = _DAT_1409be2e8;
    if ((*(int *)(param_1 + 0xada8) == -1) ||
       (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
      FUN_1404461f0(param_1 + 0xadb0);
    }
    uVar44 = local_eb0;
    if (*(char *)(local_eb0 + 0x20) != '\0') {
      uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
      if (bVar17) {
        _DAT_1409be30c = _DAT_14090c4c0;
        uRam00000001409be314 = _UNK_14090c4c8;
        pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x80d);
        FUN_140015900(param_1 + 0xd5a0,pwVar21);
        if (DAT_140a54e91 != '\0') {
          lVar24 = FUN_14003aaa0(param_1,"building_addstation_dist");
          if (*(longlong *)(param_1 + 0xd428) == lVar24) {
            if ((*(char *)(param_1 + 0x14) == '\0') &&
               (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                   ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_cancel"), bVar17)) {
              pcVar23 = C3D_CONFIGURATOR::GetValueString
                                  ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_cancel");
              C3D_SOUNDMANAGER::PlaySound2D
                        (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar23,true,fVar10,(char *)0x0);
            }
            *(undefined8 *)(param_1 + 0xd428) = 0;
          }
          else {
            if ((*(char *)(param_1 + 0x14) == '\0') &&
               (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                   ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
              pcVar23 = C3D_CONFIGURATOR::GetValueString
                                  ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
              C3D_SOUNDMANAGER::PlaySound2D
                        (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar23,true,fVar10,(char *)0x0);
            }
            uVar25 = FUN_14003aaa0(param_1,"building_addstation_dist");
            *(undefined8 *)(param_1 + 0xd428) = uVar25;
            *(longlong *)(param_1 + 0xd298) = lVar38;
            *(ulonglong *)(param_1 + 0xd2a0) = uVar44;
            *(undefined1 *)(param_1 + 0xf5b0) = 0;
            *(undefined4 *)(param_1 + 0xf5c0) = 0xbf800000;
            *(undefined8 *)(param_1 + 0xf5b8) = 0;
            FUN_140201cc0(param_1 + 0xf5c8);
            *(undefined8 *)(param_1 + 0xf5f0) = *(undefined8 *)(param_1 + 0xf5e8);
          }
        }
      }
    }
    C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
    if ((DAT_1409e8088 != -1) &&
       ((iVar20 = strcmp(&DAT_1409e8258,"SetDistOffSource"), iVar20 == 0 ||
        (iVar20 = strcmp(&DAT_1409e8258,"SetDistOffDestination"), iVar20 == 0)))) {
      do {
        cVar16 = *pcVar46;
        cVar2 = pcVar46[*(longlong *)(lVar38 + 0x318) + -0x1409e81c4];
        if (cVar16 != cVar2) break;
        pcVar46 = pcVar46 + 1;
      } while (cVar2 != '\0');
      if ((cVar16 == cVar2) &&
         (((*(char **)(param_1 + 0xd428) == (char *)0x0 ||
           (iVar20 = strcmp(*(char **)(param_1 + 0xd428),"building_addstation_dist"), iVar20 != 0))
          && (DAT_1409e82ec < 2)))) {
        DAT_1409e82ec = 2;
        _DAT_1409e82f8 = fVar59 + local_ec4 + DAT_140992088 * fVar49;
        _DAT_1409e82fc = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060)
        ;
        _DAT_1409e82fc = _DAT_1409e82fc + local_ec8;
      }
    }
    (**(code **)(**(longlong **)(param_1 + 0xb320) + 0x70))
              (*(longlong **)(param_1 + 0xb320),0,DAT_1409ead08);
    local_ea0 = 0;
    _DAT_1409be2f0 = CONCAT44(local_ec8,fVar59 + local_ec4 + DAT_140992088 * DAT_14090aaa8);
    DAT_1409be2f8 = 0;
    _DAT_1409be2e8 = DAT_140992088 * fVar50;
    _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
    uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
    _DAT_1409be2ec = _DAT_1409be2e8;
    if ((*(int *)(param_1 + 0xada8) == -1) ||
       (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
      FUN_1404461f0(param_1 + 0xadb0);
    }
    fVar49 = DAT_14090aa28;
    if (*(char *)(uVar44 + 0x20) != '\0') {
      uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
      if (bVar17) {
        _DAT_1409be30c = _DAT_14090c4c0;
        uRam00000001409be314 = _UNK_14090c4c8;
        pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x809);
        FUN_140015900(param_1 + 0xd5a0,pwVar21);
        if ((*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0) &&
           (DAT_140a54e91 != '\0')) {
          if ((*(char *)(param_1 + 0x14) == '\0') &&
             (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                 ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
            pcVar46 = C3D_CONFIGURATOR::GetValueString
                                ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
            C3D_SOUNDMANAGER::PlaySound2D
                      (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,(char *)0x0);
          }
          FUN_14003c1d0(*(undefined8 *)(param_1 + 0x13bf0));
          *(undefined1 *)(*(longlong *)(param_1 + 0x13bf0) + 0x17) = 1;
          lVar24 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          uVar55 = *(undefined4 *)(lVar24 + 4);
          puVar26 = (undefined4 *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          puVar6 = *(undefined4 **)(param_1 + 0x13bf0);
          *puVar6 = *puVar26;
          puVar6[1] = uVar55;
          fVar53 = DAT_140992088;
          fVar47 = DAT_140992088 * DAT_14090ad18;
          lVar24 = *(longlong *)(param_1 + 0x13bf0);
          *(undefined1 *)(lVar24 + 0x16) = 0;
          *(float *)(lVar24 + 8) = fVar47;
          fVar53 = fVar53 * fVar49;
          if (fVar47 < fVar53) {
            *(float *)(lVar24 + 8) = fVar53;
          }
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88c1);
          FUN_140600350(*(undefined8 *)(param_1 + 0x13bf0),pwVar21);
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x809);
          FUN_1406e6fd0(*(undefined8 *)(param_1 + 0x13bf0),pwVar21);
          lVar24 = *(longlong *)(param_1 + 0x13bf0);
          *(code **)(lVar24 + 0x78) = FUN_140745140;
          *(longlong *)(lVar24 + 0x70) = lVar38;
          FUN_14003be20(*(undefined8 *)(param_1 + 0x13bf0),uVar44);
        }
      }
    }
    C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
    (**(code **)(**(longlong **)(param_1 + 0xbd80) + 0x70))
              (*(longlong **)(param_1 + 0xbd80),0,DAT_1409ead08);
    local_ea0 = 0;
    _DAT_1409be2f0 = CONCAT44(local_ec8,fVar59 + local_ec4 + DAT_140992088 * DAT_14090aba0);
    DAT_1409be2f8 = 0;
    _DAT_1409be2e8 = DAT_140992088 * fVar50;
    _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
    uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
    _DAT_1409be2ec = _DAT_1409be2e8;
    if ((*(int *)(param_1 + 0xada8) == -1) ||
       (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
      FUN_1404461f0(param_1 + 0xadb0);
    }
    fVar53 = DAT_14090aeb8;
    uVar41 = uVar43;
    if (*(char *)(uVar44 + 0x20) != '\0') {
      uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
      if (bVar17) {
        _DAT_1409be30c = _DAT_14090c4c0;
        uRam00000001409be314 = _UNK_14090c4c8;
        pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88c2);
        FUN_140015900(param_1 + 0xd5a0,pwVar21);
        uVar41 = 0;
        if ((*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0) &&
           (DAT_140a54e91 != '\0')) {
          if ((*(char *)(param_1 + 0x14) == '\0') &&
             (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                 ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
            pcVar46 = C3D_CONFIGURATOR::GetValueString
                                ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
            C3D_SOUNDMANAGER::PlaySound2D
                      (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,(char *)0x0);
          }
          FUN_14003c1d0(*(undefined8 *)(param_1 + 0x13bf0));
          lVar24 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          uVar55 = *(undefined4 *)(lVar24 + 4);
          puVar26 = (undefined4 *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
          puVar6 = *(undefined4 **)(param_1 + 0x13bf0);
          *puVar6 = *puVar26;
          puVar6[1] = uVar55;
          fVar47 = DAT_140992088;
          fVar48 = DAT_140992088 * fVar53;
          lVar24 = *(longlong *)(param_1 + 0x13bf0);
          *(undefined1 *)(lVar24 + 0x16) = 0;
          *(float *)(lVar24 + 8) = fVar48;
          fVar47 = fVar47 * fVar49;
          if (fVar48 < fVar47) {
            *(float *)(lVar24 + 8) = fVar47;
          }
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x88c2);
          FUN_140600350(*(undefined8 *)(param_1 + 0x13bf0),pwVar21);
          fVar49 = DAT_14090ae38;
          uVar35 = 0;
          uVar41 = uVar43;
          if (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0) {
            lVar24 = param_1 + 0x117bc;
            do {
              _Memory = (void *)0x0;
              FUN_1403329e0(param_1,*(undefined8 *)
                                     (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar35 * 8) + 8)
                            ,0x100,1);
              cVar16 = FUN_14042a760(DAT_140992088 * fVar49,local_2e8,lVar24,DAT_140994200);
              FUN_14003c4b0(*(undefined8 *)(param_1 + 0x13bf0),local_2e8,(float)uVar35,0);
              if (cVar16 != '\0') {
                local_d18._0_2_ = 0;
                local_d18._2_2_ = 0;
                local_d18._4_4_ = 0;
                uStack_d10 = 0;
                pvVar39 = _Memory;
                if (lVar24 != 0) {
                  FUN_140034b80(&local_d18,0xffffffff,lVar24);
                  pvVar39 = (void *)(ulonglong)uStack_d10;
                  _Memory = (void *)CONCAT44(local_d18._4_4_,
                                             CONCAT22(local_d18._2_2_,(undefined2)local_d18));
                }
                lVar30 = *(longlong *)(param_1 + 0x13bf0);
                puVar36 = (undefined8 *)
                          (*(longlong *)(lVar30 + 0x50) +
                          (longlong)
                          ((int)((*(longlong *)(lVar30 + 0x58) - *(longlong *)(lVar30 + 0x50)) /
                                0x30) + -1) * 0x30 + 0x10);
                if (puVar36 != &local_d18) {
                  if (_Memory == (void *)0x0) {
                    if ((void *)*puVar36 != (void *)0x0) {
                      free((void *)*puVar36);
                      *puVar36 = 0;
                      *(undefined4 *)(puVar36 + 1) = 0;
                    }
                  }
                  else {
                    FUN_140034b80(puVar36,pvVar39,_Memory);
                  }
                }
                if (_Memory != (void *)0x0) {
                  free(_Memory);
                  local_d18._0_2_ = 0;
                  local_d18._2_2_ = 0;
                  local_d18._4_4_ = 0;
                  uStack_d10 = 0;
                }
              }
              uVar35 = (ulonglong)((int)uVar35 + 1);
              uVar44 = local_eb0;
              piVar42 = local_e90;
              uVar41 = local_e78;
            } while (uVar35 < (ulonglong)
                              (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3));
          }
          *(code **)(*(longlong *)(param_1 + 0x13bf0) + 0x80) = FUN_140040c30;
          lVar24 = *(longlong *)(param_1 + 0x13bf0);
          *(code **)(lVar24 + 0x78) = FUN_140745160;
          *(longlong *)(lVar24 + 0x70) = lVar38;
          FUN_14003be20(*(undefined8 *)(param_1 + 0x13bf0),uVar44);
        }
      }
    }
    C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
    (**(code **)(**(longlong **)(param_1 + 0xb348) + 0x70))
              (*(longlong **)(param_1 + 0xb348),0,DAT_1409ead08);
    local_ea0 = 0;
    _DAT_1409be2f0 = CONCAT44(local_ec8,fVar59 + local_ec4 + DAT_140992088 * DAT_14090ac68);
    DAT_1409be2f8 = 0;
    _DAT_1409be2e8 = DAT_140992088 * fVar50;
    _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
    uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
    uVar35 = 0;
    lVar30 = *(longlong *)(param_1 + 0xd288) - *(longlong *)(param_1 + 0xd280);
    lVar24 = lVar30 >> 0x3f;
    uVar43 = uVar35;
    _DAT_1409be2ec = _DAT_1409be2e8;
    if (lVar30 / 0x2d0 + lVar24 != lVar24) {
      do {
        iVar20 = strcmp((char *)(*(longlong *)(param_1 + 0xd280) + uVar43 * 0x2d0),
                        "building_copy_dist");
        if (iVar20 == 0) {
          uVar35 = *(longlong *)(param_1 + 0xd280) + uVar43 * 0x2d0;
          break;
        }
        uVar43 = (ulonglong)((int)uVar43 + 1);
      } while (uVar43 < (ulonglong)
                        ((*(longlong *)(param_1 + 0xd288) - *(longlong *)(param_1 + 0xd280)) / 0x2d0
                        ));
    }
    if (*(ulonglong *)(param_1 + 0xd428) == uVar35) {
      _DAT_1409be30c = _DAT_14090c4c0;
      uRam00000001409be314 = _UNK_14090c4c8;
    }
    if ((*(int *)(param_1 + 0xada8) == -1) ||
       (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
      FUN_1404461f0(param_1 + 0xadb0);
    }
    uVar44 = local_eb0;
    if (*(char *)(local_eb0 + 0x20) != '\0') {
      uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
      if (bVar17) {
        if (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 == 0) {
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x822);
          FUN_140015900(param_1 + 0xd5a0,pwVar21);
        }
        else {
          _DAT_1409be30c = _DAT_14090c4c0;
          uRam00000001409be314 = _UNK_14090c4c8;
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x815);
          FUN_140015900(param_1 + 0xd5a0,pwVar21);
          if (DAT_140a54e91 != '\0') {
            if ((*(char *)(param_1 + 0x14) == '\0') &&
               (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                   ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
              pcVar46 = C3D_CONFIGURATOR::GetValueString
                                  ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
              C3D_SOUNDMANAGER::PlaySound2D
                        (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,(char *)0x0);
            }
            if (*(ulonglong *)(param_1 + 0xd428) == uVar35) {
              *(undefined8 *)(param_1 + 0xd428) = 0;
            }
            else {
              *(ulonglong *)(param_1 + 0xd428) = uVar35;
              *(longlong *)(param_1 + 0xd298) = lVar38;
              *(ulonglong *)(param_1 + 0xd2a0) = uVar44;
              *(undefined1 *)(param_1 + 0xf5b0) = 0;
              *(undefined4 *)(param_1 + 0xf5c0) = 0xbf800000;
              *(undefined8 *)(param_1 + 0xf5b8) = 0;
              FUN_140201cc0(param_1 + 0xf5c8);
              *(undefined8 *)(param_1 + 0xf5f0) = *(undefined8 *)(param_1 + 0xf5e8);
            }
          }
        }
      }
    }
    C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
    (**(code **)(**(longlong **)(param_1 + 0xb400) + 0x70))(*(longlong **)(param_1 + 0xb400),0);
    local_ea0 = 0;
    _DAT_1409be2f0 = CONCAT44(local_ec8,fVar59 + local_ec4 + DAT_140992088 * _DAT_14090ad44);
    DAT_1409be2f8 = 0;
    _DAT_1409be2e8 = DAT_140992088 * fVar50;
    _DAT_1409be30c = CONCAT44(_UNK_14090c244,_DAT_14090c240);
    uRam00000001409be314 = CONCAT44(_UNK_14090c24c,_UNK_14090c248);
    _DAT_1409be2ec = _DAT_1409be2e8;
    cVar16 = FUN_1401e9f10();
    if ((*(int *)(param_1 + 0xada8) == -1) ||
       (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
      FUN_1404461f0(param_1 + 0xadb0);
    }
    if (*(char *)(uVar44 + 0x20) != '\0') {
      uVar25 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar25,0,0);
      if (bVar17) {
        if (cVar16 == '\0') {
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110f);
          FUN_140015900(param_1 + 0xd5a0,pwVar21);
        }
        else {
          _DAT_1409be30c = _DAT_14090c4c0;
          uRam00000001409be314 = _UNK_14090c4c8;
          pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x110c);
          FUN_140015900(param_1 + 0xd5a0,pwVar21);
          if (DAT_140a54e91 != '\0') {
            if ((*(char *)(param_1 + 0x14) == '\0') &&
               (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                   ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
              pcVar46 = C3D_CONFIGURATOR::GetValueString
                                  ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
              C3D_SOUNDMANAGER::PlaySound2D
                        (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,(char *)0x0);
            }
            FUN_1401ea1a0(param_1,lVar38);
          }
        }
      }
    }
    C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
    local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a968;
    fVar49 = (float)FUN_140769300(param_1);
    local_ec8 = local_ec8 + fVar49;
    local_ec0 = 0;
    local_d18._0_2_ = 0;
    local_d18._4_4_ = 0;
    uStack_d10 = 0;
    uStack_d0c = 0;
    if ((lVar38 == 0) || (*(longlong *)(lVar38 + 0xc78) - *(longlong *)(lVar38 + 0xc70) >> 3 == 0))
    {
      local_d18._0_2_ = 1;
      if ((lVar38 == 0) || (*(longlong *)(lVar38 + 0x318) == 0)) {
        local_d18._4_4_ = 0x1000;
        uStack_d10 = DAT_14090aafc;
        uStack_d0c = DAT_14090a618;
        local_ec4 = fVar64;
      }
      else {
        local_ec4 = fVar64;
        if (*(int *)(*(longlong *)(lVar38 + 0x318) + 0x360) == 9) {
          local_d18._4_4_ = 0x100;
          uStack_d10 = 0x41c80000;
          uStack_d0c = DAT_14090a280;
        }
      }
    }
    else {
      local_ec4 = fVar64;
      FUN_1404439e0(&local_d18);
    }
    fVar49 = DAT_14090a86c;
    dVar12 = DAT_14090a1f0;
    dVar11 = DAT_14090a1c0;
    if (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 == 0) {
      C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x80c);
      C3D_FONTMANAGER::PrintLeftUnicode
                ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
                 local_ec4 - DAT_140992088 * fVar54,DAT_140992088 * fVar52 + local_ec8,0xff990000,
                 L"%ls");
      local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a928;
    }
    else {
      local_eb8[0] = 0;
      if (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0) {
        local_e80 = 0;
        local_e78 = 0;
        local_d98 = 0;
        local_db0 = 0;
        local_e48 = 0;
        uVar55 = _DAT_14090c240;
        uVar56 = _UNK_14090c244;
        uVar57 = _UNK_14090c248;
        uVar58 = _UNK_14090c24c;
        uVar65 = _DAT_14090c240;
        uVar66 = _UNK_14090c244;
        uVar67 = _UNK_14090c248;
        uVar68 = _UNK_14090c24c;
        iVar20 = 0;
        uVar60 = _DAT_14090c240;
        uVar61 = _UNK_14090c244;
        uVar62 = _UNK_14090c248;
        uVar63 = _UNK_14090c24c;
        do {
          uVar43 = local_e78;
          uVar40 = local_eb8[0];
          uVar25 = *(undefined8 *)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) + 8)
          ;
          FUN_1403329e0(param_1,uVar25,0x100,1);
          cVar16 = FUN_14042a760(DAT_140992088 * fVar53,local_4e8,param_1 + 0x117bc,DAT_140995220);
          if ((cVar16 != '\0') &&
             (lVar24 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90),
             fVar50 = *(float *)(local_eb0 + 8) + *(float *)(local_eb0 + 0x2c) +
                      *(float *)(local_eb0 + 0x51c),
             *(float *)(lVar24 + 4) <= fVar50 && fVar50 != *(float *)(lVar24 + 4))) {
            fVar59 = DAT_140992088 * DAT_14090a8a0 + local_ec8;
            fVar52 = DAT_140992088 * fVar53 + local_ec4;
            fVar54 = local_ec8 - DAT_140992088 * DAT_14090a730;
            fVar64 = DAT_140992088 * DAT_14090a8a0 + local_ec4;
            lVar24 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
            fVar50 = C3D_PANEL2D::GetClipHeightOffsetRealWithZoom((C3D_PANEL2D *)&DAT_1409be060);
            fVar50 = *(float *)(lVar24 + 4) - fVar50;
            pfVar27 = (float *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
            uVar55 = _DAT_14090c240;
            uVar56 = _UNK_14090c244;
            uVar57 = _UNK_14090c248;
            uVar58 = _UNK_14090c24c;
            uVar65 = _DAT_14090c240;
            uVar66 = _UNK_14090c244;
            uVar67 = _UNK_14090c248;
            uVar68 = _UNK_14090c24c;
            fVar53 = DAT_14090aeb8;
            iVar20 = local_ec0;
            uVar60 = _DAT_14090c240;
            uVar61 = _UNK_14090c244;
            uVar62 = _UNK_14090c248;
            uVar63 = _UNK_14090c24c;
            if (((fVar64 - *pfVar27) * (fVar52 - *pfVar27) <= 0.0) &&
               ((fVar54 - fVar50) * (fVar59 - fVar50) <= 0.0)) {
              FUN_140015900(param_1 + 0xd5a0,param_1 + 0x117bc);
              uVar55 = _DAT_14090c240;
              uVar56 = _UNK_14090c244;
              uVar57 = _UNK_14090c248;
              uVar58 = _UNK_14090c24c;
              uVar65 = _DAT_14090c240;
              uVar66 = _UNK_14090c244;
              uVar67 = _UNK_14090c248;
              uVar68 = _UNK_14090c24c;
              fVar53 = DAT_14090aeb8;
              iVar20 = local_ec0;
              uVar60 = _DAT_14090c240;
              uVar61 = _UNK_14090c244;
              uVar62 = _UNK_14090c248;
              uVar63 = _UNK_14090c24c;
            }
          }
          C3D_FONTMANAGER::PrintLeftUnicode
                    ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
                     DAT_140992088 * DAT_14090a8a0 + local_ec4,
                     local_ec8 - DAT_140992088 * DAT_14090a444,0xff990000,L"%ls");
          plVar29 = (longlong *)(**(code **)(*DAT_140995220 + 0x28))(DAT_140995220,local_cf8,0,0);
          local_d08 = *plVar29;
          uStack_d00 = plVar29[1];
          local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a928;
          if (**(char **)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) == '\0') {
            (**(code **)(**(longlong **)(param_1 + 0xb500) + 0x70))
                      (*(longlong **)(param_1 + 0xb500),0,DAT_1409ead08);
            fVar50 = (float)(int)uStack_d00;
            local_ea0 = 0;
            _DAT_1409be2f0 =
                 CONCAT44(local_ec8 - DAT_140992088 * fVar49,
                          fVar50 + local_ec4 + DAT_140992088 * DAT_14090a9f0);
            DAT_1409be2f8 = 0;
            _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
            _DAT_1409be30c = _DAT_14090c4c0;
            uRam00000001409be314 = _UNK_14090c4c8;
            _DAT_1409be2ec = _DAT_1409be2e8;
            if ((*(int *)(param_1 + 0xada8) == -1) ||
               (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
              FUN_1404461f0(param_1 + 0xadb0);
            }
            if (*(char *)(local_eb0 + 0x20) != '\0') {
              uVar28 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
              bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar28,0,0);
              if (bVar17) {
                pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b7);
                FUN_140015900(param_1 + 0xd5a0,pwVar21);
                if (DAT_140a54e91 != '\0') {
                  DAT_140a54e91 = '\0';
                  local_d08 = CONCAT44(local_d18._4_4_,
                                       CONCAT22(local_d18._2_2_,(undefined2)local_d18));
                  uStack_d00 = CONCAT44(uStack_d0c,uStack_d10);
                  lVar24 = *(longlong *)(lVar38 + 0xd38);
                  uVar18 = FUN_140130650();
                  **(undefined1 **)(lVar24 + uVar43 * 8) = uVar18;
                  if ((**(char **)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) != '\0') &&
                     (lVar38 != 0)) {
                    *(undefined1 *)(lVar38 + 0xd50) = 1;
                  }
                }
              }
            }
LAB_140744162:
            C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
            local_ec0 = local_ec0 + 1;
            local_e80 = local_e80 + 1;
            iVar20 = (int)(fVar50 + DAT_14090a9f0);
          }
          else {
            if (((piVar42 != (int *)0x0) && ((ulonglong)(longlong)iVar20 < piVar42)) &&
               (local_e68[local_e80] == uVar40)) {
              (**(code **)(**(longlong **)(param_1 + 0xb500) + 0x70))
                        (*(longlong **)(param_1 + 0xb500),0,DAT_1409ead08);
              fVar50 = (float)(int)uStack_d00;
              local_da4 = 0;
              _DAT_1409be2f0 =
                   CONCAT44(local_ec8 - DAT_140992088 * fVar49,
                            fVar50 + local_ec4 + DAT_140992088 * DAT_14090a9f0);
              DAT_1409be2f8 = 0;
              _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
              _DAT_1409be30c = _DAT_14090c4c0;
              uRam00000001409be314 = _UNK_14090c4c8;
              _DAT_1409be2ec = _DAT_1409be2e8;
              if ((*(int *)(param_1 + 0xada8) == -1) ||
                 (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
                FUN_1404461f0(param_1 + 0xadb0);
              }
              uVar28 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
              bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar28,0,0);
              if (bVar17) {
                iVar33 = 0x5c4;
                if ((*(uint *)(local_dd8[0] + ((ulonglong)(longlong)iVar20 >> 5) * 4) &
                    1 << ((byte)iVar20 & 0x1f)) == 0) {
                  iVar33 = 0x5b4;
                }
                pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar33);
                FUN_140015900(param_1 + 0xd5a0,pwVar21);
              }
              goto LAB_140744162;
            }
            iVar20 = (int)uStack_d00;
          }
          lVar24 = param_1 + 0xadb0;
          (**(code **)(**(longlong **)(param_1 + 46000) + 0x70))
                    (*(longlong **)(param_1 + 46000),0,DAT_1409ead08);
          fVar50 = (float)iVar20;
          _DAT_1409be2f0 =
               CONCAT44(local_ec8 - DAT_140992088 * fVar49,
                        fVar50 + local_ec4 + DAT_140992088 * DAT_14090a9f0);
          DAT_1409be2f8 = local_d98;
          _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
          _DAT_1409be30c = CONCAT44(uVar56,uVar55);
          uRam00000001409be314 = CONCAT44(uVar58,uVar57);
          _DAT_1409be2ec = _DAT_1409be2e8;
          if ((*(int *)(param_1 + 0xada8) == -1) ||
             (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
            FUN_1404461f0(lVar24);
          }
          uVar44 = local_eb0;
          if (*(char *)(local_eb0 + 0x20) != '\0') {
            uVar28 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
            bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar28,0,0);
            if (bVar17) {
              _DAT_1409be30c = _DAT_14090c4c0;
              uRam00000001409be314 = _UNK_14090c4c8;
              pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x143);
              FUN_140015900(param_1 + 0xd5a0,pwVar21);
              if (DAT_140a54e91 != '\0') {
                DAT_140a54e91 = '\0';
                if ((*(char *)(param_1 + 0x14) == '\0') &&
                   (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                       ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)
                   ) {
                  pcVar46 = C3D_CONFIGURATOR::GetValueString
                                      ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
                  C3D_SOUNDMANAGER::PlaySound2D
                            (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,
                             (char *)0x0);
                }
                FUN_1403d7740(param_1,uVar25,0);
              }
            }
          }
          C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
          (**(code **)(**(longlong **)(param_1 + 0xb3a8) + 0x70))
                    (*(longlong **)(param_1 + 0xb3a8),0,DAT_1409ead08);
          _DAT_1409be2f0 =
               CONCAT44(local_ec8 - DAT_140992088 * fVar49,
                        fVar50 + local_ec4 + DAT_140992088 * DAT_14090ab2c);
          DAT_1409be2f8 = local_db0;
          _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
          _DAT_1409be30c = CONCAT44(uVar61,uVar60);
          uRam00000001409be314 = CONCAT44(uVar63,uVar62);
          _DAT_1409be2ec = _DAT_1409be2e8;
          if ((*(int *)(param_1 + 0xada8) == -1) ||
             (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
            FUN_1404461f0(lVar24);
          }
          if (*(char *)(uVar44 + 0x20) != '\0') {
            uVar28 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
            bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar28,0,0);
            if (bVar17) {
              _DAT_1409be30c = _DAT_14090c4c0;
              uRam00000001409be314 = _UNK_14090c4c8;
              pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x5b1);
              FUN_140015900(param_1 + 0xd5a0,pwVar21);
              if (DAT_140a54e91 != '\0') {
                DAT_140a54e91 = '\0';
                if ((*(char *)(param_1 + 0x14) == '\0') &&
                   (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                       ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)
                   ) {
                  pcVar46 = C3D_CONFIGURATOR::GetValueString
                                      ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
                  C3D_SOUNDMANAGER::PlaySound2D
                            (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,
                             (char *)0x0);
                }
                FUN_1406ee2a0(param_1,2,uVar25,0);
              }
            }
          }
          C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
          (**(code **)(**(longlong **)(param_1 + 0xb320) + 0x70))
                    (*(longlong **)(param_1 + 0xb320),0);
          _DAT_1409be2f0 =
               CONCAT44(local_ec8 - DAT_140992088 * fVar49,
                        fVar50 + local_ec4 + DAT_140992088 * DAT_14090abfc);
          DAT_1409be2f8 = local_e48;
          _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9cc;
          _DAT_1409be30c = CONCAT44(uVar66,uVar65);
          uRam00000001409be314 = CONCAT44(uVar68,uVar67);
          _DAT_1409be2ec = _DAT_1409be2e8;
          if ((*(int *)(param_1 + 0xada8) == -1) ||
             (*(int *)(param_1 + 0xada8) == *(int *)(param_1 + 0xadf0))) {
            FUN_1404461f0(lVar24);
          }
          uVar44 = local_eb0;
          if (*(char *)(local_eb0 + 0x20) == '\0') {
LAB_140744822:
            C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar10,fVar10,0.0,true);
            uVar4 = *(uint *)(uVar44 + 0x310 + uVar43 * 4);
            cVar16 = FUN_1406f49d0(param_1,uVar44,local_ec4 - DAT_140992088 * DAT_14090a828,
                                   local_ec8 - DAT_140992088 * fVar49);
            plVar29 = local_e40;
            uVar41 = 0;
            *(uint *)(uVar44 + 0x310 + uVar43 * 4) = (uint)(cVar16 != '\0');
            if (((uVar4 != (cVar16 != '\0')) && (DAT_140a54bad != '\0')) &&
               (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3 != 0)) {
              do {
                if (uVar40 != (uint)uVar41) {
                  *(undefined4 *)(uVar44 + 0x310 + uVar41 * 4) =
                       *(undefined4 *)(uVar44 + 0x310 + uVar43 * 4);
                }
                uVar41 = (ulonglong)((uint)uVar41 + 1);
              } while (uVar41 < (ulonglong)
                                (*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3)
                      );
            }
            if (*(int *)(uVar44 + 0x310 + uVar43 * 4) == 0) {
              local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a9f0;
              plStack_e38 = local_e40;
              lStack_e00 = local_e08;
              FUN_14019c7d0(param_1,uVar25);
              uVar43 = 0;
              uVar44 = lStack_e00 - local_e08 >> 3;
              lVar24 = local_e08;
              plVar22 = plVar29;
              if (uVar44 != 0) {
                do {
                  uVar41 = 0;
                  plVar1 = (longlong *)(lVar24 + uVar43 * 8);
                  uVar35 = lStack_de8 - local_df0 >> 3;
                  if (uVar35 != 0) {
                    do {
                      if (*plVar1 == *(longlong *)(local_df0 + uVar41 * 8)) {
                        if ((plVar1 < plVar29) && (plVar22 <= plVar1)) {
                          lVar24 = (longlong)plVar1 - (longlong)plVar22;
                          if (plVar29 == local_e30) {
                            FUN_1400b1700(&local_e40,1);
                            plVar29 = plStack_e38;
                            plVar22 = local_e40;
                          }
                          if (plVar29 != (longlong *)0x0) {
                            lVar24 = plVar22[lVar24 >> 3];
LAB_140744a25:
                            *plVar29 = lVar24;
                          }
                        }
                        else {
                          if (plVar29 == local_e30) {
                            FUN_1400b1700(&local_e40,1);
                            plVar29 = plStack_e38;
                            plVar22 = local_e40;
                          }
                          if (plVar29 != (longlong *)0x0) {
                            lVar24 = *plVar1;
                            goto LAB_140744a25;
                          }
                        }
                        plVar29 = plVar29 + 1;
                        lVar24 = local_e08;
                        plStack_e38 = plVar29;
                        break;
                      }
                      uVar41 = (ulonglong)((int)uVar41 + 1);
                      lVar24 = local_e08;
                    } while (uVar41 < uVar35);
                  }
                  uVar43 = (ulonglong)((int)uVar43 + 1);
                  param_1 = local_e28;
                } while (uVar43 < uVar44);
              }
              uVar43 = local_e78;
              lVar24 = *(longlong *)
                        (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) + 0x10);
              lVar30 = *(longlong *)(lVar24 + 0x10);
              lVar24 = *(longlong *)(lVar24 + 8);
              cVar16 = **(char **)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) +
                                  0x10);
              lVar5 = *(longlong *)
                       (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) + 0x10);
              lVar7 = *(longlong *)(lVar5 + 0xd8);
              lVar5 = *(longlong *)(lVar5 + 0xd0);
              cVar2 = *(char *)(*(longlong *)
                                 (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) +
                                 0x10) + 200);
              fVar50 = (float)FUN_1407ebfe0(param_1,*(longlong *)
                                                     (*(longlong *)
                                                       (*(longlong *)(lVar38 + 0xd38) +
                                                       local_e78 * 8) + 0x10) + 200,local_ec8,
                                            *(undefined8 *)
                                             (*(longlong *)
                                               (*(longlong *)(lVar38 + 0xd38) + local_e78 * 8) +
                                             0x10));
              if (*(char *)(lVar38 + 0xd50) == '\0') {
                lVar8 = *(longlong *)
                         (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10);
                if ((((longlong)(int)(lVar30 - lVar24 >> 4) ==
                      *(longlong *)(lVar8 + 0x10) - *(longlong *)(lVar8 + 8) >> 4) &&
                    (cVar16 == **(char **)(*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8)
                                          + 0x10))) &&
                   ((lVar24 = *(longlong *)
                               (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10),
                    (longlong)(int)(lVar7 - lVar5 >> 4) ==
                    *(longlong *)(lVar24 + 0xd8) - *(longlong *)(lVar24 + 0xd0) >> 4 &&
                    (cVar2 == *(char *)(*(longlong *)
                                         (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8)
                                         + 0x10) + 200))))) {
                  cVar16 = '\0';
                }
                else {
                  cVar16 = '\x01';
                }
                *(char *)(lVar38 + 0xd50) = cVar16;
                if (cVar16 != '\0') {
                  *(undefined4 *)(lVar38 + 0xfc0) = 0x40400000;
                }
              }
              dVar51 = (double)*(float *)(*(longlong *)
                                           (*(longlong *)
                                             (*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10) +
                                         4);
              if (0.0 <= dVar51) {
                if (dVar11 < dVar51) {
                  dVar51 = dVar11;
                }
              }
              else {
                dVar51 = 0.0;
              }
              *(float *)(*(longlong *)
                          (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10) + 4) =
                   (float)dVar51;
              local_e88 = (double)*(float *)(*(longlong *)
                                              (*(longlong *)
                                                (*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10)
                                            + 0xcc);
              if (DAT_14090a048 <= local_e88) {
                if (dVar12 < local_e88) {
                  local_e88 = dVar12;
                }
              }
              else {
                local_e88 = DAT_14090a048;
              }
              *(float *)(*(longlong *)
                          (*(longlong *)(*(longlong *)(lVar38 + 0xd38) + uVar43 * 8) + 0x10) + 0xcc)
                   = (float)local_e88;
              local_ec8 = local_ec8 + fVar50;
              uVar40 = local_eb8[0];
            }
            else {
              local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a928;
            }
          }
          else {
            uVar28 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
            bVar17 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar28,0,0);
            if (!bVar17) goto LAB_140744822;
            _DAT_1409be30c = _DAT_14090c4c0;
            uRam00000001409be314 = _UNK_14090c4c8;
            pwVar21 = C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x80a);
            FUN_140015900(param_1 + 0xd5a0,pwVar21);
            if (DAT_140a54e91 == '\0') goto LAB_140744822;
            DAT_140a54e91 = '\0';
            if ((*(char *)(param_1 + 0x14) == '\0') &&
               (bVar17 = C3D_CONFIGURATOR::GetValueExist
                                   ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar17)) {
              pcVar46 = C3D_CONFIGURATOR::GetValueString
                                  ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
              C3D_SOUNDMANAGER::PlaySound2D
                        (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar46,true,fVar10,(char *)0x0);
            }
            uVar25 = FUN_1401c7e80();
            FUN_140162b40(uVar25,lVar38,uVar40);
            uVar43 = uVar43 - 1;
            uVar40 = uVar40 - 1;
          }
          local_eb8[0] = uVar40 + 1;
          local_e78 = uVar43 + 1;
          uVar44 = local_eb0;
          piVar42 = local_e90;
          uVar41 = local_df8;
          iVar20 = local_ec0;
        } while ((ulonglong)(longlong)(int)local_eb8[0] <
                 (ulonglong)(*(longlong *)(lVar38 + 0xd40) - *(longlong *)(lVar38 + 0xd38) >> 3));
      }
    }
    local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a928;
    if (local_e08 != 0) {
      FUN_140001a50(local_e08,(longlong)(uVar41 - local_e08) >> 3,8);
    }
    piVar45 = local_e68;
    if (local_e40 != (longlong *)0x0) {
      FUN_140001a50(local_e40,(longlong)local_e30 - (longlong)local_e40 >> 3,8);
      piVar45 = local_e68;
    }
  }
  else {
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x814);
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
               local_ec4 - DAT_140992088 * DAT_14090a730,DAT_140992088 * fVar52 + local_ec8,
               0xff990000,L"%ls: %d / 20");
    local_ec8 = local_ec8 + DAT_140992088 * fVar50;
  }
  local_ec4 = local_ec4 - DAT_140992088 * DAT_14090a800;
  if ((*(int *)(param_1 + 0x5b0) == 2) &&
     (iVar20 = (int)((*(longlong *)(lVar38 + 0x978) - *(longlong *)(lVar38 + 0x970)) / 0xe0) + -1,
     -1 < iVar20)) {
    lVar30 = (longlong)iVar20;
    lVar24 = lVar30 * 0xe0;
    do {
      lVar5 = *(longlong *)(lVar38 + 0x970);
      if ((*(longlong *)(lVar24 + 8 + lVar5) - *(longlong *)(lVar24 + lVar5) >> 4 != 0) &&
         ((((**(longlong **)(lVar24 + lVar5) == *(longlong *)(param_1 + 0xc2e0) &&
            (*(longlong *)(lVar38 + 0x318) != 0)) &&
           (*(char *)(*(longlong *)(lVar38 + 0x318) + 0x74b) == '\0')) ||
          (((**(longlong **)(lVar24 + *(longlong *)(lVar38 + 0x970)) ==
             *(longlong *)(param_1 + 0xc2f8) && (*(longlong *)(lVar38 + 0x318) != 0)) &&
           (*(char *)(*(longlong *)(lVar38 + 0x318) + 0x74b) == '\x01')))))) {
        lVar38 = (longlong)iVar20 * 0xe0 + *(longlong *)(lVar38 + 0x970);
        if (lVar38 != 0) {
          local_ec8 = local_ec8 + DAT_140992088 * DAT_14090a928;
          FUN_140706c00(param_1,uVar44,lVar38,&local_ec4);
        }
        break;
      }
      iVar20 = iVar20 + -1;
      lVar24 = lVar24 + -0xe0;
      lVar30 = lVar30 + -1;
    } while (-1 < lVar30);
  }
  FUN_1406f4b40(param_1,uVar44,&local_ec4,&local_ec8);
  *(float *)(uVar44 + 0x250) = local_ec8;
  if (local_dd8[0] != 0) {
    FUN_140001a50(local_dd8[0],local_dd8[2] - local_dd8[0] >> 2,4);
  }
  if (piVar45 != (int *)0x0) {
    FUN_140001a50(piVar45,(longlong)local_e70 - (longlong)piVar45 >> 2,4);
  }
  if (local_df0 != 0) {
    FUN_140001a50(local_df0,local_de0 - local_df0 >> 3,8);
  }
  return;
code_r0x000140741d1c:
  uVar40 = (int)uVar41 + 1;
  uVar37 = uVar37 + 0x68;
  uVar41 = (ulonglong)uVar40;
  if ((ulonglong)((*(longlong *)(param_1 + 0x144b0) - *(longlong *)(param_1 + 0x144a8)) / 0x68) <=
      (ulonglong)(longlong)(int)uVar40) goto LAB_140741e6f;
  goto LAB_140741d00;
}



```

---

## 2. Full Disassembly of FUN_140741170 (0x140741170–0x140745100)

```asm
; Range: 0x140741170 to 0x140745100
140741170:  48 8B C4                 MOV RAX,RSP
140741173:  55                       PUSH RBP
140741174:  56                       PUSH RSI
140741175:  57                       PUSH RDI
140741176:  41 54                    PUSH R12
140741178:  41 55                    PUSH R13
14074117a:  41 56                    PUSH R14
14074117c:  41 57                    PUSH R15
14074117e:  48 8D A8 B8 F1 FF FF     LEA RBP,[RAX + -0xe48]
140741185:  48 81 EC 20 0F 00 00     SUB RSP,0xf20
14074118c:  48 C7 85 B8 00 00 00 FE FF FF FF MOV qword ptr [RBP + 0xb8],-0x2
140741197:  48 89 58 18              MOV qword ptr [RAX + 0x18],RBX
14074119b:  0F 29 70 B8              MOVAPS xmmword ptr [RAX + -0x48],XMM6
14074119f:  0F 29 78 A8              MOVAPS xmmword ptr [RAX + -0x58],XMM7
1407411a3:  44 0F 29 40 98           MOVAPS xmmword ptr [RAX + -0x68],XMM8
1407411a8:  44 0F 29 48 88           MOVAPS xmmword ptr [RAX + -0x78],XMM9
1407411ad:  44 0F 29 90 78 FF FF FF  MOVAPS xmmword ptr [RAX + -0x88],XMM10
1407411b5:  44 0F 29 98 68 FF FF FF  MOVAPS xmmword ptr [RAX + -0x98],XMM11
1407411bd:  44 0F 29 A0 58 FF FF FF  MOVAPS xmmword ptr [RAX + -0xa8],XMM12
1407411c5:  44 0F 29 A8 48 FF FF FF  MOVAPS xmmword ptr [RAX + -0xb8],XMM13
1407411cd:  44 0F 29 B0 38 FF FF FF  MOVAPS xmmword ptr [RAX + -0xc8],XMM14
1407411d5:  44 0F 29 B8 28 FF FF FF  MOVAPS xmmword ptr [RAX + -0xd8],XMM15
1407411dd:  48 8B 05 64 0E 25 00     MOV RAX,qword ptr [0x140992048]
1407411e4:  48 33 C4                 XOR RAX,RSP
1407411e7:  48 89 85 60 0D 00 00     MOV qword ptr [RBP + 0xd60],RAX
1407411ee:  48 8B FA                 MOV RDI,RDX
1407411f1:  48 89 55 98              MOV qword ptr [RBP + -0x68],RDX
1407411f5:  4C 8B F1                 MOV R14,RCX
1407411f8:  48 89 4D 20              MOV qword ptr [RBP + 0x20],RCX
1407411fc:  48 8B 81 C0 CA 00 00     MOV RAX,qword ptr [RCX + 0xcac0]
140741203:  48 8B 18                 MOV RBX,qword ptr [RAX]
140741206:  80 7A 01 00              CMP byte ptr [RDX + 0x1],0x0
14074120a:  0F 85 C6 3E 00 00        JNZ 0x1407450d6
140741210:  0F 57 C0                 XORPS XMM0,XMM0
140741213:  F3 0F 7F 45 58           MOVDQU xmmword ptr [RBP + 0x58],XMM0
140741218:  33 C9                    XOR ECX,ECX
14074121a:  48 89 4D 68              MOV qword ptr [RBP + 0x68],RCX
14074121e:  F3 0F 7F 45 E0           MOVDQU xmmword ptr [RBP + -0x20],XMM0
140741223:  48 89 4D D8              MOV qword ptr [RBP + -0x28],RCX
140741227:  48 89 4D F0              MOV qword ptr [RBP + -0x10],RCX
14074122b:  89 4D 90                 MOV dword ptr [RBP + -0x70],ECX
14074122e:  F3 0F 7F 45 70           MOVDQU xmmword ptr [RBP + 0x70],XMM0
140741233:  48 89 8D 80 00 00 00     MOV qword ptr [RBP + 0x80],RCX
14074123a:  4C 8D 45 90              LEA R8,[RBP + -0x70]
14074123e:  33 D2                    XOR EDX,EDX
140741240:  48 8D 4D 70              LEA RCX,[RBP + 0x70]
140741244:  E8 B7 BB 98 FF           CALL 0x1400cce00
140741249:  45 33 DB                 XOR R11D,R11D
14074124c:  45 8B FB                 MOV R15D,R11D
14074124f:  4C 89 5D D0              MOV qword ptr [RBP + -0x30],R11
140741253:  4C 89 9D 88 00 00 00     MOV qword ptr [RBP + 0x88],R11
14074125a:  F3 44 0F 10 05 25 0E 25 00 MOVSS XMM8,dword ptr [0x140992088]
140741263:  41 0F 28 C0              MOVAPS XMM0,XMM8
140741267:  F3 0F 59 05 39 94 1C 00  MULSS XMM0,dword ptr [0x14090a6a8]
14074126f:  F3 44 0F 10 4F 04        MOVSS XMM9,dword ptr [RDI + 0x4]
140741275:  F3 44 0F 5C C8           SUBSS XMM9,XMM0
14074127a:  F3 44 0F 58 4F 28        ADDSS XMM9,dword ptr [RDI + 0x28]
140741280:  41 0F 28 C0              MOVAPS XMM0,XMM8
140741284:  F3 0F 10 35 40 97 1C 00  MOVSS XMM6,dword ptr [0x14090a9cc]
14074128c:  F3 0F 59 C6              MULSS XMM0,XMM6
140741290:  F3 44 0F 58 C8           ADDSS XMM9,XMM0
140741295:  F3 44 0F 11 4D 84        MOVSS dword ptr [RBP + -0x7c],XMM9
14074129b:  F3 0F 10 4F 08           MOVSS XMM1,dword ptr [RDI + 0x8]
1407412a0:  F3 0F 58 4F 2C           ADDSS XMM1,dword ptr [RDI + 0x2c]
1407412a5:  41 0F 28 C0              MOVAPS XMM0,XMM8
1407412a9:  F3 44 0F 10 3D 3E 97 1C 00 MOVSS XMM15,dword ptr [0x14090a9f0]
1407412b2:  F3 41 0F 59 C7           MULSS XMM0,XMM15
1407412b7:  F3 0F 58 C1              ADDSS XMM0,XMM1
1407412bb:  F3 0F 11 45 80           MOVSS dword ptr [RBP + -0x80],XMM0
1407412c0:  F3 44 0F 59 05 BB 98 1C 00 MULSS XMM8,dword ptr [0x14090ab84]
1407412c9:  F3 44 0F 58 C1           ADDSS XMM8,XMM1
1407412ce:  48 8B B7 40 02 00 00     MOV RSI,qword ptr [RDI + 0x240]
1407412d5:  44 88 5D 8C              MOV byte ptr [RBP + -0x74],R11B
1407412d9:  48 3B FB                 CMP RDI,RBX
1407412dc:  0F 85 19 01 00 00        JNZ 0x1407413fb
1407412e2:  45 8B E3                 MOV R12D,R11D
1407412e5:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407412ec:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407412f3:  48 C1 F8 03              SAR RAX,0x3
1407412f7:  48 85 C0                 TEST RAX,RAX
1407412fa:  0F 84 FB 00 00 00        JZ 0x1407413fb
140741300:  49 8D 9E C0 1D 01 00     LEA RBX,[R14 + 0x11dc0]
140741307:  41 8B FB                 MOV EDI,R11D
14074130a:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
140741310:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741317:  4C 8B 1C 07              MOV R11,qword ptr [RDI + RAX*0x1]
14074131b:  4D 8B 53 08              MOV R10,qword ptr [R11 + 0x8]
14074131f:  48 85 DB                 TEST RBX,RBX
140741322:  0F 84 A7 00 00 00        JZ 0x1407413cf
140741328:  41 8B D7                 MOV EDX,R15D
14074132b:  4C 8B 03                 MOV R8,qword ptr [RBX]
14074132e:  48 8B 43 08              MOV RAX,qword ptr [RBX + 0x8]
140741332:  49 2B C0                 SUB RAX,R8
140741335:  48 C1 F8 03              SAR RAX,0x3
140741339:  48 85 C0                 TEST RAX,RAX
14074133c:  0F 84 8D 00 00 00        JZ 0x1407413cf
140741342:  49 8B CF                 MOV RCX,R15
140741345:  4C 8B 4B 08              MOV R9,qword ptr [RBX + 0x8]
140741349:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
140741350:  4D 3B 14 C8              CMP R10,qword ptr [R8 + RCX*0x8]
140741354:  74 15                    JZ 0x14074136b
140741356:  FF C2                    INC EDX
140741358:  49 8B C1                 MOV RAX,R9
14074135b:  49 2B C0                 SUB RAX,R8
14074135e:  48 C1 F8 03              SAR RAX,0x3
140741362:  8B CA                    MOV ECX,EDX
140741364:  48 3B C8                 CMP RCX,RAX
140741367:  72 E7                    JC 0x140741350
140741369:  EB 64                    JMP 0x1407413cf
14074136b:  49 8B 43 10              MOV RAX,qword ptr [R11 + 0x10]
14074136f:  0F B6 08                 MOVZX ECX,byte ptr [RAX]
140741372:  84 C9                    TEST CL,CL
140741374:  74 1F                    JZ 0x140741395
140741376:  44 38 B8 C8 00 00 00     CMP byte ptr [RAX + 0xc8],R15B
14074137d:  74 09                    JZ 0x140741388
14074137f:  0F 10 05 E2 7A 1C 00     MOVUPS XMM0,xmmword ptr [0x140908e68]
140741386:  EB 26                    JMP 0x1407413ae
140741388:  84 C9                    TEST CL,CL
14074138a:  74 09                    JZ 0x140741395
14074138c:  0F 10 05 75 7A 1C 00     MOVUPS XMM0,xmmword ptr [0x140908e08]
140741393:  EB 19                    JMP 0x1407413ae
140741395:  44 38 B8 C8 00 00 00     CMP byte ptr [RAX + 0xc8],R15B
14074139c:  74 09                    JZ 0x1407413a7
14074139e:  0F 10 05 53 7A 1C 00     MOVUPS XMM0,xmmword ptr [0x140908df8]
1407413a5:  EB 07                    JMP 0x1407413ae
1407413a7:  0F 10 05 3A 7A 1C 00     MOVUPS XMM0,xmmword ptr [0x140908de8]
1407413ae:  44 88 7C 24 20           MOV byte ptr [RSP + 0x20],R15B
1407413b3:  0F 29 85 40 01 00 00     MOVAPS xmmword ptr [RBP + 0x140],XMM0
1407413ba:  4C 8D 8D 40 01 00 00     LEA R9,[RBP + 0x140]
1407413c1:  45 33 C0                 XOR R8D,R8D
1407413c4:  49 8B D2                 MOV RDX,R10
1407413c7:  49 8B CE                 MOV RCX,R14
1407413ca:  E8 81 B1 CC FF           CALL 0x14040c550
1407413cf:  41 FF C4                 INC R12D
1407413d2:  48 83 C7 08              ADD RDI,0x8
1407413d6:  48 8B 8E 40 0D 00 00     MOV RCX,qword ptr [RSI + 0xd40]
1407413dd:  48 2B 8E 38 0D 00 00     SUB RCX,qword ptr [RSI + 0xd38]
1407413e4:  48 C1 F9 03              SAR RCX,0x3
1407413e8:  49 63 C4                 MOVSXD RAX,R12D
1407413eb:  48 3B C1                 CMP RAX,RCX
1407413ee:  0F 82 1C FF FF FF        JC 0x140741310
1407413f4:  4C 8B 7D D0              MOV R15,qword ptr [RBP + -0x30]
1407413f8:  45 33 DB                 XOR R11D,R11D
1407413fb:  45 8B EB                 MOV R13D,R11D
1407413fe:  48 8D 96 70 0C 00 00     LEA RDX,[RSI + 0xc70]
140741405:  48 8B 42 08              MOV RAX,qword ptr [RDX + 0x8]
140741409:  48 2B 02                 SUB RAX,qword ptr [RDX]
14074140c:  48 C1 F8 03              SAR RAX,0x3
140741410:  48 85 C0                 TEST RAX,RAX
140741413:  0F 84 35 01 00 00        JZ 0x14074154e
140741419:  4D 8B E3                 MOV R12,R11
14074141c:  0F 1F 40 00              NOP dword ptr [RAX]
140741420:  48 8B 02                 MOV RAX,qword ptr [RDX]
140741423:  49 8B 1C 04              MOV RBX,qword ptr [R12 + RAX*0x1]
140741427:  48 8B 83 08 17 00 00     MOV RAX,qword ptr [RBX + 0x1708]
14074142e:  48 85 C0                 TEST RAX,RAX
140741431:  74 1B                    JZ 0x14074144e
140741433:  83 B8 00 86 00 00 11     CMP dword ptr [RAX + 0x8600],0x11
14074143a:  75 12                    JNZ 0x14074144e
14074143c:  48 8D 55 58              LEA RDX,[RBP + 0x58]
140741440:  48 8D 0D C9 3A 29 00     LEA RCX,[0x1409d4f10]
140741447:  E8 04 B3 A5 FF           CALL 0x14019c750
14074144c:  EB 64                    JMP 0x1407414b2
14074144e:  41 8B FB                 MOV EDI,R11D
140741451:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
140741458:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
14074145f:  48 C1 F8 04              SAR RAX,0x4
140741463:  48 85 C0                 TEST RAX,RAX
140741466:  74 4D                    JZ 0x1407414b5
140741468:  4D 8B C3                 MOV R8,R11
14074146b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140741470:  4D 03 C0                 ADD R8,R8
140741473:  48 8B 83 B8 03 00 00     MOV RAX,qword ptr [RBX + 0x3b8]
14074147a:  4A 8B 0C C0              MOV RCX,qword ptr [RAX + R8*0x8]
14074147e:  83 79 44 00              CMP dword ptr [RCX + 0x44],0x0
140741482:  7C 12                    JL 0x140741496
140741484:  83 BB 94 17 00 00 00     CMP dword ptr [RBX + 0x1794],0x0
14074148b:  75 09                    JNZ 0x140741496
14074148d:  48 8D 55 58              LEA RDX,[RBP + 0x58]
140741491:  E8 6A 85 9B FF           CALL 0x1400f9a00
140741496:  FF C7                    INC EDI
140741498:  48 8B 83 C0 03 00 00     MOV RAX,qword ptr [RBX + 0x3c0]
14074149f:  48 2B 83 B8 03 00 00     SUB RAX,qword ptr [RBX + 0x3b8]
1407414a6:  48 C1 F8 04              SAR RAX,0x4
1407414aa:  44 8B C7                 MOV R8D,EDI
1407414ad:  4C 3B C0                 CMP R8,RAX
1407414b0:  72 BE                    JC 0x140741470
1407414b2:  45 33 DB                 XOR R11D,R11D
1407414b5:  41 8B FB                 MOV EDI,R11D
1407414b8:  48 8B 83 A8 03 00 00     MOV RAX,qword ptr [RBX + 0x3a8]
1407414bf:  48 2B 83 A0 03 00 00     SUB RAX,qword ptr [RBX + 0x3a0]
1407414c6:  48 C1 F8 03              SAR RAX,0x3
1407414ca:  48 85 C0                 TEST RAX,RAX
1407414cd:  74 3C                    JZ 0x14074150b
1407414cf:  4D 8B D3                 MOV R10,R11
1407414d2:  48 8B 8B A0 03 00 00     MOV RCX,qword ptr [RBX + 0x3a0]
1407414d9:  45 33 C9                 XOR R9D,R9D
1407414dc:  41 B0 01                 MOV R8B,0x1
1407414df:  48 8D 55 58              LEA RDX,[RBP + 0x58]
1407414e3:  4A 8B 0C D1              MOV RCX,qword ptr [RCX + R10*0x8]
1407414e7:  E8 A4 D8 F7 FF           CALL 0x1406bed90
1407414ec:  FF C7                    INC EDI
1407414ee:  48 8B 83 A8 03 00 00     MOV RAX,qword ptr [RBX + 0x3a8]
1407414f5:  48 2B 83 A0 03 00 00     SUB RAX,qword ptr [RBX + 0x3a0]
1407414fc:  48 C1 F8 03              SAR RAX,0x3
140741500:  44 8B D7                 MOV R10D,EDI
140741503:  4C 3B D0                 CMP R10,RAX
140741506:  72 CA                    JC 0x1407414d2
140741508:  45 33 DB                 XOR R11D,R11D
14074150b:  80 7D 8C 00              CMP byte ptr [RBP + -0x74],0x0
14074150f:  75 18                    JNZ 0x140741529
140741511:  48 83 BB 20 05 00 00 00  CMP qword ptr [RBX + 0x520],0x0
140741519:  75 0A                    JNZ 0x140741525
14074151b:  48 83 BB 18 05 00 00 00  CMP qword ptr [RBX + 0x518],0x0
140741523:  74 04                    JZ 0x140741529
140741525:  C6 45 8C 01              MOV byte ptr [RBP + -0x74],0x1
140741529:  41 FF C5                 INC R13D
14074152c:  49 83 C4 08              ADD R12,0x8
140741530:  48 8D 96 70 0C 00 00     LEA RDX,[RSI + 0xc70]
140741537:  48 8B 4A 08              MOV RCX,qword ptr [RDX + 0x8]
14074153b:  48 2B 0A                 SUB RCX,qword ptr [RDX]
14074153e:  48 C1 F9 03              SAR RCX,0x3
140741542:  49 63 C5                 MOVSXD RAX,R13D
140741545:  48 3B C1                 CMP RAX,RCX
140741548:  0F 82 D2 FE FF FF        JC 0x140741420
14074154e:  C6 45 90 00              MOV byte ptr [RBP + -0x70],0x0
140741552:  40 32 FF                 XOR DIL,DIL
140741555:  89 7D B0                 MOV dword ptr [RBP + -0x50],EDI
140741558:  32 C9                    XOR CL,CL
14074155a:  89 4D C0                 MOV dword ptr [RBP + -0x40],ECX
14074155d:  45 32 E4                 XOR R12B,R12B
140741560:  45 32 ED                 XOR R13B,R13B
140741563:  44 89 6D C8              MOV dword ptr [RBP + -0x38],R13D
140741567:  44 89 5D 88              MOV dword ptr [RBP + -0x78],R11D
14074156b:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140741572:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
140741579:  48 C1 F8 03              SAR RAX,0x3
14074157d:  45 0F 57 E4              XORPS XMM12,XMM12
140741581:  48 85 C0                 TEST RAX,RAX
140741584:  48 8B 45 E8              MOV RAX,qword ptr [RBP + -0x18]
140741588:  48 89 45 B8              MOV qword ptr [RBP + -0x48],RAX
14074158c:  0F 84 34 09 00 00        JZ 0x140741ec6
140741592:  49 8B DB                 MOV RBX,R11
140741595:  F3 0F 10 35 F3 85 1C 00  MOVSS XMM6,dword ptr [0x140909b90]
14074159d:  0F 1F 00                 NOP dword ptr [RAX]
1407415a0:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407415a7:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407415ab:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1407415af:  48 89 45 D0              MOV qword ptr [RBP + -0x30],RAX
1407415b3:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407415ba:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407415be:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
1407415c2:  80 38 00                 CMP byte ptr [RAX],0x0
1407415c5:  0F 84 47 02 00 00        JZ 0x140741812
1407415cb:  45 32 C9                 XOR R9B,R9B
1407415ce:  45 32 D2                 XOR R10B,R10B
1407415d1:  45 8B C3                 MOV R8D,R11D
1407415d4:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407415db:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407415df:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1407415e3:  48 8B 88 78 09 00 00     MOV RCX,qword ptr [RAX + 0x978]
1407415ea:  48 2B 88 70 09 00 00     SUB RCX,qword ptr [RAX + 0x970]
1407415f1:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
1407415fb:  48 F7 E9                 IMUL RCX
1407415fe:  48 C1 FA 06              SAR RDX,0x6
140741602:  48 8B C2                 MOV RAX,RDX
140741605:  48 C1 E8 3F              SHR RAX,0x3f
140741609:  48 03 D0                 ADD RDX,RAX
14074160c:  0F 84 BB 00 00 00        JZ 0x1407416cd
140741612:  48 BF 25 49 92 24 49 92 24 49 MOV RDI,0x4924924924924925
14074161c:  41 BE 01 00 00 00        MOV R14D,0x1
140741622:  0F 1F 40 00              NOP dword ptr [RAX]
140741626:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140741630:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741637:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
14074163b:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
14074163f:  49 69 D3 E0 00 00 00     IMUL RDX,R11,0xe0
140741646:  48 8B 80 70 09 00 00     MOV RAX,qword ptr [RAX + 0x970]
14074164d:  45 0F B6 C9              MOVZX R9D,R9B
140741651:  83 BC 02 90 00 00 00 02  CMP dword ptr [RDX + RAX*0x1 + 0x90],0x2
140741659:  45 0F 44 CE              CMOVZ R9D,R14D
14074165d:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741664:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741668:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
14074166c:  48 8B 88 70 09 00 00     MOV RCX,qword ptr [RAX + 0x970]
140741673:  45 0F B6 D2              MOVZX R10D,R10B
140741677:  44 39 B4 0A 90 00 00 00  CMP dword ptr [RDX + RCX*0x1 + 0x90],R14D
14074167f:  45 0F 44 D6              CMOVZ R10D,R14D
140741683:  41 FF C0                 INC R8D
140741686:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074168d:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741691:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
140741695:  48 8B 88 78 09 00 00     MOV RCX,qword ptr [RAX + 0x978]
14074169c:  48 2B 88 70 09 00 00     SUB RCX,qword ptr [RAX + 0x970]
1407416a3:  48 8B C7                 MOV RAX,RDI
1407416a6:  48 F7 E9                 IMUL RCX
1407416a9:  48 C1 FA 06              SAR RDX,0x6
1407416ad:  48 8B C2                 MOV RAX,RDX
1407416b0:  48 C1 E8 3F              SHR RAX,0x3f
1407416b4:  48 03 D0                 ADD RDX,RAX
1407416b7:  45 8B D8                 MOV R11D,R8D
1407416ba:  4C 3B DA                 CMP R11,RDX
1407416bd:  0F 82 6D FF FF FF        JC 0x140741630
1407416c3:  4C 8B 75 20              MOV R14,qword ptr [RBP + 0x20]
1407416c7:  8B 7D B0                 MOV EDI,dword ptr [RBP + -0x50]
1407416ca:  45 33 DB                 XOR R11D,R11D
1407416cd:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407416d4:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407416d8:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
1407416dc:  48 8B 48 10              MOV RCX,qword ptr [RAX + 0x10]
1407416e0:  48 2B 48 08              SUB RCX,qword ptr [RAX + 0x8]
1407416e4:  48 C1 F9 04              SAR RCX,0x4
1407416e8:  48 85 C9                 TEST RCX,RCX
1407416eb:  0F 84 DD 00 00 00        JZ 0x1407417ce
1407416f1:  33 C0                    XOR EAX,EAX
1407416f3:  44 8B C0                 MOV R8D,EAX
1407416f6:  44 8D 68 01              LEA R13D,[RAX + 0x1]
1407416fa:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
140741700:  45 84 C9                 TEST R9B,R9B
140741703:  74 6A                    JZ 0x14074176f
140741705:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074170c:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741710:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
140741714:  49 8B D0                 MOV RDX,R8
140741717:  48 03 D2                 ADD RDX,RDX
14074171a:  48 8B 49 08              MOV RCX,qword ptr [RCX + 0x8]
14074171e:  49 8B 86 D0 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3d0]
140741725:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
140741729:  74 40                    JZ 0x14074176b
14074172b:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741732:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741736:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
14074173a:  48 8B 49 08              MOV RCX,qword ptr [RCX + 0x8]
14074173e:  49 8B 86 D8 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3d8]
140741745:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
140741749:  74 20                    JZ 0x14074176b
14074174b:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741752:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741756:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
14074175a:  48 8B 49 08              MOV RCX,qword ptr [RCX + 0x8]
14074175e:  49 8B 86 C8 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3c8]
140741765:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
140741769:  75 04                    JNZ 0x14074176f
14074176b:  41 0F B6 FD              MOVZX EDI,R13B
14074176f:  45 84 D2                 TEST R10B,R10B
140741772:  74 29                    JZ 0x14074179d
140741774:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074177b:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
14074177f:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741783:  48 8B 48 08              MOV RCX,qword ptr [RAX + 0x8]
140741787:  4D 03 C0                 ADD R8,R8
14074178a:  45 0F B6 E4              MOVZX R12D,R12B
14074178e:  49 8B 86 E0 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3e0]
140741795:  4A 39 04 C1              CMP qword ptr [RCX + R8*0x8],RAX
140741799:  45 0F 44 E5              CMOVZ R12D,R13D
14074179d:  41 FF C3                 INC R11D
1407417a0:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407417a7:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407417ab:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
1407417af:  48 8B 48 10              MOV RCX,qword ptr [RAX + 0x10]
1407417b3:  48 2B 48 08              SUB RCX,qword ptr [RAX + 0x8]
1407417b7:  48 C1 F9 04              SAR RCX,0x4
1407417bb:  45 8B C3                 MOV R8D,R11D
1407417be:  4C 3B C1                 CMP R8,RCX
1407417c1:  0F 82 39 FF FF FF        JC 0x140741700
1407417c7:  89 7D B0                 MOV dword ptr [RBP + -0x50],EDI
1407417ca:  44 8B 6D C8              MOV R13D,dword ptr [RBP + -0x38]
1407417ce:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407417d5:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407417d9:  48 8B 51 10              MOV RDX,qword ptr [RCX + 0x10]
1407417dd:  48 8B 42 10              MOV RAX,qword ptr [RDX + 0x10]
1407417e1:  48 2B 42 08              SUB RAX,qword ptr [RDX + 0x8]
1407417e5:  48 83 F8 10              CMP RAX,0x10
1407417e9:  73 24                    JNC 0x14074180f
1407417eb:  40 0F B6 FF              MOVZX EDI,DIL
1407417ef:  45 84 C9                 TEST R9B,R9B
1407417f2:  B8 01 00 00 00           MOV EAX,0x1
1407417f7:  0F 45 F8                 CMOVNZ EDI,EAX
1407417fa:  89 7D B0                 MOV dword ptr [RBP + -0x50],EDI
1407417fd:  45 0F B6 E4              MOVZX R12D,R12B
140741801:  45 84 D2                 TEST R10B,R10B
140741804:  8B F8                    MOV EDI,EAX
140741806:  44 0F 45 E0              CMOVNZ R12D,EAX
14074180a:  45 33 DB                 XOR R11D,R11D
14074180d:  EB 08                    JMP 0x140741817
14074180f:  45 33 DB                 XOR R11D,R11D
140741812:  BF 01 00 00 00           MOV EDI,0x1
140741817:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074181e:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741822:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741826:  80 B8 C8 00 00 00 00     CMP byte ptr [RAX + 0xc8],0x0
14074182d:  0F 84 4B 02 00 00        JZ 0x140741a7e
140741833:  45 32 C9                 XOR R9B,R9B
140741836:  45 32 D2                 XOR R10B,R10B
140741839:  45 8B C3                 MOV R8D,R11D
14074183c:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741843:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741847:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
14074184b:  48 8B 88 78 09 00 00     MOV RCX,qword ptr [RAX + 0x978]
140741852:  48 2B 88 70 09 00 00     SUB RCX,qword ptr [RAX + 0x970]
140741859:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
140741863:  48 F7 E9                 IMUL RCX
140741866:  48 C1 FA 06              SAR RDX,0x6
14074186a:  48 8B C2                 MOV RAX,RDX
14074186d:  48 C1 E8 3F              SHR RAX,0x3f
140741871:  48 03 D0                 ADD RDX,RAX
140741874:  0F 84 B0 00 00 00        JZ 0x14074192a
14074187a:  49 BE 25 49 92 24 49 92 24 49 MOV R14,0x4924924924924925
140741884:  0F 1F 40 00              NOP dword ptr [RAX]
140741888:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
140741890:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741897:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
14074189b:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
14074189f:  49 69 D3 E0 00 00 00     IMUL RDX,R11,0xe0
1407418a6:  48 8B 80 70 09 00 00     MOV RAX,qword ptr [RAX + 0x970]
1407418ad:  45 0F B6 C9              MOVZX R9D,R9B
1407418b1:  83 BC 10 90 00 00 00 02  CMP dword ptr [RAX + RDX*0x1 + 0x90],0x2
1407418b9:  44 0F 44 CF              CMOVZ R9D,EDI
1407418bd:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407418c4:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407418c8:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1407418cc:  48 8B 88 70 09 00 00     MOV RCX,qword ptr [RAX + 0x970]
1407418d3:  45 0F B6 D2              MOVZX R10D,R10B
1407418d7:  83 BC 11 90 00 00 00 01  CMP dword ptr [RCX + RDX*0x1 + 0x90],0x1
1407418df:  44 0F 44 D7              CMOVZ R10D,EDI
1407418e3:  41 FF C0                 INC R8D
1407418e6:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407418ed:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407418f1:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
1407418f5:  48 8B 88 78 09 00 00     MOV RCX,qword ptr [RAX + 0x978]
1407418fc:  48 2B 88 70 09 00 00     SUB RCX,qword ptr [RAX + 0x970]
140741903:  49 8B C6                 MOV RAX,R14
140741906:  48 F7 E9                 IMUL RCX
140741909:  48 C1 FA 06              SAR RDX,0x6
14074190d:  48 8B C2                 MOV RAX,RDX
140741910:  48 C1 E8 3F              SHR RAX,0x3f
140741914:  48 03 D0                 ADD RDX,RAX
140741917:  45 8B D8                 MOV R11D,R8D
14074191a:  4C 3B DA                 CMP R11,RDX
14074191d:  0F 82 6D FF FF FF        JC 0x140741890
140741923:  4C 8B 75 20              MOV R14,qword ptr [RBP + 0x20]
140741927:  45 33 DB                 XOR R11D,R11D
14074192a:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741931:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741935:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741939:  48 8B 88 D8 00 00 00     MOV RCX,qword ptr [RAX + 0xd8]
140741940:  48 2B 88 D0 00 00 00     SUB RCX,qword ptr [RAX + 0xd0]
140741947:  48 C1 F9 04              SAR RCX,0x4
14074194b:  48 85 C9                 TEST RCX,RCX
14074194e:  0F 84 E9 00 00 00        JZ 0x140741a3d
140741954:  33 C0                    XOR EAX,EAX
140741956:  44 8B C0                 MOV R8D,EAX
140741959:  0F 1F 80 00 00 00 00     NOP dword ptr [RAX]
140741960:  45 84 C9                 TEST R9B,R9B
140741963:  74 73                    JZ 0x1407419d8
140741965:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074196c:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741970:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
140741974:  49 8B D0                 MOV RDX,R8
140741977:  48 03 D2                 ADD RDX,RDX
14074197a:  48 8B 89 D0 00 00 00     MOV RCX,qword ptr [RCX + 0xd0]
140741981:  49 8B 86 D0 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3d0]
140741988:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
14074198c:  74 46                    JZ 0x1407419d4
14074198e:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741995:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741999:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
14074199d:  48 8B 89 D0 00 00 00     MOV RCX,qword ptr [RCX + 0xd0]
1407419a4:  49 8B 86 D8 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3d8]
1407419ab:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
1407419af:  74 23                    JZ 0x1407419d4
1407419b1:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407419b8:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407419bc:  48 8B 49 10              MOV RCX,qword ptr [RCX + 0x10]
1407419c0:  48 8B 89 D0 00 00 00     MOV RCX,qword ptr [RCX + 0xd0]
1407419c7:  49 8B 86 C8 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3c8]
1407419ce:  48 39 04 D1              CMP qword ptr [RCX + RDX*0x8],RAX
1407419d2:  75 04                    JNZ 0x1407419d8
1407419d4:  C6 45 C0 01              MOV byte ptr [RBP + -0x40],0x1
1407419d8:  45 84 D2                 TEST R10B,R10B
1407419db:  74 2C                    JZ 0x140741a09
1407419dd:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
1407419e4:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
1407419e8:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
1407419ec:  48 8B 88 D0 00 00 00     MOV RCX,qword ptr [RAX + 0xd0]
1407419f3:  4D 03 C0                 ADD R8,R8
1407419f6:  45 0F B6 ED              MOVZX R13D,R13B
1407419fa:  49 8B 86 E0 C3 00 00     MOV RAX,qword ptr [R14 + 0xc3e0]
140741a01:  4A 39 04 C1              CMP qword ptr [RCX + R8*0x8],RAX
140741a05:  44 0F 44 EF              CMOVZ R13D,EDI
140741a09:  41 FF C3                 INC R11D
140741a0c:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741a13:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741a17:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741a1b:  48 8B 88 D8 00 00 00     MOV RCX,qword ptr [RAX + 0xd8]
140741a22:  48 2B 88 D0 00 00 00     SUB RCX,qword ptr [RAX + 0xd0]
140741a29:  48 C1 F9 04              SAR RCX,0x4
140741a2d:  45 8B C3                 MOV R8D,R11D
140741a30:  4C 3B C1                 CMP R8,RCX
140741a33:  0F 82 27 FF FF FF        JC 0x140741960
140741a39:  44 89 6D C8              MOV dword ptr [RBP + -0x38],R13D
140741a3d:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741a44:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741a48:  48 8B 51 10              MOV RDX,qword ptr [RCX + 0x10]
140741a4c:  48 8B 82 D8 00 00 00     MOV RAX,qword ptr [RDX + 0xd8]
140741a53:  48 2B 82 D0 00 00 00     SUB RAX,qword ptr [RDX + 0xd0]
140741a5a:  48 83 F8 10              CMP RAX,0x10
140741a5e:  73 1E                    JNC 0x140741a7e
140741a60:  8B 45 C0                 MOV EAX,dword ptr [RBP + -0x40]
140741a63:  0F B6 C0                 MOVZX EAX,AL
140741a66:  45 84 C9                 TEST R9B,R9B
140741a69:  0F 45 C7                 CMOVNZ EAX,EDI
140741a6c:  89 45 C0                 MOV dword ptr [RBP + -0x40],EAX
140741a6f:  45 0F B6 ED              MOVZX R13D,R13B
140741a73:  45 84 D2                 TEST R10B,R10B
140741a76:  44 0F 45 EF              CMOVNZ R13D,EDI
140741a7a:  44 89 6D C8              MOV dword ptr [RBP + -0x38],R13D
140741a7e:  48 8B 55 D0              MOV RDX,qword ptr [RBP + -0x30]
140741a82:  48 8B 82 18 03 00 00     MOV RAX,qword ptr [RDX + 0x318]
140741a89:  48 85 C0                 TEST RAX,RAX
140741a8c:  0F 84 DA 03 00 00        JZ 0x140741e6c
140741a92:  83 B8 60 03 00 00 00     CMP dword ptr [RAX + 0x360],0x0
140741a99:  0F 85 CD 03 00 00        JNZ 0x140741e6c
140741a9f:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741aa6:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741aaa:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741aae:  80 38 00                 CMP byte ptr [RAX],0x0
140741ab1:  0F 84 D2 01 00 00        JZ 0x140741c89
140741ab7:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741abe:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741ac2:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741ac6:  F3 0F 10 40 04           MOVSS XMM0,dword ptr [RAX + 0x4]
140741acb:  41 0F 2F C4              COMISS XMM0,XMM12
140741acf:  0F 86 B4 01 00 00        JBE 0x140741c89
140741ad5:  C6 44 24 30 00           MOV byte ptr [RSP + 0x30],0x0
140741ada:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140741adf:  C6 44 24 20 01           MOV byte ptr [RSP + 0x20],0x1
140741ae4:  41 0F 28 DC              MOVAPS XMM3,XMM12
140741ae8:  45 33 C0                 XOR R8D,R8D
140741aeb:  49 8B CE                 MOV RCX,R14
140741aee:  E8 3D 29 CE FF           CALL 0x140424430
140741af3:  45 33 D2                 XOR R10D,R10D
140741af6:  45 8B CA                 MOV R9D,R10D
140741af9:  49 8B 8E B0 44 01 00     MOV RCX,qword ptr [R14 + 0x144b0]
140741b00:  49 2B 8E A8 44 01 00     SUB RCX,qword ptr [R14 + 0x144a8]
140741b07:  48 BF C5 4E EC C4 4E EC C4 4E MOV RDI,0x4ec4ec4ec4ec4ec5
140741b11:  48 8B C7                 MOV RAX,RDI
140741b14:  48 F7 E9                 IMUL RCX
140741b17:  48 C1 FA 05              SAR RDX,0x5
140741b1b:  48 8B C2                 MOV RAX,RDX
140741b1e:  48 C1 E8 3F              SHR RAX,0x3f
140741b22:  48 03 D0                 ADD RDX,RAX
140741b25:  74 56                    JZ 0x140741b7d
140741b27:  45 8B C2                 MOV R8D,R10D
140741b2a:  66 0F 1F 44 00 00        NOP word ptr [RAX + RAX*0x1]
140741b30:  49 8B 86 A8 44 01 00     MOV RAX,qword ptr [R14 + 0x144a8]
140741b37:  49 8B 4C 00 30           MOV RCX,qword ptr [R8 + RAX*0x1 + 0x30]
140741b3c:  48 8B 81 18 03 00 00     MOV RAX,qword ptr [RCX + 0x318]
140741b43:  83 B8 60 03 00 00 0B     CMP dword ptr [RAX + 0x360],0xb
140741b4a:  74 3A                    JZ 0x140741b86
140741b4c:  41 FF C1                 INC R9D
140741b4f:  49 83 C0 68              ADD R8,0x68
140741b53:  49 8B 8E B0 44 01 00     MOV RCX,qword ptr [R14 + 0x144b0]
140741b5a:  49 2B 8E A8 44 01 00     SUB RCX,qword ptr [R14 + 0x144a8]
140741b61:  48 8B C7                 MOV RAX,RDI
140741b64:  48 F7 E9                 IMUL RCX
140741b67:  48 C1 FA 05              SAR RDX,0x5
140741b6b:  48 8B C2                 MOV RAX,RDX
140741b6e:  48 C1 E8 3F              SHR RAX,0x3f
140741b72:  48 03 D0                 ADD RDX,RAX
140741b75:  49 63 C1                 MOVSXD RAX,R9D
140741b78:  48 3B C2                 CMP RAX,RDX
140741b7b:  72 B3                    JC 0x140741b30
140741b7d:  48 8B 55 D0              MOV RDX,qword ptr [RBP + -0x30]
140741b81:  E9 0D 01 00 00           JMP 0x140741c93
140741b86:  48 8D 45 88              LEA RAX,[RBP + -0x78]
140741b8a:  48 8B 4D B8              MOV RCX,qword ptr [RBP + -0x48]
140741b8e:  48 3B C1                 CMP RAX,RCX
140741b91:  73 49                    JNC 0x140741bdc
140741b93:  48 8D 55 88              LEA RDX,[RBP + -0x78]
140741b97:  48 8B 45 E0              MOV RAX,qword ptr [RBP + -0x20]
140741b9b:  48 3B C2                 CMP RAX,RDX
140741b9e:  77 3C                    JA 0x140741bdc
140741ba0:  48 8D 7D 88              LEA RDI,[RBP + -0x78]
140741ba4:  48 2B F8                 SUB RDI,RAX
140741ba7:  48 C1 FF 02              SAR RDI,0x2
140741bab:  48 3B 4D D8              CMP RCX,qword ptr [RBP + -0x28]
140741baf:  75 21                    JNZ 0x140741bd2
140741bb1:  BA 01 00 00 00           MOV EDX,0x1
140741bb6:  48 8D 4D E0              LEA RCX,[RBP + -0x20]
140741bba:  E8 81 C8 8D FF           CALL 0x14001e440
140741bbf:  48 8B 45 F0              MOV RAX,qword ptr [RBP + -0x10]
140741bc3:  48 89 45 D8              MOV qword ptr [RBP + -0x28],RAX
140741bc7:  48 8B 4D E8              MOV RCX,qword ptr [RBP + -0x18]
140741bcb:  48 8B 45 E0              MOV RAX,qword ptr [RBP + -0x20]
140741bcf:  45 33 D2                 XOR R10D,R10D
140741bd2:  48 85 C9                 TEST RCX,RCX
140741bd5:  74 32                    JZ 0x140741c09
140741bd7:  8B 04 B8                 MOV EAX,dword ptr [RAX + RDI*0x4]
140741bda:  EB 2B                    JMP 0x140741c07
140741bdc:  48 3B 4D D8              CMP RCX,qword ptr [RBP + -0x28]
140741be0:  75 1D                    JNZ 0x140741bff
140741be2:  BA 01 00 00 00           MOV EDX,0x1
140741be7:  48 8D 4D E0              LEA RCX,[RBP + -0x20]
140741beb:  E8 50 C8 8D FF           CALL 0x14001e440
140741bf0:  48 8B 45 F0              MOV RAX,qword ptr [RBP + -0x10]
140741bf4:  48 89 45 D8              MOV qword ptr [RBP + -0x28],RAX
140741bf8:  48 8B 4D E8              MOV RCX,qword ptr [RBP + -0x18]
140741bfc:  45 33 D2                 XOR R10D,R10D
140741bff:  48 85 C9                 TEST RCX,RCX
140741c02:  74 05                    JZ 0x140741c09
140741c04:  8B 45 88                 MOV EAX,dword ptr [RBP + -0x78]
140741c07:  89 01                    MOV dword ptr [RCX],EAX
140741c09:  48 83 C1 04              ADD RCX,0x4
140741c0d:  48 89 4D E8              MOV qword ptr [RBP + -0x18],RCX
140741c11:  48 89 4D B8              MOV qword ptr [RBP + -0x48],RCX
140741c15:  C6 45 8D 01              MOV byte ptr [RBP + -0x73],0x1
140741c19:  48 8B 4D 70              MOV RCX,qword ptr [RBP + 0x70]
140741c1d:  48 89 8D 30 01 00 00     MOV qword ptr [RBP + 0x130],RCX
140741c24:  4C 89 95 38 01 00 00     MOV qword ptr [RBP + 0x138],R10
140741c2b:  4D 85 FF                 TEST R15,R15
140741c2e:  74 46                    JZ 0x140741c76
140741c30:  79 27                    JNS 0x140741c59
140741c32:  49 8B C7                 MOV RAX,R15
140741c35:  48 F7 D8                 NEG RAX
140741c38:  74 1F                    JZ 0x140741c59
140741c3a:  48 83 C8 FF              OR RAX,-0x1
140741c3e:  49 2B C7                 SUB RAX,R15
140741c41:  48 C1 E8 05              SHR RAX,0x5
140741c45:  48 8D 04 85 04 00 00 00  LEA RAX,[0x4 + RAX*0x4]
140741c4d:  48 2B C8                 SUB RCX,RAX
140741c50:  48 89 8D 30 01 00 00     MOV qword ptr [RBP + 0x130],RCX
140741c57:  EB 12                    JMP 0x140741c6b
140741c59:  49 8B C7                 MOV RAX,R15
140741c5c:  48 C1 E8 05              SHR RAX,0x5
140741c60:  48 8D 04 81              LEA RAX,[RCX + RAX*0x4]
140741c64:  48 89 85 30 01 00 00     MOV qword ptr [RBP + 0x130],RAX
140741c6b:  41 83 E7 1F              AND R15D,0x1f
140741c6f:  4C 89 BD 38 01 00 00     MOV qword ptr [RBP + 0x138],R15
140741c76:  0F 28 85 30 01 00 00     MOVAPS XMM0,xmmword ptr [RBP + 0x130]
140741c7d:  48 8D 95 D0 00 00 00     LEA RDX,[RBP + 0xd0]
140741c84:  E9 B5 01 00 00           JMP 0x140741e3e
140741c89:  48 BF C5 4E EC C4 4E EC C4 4E MOV RDI,0x4ec4ec4ec4ec4ec5
140741c93:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741c9a:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741c9e:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140741ca2:  80 B8 C8 00 00 00 00     CMP byte ptr [RAX + 0xc8],0x0
140741ca9:  0F 84 BD 01 00 00        JZ 0x140741e6c
140741caf:  C6 44 24 30 00           MOV byte ptr [RSP + 0x30],0x0
140741cb4:  C6 44 24 28 00           MOV byte ptr [RSP + 0x28],0x0
140741cb9:  C6 44 24 20 01           MOV byte ptr [RSP + 0x20],0x1
140741cbe:  0F 28 DE                 MOVAPS XMM3,XMM6
140741cc1:  45 33 C0                 XOR R8D,R8D
140741cc4:  49 8B CE                 MOV RCX,R14
140741cc7:  E8 64 27 CE FF           CALL 0x140424430
140741ccc:  45 33 DB                 XOR R11D,R11D
140741ccf:  45 8B CB                 MOV R9D,R11D
140741cd2:  49 8B 8E B0 44 01 00     MOV RCX,qword ptr [R14 + 0x144b0]
140741cd9:  49 2B 8E A8 44 01 00     SUB RCX,qword ptr [R14 + 0x144a8]
140741ce0:  48 8B C7                 MOV RAX,RDI
140741ce3:  48 F7 E9                 IMUL RCX
140741ce6:  48 C1 FA 05              SAR RDX,0x5
140741cea:  48 8B C2                 MOV RAX,RDX
140741ced:  48 C1 E8 3F              SHR RAX,0x3f
140741cf1:  48 03 D0                 ADD RDX,RAX
140741cf4:  0F 84 75 01 00 00        JZ 0x140741e6f
140741cfa:  45 8B C3                 MOV R8D,R11D
140741cfd:  0F 1F 00                 NOP dword ptr [RAX]
140741d00:  49 8B 86 A8 44 01 00     MOV RAX,qword ptr [R14 + 0x144a8]
140741d07:  49 8B 4C 00 30           MOV RCX,qword ptr [R8 + RAX*0x1 + 0x30]
140741d0c:  48 8B 81 18 03 00 00     MOV RAX,qword ptr [RCX + 0x318]
140741d13:  83 B8 60 03 00 00 0B     CMP dword ptr [RAX + 0x360],0xb
140741d1a:  74 36                    JZ 0x140741d52
140741d1c:  41 FF C1                 INC R9D
140741d1f:  49 83 C0 68              ADD R8,0x68
140741d23:  49 8B 8E B0 44 01 00     MOV RCX,qword ptr [R14 + 0x144b0]
140741d2a:  49 2B 8E A8 44 01 00     SUB RCX,qword ptr [R14 + 0x144a8]
140741d31:  48 8B C7                 MOV RAX,RDI
140741d34:  48 F7 E9                 IMUL RCX
140741d37:  48 C1 FA 05              SAR RDX,0x5
140741d3b:  48 8B C2                 MOV RAX,RDX
140741d3e:  48 C1 E8 3F              SHR RAX,0x3f
140741d42:  48 03 D0                 ADD RDX,RAX
140741d45:  49 63 C1                 MOVSXD RAX,R9D
140741d48:  48 3B C2                 CMP RAX,RDX
140741d4b:  72 B3                    JC 0x140741d00
140741d4d:  E9 1D 01 00 00           JMP 0x140741e6f
140741d52:  48 8D 45 88              LEA RAX,[RBP + -0x78]
140741d56:  48 8B 4D B8              MOV RCX,qword ptr [RBP + -0x48]
140741d5a:  48 3B C1                 CMP RAX,RCX
140741d5d:  73 49                    JNC 0x140741da8
140741d5f:  48 8D 55 88              LEA RDX,[RBP + -0x78]
140741d63:  48 8B 45 E0              MOV RAX,qword ptr [RBP + -0x20]
140741d67:  48 3B C2                 CMP RAX,RDX
140741d6a:  77 3C                    JA 0x140741da8
140741d6c:  48 8D 7D 88              LEA RDI,[RBP + -0x78]
140741d70:  48 2B F8                 SUB RDI,RAX
140741d73:  48 C1 FF 02              SAR RDI,0x2
140741d77:  48 3B 4D D8              CMP RCX,qword ptr [RBP + -0x28]
140741d7b:  75 21                    JNZ 0x140741d9e
140741d7d:  BA 01 00 00 00           MOV EDX,0x1
140741d82:  48 8D 4D E0              LEA RCX,[RBP + -0x20]
140741d86:  E8 B5 C6 8D FF           CALL 0x14001e440
140741d8b:  48 8B 45 F0              MOV RAX,qword ptr [RBP + -0x10]
140741d8f:  48 89 45 D8              MOV qword ptr [RBP + -0x28],RAX
140741d93:  48 8B 4D E8              MOV RCX,qword ptr [RBP + -0x18]
140741d97:  48 8B 45 E0              MOV RAX,qword ptr [RBP + -0x20]
140741d9b:  45 33 DB                 XOR R11D,R11D
140741d9e:  48 85 C9                 TEST RCX,RCX
140741da1:  74 32                    JZ 0x140741dd5
140741da3:  8B 04 B8                 MOV EAX,dword ptr [RAX + RDI*0x4]
140741da6:  EB 2B                    JMP 0x140741dd3
140741da8:  48 3B 4D D8              CMP RCX,qword ptr [RBP + -0x28]
140741dac:  75 1D                    JNZ 0x140741dcb
140741dae:  BA 01 00 00 00           MOV EDX,0x1
140741db3:  48 8D 4D E0              LEA RCX,[RBP + -0x20]
140741db7:  E8 84 C6 8D FF           CALL 0x14001e440
140741dbc:  48 8B 45 F0              MOV RAX,qword ptr [RBP + -0x10]
140741dc0:  48 89 45 D8              MOV qword ptr [RBP + -0x28],RAX
140741dc4:  48 8B 4D E8              MOV RCX,qword ptr [RBP + -0x18]
140741dc8:  45 33 DB                 XOR R11D,R11D
140741dcb:  48 85 C9                 TEST RCX,RCX
140741dce:  74 05                    JZ 0x140741dd5
140741dd0:  8B 45 88                 MOV EAX,dword ptr [RBP + -0x78]
140741dd3:  89 01                    MOV dword ptr [RCX],EAX
140741dd5:  48 83 C1 04              ADD RCX,0x4
140741dd9:  48 89 4D E8              MOV qword ptr [RBP + -0x18],RCX
140741ddd:  48 89 4D B8              MOV qword ptr [RBP + -0x48],RCX
140741de1:  C6 45 8D 00              MOV byte ptr [RBP + -0x73],0x0
140741de5:  48 8B 4D 70              MOV RCX,qword ptr [RBP + 0x70]
140741de9:  48 89 4D 30              MOV qword ptr [RBP + 0x30],RCX
140741ded:  4C 89 5D 38              MOV qword ptr [RBP + 0x38],R11
140741df1:  4D 85 FF                 TEST R15,R15
140741df4:  74 3D                    JZ 0x140741e33
140741df6:  79 24                    JNS 0x140741e1c
140741df8:  49 8B C7                 MOV RAX,R15
140741dfb:  48 F7 D8                 NEG RAX
140741dfe:  74 1C                    JZ 0x140741e1c
140741e00:  48 83 C8 FF              OR RAX,-0x1
140741e04:  49 2B C7                 SUB RAX,R15
140741e07:  48 C1 E8 05              SHR RAX,0x5
140741e0b:  48 8D 04 85 04 00 00 00  LEA RAX,[0x4 + RAX*0x4]
140741e13:  48 2B C8                 SUB RCX,RAX
140741e16:  48 89 4D 30              MOV qword ptr [RBP + 0x30],RCX
140741e1a:  EB 0F                    JMP 0x140741e2b
140741e1c:  49 8B C7                 MOV RAX,R15
140741e1f:  48 C1 E8 05              SHR RAX,0x5
140741e23:  48 8D 04 81              LEA RAX,[RCX + RAX*0x4]
140741e27:  48 89 45 30              MOV qword ptr [RBP + 0x30],RAX
140741e2b:  41 83 E7 1F              AND R15D,0x1f
140741e2f:  4C 89 7D 38              MOV qword ptr [RBP + 0x38],R15
140741e33:  0F 28 45 30              MOVAPS XMM0,xmmword ptr [RBP + 0x30]
140741e37:  48 8D 95 C0 00 00 00     LEA RDX,[RBP + 0xc0]
140741e3e:  48 8D 45 8D              LEA RAX,[RBP + -0x73]
140741e42:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140741e47:  66 0F 7F 85 40 01 00 00  MOVDQA xmmword ptr [RBP + 0x140],XMM0
140741e4f:  41 B9 01 00 00 00        MOV R9D,0x1
140741e55:  4C 8D 85 40 01 00 00     LEA R8,[RBP + 0x140]
140741e5c:  48 8D 4D 70              LEA RCX,[RBP + 0x70]
140741e60:  E8 0B 2E AC FF           CALL 0x140204c70
140741e65:  4C 8B BD 88 00 00 00     MOV R15,qword ptr [RBP + 0x88]
140741e6c:  45 33 DB                 XOR R11D,R11D
140741e6f:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140741e76:  48 8B 0C D8              MOV RCX,qword ptr [RAX + RBX*0x8]
140741e7a:  44 8B 45 90              MOV R8D,dword ptr [RBP + -0x70]
140741e7e:  45 0F B6 C0              MOVZX R8D,R8B
140741e82:  80 39 00                 CMP byte ptr [RCX],0x0
140741e85:  B8 01 00 00 00           MOV EAX,0x1
140741e8a:  44 0F 44 C0              CMOVZ R8D,EAX
140741e8e:  44 89 45 90              MOV dword ptr [RBP + -0x70],R8D
140741e92:  8B 55 88                 MOV EDX,dword ptr [RBP + -0x78]
140741e95:  FF C2                    INC EDX
140741e97:  89 55 88                 MOV dword ptr [RBP + -0x78],EDX
140741e9a:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140741ea1:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
140741ea8:  48 C1 F8 03              SAR RAX,0x3
140741eac:  48 63 DA                 MOVSXD RBX,EDX
140741eaf:  48 3B D8                 CMP RBX,RAX
140741eb2:  8B 7D B0                 MOV EDI,dword ptr [RBP + -0x50]
140741eb5:  0F 82 E5 F6 FF FF        JC 0x1407415a0
140741ebb:  F3 0F 10 35 09 8B 1C 00  MOVSS XMM6,dword ptr [0x14090a9cc]
140741ec3:  8B 4D C0                 MOV ECX,dword ptr [RBP + -0x40]
140741ec6:  45 32 FF                 XOR R15B,R15B
140741ec9:  40 84 FF                 TEST DIL,DIL
140741ecc:  48 8D BE 70 0C 00 00     LEA RDI,[RSI + 0xc70]
140741ed3:  74 67                    JZ 0x140741f3c
140741ed5:  84 C9                    TEST CL,CL
140741ed7:  74 63                    JZ 0x140741f3c
140741ed9:  41 B7 01                 MOV R15B,0x1
140741edc:  45 8B C3                 MOV R8D,R11D
140741edf:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140741ee3:  48 2B 07                 SUB RAX,qword ptr [RDI]
140741ee6:  48 C1 F8 03              SAR RAX,0x3
140741eea:  48 85 C0                 TEST RAX,RAX
140741eed:  74 4D                    JZ 0x140741f3c
140741eef:  49 8B D3                 MOV RDX,R11
140741ef2:  0F 1F 40 00              NOP dword ptr [RAX]
140741ef6:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140741f00:  48 8B 07                 MOV RAX,qword ptr [RDI]
140741f03:  48 8B 0C D0              MOV RCX,qword ptr [RAX + RDX*0x8]
140741f07:  48 83 B9 08 17 00 00 00  CMP qword ptr [RCX + 0x1708],0x0
140741f0f:  74 10                    JZ 0x140741f21
140741f11:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
140741f18:  83 B8 00 86 00 00 02     CMP dword ptr [RAX + 0x8600],0x2
140741f1f:  74 18                    JZ 0x140741f39
140741f21:  41 FF C0                 INC R8D
140741f24:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140741f28:  48 2B 07                 SUB RAX,qword ptr [RDI]
140741f2b:  48 C1 F8 03              SAR RAX,0x3
140741f2f:  41 8B D0                 MOV EDX,R8D
140741f32:  48 3B D0                 CMP RDX,RAX
140741f35:  73 05                    JNC 0x140741f3c
140741f37:  EB C7                    JMP 0x140741f00
140741f39:  45 32 FF                 XOR R15B,R15B
140741f3c:  45 84 E4                 TEST R12B,R12B
140741f3f:  74 58                    JZ 0x140741f99
140741f41:  45 84 ED                 TEST R13B,R13B
140741f44:  74 53                    JZ 0x140741f99
140741f46:  41 B4 01                 MOV R12B,0x1
140741f49:  45 8B C3                 MOV R8D,R11D
140741f4c:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140741f50:  48 2B 07                 SUB RAX,qword ptr [RDI]
140741f53:  48 C1 F8 03              SAR RAX,0x3
140741f57:  48 85 C0                 TEST RAX,RAX
140741f5a:  74 40                    JZ 0x140741f9c
140741f5c:  49 8B D3                 MOV RDX,R11
140741f5f:  90                       NOP
140741f60:  48 8B 07                 MOV RAX,qword ptr [RDI]
140741f63:  48 8B 0C D0              MOV RCX,qword ptr [RAX + RDX*0x8]
140741f67:  48 83 B9 08 17 00 00 00  CMP qword ptr [RCX + 0x1708],0x0
140741f6f:  74 10                    JZ 0x140741f81
140741f71:  48 8B 81 08 17 00 00     MOV RAX,qword ptr [RCX + 0x1708]
140741f78:  83 B8 00 86 00 00 01     CMP dword ptr [RAX + 0x8600],0x1
140741f7f:  74 18                    JZ 0x140741f99
140741f81:  41 FF C0                 INC R8D
140741f84:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140741f88:  48 2B 07                 SUB RAX,qword ptr [RDI]
140741f8b:  48 C1 F8 03              SAR RAX,0x3
140741f8f:  41 8B D0                 MOV EDX,R8D
140741f92:  48 3B D0                 CMP RDX,RAX
140741f95:  73 05                    JNC 0x140741f9c
140741f97:  EB C7                    JMP 0x140741f60
140741f99:  45 32 E4                 XOR R12B,R12B
140741f9c:  F3 0F 10 5D 80           MOVSS XMM3,dword ptr [RBP + -0x80]
140741fa1:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
140741fa6:  48 8B D6                 MOV RDX,RSI
140741fa9:  49 8B CE                 MOV RCX,R14
140741fac:  E8 FF 16 FB FF           CALL 0x1406f36b0
140741fb1:  F3 0F 10 05 CF 00 25 00  MOVSS XMM0,dword ptr [0x140992088]
140741fb9:  F3 0F 59 05 D7 8B 1C 00  MULSS XMM0,dword ptr [0x14090ab98]
140741fc1:  F3 41 0F 58 C1           ADDSS XMM0,XMM9
140741fc6:  F3 0F 11 45 84           MOVSS dword ptr [RBP + -0x7c],XMM0
140741fcb:  33 C0                    XOR EAX,EAX
140741fcd:  66 89 85 60 01 00 00     MOV word ptr [RBP + 0x160],AX
140741fd4:  32 DB                    XOR BL,BL
140741fd6:  48 8B D6                 MOV RDX,RSI
140741fd9:  49 8B CE                 MOV RCX,R14
140741fdc:  E8 FF 9F A7 FF           CALL 0x1401bbfe0
140741fe1:  84 C0                    TEST AL,AL
140741fe3:  0F 84 95 00 00 00        JZ 0x14074207e
140741fe9:  B3 01                    MOV BL,0x1
140741feb:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140741ff2:  48 83 E9 02              SUB RCX,0x2
140741ff6:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140741ffa:  66 83 39 00              CMP word ptr [RCX],0x0
140741ffe:  75 F6                    JNZ 0x140741ff6
140742000:  8B 05 6E A1 1B 00        MOV EAX,dword ptr [0x1408fc174]
140742006:  89 01                    MOV dword ptr [RCX],EAX
140742008:  0F B7 05 69 A1 1B 00     MOVZX EAX,word ptr [0x1408fc178]
14074200f:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
140742013:  BA 5A 09 00 00           MOV EDX,0x95a
140742018:  48 8D 0D 71 55 25 00     LEA RCX,[0x140997590]
14074201f:  FF 15 6B A8 12 00        CALL qword ptr [0x14086c890]
140742025:  4C 8B C0                 MOV R8,RAX
140742028:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
14074202f:  48 83 E9 02              SUB RCX,0x2
140742033:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742037:  66 83 39 00              CMP word ptr [RCX],0x0
14074203b:  75 F6                    JNZ 0x140742033
14074203d:  33 D2                    XOR EDX,EDX
14074203f:  90                       NOP
140742040:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742045:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742049:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074204d:  66 85 C0                 TEST AX,AX
140742050:  75 EE                    JNZ 0x140742040
140742052:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742059:  48 83 E9 02              SUB RCX,0x2
14074205d:  0F 1F 00                 NOP dword ptr [RAX]
140742060:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742064:  66 83 39 00              CMP word ptr [RCX],0x0
140742068:  75 F6                    JNZ 0x140742060
14074206a:  8B 05 0C A1 1B 00        MOV EAX,dword ptr [0x1408fc17c]
140742070:  89 01                    MOV dword ptr [RCX],EAX
140742072:  0F B7 05 07 A1 1B 00     MOVZX EAX,word ptr [0x1408fc180]
140742079:  E9 9B 00 00 00           JMP 0x140742119
14074207e:  E8 CD 9C A7 FF           CALL 0x1401bbd50
140742083:  84 C0                    TEST AL,AL
140742085:  0F 84 92 00 00 00        JZ 0x14074211d
14074208b:  B3 01                    MOV BL,0x1
14074208d:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742094:  48 83 E9 02              SUB RCX,0x2
140742098:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
1407420a0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407420a4:  66 83 39 00              CMP word ptr [RCX],0x0
1407420a8:  75 F6                    JNZ 0x1407420a0
1407420aa:  8B 05 A4 42 14 00        MOV EAX,dword ptr [0x140886354]
1407420b0:  89 01                    MOV dword ptr [RCX],EAX
1407420b2:  BA 54 09 00 00           MOV EDX,0x954
1407420b7:  48 8D 0D D2 54 25 00     LEA RCX,[0x140997590]
1407420be:  FF 15 CC A7 12 00        CALL qword ptr [0x14086c890]
1407420c4:  4C 8B C0                 MOV R8,RAX
1407420c7:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407420ce:  48 83 E9 02              SUB RCX,0x2
1407420d2:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407420d6:  66 83 39 00              CMP word ptr [RCX],0x0
1407420da:  75 F6                    JNZ 0x1407420d2
1407420dc:  33 D2                    XOR EDX,EDX
1407420de:  66 90                    NOP
1407420e0:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
1407420e5:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
1407420e9:  48 8D 52 01              LEA RDX,[RDX + 0x1]
1407420ed:  66 85 C0                 TEST AX,AX
1407420f0:  75 EE                    JNZ 0x1407420e0
1407420f2:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407420f9:  48 83 E9 02              SUB RCX,0x2
1407420fd:  0F 1F 00                 NOP dword ptr [RAX]
140742100:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742104:  66 83 39 00              CMP word ptr [RCX],0x0
140742108:  75 F6                    JNZ 0x140742100
14074210a:  8B 05 44 A0 1B 00        MOV EAX,dword ptr [0x1408fc154]
140742110:  89 01                    MOV dword ptr [RCX],EAX
140742112:  0F B7 05 3F A0 1B 00     MOVZX EAX,word ptr [0x1408fc158]
140742119:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
14074211d:  4C 8D 85 60 01 00 00     LEA R8,[RBP + 0x160]
140742124:  48 8B D6                 MOV RDX,RSI
140742127:  49 8B CE                 MOV RCX,R14
14074212a:  E8 A1 87 02 00           CALL 0x14076a8d0
14074212f:  0F B6 DB                 MOVZX EBX,BL
140742132:  84 C0                    TEST AL,AL
140742134:  B8 01 00 00 00           MOV EAX,0x1
140742139:  0F 45 D8                 CMOVNZ EBX,EAX
14074213c:  48 8B D6                 MOV RDX,RSI
14074213f:  49 8B CE                 MOV RCX,R14
140742142:  E8 29 9D A7 FF           CALL 0x1401bbe70
140742147:  84 C0                    TEST AL,AL
140742149:  75 0D                    JNZ 0x140742158
14074214b:  41 38 86 D8 CB 00 00     CMP byte ptr [R14 + 0xcbd8],AL
140742152:  0F 84 95 00 00 00        JZ 0x1407421ed
140742158:  B3 01                    MOV BL,0x1
14074215a:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742161:  48 83 E9 02              SUB RCX,0x2
140742165:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742169:  66 83 39 00              CMP word ptr [RCX],0x0
14074216d:  75 F6                    JNZ 0x140742165
14074216f:  8B 05 E7 9F 1B 00        MOV EAX,dword ptr [0x1408fc15c]
140742175:  89 01                    MOV dword ptr [RCX],EAX
140742177:  0F B7 05 E2 9F 1B 00     MOVZX EAX,word ptr [0x1408fc160]
14074217e:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
140742182:  BA 97 05 00 00           MOV EDX,0x597
140742187:  48 8D 0D 02 54 25 00     LEA RCX,[0x140997590]
14074218e:  FF 15 FC A6 12 00        CALL qword ptr [0x14086c890]
140742194:  4C 8B C0                 MOV R8,RAX
140742197:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
14074219e:  48 83 E9 02              SUB RCX,0x2
1407421a2:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407421a6:  66 83 39 00              CMP word ptr [RCX],0x0
1407421aa:  75 F6                    JNZ 0x1407421a2
1407421ac:  33 D2                    XOR EDX,EDX
1407421ae:  66 90                    NOP
1407421b0:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
1407421b5:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
1407421b9:  48 8D 52 01              LEA RDX,[RDX + 0x1]
1407421bd:  66 85 C0                 TEST AX,AX
1407421c0:  75 EE                    JNZ 0x1407421b0
1407421c2:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407421c9:  48 83 E9 02              SUB RCX,0x2
1407421cd:  0F 1F 00                 NOP dword ptr [RAX]
1407421d0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407421d4:  66 83 39 00              CMP word ptr [RCX],0x0
1407421d8:  75 F6                    JNZ 0x1407421d0
1407421da:  8B 05 84 9F 1B 00        MOV EAX,dword ptr [0x1408fc164]
1407421e0:  89 01                    MOV dword ptr [RCX],EAX
1407421e2:  0F B7 05 7F 9F 1B 00     MOVZX EAX,word ptr [0x1408fc168]
1407421e9:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407421ed:  45 84 FF                 TEST R15B,R15B
1407421f0:  75 0D                    JNZ 0x1407421ff
1407421f2:  45 38 BE D8 CB 00 00     CMP byte ptr [R14 + 0xcbd8],R15B
1407421f9:  0F 84 9E 00 00 00        JZ 0x14074229d
1407421ff:  B3 01                    MOV BL,0x1
140742201:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742208:  48 83 E9 02              SUB RCX,0x2
14074220c:  0F 1F 40 00              NOP dword ptr [RAX]
140742210:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742214:  66 83 39 00              CMP word ptr [RCX],0x0
140742218:  75 F6                    JNZ 0x140742210
14074221a:  8B 05 4C 9F 1B 00        MOV EAX,dword ptr [0x1408fc16c]
140742220:  89 01                    MOV dword ptr [RCX],EAX
140742222:  0F B7 05 47 9F 1B 00     MOVZX EAX,word ptr [0x1408fc170]
140742229:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
14074222d:  BA 0D 11 00 00           MOV EDX,0x110d
140742232:  48 8D 0D 57 53 25 00     LEA RCX,[0x140997590]
140742239:  FF 15 51 A6 12 00        CALL qword ptr [0x14086c890]
14074223f:  4C 8B C0                 MOV R8,RAX
140742242:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742249:  48 83 E9 02              SUB RCX,0x2
14074224d:  0F 1F 00                 NOP dword ptr [RAX]
140742250:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742254:  66 83 39 00              CMP word ptr [RCX],0x0
140742258:  75 F6                    JNZ 0x140742250
14074225a:  33 D2                    XOR EDX,EDX
14074225c:  0F 1F 40 00              NOP dword ptr [RAX]
140742260:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742265:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742269:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074226d:  66 85 C0                 TEST AX,AX
140742270:  75 EE                    JNZ 0x140742260
140742272:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742279:  48 83 E9 02              SUB RCX,0x2
14074227d:  0F 1F 00                 NOP dword ptr [RAX]
140742280:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742284:  66 83 39 00              CMP word ptr [RCX],0x0
140742288:  75 F6                    JNZ 0x140742280
14074228a:  8B 05 D0 9D 1B 00        MOV EAX,dword ptr [0x1408fc060]
140742290:  89 01                    MOV dword ptr [RCX],EAX
140742292:  0F B7 05 CB 9D 1B 00     MOVZX EAX,word ptr [0x1408fc064]
140742299:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
14074229d:  45 84 E4                 TEST R12B,R12B
1407422a0:  75 0D                    JNZ 0x1407422af
1407422a2:  45 38 A6 D8 CB 00 00     CMP byte ptr [R14 + 0xcbd8],R12B
1407422a9:  0F 84 9E 00 00 00        JZ 0x14074234d
1407422af:  B3 01                    MOV BL,0x1
1407422b1:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407422b8:  48 83 E9 02              SUB RCX,0x2
1407422bc:  0F 1F 40 00              NOP dword ptr [RAX]
1407422c0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407422c4:  66 83 39 00              CMP word ptr [RCX],0x0
1407422c8:  75 F6                    JNZ 0x1407422c0
1407422ca:  8B 05 98 9D 1B 00        MOV EAX,dword ptr [0x1408fc068]
1407422d0:  89 01                    MOV dword ptr [RCX],EAX
1407422d2:  0F B7 05 93 9D 1B 00     MOVZX EAX,word ptr [0x1408fc06c]
1407422d9:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407422dd:  BA 0E 11 00 00           MOV EDX,0x110e
1407422e2:  48 8D 0D A7 52 25 00     LEA RCX,[0x140997590]
1407422e9:  FF 15 A1 A5 12 00        CALL qword ptr [0x14086c890]
1407422ef:  4C 8B C0                 MOV R8,RAX
1407422f2:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407422f9:  48 83 E9 02              SUB RCX,0x2
1407422fd:  0F 1F 00                 NOP dword ptr [RAX]
140742300:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742304:  66 83 39 00              CMP word ptr [RCX],0x0
140742308:  75 F6                    JNZ 0x140742300
14074230a:  33 D2                    XOR EDX,EDX
14074230c:  0F 1F 40 00              NOP dword ptr [RAX]
140742310:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742315:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742319:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074231d:  66 85 C0                 TEST AX,AX
140742320:  75 EE                    JNZ 0x140742310
140742322:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742329:  48 83 E9 02              SUB RCX,0x2
14074232d:  0F 1F 00                 NOP dword ptr [RAX]
140742330:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742334:  66 83 39 00              CMP word ptr [RCX],0x0
140742338:  75 F6                    JNZ 0x140742330
14074233a:  8B 05 30 9D 1B 00        MOV EAX,dword ptr [0x1408fc070]
140742340:  89 01                    MOV dword ptr [RCX],EAX
140742342:  0F B7 05 2B 9D 1B 00     MOVZX EAX,word ptr [0x1408fc074]
140742349:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
14074234d:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140742351:  48 2B 07                 SUB RAX,qword ptr [RDI]
140742354:  48 83 F8 08              CMP RAX,0x8
140742358:  0F 82 C0 00 00 00        JC 0x14074241e
14074235e:  41 80 BE D8 CB 00 00 00  CMP byte ptr [R14 + 0xcbd8],0x0
140742366:  0F 85 B2 00 00 00        JNZ 0x14074241e
14074236c:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140742373:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
14074237a:  48 83 F8 08              CMP RAX,0x8
14074237e:  0F 83 39 01 00 00        JNC 0x1407424bd
140742384:  B3 01                    MOV BL,0x1
140742386:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
14074238d:  48 83 E9 02              SUB RCX,0x2
140742391:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742395:  66 83 39 00              CMP word ptr [RCX],0x0
140742399:  75 F6                    JNZ 0x140742391
14074239b:  8B 05 A7 9C 1B 00        MOV EAX,dword ptr [0x1408fc048]
1407423a1:  89 01                    MOV dword ptr [RCX],EAX
1407423a3:  0F B7 05 A2 9C 1B 00     MOVZX EAX,word ptr [0x1408fc04c]
1407423aa:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407423ae:  BA 9A 05 00 00           MOV EDX,0x59a
1407423b3:  48 8D 0D D6 51 25 00     LEA RCX,[0x140997590]
1407423ba:  FF 15 D0 A4 12 00        CALL qword ptr [0x14086c890]
1407423c0:  4C 8B C0                 MOV R8,RAX
1407423c3:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407423ca:  48 83 E9 02              SUB RCX,0x2
1407423ce:  66 90                    NOP
1407423d0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407423d4:  66 83 39 00              CMP word ptr [RCX],0x0
1407423d8:  75 F6                    JNZ 0x1407423d0
1407423da:  33 D2                    XOR EDX,EDX
1407423dc:  0F 1F 40 00              NOP dword ptr [RAX]
1407423e0:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
1407423e5:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
1407423e9:  48 8D 52 01              LEA RDX,[RDX + 0x1]
1407423ed:  66 85 C0                 TEST AX,AX
1407423f0:  75 EE                    JNZ 0x1407423e0
1407423f2:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407423f9:  48 83 E9 02              SUB RCX,0x2
1407423fd:  0F 1F 00                 NOP dword ptr [RAX]
140742400:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742404:  66 83 39 00              CMP word ptr [RCX],0x0
140742408:  75 F6                    JNZ 0x140742400
14074240a:  8B 05 40 9C 1B 00        MOV EAX,dword ptr [0x1408fc050]
140742410:  89 01                    MOV dword ptr [RCX],EAX
140742412:  0F B7 05 3B 9C 1B 00     MOVZX EAX,word ptr [0x1408fc054]
140742419:  E9 9B 00 00 00           JMP 0x1407424b9
14074241e:  B3 01                    MOV BL,0x1
140742420:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742427:  48 83 E9 02              SUB RCX,0x2
14074242b:  0F 1F 44 00 00           NOP dword ptr [RAX + RAX*0x1]
140742430:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742434:  66 83 39 00              CMP word ptr [RCX],0x0
140742438:  75 F6                    JNZ 0x140742430
14074243a:  8B 05 38 9C 1B 00        MOV EAX,dword ptr [0x1408fc078]
140742440:  89 01                    MOV dword ptr [RCX],EAX
140742442:  0F B7 05 33 9C 1B 00     MOVZX EAX,word ptr [0x1408fc07c]
140742449:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
14074244d:  BA B0 05 00 00           MOV EDX,0x5b0
140742452:  48 8D 0D 37 51 25 00     LEA RCX,[0x140997590]
140742459:  FF 15 31 A4 12 00        CALL qword ptr [0x14086c890]
14074245f:  4C 8B C0                 MOV R8,RAX
140742462:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742469:  48 83 E9 02              SUB RCX,0x2
14074246d:  0F 1F 00                 NOP dword ptr [RAX]
140742470:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742474:  66 83 39 00              CMP word ptr [RCX],0x0
140742478:  75 F6                    JNZ 0x140742470
14074247a:  33 D2                    XOR EDX,EDX
14074247c:  0F 1F 40 00              NOP dword ptr [RAX]
140742480:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742485:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742489:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074248d:  66 85 C0                 TEST AX,AX
140742490:  75 EE                    JNZ 0x140742480
140742492:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742499:  48 83 E9 02              SUB RCX,0x2
14074249d:  0F 1F 00                 NOP dword ptr [RAX]
1407424a0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407424a4:  66 83 39 00              CMP word ptr [RCX],0x0
1407424a8:  75 F6                    JNZ 0x1407424a0
1407424aa:  8B 05 90 9B 1B 00        MOV EAX,dword ptr [0x1408fc040]
1407424b0:  89 01                    MOV dword ptr [RCX],EAX
1407424b2:  0F B7 05 8B 9B 1B 00     MOVZX EAX,word ptr [0x1408fc044]
1407424b9:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407424bd:  80 7D 90 00              CMP byte ptr [RBP + -0x70],0x0
1407424c1:  0F 84 97 00 00 00        JZ 0x14074255e
1407424c7:  B3 01                    MOV BL,0x1
1407424c9:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407424d0:  48 83 E9 02              SUB RCX,0x2
1407424d4:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407424d8:  66 83 39 00              CMP word ptr [RCX],0x0
1407424dc:  75 F6                    JNZ 0x1407424d4
1407424de:  8B 05 74 9B 1B 00        MOV EAX,dword ptr [0x1408fc058]
1407424e4:  89 01                    MOV dword ptr [RCX],EAX
1407424e6:  0F B7 05 6F 9B 1B 00     MOVZX EAX,word ptr [0x1408fc05c]
1407424ed:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407424f1:  BA B6 05 00 00           MOV EDX,0x5b6
1407424f6:  48 8D 0D 93 50 25 00     LEA RCX,[0x140997590]
1407424fd:  FF 15 8D A3 12 00        CALL qword ptr [0x14086c890]
140742503:  4C 8B C0                 MOV R8,RAX
140742506:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
14074250d:  48 83 E9 02              SUB RCX,0x2
140742511:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742515:  66 83 39 00              CMP word ptr [RCX],0x0
140742519:  75 F6                    JNZ 0x140742511
14074251b:  33 D2                    XOR EDX,EDX
14074251d:  0F 1F 00                 NOP dword ptr [RAX]
140742520:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742525:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742529:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074252d:  66 85 C0                 TEST AX,AX
140742530:  75 EE                    JNZ 0x140742520
140742532:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742539:  48 83 E9 02              SUB RCX,0x2
14074253d:  0F 1F 00                 NOP dword ptr [RAX]
140742540:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742544:  66 83 39 00              CMP word ptr [RCX],0x0
140742548:  75 F6                    JNZ 0x140742540
14074254a:  8B 05 60 9B 1B 00        MOV EAX,dword ptr [0x1408fc0b0]
140742550:  89 01                    MOV dword ptr [RCX],EAX
140742552:  0F B7 05 5B 9B 1B 00     MOVZX EAX,word ptr [0x1408fc0b4]
140742559:  E9 9B 00 00 00           JMP 0x1407425f9
14074255e:  80 7D 8C 00              CMP byte ptr [RBP + -0x74],0x0
140742562:  0F 84 95 00 00 00        JZ 0x1407425fd
140742568:  B3 01                    MOV BL,0x1
14074256a:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742571:  48 83 E9 02              SUB RCX,0x2
140742575:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742579:  66 83 39 00              CMP word ptr [RCX],0x0
14074257d:  75 F6                    JNZ 0x140742575
14074257f:  8B 05 33 9B 1B 00        MOV EAX,dword ptr [0x1408fc0b8]
140742585:  89 01                    MOV dword ptr [RCX],EAX
140742587:  0F B7 05 2E 9B 1B 00     MOVZX EAX,word ptr [0x1408fc0bc]
14074258e:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
140742592:  BA E2 0F 00 00           MOV EDX,0xfe2
140742597:  48 8D 0D F2 4F 25 00     LEA RCX,[0x140997590]
14074259e:  FF 15 EC A2 12 00        CALL qword ptr [0x14086c890]
1407425a4:  4C 8B C0                 MOV R8,RAX
1407425a7:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407425ae:  48 83 E9 02              SUB RCX,0x2
1407425b2:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407425b6:  66 83 39 00              CMP word ptr [RCX],0x0
1407425ba:  75 F6                    JNZ 0x1407425b2
1407425bc:  33 D2                    XOR EDX,EDX
1407425be:  66 90                    NOP
1407425c0:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
1407425c5:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
1407425c9:  48 8D 52 01              LEA RDX,[RDX + 0x1]
1407425cd:  66 85 C0                 TEST AX,AX
1407425d0:  75 EE                    JNZ 0x1407425c0
1407425d2:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
1407425d9:  48 83 E9 02              SUB RCX,0x2
1407425dd:  0F 1F 00                 NOP dword ptr [RAX]
1407425e0:  48 8D 49 02              LEA RCX,[RCX + 0x2]
1407425e4:  66 83 39 00              CMP word ptr [RCX],0x0
1407425e8:  75 F6                    JNZ 0x1407425e0
1407425ea:  8B 05 D0 9A 1B 00        MOV EAX,dword ptr [0x1408fc0c0]
1407425f0:  89 01                    MOV dword ptr [RCX],EAX
1407425f2:  0F B7 05 CB 9A 1B 00     MOVZX EAX,word ptr [0x1408fc0c4]
1407425f9:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407425fd:  4C 8B 65 B8              MOV R12,qword ptr [RBP + -0x48]
140742601:  4C 8B 6D E0              MOV R13,qword ptr [RBP + -0x20]
140742605:  4D 2B E5                 SUB R12,R13
140742608:  49 C1 FC 02              SAR R12,0x2
14074260c:  4C 89 65 B8              MOV qword ptr [RBP + -0x48],R12
140742610:  4D 85 E4                 TEST R12,R12
140742613:  0F 84 96 00 00 00        JZ 0x1407426af
140742619:  B3 01                    MOV BL,0x1
14074261b:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742622:  48 83 E9 02              SUB RCX,0x2
140742626:  48 8D 49 02              LEA RCX,[RCX + 0x2]
14074262a:  66 83 39 00              CMP word ptr [RCX],0x0
14074262e:  75 F6                    JNZ 0x140742626
140742630:  8B 05 92 9A 1B 00        MOV EAX,dword ptr [0x1408fc0c8]
140742636:  89 01                    MOV dword ptr [RCX],EAX
140742638:  0F B7 05 8D 9A 1B 00     MOVZX EAX,word ptr [0x1408fc0cc]
14074263f:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
140742643:  BA B3 05 00 00           MOV EDX,0x5b3
140742648:  48 8D 0D 41 4F 25 00     LEA RCX,[0x140997590]
14074264f:  FF 15 3B A2 12 00        CALL qword ptr [0x14086c890]
140742655:  4C 8B C0                 MOV R8,RAX
140742658:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
14074265f:  48 83 E9 02              SUB RCX,0x2
140742663:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742667:  66 83 39 00              CMP word ptr [RCX],0x0
14074266b:  75 F6                    JNZ 0x140742663
14074266d:  33 D2                    XOR EDX,EDX
14074266f:  90                       NOP
140742670:  41 0F B7 04 50           MOVZX EAX,word ptr [R8 + RDX*0x2]
140742675:  66 89 04 51              MOV word ptr [RCX + RDX*0x2],AX
140742679:  48 8D 52 01              LEA RDX,[RDX + 0x1]
14074267d:  66 85 C0                 TEST AX,AX
140742680:  75 EE                    JNZ 0x140742670
140742682:  48 8D 8D 60 01 00 00     LEA RCX,[RBP + 0x160]
140742689:  48 83 E9 02              SUB RCX,0x2
14074268d:  0F 1F 00                 NOP dword ptr [RAX]
140742690:  48 8D 49 02              LEA RCX,[RCX + 0x2]
140742694:  66 83 39 00              CMP word ptr [RCX],0x0
140742698:  75 F6                    JNZ 0x140742690
14074269a:  8B 05 E0 99 1B 00        MOV EAX,dword ptr [0x1408fc080]
1407426a0:  89 01                    MOV dword ptr [RCX],EAX
1407426a2:  0F B7 05 DB 99 1B 00     MOVZX EAX,word ptr [0x1408fc084]
1407426a9:  66 89 41 04              MOV word ptr [RCX + 0x4],AX
1407426ad:  EB 04                    JMP 0x1407426b3
1407426af:  84 DB                    TEST BL,BL
1407426b1:  74 09                    JZ 0x1407426bc
1407426b3:  49 8B 8E 00 B5 00 00     MOV RCX,qword ptr [R14 + 0xb500]
1407426ba:  EB 07                    JMP 0x1407426c3
1407426bc:  49 8B 8E 18 B5 00 00     MOV RCX,qword ptr [R14 + 0xb518]
1407426c3:  48 8B 01                 MOV RAX,qword ptr [RCX]
1407426c6:  4C 8B 05 3B 86 2A 00     MOV R8,qword ptr [0x1409ead08]
1407426cd:  33 D2                    XOR EDX,EDX
1407426cf:  FF 50 70                 CALL qword ptr [RAX + 0x70]
1407426d2:  0F 28 05 E7 9D 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1407426d9:  0F 11 05 2C BC 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407426e0:  F3 0F 10 1D A0 F9 24 00  MOVSS XMM3,dword ptr [0x140992088]
1407426e8:  0F 28 CB                 MOVAPS XMM1,XMM3
1407426eb:  F3 0F 59 CE              MULSS XMM1,XMM6
1407426ef:  41 0F 28 D0              MOVAPS XMM2,XMM8
1407426f3:  F3 0F 5C D1              SUBSS XMM2,XMM1
1407426f7:  0F 28 C3                 MOVAPS XMM0,XMM3
1407426fa:  F3 0F 59 05 8E 83 1C 00  MULSS XMM0,dword ptr [0x14090aa90]
140742702:  F3 41 0F 58 C1           ADDSS XMM0,XMM9
140742707:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
14074270e:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140742711:  F2 0F 11 05 D7 BB 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140742719:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
14074271c:  89 05 D6 BB 27 00        MOV dword ptr [0x1409be2f8],EAX
140742722:  F3 0F 59 1D 8E 83 1C 00  MULSS XMM3,dword ptr [0x14090aab8]
14074272a:  F3 0F 11 1D B6 BB 27 00  MOVSS dword ptr [0x1409be2e8],XMM3
140742732:  F3 0F 11 1D B2 BB 27 00  MOVSS dword ptr [0x1409be2ec],XMM3
14074273a:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14074273f:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140742746:  F3 44 0F 10 1D 09 78 1C 00 MOVSS XMM11,dword ptr [0x140909f58]
14074274f:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140742756:  41 0F 28 DB              MOVAPS XMM3,XMM11
14074275a:  41 0F 28 D4              MOVAPS XMM2,XMM12
14074275e:  41 0F 28 CC              MOVAPS XMM1,XMM12
140742762:  48 8D 0D F7 B8 27 00     LEA RCX,[0x1409be060]
140742769:  FF 15 F1 A0 12 00        CALL qword ptr [0x14086c860]
14074276f:  41 0F 28 F4              MOVAPS XMM6,XMM12
140742773:  F3 0F 10 3D 51 81 1C 00  MOVSS XMM7,dword ptr [0x14090a8cc]
14074277b:  84 DB                    TEST BL,BL
14074277d:  0F 84 C7 00 00 00        JZ 0x14074284a
140742783:  F3 0F 10 05 FD F8 24 00  MOVSS XMM0,dword ptr [0x140992088]
14074278b:  F3 41 0F 59 C4           MULSS XMM0,XMM12
140742790:  48 8B 0D 71 1A 25 00     MOV RCX,qword ptr [0x140994208]
140742797:  48 8B 01                 MOV RAX,qword ptr [RCX]
14074279a:  0F 28 D8                 MOVAPS XMM3,XMM0
14074279d:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
1407427a2:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
1407427a7:  F3 0F 5C D0              SUBSS XMM2,XMM0
1407427ab:  48 8D 95 60 01 00 00     LEA RDX,[RBP + 0x160]
1407427b2:  48 89 54 24 20           MOV qword ptr [RSP + 0x20],RDX
1407427b7:  66 41 0F 7E D9           MOVD R9D,XMM3
1407427bc:  66 41 0F 7E D0           MOVD R8D,XMM2
1407427c1:  48 8D 95 40 01 00 00     LEA RDX,[RBP + 0x140]
1407427c8:  FF 50 28                 CALL qword ptr [RAX + 0x28]
1407427cb:  66 0F 6E 8D 4C 01 00 00  MOVD XMM1,dword ptr [RBP + 0x14c]
1407427d3:  0F 5B C9                 CVTDQ2PS XMM1,XMM1
1407427d6:  F3 0F 10 15 AA F8 24 00  MOVSS XMM2,dword ptr [0x140992088]
1407427de:  0F 28 C2                 MOVAPS XMM0,XMM2
1407427e1:  F3 0F 59 05 3F 80 1C 00  MULSS XMM0,dword ptr [0x14090a828]
1407427e9:  F3 41 0F 58 C0           ADDSS XMM0,XMM8
1407427ee:  0F 2F C8                 COMISS XMM1,XMM0
1407427f1:  76 13                    JBE 0x140742806
1407427f3:  0F 28 F1                 MOVAPS XMM6,XMM1
1407427f6:  F3 41 0F 5C F0           SUBSS XMM6,XMM8
1407427fb:  0F 28 C2                 MOVAPS XMM0,XMM2
1407427fe:  F3 0F 59 C7              MULSS XMM0,XMM7
140742802:  F3 0F 58 F0              ADDSS XMM6,XMM0
140742806:  F3 41 0F 59 D4           MULSS XMM2,XMM12
14074280b:  0F 28 DA                 MOVAPS XMM3,XMM2
14074280e:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
140742813:  F3 0F 58 55 84           ADDSS XMM2,dword ptr [RBP + -0x7c]
140742818:  48 8D 85 60 01 00 00     LEA RAX,[RBP + 0x160]
14074281f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140742824:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
14074282c:  66 41 0F 7E D9           MOVD R9D,XMM3
140742831:  66 41 0F 7E D0           MOVD R8D,XMM2
140742836:  48 8B 15 CB 19 25 00     MOV RDX,qword ptr [0x140994208]
14074283d:  48 8D 0D 6C 47 25 00     LEA RCX,[0x140996fb0]
140742844:  FF 15 36 A0 12 00        CALL qword ptr [0x14086c880]
14074284a:  F3 44 0F 11 4D 84        MOVSS dword ptr [RBP + -0x7c],XMM9
140742850:  F3 41 0F 58 F0           ADDSS XMM6,XMM8
140742855:  F3 0F 11 75 80           MOVSS dword ptr [RBP + -0x80],XMM6
14074285a:  BA 14 02 00 00           MOV EDX,0x214
14074285f:  48 8D 0D 2A 4D 25 00     LEA RCX,[0x140997590]
140742866:  FF 15 24 A0 12 00        CALL qword ptr [0x14086c890]
14074286c:  F3 0F 10 0D 14 F8 24 00  MOVSS XMM1,dword ptr [0x140992088]
140742874:  0F 28 C1                 MOVAPS XMM0,XMM1
140742877:  F3 44 0F 10 2D C4 7B 1C 00 MOVSS XMM13,dword ptr [0x14090a444]
140742880:  F3 41 0F 59 C5           MULSS XMM0,XMM13
140742885:  F3 0F 10 5D 80           MOVSS XMM3,dword ptr [RBP + -0x80]
14074288a:  F3 0F 5C D8              SUBSS XMM3,XMM0
14074288e:  F3 0F 10 35 1A 7F 1C 00  MOVSS XMM6,dword ptr [0x14090a7b0]
140742896:  F3 0F 59 CE              MULSS XMM1,XMM6
14074289a:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
14074289f:  F3 0F 5C D1              SUBSS XMM2,XMM1
1407428a3:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
1407428a8:  48 8D 05 D9 97 1B 00     LEA RAX,[0x1408fc088]
1407428af:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407428b4:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
1407428bc:  66 41 0F 7E D9           MOVD R9D,XMM3
1407428c1:  66 41 0F 7E D0           MOVD R8D,XMM2
1407428c6:  48 8B 15 53 29 25 00     MOV RDX,qword ptr [0x140995220]
1407428cd:  48 8D 0D DC 46 25 00     LEA RCX,[0x140996fb0]
1407428d4:  FF 15 A6 9F 12 00        CALL qword ptr [0x14086c880]
1407428da:  48 8B 05 3F 29 25 00     MOV RAX,qword ptr [0x140995220]
1407428e1:  48 8B 18                 MOV RBX,qword ptr [RAX]
1407428e4:  BA 14 02 00 00           MOV EDX,0x214
1407428e9:  48 8D 0D A0 4C 25 00     LEA RCX,[0x140997590]
1407428f0:  FF 15 9A 9F 12 00        CALL qword ptr [0x14086c890]
1407428f6:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407428fb:  48 8D 05 96 97 1B 00     LEA RAX,[0x1408fc098]
140742902:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140742907:  41 0F 28 DC              MOVAPS XMM3,XMM12
14074290b:  66 41 0F 7E D9           MOVD R9D,XMM3
140742910:  41 0F 28 D4              MOVAPS XMM2,XMM12
140742914:  66 41 0F 7E D0           MOVD R8D,XMM2
140742919:  48 8D 95 30 01 00 00     LEA RDX,[RBP + 0x130]
140742920:  48 8B 0D F9 28 25 00     MOV RCX,qword ptr [0x140995220]
140742927:  FF 53 28                 CALL qword ptr [RBX + 0x28]
14074292a:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
14074292d:  0F 11 85 40 01 00 00     MOVUPS xmmword ptr [RBP + 0x140],XMM0
140742934:  33 DB                    XOR EBX,EBX
140742936:  44 8B C3                 MOV R8D,EBX
140742939:  48 85 F6                 TEST RSI,RSI
14074293c:  74 49                    JZ 0x140742987
14074293e:  8B D3                    MOV EDX,EBX
140742940:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140742944:  48 2B 07                 SUB RAX,qword ptr [RDI]
140742947:  48 C1 F8 03              SAR RAX,0x3
14074294b:  48 85 C0                 TEST RAX,RAX
14074294e:  74 37                    JZ 0x140742987
140742950:  8B CB                    MOV ECX,EBX
140742952:  48 8B 07                 MOV RAX,qword ptr [RDI]
140742955:  48 8B 0C C8              MOV RCX,qword ptr [RAX + RCX*0x8]
140742959:  48 85 C9                 TEST RCX,RCX
14074295c:  74 15                    JZ 0x140742973
14074295e:  48 39 99 98 03 00 00     CMP qword ptr [RCX + 0x398],RBX
140742965:  75 0C                    JNZ 0x140742973
140742967:  48 39 B1 F8 04 00 00     CMP qword ptr [RCX + 0x4f8],RSI
14074296e:  75 03                    JNZ 0x140742973
140742970:  41 FF C0                 INC R8D
140742973:  FF C2                    INC EDX
140742975:  48 8B 47 08              MOV RAX,qword ptr [RDI + 0x8]
140742979:  48 2B 07                 SUB RAX,qword ptr [RDI]
14074297c:  48 C1 F8 03              SAR RAX,0x3
140742980:  8B CA                    MOV ECX,EDX
140742982:  48 3B C8                 CMP RCX,RAX
140742985:  72 CB                    JC 0x140742952
140742987:  F3 0F 10 0D F9 F6 24 00  MOVSS XMM1,dword ptr [0x140992088]
14074298f:  0F 28 C1                 MOVAPS XMM0,XMM1
140742992:  F3 41 0F 59 C5           MULSS XMM0,XMM13
140742997:  F3 0F 10 5D 80           MOVSS XMM3,dword ptr [RBP + -0x80]
14074299c:  F3 0F 5C D8              SUBSS XMM3,XMM0
1407429a0:  F3 0F 59 CE              MULSS XMM1,XMM6
1407429a4:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
1407429a9:  F3 0F 5C D1              SUBSS XMM2,XMM1
1407429ad:  66 0F 6E 85 48 01 00 00  MOVD XMM0,dword ptr [RBP + 0x148]
1407429b5:  0F 5B C0                 CVTDQ2PS XMM0,XMM0
1407429b8:  F3 0F 58 D0              ADDSS XMM2,XMM0
1407429bc:  44 89 44 24 30           MOV dword ptr [RSP + 0x30],R8D
1407429c1:  48 8D 05 E0 96 1B 00     LEA RAX,[0x1408fc0a8]
1407429c8:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
1407429cd:  C7 44 24 20 00 00 00 FF  MOV dword ptr [RSP + 0x20],0xff000000
1407429d5:  66 41 0F 7E D9           MOVD R9D,XMM3
1407429da:  66 41 0F 7E D0           MOVD R8D,XMM2
1407429df:  48 8B 15 3A 28 25 00     MOV RDX,qword ptr [0x140995220]
1407429e6:  48 8D 0D C3 45 25 00     LEA RCX,[0x140996fb0]
1407429ed:  FF 15 8D 9E 12 00        CALL qword ptr [0x14086c880]
1407429f3:  F3 0F 10 15 8D F6 24 00  MOVSS XMM2,dword ptr [0x140992088]
1407429fb:  0F 28 CA                 MOVAPS XMM1,XMM2
1407429fe:  F3 0F 59 CF              MULSS XMM1,XMM7
140742a02:  41 0F 28 C1              MOVAPS XMM0,XMM9
140742a06:  F3 0F 5C C1              SUBSS XMM0,XMM1
140742a0a:  F3 0F 11 45 84           MOVSS dword ptr [RBP + -0x7c],XMM0
140742a0f:  F3 0F 59 15 9D 7F 1C 00  MULSS XMM2,dword ptr [0x14090a9b4]
140742a17:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140742a1c:  F3 0F 58 C2              ADDSS XMM0,XMM2
140742a20:  F3 0F 11 45 80           MOVSS dword ptr [RBP + -0x80],XMM0
140742a25:  88 5C 24 38              MOV byte ptr [RSP + 0x38],BL
140742a29:  88 5C 24 28              MOV byte ptr [RSP + 0x28],BL
140742a2d:  88 5C 24 20              MOV byte ptr [RSP + 0x20],BL
140742a31:  48 8B 7D 98              MOV RDI,qword ptr [RBP + -0x68]
140742a35:  4C 8B CF                 MOV R9,RDI
140742a38:  4C 8D 45 80              LEA R8,[RBP + -0x80]
140742a3c:  48 8D 55 84              LEA RDX,[RBP + -0x7c]
140742a40:  49 8B CE                 MOV RCX,R14
140742a43:  E8 48 1F 06 00           CALL 0x1407a4990
140742a48:  F3 44 0F 11 4D 84        MOVSS dword ptr [RBP + -0x7c],XMM9
140742a4e:  F3 0F 10 05 32 F6 24 00  MOVSS XMM0,dword ptr [0x140992088]
140742a56:  0F 28 D8                 MOVAPS XMM3,XMM0
140742a59:  F3 0F 59 DF              MULSS XMM3,XMM7
140742a5d:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
140742a62:  F3 0F 10 35 36 7F 1C 00  MOVSS XMM6,dword ptr [0x14090a9a0]
140742a6a:  F3 0F 59 C6              MULSS XMM0,XMM6
140742a6e:  41 0F 28 D1              MOVAPS XMM2,XMM9
140742a72:  F3 0F 5C D0              SUBSS XMM2,XMM0
140742a76:  F3 0F 11 74 24 28        MOVSS dword ptr [RSP + 0x28],XMM6
140742a7c:  0F B6 87 0C 03 00 00     MOVZX EAX,byte ptr [RDI + 0x30c]
140742a83:  88 44 24 20              MOV byte ptr [RSP + 0x20],AL
140742a87:  48 8B D7                 MOV RDX,RDI
140742a8a:  49 8B CE                 MOV RCX,R14
140742a8d:  E8 3E 1F FB FF           CALL 0x1406f49d0
140742a92:  0F B6 15 A2 ED 29 00     MOVZX EDX,byte ptr [0x1409e183b]
140742a99:  0F B6 C8                 MOVZX ECX,AL
140742a9c:  38 87 0C 03 00 00        CMP byte ptr [RDI + 0x30c],AL
140742aa2:  0F 45 D1                 CMOVNZ EDX,ECX
140742aa5:  88 15 90 ED 29 00        MOV byte ptr [0x1409e183b],DL
140742aab:  88 87 0C 03 00 00        MOV byte ptr [RDI + 0x30c],AL
140742ab1:  4C 8D 3D 0C 57 2A 00     LEA R15,[0x1409e81c4]
140742ab8:  84 C0                    TEST AL,AL
140742aba:  0F 84 AB 00 00 00        JZ 0x140742b6b
140742ac0:  83 3D C1 55 2A 00 FF     CMP dword ptr [0x1409e8088],-0x1
140742ac7:  0F 84 9E 00 00 00        JZ 0x140742b6b
140742acd:  48 8D 15 BC 97 1B 00     LEA RDX,[0x1408fc290]
140742ad4:  48 8D 0D 7D 57 2A 00     LEA RCX,[0x1409e8258]
140742adb:  E8 40 CA 10 00           CALL 0x14084f520
140742ae0:  85 C0                    TEST EAX,EAX
140742ae2:  74 17                    JZ 0x140742afb
140742ae4:  48 8D 15 BD 97 1B 00     LEA RDX,[0x1408fc2a8]
140742aeb:  48 8D 0D 66 57 2A 00     LEA RCX,[0x1409e8258]
140742af2:  E8 29 CA 10 00           CALL 0x14084f520
140742af7:  85 C0                    TEST EAX,EAX
140742af9:  75 70                    JNZ 0x140742b6b
140742afb:  49 8B C7                 MOV RAX,R15
140742afe:  4C 8B 86 18 03 00 00     MOV R8,qword ptr [RSI + 0x318]
140742b05:  4D 2B C7                 SUB R8,R15
140742b08:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
140742b10:  0F B6 10                 MOVZX EDX,byte ptr [RAX]
140742b13:  42 0F B6 0C 00           MOVZX ECX,byte ptr [RAX + R8*0x1]
140742b18:  2B D1                    SUB EDX,ECX
140742b1a:  75 07                    JNZ 0x140742b23
140742b1c:  48 FF C0                 INC RAX
140742b1f:  85 C9                    TEST ECX,ECX
140742b21:  75 ED                    JNZ 0x140742b10
140742b23:  85 D2                    TEST EDX,EDX
140742b25:  75 44                    JNZ 0x140742b6b
140742b27:  83 3D BE 57 2A 00 02     CMP dword ptr [0x1409e82ec],0x2
140742b2e:  7D 3B                    JGE 0x140742b6b
140742b30:  C7 05 B2 57 2A 00 02 00 00 00 MOV dword ptr [0x1409e82ec],0x2
140742b3a:  F3 0F 10 15 46 F5 24 00  MOVSS XMM2,dword ptr [0x140992088]
140742b42:  0F 28 CA                 MOVAPS XMM1,XMM2
140742b45:  F3 0F 59 CE              MULSS XMM1,XMM6
140742b49:  F3 0F 10 45 84           MOVSS XMM0,dword ptr [RBP + -0x7c]
140742b4e:  F3 0F 5C C1              SUBSS XMM0,XMM1
140742b52:  F3 0F 11 05 9E 57 2A 00  MOVSS dword ptr [0x1409e82f8],XMM0
140742b5a:  F3 0F 59 D7              MULSS XMM2,XMM7
140742b5e:  F3 0F 58 55 80           ADDSS XMM2,dword ptr [RBP + -0x80]
140742b63:  F3 0F 11 15 91 57 2A 00  MOVSS dword ptr [0x1409e82fc],XMM2
140742b6b:  38 9F 0C 03 00 00        CMP byte ptr [RDI + 0x30c],BL
140742b71:  0F 85 9B 22 00 00        JNZ 0x140744e12
140742b77:  0F 57 C0                 XORPS XMM0,XMM0
140742b7a:  F3 0F 7F 45 08           MOVDQU xmmword ptr [RBP + 0x8],XMM0
140742b7f:  48 89 5D 18              MOV qword ptr [RBP + 0x18],RBX
140742b83:  F3 0F 7F 45 40           MOVDQU xmmword ptr [RBP + 0x40],XMM0
140742b88:  4C 8B EB                 MOV R13,RBX
140742b8b:  48 89 5D D0              MOV qword ptr [RBP + -0x30],RBX
140742b8f:  48 89 5D 50              MOV qword ptr [RBP + 0x50],RBX
140742b93:  C6 44 24 20 01           MOV byte ptr [RSP + 0x20],0x1
140742b98:  4C 8D 4D 90              LEA R9,[RBP + -0x70]
140742b9c:  4C 8D 45 C8              LEA R8,[RBP + -0x38]
140742ba0:  48 8B D6                 MOV RDX,RSI
140742ba3:  E8 F8 77 02 00           CALL 0x14076a3a0
140742ba8:  48 8B 9E 40 0D 00 00     MOV RBX,qword ptr [RSI + 0xd40]
140742baf:  48 2B 9E 38 0D 00 00     SUB RBX,qword ptr [RSI + 0xd38]
140742bb6:  48 C1 FB 03              SAR RBX,0x3
140742bba:  BA 14 08 00 00           MOV EDX,0x814
140742bbf:  48 8D 0D CA 49 25 00     LEA RCX,[0x140997590]
140742bc6:  FF 15 C4 9C 12 00        CALL qword ptr [0x14086c890]
140742bcc:  F3 0F 10 05 B4 F4 24 00  MOVSS XMM0,dword ptr [0x140992088]
140742bd4:  0F 28 D8                 MOVAPS XMM3,XMM0
140742bd7:  F3 41 0F 59 DD           MULSS XMM3,XMM13
140742bdc:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
140742be1:  F3 44 0F 10 35 46 7B 1C 00 MOVSS XMM14,dword ptr [0x14090a730]
140742bea:  F3 41 0F 59 C6           MULSS XMM0,XMM14
140742bef:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
140742bf4:  F3 0F 5C D0              SUBSS XMM2,XMM0
140742bf8:  48 89 5C 24 38           MOV qword ptr [RSP + 0x38],RBX
140742bfd:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140742c02:  48 8D 05 B7 96 1B 00     LEA RAX,[0x1408fc2c0]
140742c09:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140742c0e:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
140742c16:  66 41 0F 7E D9           MOVD R9D,XMM3
140742c1b:  66 41 0F 7E D0           MOVD R8D,XMM2
140742c20:  48 8B 15 F9 25 25 00     MOV RDX,qword ptr [0x140995220]
140742c27:  48 8D 0D 82 43 25 00     LEA RCX,[0x140996fb0]
140742c2e:  FF 15 4C 9C 12 00        CALL qword ptr [0x14086c880]
140742c34:  48 8B 9E 40 0D 00 00     MOV RBX,qword ptr [RSI + 0xd40]
140742c3b:  48 2B 9E 38 0D 00 00     SUB RBX,qword ptr [RSI + 0xd38]
140742c42:  48 C1 FB 03              SAR RBX,0x3
140742c46:  48 8B 05 D3 25 25 00     MOV RAX,qword ptr [0x140995220]
140742c4d:  48 8B 38                 MOV RDI,qword ptr [RAX]
140742c50:  BA 14 08 00 00           MOV EDX,0x814
140742c55:  48 8D 0D 34 49 25 00     LEA RCX,[0x140997590]
140742c5c:  FF 15 2E 9C 12 00        CALL qword ptr [0x14086c890]
140742c62:  48 89 5C 24 30           MOV qword ptr [RSP + 0x30],RBX
140742c67:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140742c6c:  48 8D 05 6D 96 1B 00     LEA RAX,[0x1408fc2e0]
140742c73:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140742c78:  41 0F 28 DC              MOVAPS XMM3,XMM12
140742c7c:  66 41 0F 7E D9           MOVD R9D,XMM3
140742c81:  41 0F 28 D4              MOVAPS XMM2,XMM12
140742c85:  66 41 0F 7E D0           MOVD R8D,XMM2
140742c8a:  48 8D 95 40 01 00 00     LEA RDX,[RBP + 0x140]
140742c91:  48 8B 0D 88 25 25 00     MOV RCX,qword ptr [0x140995220]
140742c98:  FF 57 28                 CALL qword ptr [RDI + 0x28]
140742c9b:  0F 10 30                 MOVUPS XMM6,xmmword ptr [RAX]
140742c9e:  F3 0F 10 0D E2 F3 24 00  MOVSS XMM1,dword ptr [0x140992088]
140742ca6:  F3 0F 59 CF              MULSS XMM1,XMM7
140742caa:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140742caf:  F3 0F 58 C1              ADDSS XMM0,XMM1
140742cb3:  F3 0F 11 45 80           MOVSS dword ptr [RBP + -0x80],XMM0
140742cb8:  49 8B 8E 30 B3 00 00     MOV RCX,qword ptr [R14 + 0xb330]
140742cbf:  48 8B 01                 MOV RAX,qword ptr [RCX]
140742cc2:  4C 8B 05 3F 80 2A 00     MOV R8,qword ptr [0x1409ead08]
140742cc9:  33 D2                    XOR EDX,EDX
140742ccb:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140742cce:  66 0F 73 DE 08           PSRLDQ XMM6,0x8
140742cd3:  66 0F 7E F0              MOVD EAX,XMM6
140742cd7:  66 44 0F 6E C0           MOVD XMM8,EAX
140742cdc:  45 0F 5B C0              CVTDQ2PS XMM8,XMM8
140742ce0:  41 0F 28 D8              MOVAPS XMM3,XMM8
140742ce4:  F3 0F 58 5D 84           ADDSS XMM3,dword ptr [RBP + -0x7c]
140742ce9:  F3 0F 10 0D 97 F3 24 00  MOVSS XMM1,dword ptr [0x140992088]
140742cf1:  0F 28 C1                 MOVAPS XMM0,XMM1
140742cf4:  F3 0F 59 C7              MULSS XMM0,XMM7
140742cf8:  F3 0F 58 D8              ADDSS XMM3,XMM0
140742cfc:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140742d01:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
140742d08:  0F 28 C3                 MOVAPS XMM0,XMM3
140742d0b:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140742d0e:  F2 0F 11 05 DA B5 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140742d16:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
140742d19:  89 05 D9 B5 27 00        MOV dword ptr [0x1409be2f8],EAX
140742d1f:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140742d24:  F3 0F 11 0D BC B5 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
140742d2c:  F3 0F 11 0D B8 B5 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
140742d34:  0F 28 05 05 95 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c240]
140742d3b:  0F 11 05 CA B5 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140742d42:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
140742d49:  49 8D 9E B0 AD 00 00     LEA RBX,[R14 + 0xadb0]
140742d50:  41 83 F9 FF              CMP R9D,-0x1
140742d54:  74 06                    JZ 0x140742d5c
140742d56:  44 3B 4B 40              CMP R9D,dword ptr [RBX + 0x40]
140742d5a:  75 0B                    JNZ 0x140742d67
140742d5c:  0F 28 CB                 MOVAPS XMM1,XMM3
140742d5f:  48 8B CB                 MOV RCX,RBX
140742d62:  E8 89 34 D0 FF           CALL 0x1404461f0
140742d67:  48 8B 7D 98              MOV RDI,qword ptr [RBP + -0x68]
140742d6b:  80 7F 20 00              CMP byte ptr [RDI + 0x20],0x0
140742d6f:  0F 84 9E 01 00 00        JZ 0x140742f13
140742d75:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
140742d79:  48 8D 0D 10 1E 31 00     LEA RCX,[0x140a54b90]
140742d80:  FF 15 E2 9A 12 00        CALL qword ptr [0x14086c868]
140742d86:  48 8B D0                 MOV RDX,RAX
140742d89:  41 0F 28 DC              MOVAPS XMM3,XMM12
140742d8d:  41 0F 28 D4              MOVAPS XMM2,XMM12
140742d91:  48 8D 0D C8 B2 27 00     LEA RCX,[0x1409be060]
140742d98:  FF 15 B2 9A 12 00        CALL qword ptr [0x14086c850]
140742d9e:  84 C0                    TEST AL,AL
140742da0:  0F 84 6D 01 00 00        JZ 0x140742f13
140742da6:  0F 28 05 13 97 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140742dad:  0F 11 05 58 B5 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140742db4:  BA 0D 08 00 00           MOV EDX,0x80d
140742db9:  48 8D 0D D0 47 25 00     LEA RCX,[0x140997590]
140742dc0:  FF 15 CA 9A 12 00        CALL qword ptr [0x14086c890]
140742dc6:  48 8B D0                 MOV RDX,RAX
140742dc9:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140742dd0:  E8 2B 2B 8D FF           CALL 0x140015900
140742dd5:  80 3D B5 20 31 00 00     CMP byte ptr [0x140a54e91],0x0
140742ddc:  0F 84 31 01 00 00        JZ 0x140742f13
140742de2:  48 8D 15 47 94 1B 00     LEA RDX,[0x1408fc230]
140742de9:  49 8B CE                 MOV RCX,R14
140742dec:  E8 AF 7C 8F FF           CALL 0x14003aaa0
140742df1:  49 39 86 28 D4 00 00     CMP qword ptr [R14 + 0xd428],RAX
140742df8:  0F 84 B4 00 00 00        JZ 0x140742eb2
140742dfe:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
140742e03:  75 51                    JNZ 0x140742e56
140742e05:  48 8D 15 44 94 1B 00     LEA RDX,[0x1408fc250]
140742e0c:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140742e13:  FF 15 F7 9E 12 00        CALL qword ptr [0x14086cd10]
140742e19:  84 C0                    TEST AL,AL
140742e1b:  74 32                    JZ 0x140742e4f
140742e1d:  48 8D 15 2C 94 1B 00     LEA RDX,[0x1408fc250]
140742e24:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140742e2b:  FF 15 E7 9E 12 00        CALL qword ptr [0x14086cd18]
140742e31:  48 8B D0                 MOV RDX,RAX
140742e34:  33 C0                    XOR EAX,EAX
140742e36:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140742e3b:  41 0F 28 DB              MOVAPS XMM3,XMM11
140742e3f:  41 B0 01                 MOV R8B,0x1
140742e42:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140742e49:  FF 15 59 A4 12 00        CALL qword ptr [0x14086d2a8]
140742e4f:  49 8D 9E B0 AD 00 00     LEA RBX,[R14 + 0xadb0]
140742e56:  48 8D 15 03 94 1B 00     LEA RDX,[0x1408fc260]
140742e5d:  49 8B CE                 MOV RCX,R14
140742e60:  E8 3B 7C 8F FF           CALL 0x14003aaa0
140742e65:  49 89 86 28 D4 00 00     MOV qword ptr [R14 + 0xd428],RAX
140742e6c:  49 89 B6 98 D2 00 00     MOV qword ptr [R14 + 0xd298],RSI
140742e73:  49 89 BE A0 D2 00 00     MOV qword ptr [R14 + 0xd2a0],RDI
140742e7a:  41 C6 86 B0 F5 00 00 00  MOV byte ptr [R14 + 0xf5b0],0x0
140742e82:  41 C7 86 C0 F5 00 00 00 00 80 BF MOV dword ptr [R14 + 0xf5c0],0xbf800000
140742e8d:  33 C0                    XOR EAX,EAX
140742e8f:  49 89 86 B8 F5 00 00     MOV qword ptr [R14 + 0xf5b8],RAX
140742e96:  49 8D 8E C8 F5 00 00     LEA RCX,[R14 + 0xf5c8]
140742e9d:  E8 1E EE AB FF           CALL 0x140201cc0
140742ea2:  49 8B 86 E8 F5 00 00     MOV RAX,qword ptr [R14 + 0xf5e8]
140742ea9:  49 89 86 F0 F5 00 00     MOV qword ptr [R14 + 0xf5f0],RAX
140742eb0:  EB 61                    JMP 0x140742f13
140742eb2:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
140742eb7:  75 51                    JNZ 0x140742f0a
140742eb9:  48 8D 15 C0 93 1B 00     LEA RDX,[0x1408fc280]
140742ec0:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140742ec7:  FF 15 43 9E 12 00        CALL qword ptr [0x14086cd10]
140742ecd:  84 C0                    TEST AL,AL
140742ecf:  74 32                    JZ 0x140742f03
140742ed1:  48 8D 15 A8 93 1B 00     LEA RDX,[0x1408fc280]
140742ed8:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140742edf:  FF 15 33 9E 12 00        CALL qword ptr [0x14086cd18]
140742ee5:  48 8B D0                 MOV RDX,RAX
140742ee8:  33 C0                    XOR EAX,EAX
140742eea:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140742eef:  41 0F 28 DB              MOVAPS XMM3,XMM11
140742ef3:  41 B0 01                 MOV R8B,0x1
140742ef6:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140742efd:  FF 15 A5 A3 12 00        CALL qword ptr [0x14086d2a8]
140742f03:  49 8D 9E B0 AD 00 00     LEA RBX,[R14 + 0xadb0]
140742f0a:  33 C0                    XOR EAX,EAX
140742f0c:  49 89 86 28 D4 00 00     MOV qword ptr [R14 + 0xd428],RAX
140742f13:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140742f18:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140742f1f:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140742f26:  41 0F 28 DB              MOVAPS XMM3,XMM11
140742f2a:  41 0F 28 D4              MOVAPS XMM2,XMM12
140742f2e:  41 0F 28 CC              MOVAPS XMM1,XMM12
140742f32:  48 8D 0D 27 B1 27 00     LEA RCX,[0x1409be060]
140742f39:  FF 15 21 99 12 00        CALL qword ptr [0x14086c860]
140742f3f:  83 3D 42 51 2A 00 FF     CMP dword ptr [0x1409e8088],-0x1
140742f46:  0F 84 C6 00 00 00        JZ 0x140743012
140742f4c:  48 8D 15 F5 93 1B 00     LEA RDX,[0x1408fc348]
140742f53:  48 8D 0D FE 52 2A 00     LEA RCX,[0x1409e8258]
140742f5a:  E8 C1 C5 10 00           CALL 0x14084f520
140742f5f:  85 C0                    TEST EAX,EAX
140742f61:  74 1B                    JZ 0x140742f7e
140742f63:  48 8D 15 F6 93 1B 00     LEA RDX,[0x1408fc360]
140742f6a:  48 8D 0D E7 52 2A 00     LEA RCX,[0x1409e8258]
140742f71:  E8 AA C5 10 00           CALL 0x14084f520
140742f76:  85 C0                    TEST EAX,EAX
140742f78:  0F 85 94 00 00 00        JNZ 0x140743012
140742f7e:  48 8B 96 18 03 00 00     MOV RDX,qword ptr [RSI + 0x318]
140742f85:  49 2B D7                 SUB RDX,R15
140742f88:  0F 1F 84 00 00 00 00 00  NOP dword ptr [RAX + RAX*0x1]
140742f90:  41 0F B6 0F              MOVZX ECX,byte ptr [R15]
140742f94:  41 0F B6 04 17           MOVZX EAX,byte ptr [R15 + RDX*0x1]
140742f99:  2B C8                    SUB ECX,EAX
140742f9b:  75 07                    JNZ 0x140742fa4
140742f9d:  49 FF C7                 INC R15
140742fa0:  85 C0                    TEST EAX,EAX
140742fa2:  75 EC                    JNZ 0x140742f90
140742fa4:  85 C9                    TEST ECX,ECX
140742fa6:  75 6A                    JNZ 0x140743012
140742fa8:  49 8B 8E 28 D4 00 00     MOV RCX,qword ptr [R14 + 0xd428]
140742faf:  48 85 C9                 TEST RCX,RCX
140742fb2:  74 10                    JZ 0x140742fc4
140742fb4:  48 8D 15 BD 93 1B 00     LEA RDX,[0x1408fc378]
140742fbb:  E8 60 C5 10 00           CALL 0x14084f520
140742fc0:  85 C0                    TEST EAX,EAX
140742fc2:  74 4E                    JZ 0x140743012
140742fc4:  83 3D 21 53 2A 00 02     CMP dword ptr [0x1409e82ec],0x2
140742fcb:  7D 45                    JGE 0x140743012
140742fcd:  C7 05 15 53 2A 00 02 00 00 00 MOV dword ptr [0x1409e82ec],0x2
140742fd7:  41 0F 28 C8              MOVAPS XMM1,XMM8
140742fdb:  F3 0F 58 4D 84           ADDSS XMM1,dword ptr [RBP + -0x7c]
140742fe0:  F3 0F 10 05 A0 F0 24 00  MOVSS XMM0,dword ptr [0x140992088]
140742fe8:  F3 0F 59 C7              MULSS XMM0,XMM7
140742fec:  F3 0F 58 C8              ADDSS XMM1,XMM0
140742ff0:  F3 0F 11 0D 00 53 2A 00  MOVSS dword ptr [0x1409e82f8],XMM1
140742ff8:  48 8D 0D 61 B0 27 00     LEA RCX,[0x1409be060]
140742fff:  FF 15 5B 9C 12 00        CALL qword ptr [0x14086cc60]
140743005:  F3 0F 58 45 80           ADDSS XMM0,dword ptr [RBP + -0x80]
14074300a:  F3 0F 11 05 EA 52 2A 00  MOVSS dword ptr [0x1409e82fc],XMM0
140743012:  49 8B 8E 20 B3 00 00     MOV RCX,qword ptr [R14 + 0xb320]
140743019:  48 8B 01                 MOV RAX,qword ptr [RCX]
14074301c:  4C 8B 05 E5 7C 2A 00     MOV R8,qword ptr [0x1409ead08]
140743023:  33 D2                    XOR EDX,EDX
140743025:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140743028:  41 0F 28 D8              MOVAPS XMM3,XMM8
14074302c:  F3 0F 58 5D 84           ADDSS XMM3,dword ptr [RBP + -0x7c]
140743031:  F3 0F 10 0D 4F F0 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743039:  0F 28 C1                 MOVAPS XMM0,XMM1
14074303c:  F3 0F 59 05 64 7A 1C 00  MULSS XMM0,dword ptr [0x14090aaa8]
140743044:  F3 0F 58 D8              ADDSS XMM3,XMM0
140743048:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
14074304d:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
140743054:  0F 28 C3                 MOVAPS XMM0,XMM3
140743057:  0F 14 C2                 UNPCKLPS XMM0,XMM2
14074305a:  F2 0F 11 05 8E B2 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140743062:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
140743065:  89 05 8D B2 27 00        MOV dword ptr [0x1409be2f8],EAX
14074306b:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140743070:  F3 0F 11 0D 70 B2 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
140743078:  F3 0F 11 0D 6C B2 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
140743080:  0F 28 05 B9 91 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c240]
140743087:  0F 11 05 7E B2 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14074308e:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
140743095:  41 83 F9 FF              CMP R9D,-0x1
140743099:  74 06                    JZ 0x1407430a1
14074309b:  44 3B 4B 40              CMP R9D,dword ptr [RBX + 0x40]
14074309f:  75 0B                    JNZ 0x1407430ac
1407430a1:  0F 28 CB                 MOVAPS XMM1,XMM3
1407430a4:  48 8B CB                 MOV RCX,RBX
1407430a7:  E8 44 31 D0 FF           CALL 0x1404461f0
1407430ac:  F3 0F 10 3D 74 79 1C 00  MOVSS XMM7,dword ptr [0x14090aa28]
1407430b4:  80 7F 20 00              CMP byte ptr [RDI + 0x20],0x0
1407430b8:  0F 84 CD 01 00 00        JZ 0x14074328b
1407430be:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
1407430c2:  48 8D 0D C7 1A 31 00     LEA RCX,[0x140a54b90]
1407430c9:  FF 15 99 97 12 00        CALL qword ptr [0x14086c868]
1407430cf:  48 8B D0                 MOV RDX,RAX
1407430d2:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407430d6:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407430da:  48 8D 0D 7F AF 27 00     LEA RCX,[0x1409be060]
1407430e1:  FF 15 69 97 12 00        CALL qword ptr [0x14086c850]
1407430e7:  84 C0                    TEST AL,AL
1407430e9:  0F 84 9C 01 00 00        JZ 0x14074328b
1407430ef:  0F 28 05 CA 93 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1407430f6:  0F 11 05 0F B2 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407430fd:  BA 09 08 00 00           MOV EDX,0x809
140743102:  48 8D 0D 87 44 25 00     LEA RCX,[0x140997590]
140743109:  FF 15 81 97 12 00        CALL qword ptr [0x14086c890]
14074310f:  48 8B D0                 MOV RDX,RAX
140743112:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140743119:  E8 E2 27 8D FF           CALL 0x140015900
14074311e:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140743125:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
14074312c:  48 C1 F8 03              SAR RAX,0x3
140743130:  48 85 C0                 TEST RAX,RAX
140743133:  0F 84 52 01 00 00        JZ 0x14074328b
140743139:  80 3D 51 1D 31 00 00     CMP byte ptr [0x140a54e91],0x0
140743140:  0F 84 45 01 00 00        JZ 0x14074328b
140743146:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
14074314b:  75 51                    JNZ 0x14074319e
14074314d:  48 8D 15 44 92 1B 00     LEA RDX,[0x1408fc398]
140743154:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
14074315b:  FF 15 AF 9B 12 00        CALL qword ptr [0x14086cd10]
140743161:  84 C0                    TEST AL,AL
140743163:  74 32                    JZ 0x140743197
140743165:  48 8D 15 2C 92 1B 00     LEA RDX,[0x1408fc398]
14074316c:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743173:  FF 15 9F 9B 12 00        CALL qword ptr [0x14086cd18]
140743179:  48 8B D0                 MOV RDX,RAX
14074317c:  33 C0                    XOR EAX,EAX
14074317e:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140743183:  41 0F 28 DB              MOVAPS XMM3,XMM11
140743187:  41 B0 01                 MOV R8B,0x1
14074318a:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140743191:  FF 15 11 A1 12 00        CALL qword ptr [0x14086d2a8]
140743197:  49 8D 9E B0 AD 00 00     LEA RBX,[R14 + 0xadb0]
14074319e:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
1407431a5:  E8 26 90 8F FF           CALL 0x14003c1d0
1407431aa:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
1407431b1:  C6 40 17 01              MOV byte ptr [RAX + 0x17],0x1
1407431b5:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
1407431b9:  48 8D 0D D0 19 31 00     LEA RCX,[0x140a54b90]
1407431c0:  FF 15 A2 96 12 00        CALL qword ptr [0x14086c868]
1407431c6:  F3 0F 10 70 04           MOVSS XMM6,dword ptr [RAX + 0x4]
1407431cb:  48 8D 95 90 00 00 00     LEA RDX,[RBP + 0x90]
1407431d2:  48 8D 0D B7 19 31 00     LEA RCX,[0x140a54b90]
1407431d9:  FF 15 89 96 12 00        CALL qword ptr [0x14086c868]
1407431df:  F3 0F 10 00              MOVSS XMM0,dword ptr [RAX]
1407431e3:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
1407431ea:  F3 0F 11 00              MOVSS dword ptr [RAX],XMM0
1407431ee:  F3 0F 11 70 04           MOVSS dword ptr [RAX + 0x4],XMM6
1407431f3:  F3 0F 10 0D 8D EE 24 00  MOVSS XMM1,dword ptr [0x140992088]
1407431fb:  0F 28 C1                 MOVAPS XMM0,XMM1
1407431fe:  F3 0F 59 05 12 7B 1C 00  MULSS XMM0,dword ptr [0x14090ad18]
140743206:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
14074320d:  C6 40 16 00              MOV byte ptr [RAX + 0x16],0x0
140743211:  F3 0F 11 40 08           MOVSS dword ptr [RAX + 0x8],XMM0
140743216:  F3 0F 59 CF              MULSS XMM1,XMM7
14074321a:  0F 2F C8                 COMISS XMM1,XMM0
14074321d:  76 05                    JBE 0x140743224
14074321f:  F3 0F 11 48 08           MOVSS dword ptr [RAX + 0x8],XMM1
140743224:  BA C1 88 00 00           MOV EDX,0x88c1
140743229:  48 8D 0D 60 43 25 00     LEA RCX,[0x140997590]
140743230:  FF 15 5A 96 12 00        CALL qword ptr [0x14086c890]
140743236:  48 8B D0                 MOV RDX,RAX
140743239:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
140743240:  E8 0B D1 EB FF           CALL 0x140600350
140743245:  BA 09 08 00 00           MOV EDX,0x809
14074324a:  48 8D 0D 3F 43 25 00     LEA RCX,[0x140997590]
140743251:  FF 15 39 96 12 00        CALL qword ptr [0x14086c890]
140743257:  48 8B D0                 MOV RDX,RAX
14074325a:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
140743261:  E8 6A 3D FA FF           CALL 0x1406e6fd0
140743266:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
14074326d:  48 8D 0D CC 1E 00 00     LEA RCX,[0x140745140]
140743274:  48 89 48 78              MOV qword ptr [RAX + 0x78],RCX
140743278:  48 89 70 70              MOV qword ptr [RAX + 0x70],RSI
14074327c:  48 8B D7                 MOV RDX,RDI
14074327f:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
140743286:  E8 95 8B 8F FF           CALL 0x14003be20
14074328b:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140743290:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140743297:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
14074329e:  41 0F 28 DB              MOVAPS XMM3,XMM11
1407432a2:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407432a6:  41 0F 28 CC              MOVAPS XMM1,XMM12
1407432aa:  48 8D 0D AF AD 27 00     LEA RCX,[0x1409be060]
1407432b1:  FF 15 A9 95 12 00        CALL qword ptr [0x14086c860]
1407432b7:  49 8B 8E 80 BD 00 00     MOV RCX,qword ptr [R14 + 0xbd80]
1407432be:  48 8B 01                 MOV RAX,qword ptr [RCX]
1407432c1:  4C 8B 05 40 7A 2A 00     MOV R8,qword ptr [0x1409ead08]
1407432c8:  33 D2                    XOR EDX,EDX
1407432ca:  FF 50 70                 CALL qword ptr [RAX + 0x70]
1407432cd:  41 0F 28 D8              MOVAPS XMM3,XMM8
1407432d1:  F3 0F 58 5D 84           ADDSS XMM3,dword ptr [RBP + -0x7c]
1407432d6:  F3 0F 10 0D AA ED 24 00  MOVSS XMM1,dword ptr [0x140992088]
1407432de:  0F 28 C1                 MOVAPS XMM0,XMM1
1407432e1:  F3 0F 59 05 B7 78 1C 00  MULSS XMM0,dword ptr [0x14090aba0]
1407432e9:  F3 0F 58 D8              ADDSS XMM3,XMM0
1407432ed:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
1407432f2:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
1407432f9:  0F 28 C3                 MOVAPS XMM0,XMM3
1407432fc:  0F 14 C2                 UNPCKLPS XMM0,XMM2
1407432ff:  F2 0F 11 05 E9 AF 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140743307:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
14074330a:  89 05 E8 AF 27 00        MOV dword ptr [0x1409be2f8],EAX
140743310:  F3 41 0F 59 CF           MULSS XMM1,XMM15
140743315:  F3 0F 11 0D CB AF 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
14074331d:  F3 0F 11 0D C7 AF 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
140743325:  0F 28 05 14 8F 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c240]
14074332c:  0F 11 05 D9 AF 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140743333:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
14074333a:  41 83 F9 FF              CMP R9D,-0x1
14074333e:  74 06                    JZ 0x140743346
140743340:  44 3B 4B 40              CMP R9D,dword ptr [RBX + 0x40]
140743344:  75 0B                    JNZ 0x140743351
140743346:  0F 28 CB                 MOVAPS XMM1,XMM3
140743349:  48 8B CB                 MOV RCX,RBX
14074334c:  E8 9F 2E D0 FF           CALL 0x1404461f0
140743351:  F3 44 0F 10 15 5E 7B 1C 00 MOVSS XMM10,dword ptr [0x14090aeb8]
14074335a:  80 7F 20 00              CMP byte ptr [RDI + 0x20],0x0
14074335e:  0F 84 B2 03 00 00        JZ 0x140743716
140743364:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
140743368:  48 8D 0D 21 18 31 00     LEA RCX,[0x140a54b90]
14074336f:  FF 15 F3 94 12 00        CALL qword ptr [0x14086c868]
140743375:  48 8B D0                 MOV RDX,RAX
140743378:  41 0F 28 DC              MOVAPS XMM3,XMM12
14074337c:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743380:  48 8D 0D D9 AC 27 00     LEA RCX,[0x1409be060]
140743387:  FF 15 C3 94 12 00        CALL qword ptr [0x14086c850]
14074338d:  84 C0                    TEST AL,AL
14074338f:  0F 84 81 03 00 00        JZ 0x140743716
140743395:  0F 28 05 24 91 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
14074339c:  0F 11 05 69 AF 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407433a3:  BA C2 88 00 00           MOV EDX,0x88c2
1407433a8:  48 8D 0D E1 41 25 00     LEA RCX,[0x140997590]
1407433af:  FF 15 DB 94 12 00        CALL qword ptr [0x14086c890]
1407433b5:  48 8B D0                 MOV RDX,RAX
1407433b8:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
1407433bf:  E8 3C 25 8D FF           CALL 0x140015900
1407433c4:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407433cb:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407433d2:  48 C1 F8 03              SAR RAX,0x3
1407433d6:  48 85 C0                 TEST RAX,RAX
1407433d9:  0F 84 37 03 00 00        JZ 0x140743716
1407433df:  80 3D AB 1A 31 00 00     CMP byte ptr [0x140a54e91],0x0
1407433e6:  0F 84 2A 03 00 00        JZ 0x140743716
1407433ec:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
1407433f1:  75 4C                    JNZ 0x14074343f
1407433f3:  48 8D 15 06 8F 1B 00     LEA RDX,[0x1408fc300]
1407433fa:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743401:  FF 15 09 99 12 00        CALL qword ptr [0x14086cd10]
140743407:  84 C0                    TEST AL,AL
140743409:  74 34                    JZ 0x14074343f
14074340b:  48 8D 15 EE 8E 1B 00     LEA RDX,[0x1408fc300]
140743412:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743419:  FF 15 F9 98 12 00        CALL qword ptr [0x14086cd18]
14074341f:  48 8B D0                 MOV RDX,RAX
140743422:  33 DB                    XOR EBX,EBX
140743424:  48 89 5C 24 20           MOV qword ptr [RSP + 0x20],RBX
140743429:  41 0F 28 DB              MOVAPS XMM3,XMM11
14074342d:  41 B0 01                 MOV R8B,0x1
140743430:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140743437:  FF 15 6B 9E 12 00        CALL qword ptr [0x14086d2a8]
14074343d:  EB 02                    JMP 0x140743441
14074343f:  33 DB                    XOR EBX,EBX
140743441:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
140743448:  E8 83 8D 8F FF           CALL 0x14003c1d0
14074344d:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
140743451:  48 8D 0D 38 17 31 00     LEA RCX,[0x140a54b90]
140743458:  FF 15 0A 94 12 00        CALL qword ptr [0x14086c868]
14074345e:  F3 0F 10 70 04           MOVSS XMM6,dword ptr [RAX + 0x4]
140743463:  48 8D 95 90 00 00 00     LEA RDX,[RBP + 0x90]
14074346a:  48 8D 0D 1F 17 31 00     LEA RCX,[0x140a54b90]
140743471:  FF 15 F1 93 12 00        CALL qword ptr [0x14086c868]
140743477:  F3 0F 10 00              MOVSS XMM0,dword ptr [RAX]
14074347b:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
140743482:  F3 0F 11 00              MOVSS dword ptr [RAX],XMM0
140743486:  F3 0F 11 70 04           MOVSS dword ptr [RAX + 0x4],XMM6
14074348b:  F3 0F 10 0D F5 EB 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743493:  0F 28 C1                 MOVAPS XMM0,XMM1
140743496:  F3 41 0F 59 C2           MULSS XMM0,XMM10
14074349b:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
1407434a2:  C6 40 16 00              MOV byte ptr [RAX + 0x16],0x0
1407434a6:  F3 0F 11 40 08           MOVSS dword ptr [RAX + 0x8],XMM0
1407434ab:  F3 0F 59 CF              MULSS XMM1,XMM7
1407434af:  0F 2F C8                 COMISS XMM1,XMM0
1407434b2:  76 05                    JBE 0x1407434b9
1407434b4:  F3 0F 11 48 08           MOVSS dword ptr [RAX + 0x8],XMM1
1407434b9:  BA C2 88 00 00           MOV EDX,0x88c2
1407434be:  48 8D 0D CB 40 25 00     LEA RCX,[0x140997590]
1407434c5:  FF 15 C5 93 12 00        CALL qword ptr [0x14086c890]
1407434cb:  48 8B D0                 MOV RDX,RAX
1407434ce:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
1407434d5:  E8 76 CE EB FF           CALL 0x140600350
1407434da:  44 8B FB                 MOV R15D,EBX
1407434dd:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407434e4:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407434eb:  48 C1 F8 03              SAR RAX,0x3
1407434ef:  48 85 C0                 TEST RAX,RAX
1407434f2:  0F 84 E4 01 00 00        JZ 0x1407436dc
1407434f8:  33 FF                    XOR EDI,EDI
1407434fa:  8B CF                    MOV ECX,EDI
1407434fc:  4D 8D A6 BC 17 01 00     LEA R12,[R14 + 0x117bc]
140743503:  49 BD AB AA AA AA AA AA AA 2A MOV R13,0x2aaaaaaaaaaaaaab
14074350d:  F3 0F 10 35 17 7D 1C 00  MOVSS XMM6,dword ptr [0x14090b22c]
140743515:  F3 0F 10 3D 1B 79 1C 00  MOVSS XMM7,dword ptr [0x14090ae38]
14074351d:  0F 1F 00                 NOP dword ptr [RAX]
140743520:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140743527:  48 8B 14 C8              MOV RDX,qword ptr [RAX + RCX*0x8]
14074352b:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140743530:  41 B1 01                 MOV R9B,0x1
140743533:  41 B8 00 01 00 00        MOV R8D,0x100
140743539:  48 8B 52 08              MOV RDX,qword ptr [RDX + 0x8]
14074353d:  49 8B CE                 MOV RCX,R14
140743540:  E8 9B F4 BE FF           CALL 0x1403329e0
140743545:  F3 0F 10 05 3B EB 24 00  MOVSS XMM0,dword ptr [0x140992088]
14074354d:  F3 0F 59 C7              MULSS XMM0,XMM7
140743551:  C7 44 24 38 FF FF FF FF  MOV dword ptr [RSP + 0x38],0xffffffff
140743559:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14074355e:  B8 01 00 00 00           MOV EAX,0x1
140743563:  89 44 24 28              MOV dword ptr [RSP + 0x28],EAX
140743567:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
14074356d:  4C 8B 0D 8C 0C 25 00     MOV R9,qword ptr [0x140994200]
140743574:  4D 8B C4                 MOV R8,R12
140743577:  48 8D 95 60 0B 00 00     LEA RDX,[RBP + 0xb60]
14074357e:  E8 DD 71 CE FF           CALL 0x14042a760
140743583:  0F B6 D8                 MOVZX EBX,AL
140743586:  48 8B 8E 40 0D 00 00     MOV RCX,qword ptr [RSI + 0xd40]
14074358d:  48 2B 8E 38 0D 00 00     SUB RCX,qword ptr [RSI + 0xd38]
140743594:  48 C1 F9 03              SAR RCX,0x3
140743598:  48 83 C1 01              ADD RCX,0x1
14074359c:  0F 57 C0                 XORPS XMM0,XMM0
14074359f:  F3 48 0F 2A C1           CVTSI2SS XMM0,RCX
1407435a4:  79 04                    JNS 0x1407435aa
1407435a6:  F3 0F 58 C6              ADDSS XMM0,XMM6
1407435aa:  41 8B C7                 MOV EAX,R15D
1407435ad:  0F 57 D2                 XORPS XMM2,XMM2
1407435b0:  F3 48 0F 2A D0           CVTSI2SS XMM2,RAX
1407435b5:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
1407435bb:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407435bf:  48 8D 95 60 0B 00 00     LEA RDX,[RBP + 0xb60]
1407435c6:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
1407435cd:  E8 DE 8E 8F FF           CALL 0x14003c4b0
1407435d2:  84 DB                    TEST BL,BL
1407435d4:  0F 84 D5 00 00 00        JZ 0x1407436af
1407435da:  48 8B DF                 MOV RBX,RDI
1407435dd:  48 89 9D 30 01 00 00     MOV qword ptr [RBP + 0x130],RBX
1407435e4:  44 8B D7                 MOV R10D,EDI
1407435e7:  89 BD 38 01 00 00        MOV dword ptr [RBP + 0x138],EDI
1407435ed:  4D 85 E4                 TEST R12,R12
1407435f0:  74 20                    JZ 0x140743612
1407435f2:  4D 8B C4                 MOV R8,R12
1407435f5:  83 CA FF                 OR EDX,0xffffffff
1407435f8:  48 8D 8D 30 01 00 00     LEA RCX,[RBP + 0x130]
1407435ff:  E8 7C 15 8F FF           CALL 0x140034b80
140743604:  44 8B 95 38 01 00 00     MOV R10D,dword ptr [RBP + 0x138]
14074360b:  48 8B 9D 30 01 00 00     MOV RBX,qword ptr [RBP + 0x130]
140743612:  4D 8B 8E F0 3B 01 00     MOV R9,qword ptr [R14 + 0x13bf0]
140743619:  49 8B 49 58              MOV RCX,qword ptr [R9 + 0x58]
14074361d:  49 2B 49 50              SUB RCX,qword ptr [R9 + 0x50]
140743621:  49 8B C5                 MOV RAX,R13
140743624:  48 F7 E9                 IMUL RCX
140743627:  48 C1 FA 03              SAR RDX,0x3
14074362b:  48 8B C2                 MOV RAX,RDX
14074362e:  48 C1 E8 3F              SHR RAX,0x3f
140743632:  48 03 C2                 ADD RAX,RDX
140743635:  FF C8                    DEC EAX
140743637:  48 63 C8                 MOVSXD RCX,EAX
14074363a:  4C 8D 04 49              LEA R8,[RCX + RCX*0x2]
14074363e:  49 C1 E0 04              SHL R8,0x4
140743642:  49 8B 79 50              MOV RDI,qword ptr [R9 + 0x50]
140743646:  49 83 C0 10              ADD R8,0x10
14074364a:  49 03 F8                 ADD RDI,R8
14074364d:  48 8D 85 30 01 00 00     LEA RAX,[RBP + 0x130]
140743654:  48 3B F8                 CMP RDI,RAX
140743657:  74 36                    JZ 0x14074368f
140743659:  48 85 DB                 TEST RBX,RBX
14074365c:  74 10                    JZ 0x14074366e
14074365e:  4C 8B C3                 MOV R8,RBX
140743661:  41 8B D2                 MOV EDX,R10D
140743664:  48 8B CF                 MOV RCX,RDI
140743667:  E8 14 15 8F FF           CALL 0x140034b80
14074366c:  EB 21                    JMP 0x14074368f
14074366e:  48 8B 0F                 MOV RCX,qword ptr [RDI]
140743671:  48 85 C9                 TEST RCX,RCX
140743674:  74 19                    JZ 0x14074368f
140743676:  E8 65 A9 10 00           CALL 0x14084dfe0
14074367b:  45 33 ED                 XOR R13D,R13D
14074367e:  4C 89 2F                 MOV qword ptr [RDI],R13
140743681:  44 89 6F 08              MOV dword ptr [RDI + 0x8],R13D
140743685:  49 BD AB AA AA AA AA AA AA 2A MOV R13,0x2aaaaaaaaaaaaaab
14074368f:  48 85 DB                 TEST RBX,RBX
140743692:  74 19                    JZ 0x1407436ad
140743694:  48 8B CB                 MOV RCX,RBX
140743697:  E8 44 A9 10 00           CALL 0x14084dfe0
14074369c:  33 FF                    XOR EDI,EDI
14074369e:  48 89 BD 30 01 00 00     MOV qword ptr [RBP + 0x130],RDI
1407436a5:  89 BD 38 01 00 00        MOV dword ptr [RBP + 0x138],EDI
1407436ab:  EB 02                    JMP 0x1407436af
1407436ad:  33 FF                    XOR EDI,EDI
1407436af:  41 FF C7                 INC R15D
1407436b2:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407436b9:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407436c0:  48 C1 F8 03              SAR RAX,0x3
1407436c4:  41 8B CF                 MOV ECX,R15D
1407436c7:  48 3B C8                 CMP RCX,RAX
1407436ca:  0F 82 50 FE FF FF        JC 0x140743520
1407436d0:  4C 8B 6D D0              MOV R13,qword ptr [RBP + -0x30]
1407436d4:  4C 8B 65 B8              MOV R12,qword ptr [RBP + -0x48]
1407436d8:  48 8B 7D 98              MOV RDI,qword ptr [RBP + -0x68]
1407436dc:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
1407436e3:  48 8D 0D 46 D5 8F FF     LEA RCX,[0x140040c30]
1407436ea:  48 89 88 80 00 00 00     MOV qword ptr [RAX + 0x80],RCX
1407436f1:  49 8B 86 F0 3B 01 00     MOV RAX,qword ptr [R14 + 0x13bf0]
1407436f8:  48 8D 0D 61 1A 00 00     LEA RCX,[0x140745160]
1407436ff:  48 89 48 78              MOV qword ptr [RAX + 0x78],RCX
140743703:  48 89 70 70              MOV qword ptr [RAX + 0x70],RSI
140743707:  48 8B D7                 MOV RDX,RDI
14074370a:  49 8B 8E F0 3B 01 00     MOV RCX,qword ptr [R14 + 0x13bf0]
140743711:  E8 0A 87 8F FF           CALL 0x14003be20
140743716:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14074371b:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140743722:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140743729:  41 0F 28 DB              MOVAPS XMM3,XMM11
14074372d:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743731:  41 0F 28 CC              MOVAPS XMM1,XMM12
140743735:  48 8D 0D 24 A9 27 00     LEA RCX,[0x1409be060]
14074373c:  FF 15 1E 91 12 00        CALL qword ptr [0x14086c860]
140743742:  49 8B 8E 48 B3 00 00     MOV RCX,qword ptr [R14 + 0xb348]
140743749:  48 8B 01                 MOV RAX,qword ptr [RCX]
14074374c:  4C 8B 05 B5 75 2A 00     MOV R8,qword ptr [0x1409ead08]
140743753:  33 D2                    XOR EDX,EDX
140743755:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140743758:  41 0F 28 F0              MOVAPS XMM6,XMM8
14074375c:  F3 0F 58 75 84           ADDSS XMM6,dword ptr [RBP + -0x7c]
140743761:  F3 0F 10 0D 1F E9 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743769:  0F 28 C1                 MOVAPS XMM0,XMM1
14074376c:  F3 0F 59 05 F4 74 1C 00  MULSS XMM0,dword ptr [0x14090ac68]
140743774:  F3 0F 58 F0              ADDSS XMM6,XMM0
140743778:  F3 0F 10 7D 80           MOVSS XMM7,dword ptr [RBP + -0x80]
14074377d:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
140743784:  0F 28 C6                 MOVAPS XMM0,XMM6
140743787:  0F 14 C7                 UNPCKLPS XMM0,XMM7
14074378a:  F2 0F 11 05 5E AB 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140743792:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
140743795:  89 05 5D AB 27 00        MOV dword ptr [0x1409be2f8],EAX
14074379b:  F3 41 0F 59 CF           MULSS XMM1,XMM15
1407437a0:  F3 0F 11 0D 40 AB 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
1407437a8:  F3 0F 11 0D 3C AB 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
1407437b0:  0F 28 05 89 8A 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c240]
1407437b7:  0F 11 05 4E AB 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407437be:  45 33 FF                 XOR R15D,R15D
1407437c1:  41 8B DF                 MOV EBX,R15D
1407437c4:  49 8B 8E 88 D2 00 00     MOV RCX,qword ptr [R14 + 0xd288]
1407437cb:  49 2B 8E 80 D2 00 00     SUB RCX,qword ptr [R14 + 0xd280]
1407437d2:  48 B8 83 2D D8 82 2D D8 82 2D MOV RAX,0x2d82d82d82d82d83
1407437dc:  48 F7 E9                 IMUL RCX
1407437df:  48 C1 FA 07              SAR RDX,0x7
1407437e3:  48 8B C2                 MOV RAX,RDX
1407437e6:  48 C1 E8 3F              SHR RAX,0x3f
1407437ea:  48 03 D0                 ADD RDX,RAX
1407437ed:  74 64                    JZ 0x140743853
1407437ef:  41 8B C7                 MOV EAX,R15D
1407437f2:  48 69 F8 D0 02 00 00     IMUL RDI,RAX,0x2d0
1407437f9:  49 8B 8E 80 D2 00 00     MOV RCX,qword ptr [R14 + 0xd280]
140743800:  48 03 CF                 ADD RCX,RDI
140743803:  48 8D 15 06 8B 1B 00     LEA RDX,[0x1408fc310]
14074380a:  E8 11 BD 10 00           CALL 0x14084f520
14074380f:  85 C0                    TEST EAX,EAX
140743811:  74 34                    JZ 0x140743847
140743813:  FF C3                    INC EBX
140743815:  49 8B 8E 88 D2 00 00     MOV RCX,qword ptr [R14 + 0xd288]
14074381c:  49 2B 8E 80 D2 00 00     SUB RCX,qword ptr [R14 + 0xd280]
140743823:  48 B8 83 2D D8 82 2D D8 82 2D MOV RAX,0x2d82d82d82d82d83
14074382d:  48 F7 E9                 IMUL RCX
140743830:  48 C1 FA 07              SAR RDX,0x7
140743834:  48 8B C2                 MOV RAX,RDX
140743837:  48 C1 E8 3F              SHR RAX,0x3f
14074383b:  48 03 D0                 ADD RDX,RAX
14074383e:  8B C3                    MOV EAX,EBX
140743840:  48 3B C2                 CMP RAX,RDX
140743843:  72 AD                    JC 0x1407437f2
140743845:  EB 0C                    JMP 0x140743853
140743847:  49 8B 9E 80 D2 00 00     MOV RBX,qword ptr [R14 + 0xd280]
14074384e:  48 03 DF                 ADD RBX,RDI
140743851:  EB 03                    JMP 0x140743856
140743853:  49 8B DF                 MOV RBX,R15
140743856:  49 39 9E 28 D4 00 00     CMP qword ptr [R14 + 0xd428],RBX
14074385d:  75 0E                    JNZ 0x14074386d
14074385f:  0F 28 05 5A 8C 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140743866:  0F 11 05 9F AA 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
14074386d:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
140743874:  4D 8D BE B0 AD 00 00     LEA R15,[R14 + 0xadb0]
14074387b:  41 83 F9 FF              CMP R9D,-0x1
14074387f:  74 06                    JZ 0x140743887
140743881:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
140743885:  75 0E                    JNZ 0x140743895
140743887:  0F 28 D7                 MOVAPS XMM2,XMM7
14074388a:  0F 28 CE                 MOVAPS XMM1,XMM6
14074388d:  49 8B CF                 MOV RCX,R15
140743890:  E8 5B 29 D0 FF           CALL 0x1404461f0
140743895:  48 8B 7D 98              MOV RDI,qword ptr [RBP + -0x68]
140743899:  80 7F 20 00              CMP byte ptr [RDI + 0x20],0x0
14074389d:  0F 84 51 01 00 00        JZ 0x1407439f4
1407438a3:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
1407438a7:  48 8D 0D E2 12 31 00     LEA RCX,[0x140a54b90]
1407438ae:  FF 15 B4 8F 12 00        CALL qword ptr [0x14086c868]
1407438b4:  48 8B D0                 MOV RDX,RAX
1407438b7:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407438bb:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407438bf:  48 8D 0D 9A A7 27 00     LEA RCX,[0x1409be060]
1407438c6:  FF 15 84 8F 12 00        CALL qword ptr [0x14086c850]
1407438cc:  84 C0                    TEST AL,AL
1407438ce:  0F 84 20 01 00 00        JZ 0x1407439f4
1407438d4:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407438db:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407438e2:  48 C1 F8 03              SAR RAX,0x3
1407438e6:  48 8D 0D A3 3C 25 00     LEA RCX,[0x140997590]
1407438ed:  48 85 C0                 TEST RAX,RAX
1407438f0:  0F 84 E4 00 00 00        JZ 0x1407439da
1407438f6:  0F 28 05 C3 8B 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1407438fd:  0F 11 05 08 AA 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140743904:  BA 15 08 00 00           MOV EDX,0x815
140743909:  FF 15 81 8F 12 00        CALL qword ptr [0x14086c890]
14074390f:  48 8B D0                 MOV RDX,RAX
140743912:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140743919:  E8 E2 1F 8D FF           CALL 0x140015900
14074391e:  80 3D 6C 15 31 00 00     CMP byte ptr [0x140a54e91],0x0
140743925:  0F 84 C9 00 00 00        JZ 0x1407439f4
14074392b:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
140743930:  75 4A                    JNZ 0x14074397c
140743932:  48 8D 15 EF 89 1B 00     LEA RDX,[0x1408fc328]
140743939:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743940:  FF 15 CA 93 12 00        CALL qword ptr [0x14086cd10]
140743946:  84 C0                    TEST AL,AL
140743948:  74 32                    JZ 0x14074397c
14074394a:  48 8D 15 D7 89 1B 00     LEA RDX,[0x1408fc328]
140743951:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743958:  FF 15 BA 93 12 00        CALL qword ptr [0x14086cd18]
14074395e:  48 8B D0                 MOV RDX,RAX
140743961:  33 C0                    XOR EAX,EAX
140743963:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140743968:  41 0F 28 DB              MOVAPS XMM3,XMM11
14074396c:  41 B0 01                 MOV R8B,0x1
14074396f:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140743976:  FF 15 2C 99 12 00        CALL qword ptr [0x14086d2a8]
14074397c:  33 C0                    XOR EAX,EAX
14074397e:  49 39 9E 28 D4 00 00     CMP qword ptr [R14 + 0xd428],RBX
140743985:  74 4A                    JZ 0x1407439d1
140743987:  49 89 9E 28 D4 00 00     MOV qword ptr [R14 + 0xd428],RBX
14074398e:  49 89 B6 98 D2 00 00     MOV qword ptr [R14 + 0xd298],RSI
140743995:  49 89 BE A0 D2 00 00     MOV qword ptr [R14 + 0xd2a0],RDI
14074399c:  41 88 86 B0 F5 00 00     MOV byte ptr [R14 + 0xf5b0],AL
1407439a3:  41 C7 86 C0 F5 00 00 00 00 80 BF MOV dword ptr [R14 + 0xf5c0],0xbf800000
1407439ae:  49 89 86 B8 F5 00 00     MOV qword ptr [R14 + 0xf5b8],RAX
1407439b5:  49 8D 8E C8 F5 00 00     LEA RCX,[R14 + 0xf5c8]
1407439bc:  E8 FF E2 AB FF           CALL 0x140201cc0
1407439c1:  49 8B 86 E8 F5 00 00     MOV RAX,qword ptr [R14 + 0xf5e8]
1407439c8:  49 89 86 F0 F5 00 00     MOV qword ptr [R14 + 0xf5f0],RAX
1407439cf:  EB 23                    JMP 0x1407439f4
1407439d1:  49 89 86 28 D4 00 00     MOV qword ptr [R14 + 0xd428],RAX
1407439d8:  EB 1A                    JMP 0x1407439f4
1407439da:  BA 22 08 00 00           MOV EDX,0x822
1407439df:  FF 15 AB 8E 12 00        CALL qword ptr [0x14086c890]
1407439e5:  48 8B D0                 MOV RDX,RAX
1407439e8:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
1407439ef:  E8 0C 1F 8D FF           CALL 0x140015900
1407439f4:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
1407439f9:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140743a00:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140743a07:  41 0F 28 DB              MOVAPS XMM3,XMM11
140743a0b:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743a0f:  41 0F 28 CC              MOVAPS XMM1,XMM12
140743a13:  48 8D 0D 46 A6 27 00     LEA RCX,[0x1409be060]
140743a1a:  FF 15 40 8E 12 00        CALL qword ptr [0x14086c860]
140743a20:  49 8B 8E 00 B4 00 00     MOV RCX,qword ptr [R14 + 0xb400]
140743a27:  48 8B 01                 MOV RAX,qword ptr [RCX]
140743a2a:  4C 8B 05 D7 72 2A 00     MOV R8,qword ptr [0x1409ead08]
140743a31:  33 D2                    XOR EDX,EDX
140743a33:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140743a36:  F3 44 0F 58 45 84        ADDSS XMM8,dword ptr [RBP + -0x7c]
140743a3c:  F3 0F 10 15 44 E6 24 00  MOVSS XMM2,dword ptr [0x140992088]
140743a44:  0F 28 C2                 MOVAPS XMM0,XMM2
140743a47:  F3 0F 59 05 F5 72 1C 00  MULSS XMM0,dword ptr [0x14090ad44]
140743a4f:  F3 44 0F 58 C0           ADDSS XMM8,XMM0
140743a54:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140743a59:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
140743a60:  41 0F 28 C0              MOVAPS XMM0,XMM8
140743a64:  0F 14 C1                 UNPCKLPS XMM0,XMM1
140743a67:  F2 0F 11 05 81 A8 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140743a6f:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
140743a72:  89 05 80 A8 27 00        MOV dword ptr [0x1409be2f8],EAX
140743a78:  F3 41 0F 59 D7           MULSS XMM2,XMM15
140743a7d:  F3 0F 11 15 63 A8 27 00  MOVSS dword ptr [0x1409be2e8],XMM2
140743a85:  F3 0F 11 15 5F A8 27 00  MOVSS dword ptr [0x1409be2ec],XMM2
140743a8d:  0F 28 05 AC 87 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c240]
140743a94:  0F 11 05 71 A8 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140743a9b:  48 8B D6                 MOV RDX,RSI
140743a9e:  E8 6D 64 AA FF           CALL 0x1401e9f10
140743aa3:  0F B6 D8                 MOVZX EBX,AL
140743aa6:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
140743aad:  41 83 F9 FF              CMP R9D,-0x1
140743ab1:  74 06                    JZ 0x140743ab9
140743ab3:  45 3B 4F 40              CMP R9D,dword ptr [R15 + 0x40]
140743ab7:  75 18                    JNZ 0x140743ad1
140743ab9:  F3 0F 10 15 33 A8 27 00  MOVSS XMM2,dword ptr [0x1409be2f4]
140743ac1:  F3 0F 10 0D 27 A8 27 00  MOVSS XMM1,dword ptr [0x1409be2f0]
140743ac9:  49 8B CF                 MOV RCX,R15
140743acc:  E8 1F 27 D0 FF           CALL 0x1404461f0
140743ad1:  80 7F 20 00              CMP byte ptr [RDI + 0x20],0x0
140743ad5:  0F 84 E9 00 00 00        JZ 0x140743bc4
140743adb:  48 8D 55 F8              LEA RDX,[RBP + -0x8]
140743adf:  48 8D 0D AA 10 31 00     LEA RCX,[0x140a54b90]
140743ae6:  FF 15 7C 8D 12 00        CALL qword ptr [0x14086c868]
140743aec:  48 8B D0                 MOV RDX,RAX
140743aef:  41 0F 28 DC              MOVAPS XMM3,XMM12
140743af3:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743af7:  48 8D 0D 62 A5 27 00     LEA RCX,[0x1409be060]
140743afe:  FF 15 4C 8D 12 00        CALL qword ptr [0x14086c850]
140743b04:  84 C0                    TEST AL,AL
140743b06:  0F 84 B8 00 00 00        JZ 0x140743bc4
140743b0c:  48 8D 0D 7D 3A 25 00     LEA RCX,[0x140997590]
140743b13:  84 DB                    TEST BL,BL
140743b15:  0F 84 8F 00 00 00        JZ 0x140743baa
140743b1b:  0F 28 05 9E 89 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140743b22:  0F 11 05 E3 A7 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140743b29:  BA 0C 11 00 00           MOV EDX,0x110c
140743b2e:  FF 15 5C 8D 12 00        CALL qword ptr [0x14086c890]
140743b34:  48 8B D0                 MOV RDX,RAX
140743b37:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140743b3e:  E8 BD 1D 8D FF           CALL 0x140015900
140743b43:  80 3D 47 13 31 00 00     CMP byte ptr [0x140a54e91],0x0
140743b4a:  74 78                    JZ 0x140743bc4
140743b4c:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
140743b51:  75 4A                    JNZ 0x140743b9d
140743b53:  48 8D 15 DE 87 1B 00     LEA RDX,[0x1408fc338]
140743b5a:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743b61:  FF 15 A9 91 12 00        CALL qword ptr [0x14086cd10]
140743b67:  84 C0                    TEST AL,AL
140743b69:  74 32                    JZ 0x140743b9d
140743b6b:  48 8D 15 C6 87 1B 00     LEA RDX,[0x1408fc338]
140743b72:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140743b79:  FF 15 99 91 12 00        CALL qword ptr [0x14086cd18]
140743b7f:  48 8B D0                 MOV RDX,RAX
140743b82:  33 C0                    XOR EAX,EAX
140743b84:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140743b89:  41 0F 28 DB              MOVAPS XMM3,XMM11
140743b8d:  41 B0 01                 MOV R8B,0x1
140743b90:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140743b97:  FF 15 0B 97 12 00        CALL qword ptr [0x14086d2a8]
140743b9d:  48 8B D6                 MOV RDX,RSI
140743ba0:  49 8B CE                 MOV RCX,R14
140743ba3:  E8 F8 65 AA FF           CALL 0x1401ea1a0
140743ba8:  EB 1A                    JMP 0x140743bc4
140743baa:  BA 0F 11 00 00           MOV EDX,0x110f
140743baf:  FF 15 DB 8C 12 00        CALL qword ptr [0x14086c890]
140743bb5:  48 8B D0                 MOV RDX,RAX
140743bb8:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140743bbf:  E8 3C 1D 8D FF           CALL 0x140015900
140743bc4:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140743bc9:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
140743bd0:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140743bd7:  41 0F 28 DB              MOVAPS XMM3,XMM11
140743bdb:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743bdf:  41 0F 28 CC              MOVAPS XMM1,XMM12
140743be3:  48 8D 0D 76 A4 27 00     LEA RCX,[0x1409be060]
140743bea:  FF 15 70 8C 12 00        CALL qword ptr [0x14086c860]
140743bf0:  F3 0F 10 05 90 E4 24 00  MOVSS XMM0,dword ptr [0x140992088]
140743bf8:  F3 0F 59 05 68 6D 1C 00  MULSS XMM0,dword ptr [0x14090a968]
140743c00:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140743c05:  F3 0F 58 D0              ADDSS XMM2,XMM0
140743c09:  F3 0F 11 55 80           MOVSS dword ptr [RBP + -0x80],XMM2
140743c0e:  4C 8B CF                 MOV R9,RDI
140743c11:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140743c16:  49 8B CE                 MOV RCX,R14
140743c19:  E8 E2 56 02 00           CALL 0x140769300
140743c1e:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140743c23:  F3 0F 58 C8              ADDSS XMM1,XMM0
140743c27:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140743c2c:  F3 44 0F 11 4D 84        MOVSS dword ptr [RBP + -0x7c],XMM9
140743c32:  45 33 C0                 XOR R8D,R8D
140743c35:  41 8B D8                 MOV EBX,R8D
140743c38:  89 5D 88                 MOV dword ptr [RBP + -0x78],EBX
140743c3b:  66 89 9D 30 01 00 00     MOV word ptr [RBP + 0x130],BX
140743c42:  4C 89 85 34 01 00 00     MOV qword ptr [RBP + 0x134],R8
140743c49:  89 9D 3C 01 00 00        MOV dword ptr [RBP + 0x13c],EBX
140743c4f:  48 85 F6                 TEST RSI,RSI
140743c52:  74 29                    JZ 0x140743c7d
140743c54:  48 8D 96 70 0C 00 00     LEA RDX,[RSI + 0xc70]
140743c5b:  48 8B 42 08              MOV RAX,qword ptr [RDX + 0x8]
140743c5f:  48 2B 02                 SUB RAX,qword ptr [RDX]
140743c62:  48 C1 F8 03              SAR RAX,0x3
140743c66:  48 83 F8 01              CMP RAX,0x1
140743c6a:  72 11                    JC 0x140743c7d
140743c6c:  48 8D 8D 30 01 00 00     LEA RCX,[RBP + 0x130]
140743c73:  E8 68 FD CF FF           CALL 0x1404439e0
140743c78:  45 33 C0                 XOR R8D,R8D
140743c7b:  EB 73                    JMP 0x140743cf0
140743c7d:  66 C7 85 30 01 00 00 01 00 MOV word ptr [RBP + 0x130],0x1
140743c86:  48 85 F6                 TEST RSI,RSI
140743c89:  74 3B                    JZ 0x140743cc6
140743c8b:  48 8B 86 18 03 00 00     MOV RAX,qword ptr [RSI + 0x318]
140743c92:  48 85 C0                 TEST RAX,RAX
140743c95:  74 2F                    JZ 0x140743cc6
140743c97:  83 B8 60 03 00 00 09     CMP dword ptr [RAX + 0x360],0x9
140743c9e:  75 50                    JNZ 0x140743cf0
140743ca0:  C7 85 34 01 00 00 00 01 00 00 MOV dword ptr [RBP + 0x134],0x100
140743caa:  C7 85 38 01 00 00 00 00 C8 41 MOV dword ptr [RBP + 0x138],0x41c80000
140743cb4:  F3 0F 10 05 C4 65 1C 00  MOVSS XMM0,dword ptr [0x14090a280]
140743cbc:  F3 0F 11 85 3C 01 00 00  MOVSS dword ptr [RBP + 0x13c],XMM0
140743cc4:  EB 2A                    JMP 0x140743cf0
140743cc6:  C7 85 34 01 00 00 00 10 00 00 MOV dword ptr [RBP + 0x134],0x1000
140743cd0:  F3 0F 10 05 24 6E 1C 00  MOVSS XMM0,dword ptr [0x14090aafc]
140743cd8:  F3 0F 11 85 38 01 00 00  MOVSS dword ptr [RBP + 0x138],XMM0
140743ce0:  F3 0F 10 0D 30 69 1C 00  MOVSS XMM1,dword ptr [0x14090a618]
140743ce8:  F3 0F 11 8D 3C 01 00 00  MOVSS dword ptr [RBP + 0x13c],XMM1
140743cf0:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140743cf7:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
140743cfe:  48 C1 F8 03              SAR RAX,0x3
140743d02:  48 85 C0                 TEST RAX,RAX
140743d05:  0F 84 18 10 00 00        JZ 0x140744d23
140743d0b:  45 8B F8                 MOV R15D,R8D
140743d0e:  44 89 45 90              MOV dword ptr [RBP + -0x70],R8D
140743d12:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
140743d19:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
140743d20:  48 C1 F8 03              SAR RAX,0x3
140743d24:  48 85 C0                 TEST RAX,RAX
140743d27:  0F 84 E9 0F 00 00        JZ 0x140744d16
140743d2d:  4C 89 45 C8              MOV qword ptr [RBP + -0x38],R8
140743d31:  4D 8B E8                 MOV R13,R8
140743d34:  4C 89 45 D0              MOV qword ptr [RBP + -0x30],R8
140743d38:  89 9D B0 00 00 00        MOV dword ptr [RBP + 0xb0],EBX
140743d3e:  0F 28 3D FB 84 1C 00     MOVAPS XMM7,xmmword ptr [0x14090c240]
140743d45:  89 9D 98 00 00 00        MOV dword ptr [RBP + 0x98],EBX
140743d4b:  44 0F 28 C7              MOVAPS XMM8,XMM7
140743d4f:  89 5D 00                 MOV dword ptr [RBP],EBX
140743d52:  44 0F 28 CF              MOVAPS XMM9,XMM7
140743d56:  F3 44 0F 10 2D 0D 6B 1C 00 MOVSS XMM13,dword ptr [0x14090a86c]
140743d5f:  F2 44 0F 10 35 58 64 1C 00 MOVSD XMM14,qword ptr [0x14090a1c0]
140743d68:  F2 44 0F 10 3D 7F 64 1C 00 MOVSD XMM15,qword ptr [0x14090a1f0]
140743d71:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140743d78:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140743d7c:  48 8B 79 08              MOV RDI,qword ptr [RCX + 0x8]
140743d80:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
140743d85:  41 B1 01                 MOV R9B,0x1
140743d88:  41 B8 00 01 00 00        MOV R8D,0x100
140743d8e:  48 8B D7                 MOV RDX,RDI
140743d91:  49 8B CE                 MOV RCX,R14
140743d94:  E8 47 EC BE FF           CALL 0x1403329e0
140743d99:  F3 0F 10 05 E7 E2 24 00  MOVSS XMM0,dword ptr [0x140992088]
140743da1:  F3 41 0F 59 C2           MULSS XMM0,XMM10
140743da6:  C7 44 24 38 FF FF FF FF  MOV dword ptr [RSP + 0x38],0xffffffff
140743dae:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140743db3:  B8 01 00 00 00           MOV EAX,0x1
140743db8:  89 44 24 28              MOV dword ptr [RSP + 0x28],EAX
140743dbc:  F3 0F 11 44 24 20        MOVSS dword ptr [RSP + 0x20],XMM0
140743dc2:  4C 8B 0D 57 14 25 00     MOV R9,qword ptr [0x140995220]
140743dc9:  4D 8D 86 BC 17 01 00     LEA R8,[R14 + 0x117bc]
140743dd0:  48 8D 95 60 09 00 00     LEA RDX,[RBP + 0x960]
140743dd7:  E8 84 69 CE FF           CALL 0x14042a760
140743ddc:  84 C0                    TEST AL,AL
140743dde:  0F 84 0D 01 00 00        JZ 0x140743ef1
140743de4:  48 8D 95 E0 00 00 00     LEA RDX,[RBP + 0xe0]
140743deb:  48 8D 0D 9E 0D 31 00     LEA RCX,[0x140a54b90]
140743df2:  FF 15 70 8A 12 00        CALL qword ptr [0x14086c868]
140743df8:  48 8B 4D 98              MOV RCX,qword ptr [RBP + -0x68]
140743dfc:  F3 0F 10 41 08           MOVSS XMM0,dword ptr [RCX + 0x8]
140743e01:  F3 0F 58 41 2C           ADDSS XMM0,dword ptr [RCX + 0x2c]
140743e06:  F3 0F 58 81 1C 05 00 00  ADDSS XMM0,dword ptr [RCX + 0x51c]
140743e0e:  0F 2F 40 04              COMISS XMM0,dword ptr [RAX + 0x4]
140743e12:  0F 86 D9 00 00 00        JBE 0x140743ef1
140743e18:  F3 0F 10 0D 68 E2 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743e20:  0F 28 F9                 MOVAPS XMM7,XMM1
140743e23:  F3 0F 59 3D 75 6A 1C 00  MULSS XMM7,dword ptr [0x14090a8a0]
140743e2b:  44 0F 28 CF              MOVAPS XMM9,XMM7
140743e2f:  F3 44 0F 10 45 80        MOVSS XMM8,dword ptr [RBP + -0x80]
140743e35:  F3 45 0F 58 C8           ADDSS XMM9,XMM8
140743e3a:  0F 28 F1                 MOVAPS XMM6,XMM1
140743e3d:  F3 41 0F 59 F2           MULSS XMM6,XMM10
140743e42:  F3 0F 58 75 84           ADDSS XMM6,dword ptr [RBP + -0x7c]
140743e47:  F3 0F 59 0D E1 68 1C 00  MULSS XMM1,dword ptr [0x14090a730]
140743e4f:  F3 44 0F 5C C1           SUBSS XMM8,XMM1
140743e54:  F3 0F 58 7D 84           ADDSS XMM7,dword ptr [RBP + -0x7c]
140743e59:  48 8D 95 EC 00 00 00     LEA RDX,[RBP + 0xec]
140743e60:  48 8D 0D 29 0D 31 00     LEA RCX,[0x140a54b90]
140743e67:  FF 15 FB 89 12 00        CALL qword ptr [0x14086c868]
140743e6d:  48 8B D8                 MOV RBX,RAX
140743e70:  48 8D 0D E9 A1 27 00     LEA RCX,[0x1409be060]
140743e77:  FF 15 E3 8D 12 00        CALL qword ptr [0x14086cc60]
140743e7d:  F3 44 0F 10 53 04        MOVSS XMM10,dword ptr [RBX + 0x4]
140743e83:  F3 44 0F 5C D0           SUBSS XMM10,XMM0
140743e88:  48 8D 95 F8 00 00 00     LEA RDX,[RBP + 0xf8]
140743e8f:  48 8D 0D FA 0C 31 00     LEA RCX,[0x140a54b90]
140743e96:  FF 15 CC 89 12 00        CALL qword ptr [0x14086c868]
140743e9c:  F3 0F 5C 38              SUBSS XMM7,dword ptr [RAX]
140743ea0:  F3 0F 5C 30              SUBSS XMM6,dword ptr [RAX]
140743ea4:  F3 0F 59 FE              MULSS XMM7,XMM6
140743ea8:  44 0F 2F E7              COMISS XMM12,XMM7
140743eac:  72 28                    JC 0x140743ed6
140743eae:  F3 45 0F 5C C2           SUBSS XMM8,XMM10
140743eb3:  F3 45 0F 5C CA           SUBSS XMM9,XMM10
140743eb8:  F3 45 0F 59 C1           MULSS XMM8,XMM9
140743ebd:  45 0F 2F E0              COMISS XMM12,XMM8
140743ec1:  72 13                    JC 0x140743ed6
140743ec3:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140743eca:  49 8D 96 BC 17 01 00     LEA RDX,[R14 + 0x117bc]
140743ed1:  E8 2A 1A 8D FF           CALL 0x140015900
140743ed6:  0F 28 3D 63 83 1C 00     MOVAPS XMM7,xmmword ptr [0x14090c240]
140743edd:  44 0F 28 C7              MOVAPS XMM8,XMM7
140743ee1:  44 0F 28 CF              MOVAPS XMM9,XMM7
140743ee5:  F3 44 0F 10 15 CA 6F 1C 00 MOVSS XMM10,dword ptr [0x14090aeb8]
140743eee:  8B 5D 88                 MOV EBX,dword ptr [RBP + -0x78]
140743ef1:  F3 0F 10 15 8F E1 24 00  MOVSS XMM2,dword ptr [0x140992088]
140743ef9:  0F 28 C2                 MOVAPS XMM0,XMM2
140743efc:  F3 0F 59 05 40 65 1C 00  MULSS XMM0,dword ptr [0x14090a444]
140743f04:  F3 0F 10 5D 80           MOVSS XMM3,dword ptr [RBP + -0x80]
140743f09:  F3 0F 5C D8              SUBSS XMM3,XMM0
140743f0d:  F3 0F 59 15 8B 69 1C 00  MULSS XMM2,dword ptr [0x14090a8a0]
140743f15:  F3 0F 58 55 84           ADDSS XMM2,dword ptr [RBP + -0x7c]
140743f1a:  48 8D 85 60 09 00 00     LEA RAX,[RBP + 0x960]
140743f21:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140743f26:  48 8D 05 93 82 1B 00     LEA RAX,[0x1408fc1c0]
140743f2d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140743f32:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
140743f3a:  66 41 0F 7E D9           MOVD R9D,XMM3
140743f3f:  66 41 0F 7E D0           MOVD R8D,XMM2
140743f44:  48 8B 15 D5 12 25 00     MOV RDX,qword ptr [0x140995220]
140743f4b:  48 8D 0D 5E 30 25 00     LEA RCX,[0x140996fb0]
140743f52:  FF 15 28 89 12 00        CALL qword ptr [0x14086c880]
140743f58:  48 8B 0D C1 12 25 00     MOV RCX,qword ptr [0x140995220]
140743f5f:  48 8B 01                 MOV RAX,qword ptr [RCX]
140743f62:  48 8D 95 60 09 00 00     LEA RDX,[RBP + 0x960]
140743f69:  48 89 54 24 28           MOV qword ptr [RSP + 0x28],RDX
140743f6e:  48 8D 15 53 82 1B 00     LEA RDX,[0x1408fc1c8]
140743f75:  48 89 54 24 20           MOV qword ptr [RSP + 0x20],RDX
140743f7a:  41 0F 28 DC              MOVAPS XMM3,XMM12
140743f7e:  66 41 0F 7E D9           MOVD R9D,XMM3
140743f83:  41 0F 28 D4              MOVAPS XMM2,XMM12
140743f87:  66 41 0F 7E D0           MOVD R8D,XMM2
140743f8c:  48 8D 95 50 01 00 00     LEA RDX,[RBP + 0x150]
140743f93:  FF 50 28                 CALL qword ptr [RAX + 0x28]
140743f96:  0F 10 00                 MOVUPS XMM0,xmmword ptr [RAX]
140743f99:  0F 11 85 40 01 00 00     MOVUPS xmmword ptr [RBP + 0x140],XMM0
140743fa0:  F3 0F 10 0D E0 E0 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743fa8:  F3 0F 59 0D 78 69 1C 00  MULSS XMM1,dword ptr [0x14090a928]
140743fb0:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
140743fb5:  F3 0F 58 C1              ADDSS XMM0,XMM1
140743fb9:  F3 0F 11 45 80           MOVSS dword ptr [RBP + -0x80],XMM0
140743fbe:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140743fc5:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140743fc9:  80 39 00                 CMP byte ptr [RCX],0x0
140743fcc:  0F 85 D4 01 00 00        JNZ 0x1407441a6
140743fd2:  49 8B 8E 00 B5 00 00     MOV RCX,qword ptr [R14 + 0xb500]
140743fd9:  48 8B 01                 MOV RAX,qword ptr [RCX]
140743fdc:  4C 8B 05 25 6D 2A 00     MOV R8,qword ptr [0x1409ead08]
140743fe3:  33 D2                    XOR EDX,EDX
140743fe5:  FF 50 70                 CALL qword ptr [RAX + 0x70]
140743fe8:  F3 0F 10 0D 98 E0 24 00  MOVSS XMM1,dword ptr [0x140992088]
140743ff0:  0F 28 C1                 MOVAPS XMM0,XMM1
140743ff3:  F3 41 0F 59 C5           MULSS XMM0,XMM13
140743ff8:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140743ffd:  F3 0F 5C D0              SUBSS XMM2,XMM0
140744001:  66 0F 6E B5 48 01 00 00  MOVD XMM6,dword ptr [RBP + 0x148]
140744009:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14074400c:  0F 28 DE                 MOVAPS XMM3,XMM6
14074400f:  F3 0F 58 5D 84           ADDSS XMM3,dword ptr [RBP + -0x7c]
140744014:  0F 28 C1                 MOVAPS XMM0,XMM1
140744017:  F3 0F 59 05 D1 69 1C 00  MULSS XMM0,dword ptr [0x14090a9f0]
14074401f:  F3 0F 58 D8              ADDSS XMM3,XMM0
140744023:  C7 45 A8 00 00 00 00     MOV dword ptr [RBP + -0x58],0x0
14074402a:  0F 28 C3                 MOVAPS XMM0,XMM3
14074402d:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140744030:  F2 0F 11 05 B8 A2 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140744038:  8B 45 A8                 MOV EAX,dword ptr [RBP + -0x58]
14074403b:  89 05 B7 A2 27 00        MOV dword ptr [0x1409be2f8],EAX
140744041:  F3 0F 59 0D 83 69 1C 00  MULSS XMM1,dword ptr [0x14090a9cc]
140744049:  F3 0F 11 0D 97 A2 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
140744051:  F3 0F 11 0D 93 A2 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
140744059:  0F 28 05 60 84 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140744060:  0F 11 05 A5 A2 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140744067:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
14074406e:  4D 8D A6 B0 AD 00 00     LEA R12,[R14 + 0xadb0]
140744075:  41 83 F9 FF              CMP R9D,-0x1
140744079:  74 07                    JZ 0x140744082
14074407b:  45 3B 4C 24 40           CMP R9D,dword ptr [R12 + 0x40]
140744080:  75 0B                    JNZ 0x14074408d
140744082:  0F 28 CB                 MOVAPS XMM1,XMM3
140744085:  49 8B CC                 MOV RCX,R12
140744088:  E8 63 21 D0 FF           CALL 0x1404461f0
14074408d:  48 8B 45 98              MOV RAX,qword ptr [RBP + -0x68]
140744091:  80 78 20 00              CMP byte ptr [RAX + 0x20],0x0
140744095:  0F 84 C7 00 00 00        JZ 0x140744162
14074409b:  48 8D 95 08 01 00 00     LEA RDX,[RBP + 0x108]
1407440a2:  48 8D 0D E7 0A 31 00     LEA RCX,[0x140a54b90]
1407440a9:  FF 15 B9 87 12 00        CALL qword ptr [0x14086c868]
1407440af:  48 8B D0                 MOV RDX,RAX
1407440b2:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407440b6:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407440ba:  48 8D 0D 9F 9F 27 00     LEA RCX,[0x1409be060]
1407440c1:  FF 15 89 87 12 00        CALL qword ptr [0x14086c850]
1407440c7:  84 C0                    TEST AL,AL
1407440c9:  0F 84 93 00 00 00        JZ 0x140744162
1407440cf:  BA B7 05 00 00           MOV EDX,0x5b7
1407440d4:  48 8D 0D B5 34 25 00     LEA RCX,[0x140997590]
1407440db:  FF 15 AF 87 12 00        CALL qword ptr [0x14086c890]
1407440e1:  48 8B D0                 MOV RDX,RAX
1407440e4:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
1407440eb:  E8 10 18 8D FF           CALL 0x140015900
1407440f0:  80 3D 9A 0D 31 00 00     CMP byte ptr [0x140a54e91],0x0
1407440f7:  74 69                    JZ 0x140744162
1407440f9:  C6 05 91 0D 31 00 00     MOV byte ptr [0x140a54e91],0x0
140744100:  0F 28 85 30 01 00 00     MOVAPS XMM0,xmmword ptr [RBP + 0x130]
140744107:  66 0F 7F 85 40 01 00 00  MOVDQA xmmword ptr [RBP + 0x140],XMM0
14074410f:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744116:  48 8B D8                 MOV RBX,RAX
140744119:  4E 8B 04 E8              MOV R8,qword ptr [RAX + R13*0x8]
14074411d:  48 8D 85 40 01 00 00     LEA RAX,[RBP + 0x140]
140744124:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140744129:  C6 44 24 20 00           MOV byte ptr [RSP + 0x20],0x0
14074412e:  45 33 C9                 XOR R9D,R9D
140744131:  4D 8B 40 08              MOV R8,qword ptr [R8 + 0x8]
140744135:  48 8B D6                 MOV RDX,RSI
140744138:  E8 13 C5 9E FF           CALL 0x140130650
14074413d:  0F B6 C8                 MOVZX ECX,AL
140744140:  4A 8B 04 EB              MOV RAX,qword ptr [RBX + R13*0x8]
140744144:  88 08                    MOV byte ptr [RAX],CL
140744146:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
14074414d:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744151:  80 39 00                 CMP byte ptr [RCX],0x0
140744154:  74 0C                    JZ 0x140744162
140744156:  48 85 F6                 TEST RSI,RSI
140744159:  74 07                    JZ 0x140744162
14074415b:  C6 86 50 0D 00 00 01     MOV byte ptr [RSI + 0xd50],0x1
140744162:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140744167:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14074416e:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140744175:  41 0F 28 DB              MOVAPS XMM3,XMM11
140744179:  41 0F 28 D4              MOVAPS XMM2,XMM12
14074417d:  41 0F 28 CC              MOVAPS XMM1,XMM12
140744181:  48 8D 0D D8 9E 27 00     LEA RCX,[0x1409be060]
140744188:  FF 15 D2 86 12 00        CALL qword ptr [0x14086c860]
14074418e:  F3 0F 58 35 5A 68 1C 00  ADDSS XMM6,dword ptr [0x14090a9f0]
140744196:  F3 0F 2C DE              CVTTSS2SI EBX,XMM6
14074419a:  FF 45 88                 INC dword ptr [RBP + -0x78]
14074419d:  48 FF 45 C8              INC qword ptr [RBP + -0x38]
1407441a1:  E9 70 01 00 00           JMP 0x140744316
1407441a6:  4D 85 E4                 TEST R12,R12
1407441a9:  0F 84 5A 01 00 00        JZ 0x140744309
1407441af:  48 63 DB                 MOVSXD RBX,EBX
1407441b2:  49 3B DC                 CMP RBX,R12
1407441b5:  0F 83 4E 01 00 00        JNC 0x140744309
1407441bb:  48 8B 45 C8              MOV RAX,qword ptr [RBP + -0x38]
1407441bf:  48 8B 4D E0              MOV RCX,qword ptr [RBP + -0x20]
1407441c3:  44 39 3C 81              CMP dword ptr [RCX + RAX*0x4],R15D
1407441c7:  0F 85 3C 01 00 00        JNZ 0x140744309
1407441cd:  49 8B 8E 00 B5 00 00     MOV RCX,qword ptr [R14 + 0xb500]
1407441d4:  48 8B 01                 MOV RAX,qword ptr [RCX]
1407441d7:  4C 8B 05 2A 6B 2A 00     MOV R8,qword ptr [0x1409ead08]
1407441de:  33 D2                    XOR EDX,EDX
1407441e0:  FF 50 70                 CALL qword ptr [RAX + 0x70]
1407441e3:  F3 0F 10 0D 9D DE 24 00  MOVSS XMM1,dword ptr [0x140992088]
1407441eb:  0F 28 C1                 MOVAPS XMM0,XMM1
1407441ee:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1407441f3:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
1407441f8:  F3 0F 5C D0              SUBSS XMM2,XMM0
1407441fc:  66 0F 6E B5 48 01 00 00  MOVD XMM6,dword ptr [RBP + 0x148]
140744204:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
140744207:  0F 28 DE                 MOVAPS XMM3,XMM6
14074420a:  F3 0F 58 5D 84           ADDSS XMM3,dword ptr [RBP + -0x7c]
14074420f:  0F 28 C1                 MOVAPS XMM0,XMM1
140744212:  F3 0F 59 05 D6 67 1C 00  MULSS XMM0,dword ptr [0x14090a9f0]
14074421a:  F3 0F 58 D8              ADDSS XMM3,XMM0
14074421e:  C7 85 A4 00 00 00 00 00 00 00 MOV dword ptr [RBP + 0xa4],0x0
140744228:  0F 28 C3                 MOVAPS XMM0,XMM3
14074422b:  0F 14 C2                 UNPCKLPS XMM0,XMM2
14074422e:  F2 0F 11 05 BA A0 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140744236:  8B 85 A4 00 00 00        MOV EAX,dword ptr [RBP + 0xa4]
14074423c:  89 05 B6 A0 27 00        MOV dword ptr [0x1409be2f8],EAX
140744242:  F3 0F 59 0D 82 67 1C 00  MULSS XMM1,dword ptr [0x14090a9cc]
14074424a:  F3 0F 11 0D 96 A0 27 00  MOVSS dword ptr [0x1409be2e8],XMM1
140744252:  F3 0F 11 0D 92 A0 27 00  MOVSS dword ptr [0x1409be2ec],XMM1
14074425a:  0F 28 05 5F 82 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140744261:  0F 11 05 A4 A0 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140744268:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
14074426f:  4D 8D A6 B0 AD 00 00     LEA R12,[R14 + 0xadb0]
140744276:  41 83 F9 FF              CMP R9D,-0x1
14074427a:  74 07                    JZ 0x140744283
14074427c:  45 3B 4C 24 40           CMP R9D,dword ptr [R12 + 0x40]
140744281:  75 0B                    JNZ 0x14074428e
140744283:  0F 28 CB                 MOVAPS XMM1,XMM3
140744286:  49 8B CC                 MOV RCX,R12
140744289:  E8 62 1F D0 FF           CALL 0x1404461f0
14074428e:  48 8D 95 18 01 00 00     LEA RDX,[RBP + 0x118]
140744295:  48 8D 0D F4 08 31 00     LEA RCX,[0x140a54b90]
14074429c:  FF 15 C6 85 12 00        CALL qword ptr [0x14086c868]
1407442a2:  48 8B D0                 MOV RDX,RAX
1407442a5:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407442a9:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407442ad:  48 8D 0D AC 9D 27 00     LEA RCX,[0x1409be060]
1407442b4:  FF 15 96 85 12 00        CALL qword ptr [0x14086c850]
1407442ba:  84 C0                    TEST AL,AL
1407442bc:  0F 84 A0 FE FF FF        JZ 0x140744162
1407442c2:  48 8B D3                 MOV RDX,RBX
1407442c5:  48 C1 EA 05              SHR RDX,0x5
1407442c9:  83 E3 1F                 AND EBX,0x1f
1407442cc:  8B CB                    MOV ECX,EBX
1407442ce:  B8 01 00 00 00           MOV EAX,0x1
1407442d3:  D3 E0                    SHL EAX,CL
1407442d5:  48 8B 4D 70              MOV RCX,qword ptr [RBP + 0x70]
1407442d9:  85 04 91                 TEST dword ptr [RCX + RDX*0x4],EAX
1407442dc:  48 8D 0D AD 32 25 00     LEA RCX,[0x140997590]
1407442e3:  BA C4 05 00 00           MOV EDX,0x5c4
1407442e8:  75 05                    JNZ 0x1407442ef
1407442ea:  BA B4 05 00 00           MOV EDX,0x5b4
1407442ef:  FF 15 9B 85 12 00        CALL qword ptr [0x14086c890]
1407442f5:  48 8B D0                 MOV RDX,RAX
1407442f8:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
1407442ff:  E8 FC 15 8D FF           CALL 0x140015900
140744304:  E9 59 FE FF FF           JMP 0x140744162
140744309:  8B 9D 48 01 00 00        MOV EBX,dword ptr [RBP + 0x148]
14074430f:  4D 8D A6 B0 AD 00 00     LEA R12,[R14 + 0xadb0]
140744316:  49 8B 8E B0 B3 00 00     MOV RCX,qword ptr [R14 + 0xb3b0]
14074431d:  48 8B 01                 MOV RAX,qword ptr [RCX]
140744320:  4C 8B 05 E1 69 2A 00     MOV R8,qword ptr [0x1409ead08]
140744327:  33 D2                    XOR EDX,EDX
140744329:  FF 50 70                 CALL qword ptr [RAX + 0x70]
14074432c:  F3 0F 10 1D 54 DD 24 00  MOVSS XMM3,dword ptr [0x140992088]
140744334:  0F 28 C3                 MOVAPS XMM0,XMM3
140744337:  F3 41 0F 59 C5           MULSS XMM0,XMM13
14074433c:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140744341:  F3 0F 5C D0              SUBSS XMM2,XMM0
140744345:  66 0F 6E F3              MOVD XMM6,EBX
140744349:  0F 5B F6                 CVTDQ2PS XMM6,XMM6
14074434c:  0F 28 CE                 MOVAPS XMM1,XMM6
14074434f:  F3 0F 58 4D 84           ADDSS XMM1,dword ptr [RBP + -0x7c]
140744354:  0F 28 C3                 MOVAPS XMM0,XMM3
140744357:  F3 0F 59 05 91 66 1C 00  MULSS XMM0,dword ptr [0x14090a9f0]
14074435f:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744363:  0F 28 C1                 MOVAPS XMM0,XMM1
140744366:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140744369:  F2 0F 11 05 7F 9F 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140744371:  8B 85 B0 00 00 00        MOV EAX,dword ptr [RBP + 0xb0]
140744377:  89 05 7B 9F 27 00        MOV dword ptr [0x1409be2f8],EAX
14074437d:  F3 0F 59 1D 47 66 1C 00  MULSS XMM3,dword ptr [0x14090a9cc]
140744385:  F3 0F 11 1D 5B 9F 27 00  MOVSS dword ptr [0x1409be2e8],XMM3
14074438d:  F3 0F 11 1D 57 9F 27 00  MOVSS dword ptr [0x1409be2ec],XMM3
140744395:  0F 11 3D 70 9F 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM7
14074439c:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
1407443a3:  41 83 F9 FF              CMP R9D,-0x1
1407443a7:  74 07                    JZ 0x1407443b0
1407443a9:  45 3B 4C 24 40           CMP R9D,dword ptr [R12 + 0x40]
1407443ae:  75 08                    JNZ 0x1407443b8
1407443b0:  49 8B CC                 MOV RCX,R12
1407443b3:  E8 38 1E D0 FF           CALL 0x1404461f0
1407443b8:  48 8B 5D 98              MOV RBX,qword ptr [RBP + -0x68]
1407443bc:  80 7B 20 00              CMP byte ptr [RBX + 0x20],0x0
1407443c0:  0F 84 D2 00 00 00        JZ 0x140744498
1407443c6:  48 8D 95 C0 00 00 00     LEA RDX,[RBP + 0xc0]
1407443cd:  48 8D 0D BC 07 31 00     LEA RCX,[0x140a54b90]
1407443d4:  FF 15 8E 84 12 00        CALL qword ptr [0x14086c868]
1407443da:  48 8B D0                 MOV RDX,RAX
1407443dd:  41 0F 28 DC              MOVAPS XMM3,XMM12
1407443e1:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407443e5:  48 8D 0D 74 9C 27 00     LEA RCX,[0x1409be060]
1407443ec:  FF 15 5E 84 12 00        CALL qword ptr [0x14086c850]
1407443f2:  84 C0                    TEST AL,AL
1407443f4:  0F 84 9E 00 00 00        JZ 0x140744498
1407443fa:  0F 28 05 BF 80 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
140744401:  0F 11 05 04 9F 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140744408:  BA 43 01 00 00           MOV EDX,0x143
14074440d:  48 8D 0D 7C 31 25 00     LEA RCX,[0x140997590]
140744414:  FF 15 76 84 12 00        CALL qword ptr [0x14086c890]
14074441a:  48 8B D0                 MOV RDX,RAX
14074441d:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
140744424:  E8 D7 14 8D FF           CALL 0x140015900
140744429:  80 3D 61 0A 31 00 00     CMP byte ptr [0x140a54e91],0x0
140744430:  74 66                    JZ 0x140744498
140744432:  C6 05 58 0A 31 00 00     MOV byte ptr [0x140a54e91],0x0
140744439:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
14074443e:  75 4A                    JNZ 0x14074448a
140744440:  48 8D 15 91 7D 1B 00     LEA RDX,[0x1408fc1d8]
140744447:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
14074444e:  FF 15 BC 88 12 00        CALL qword ptr [0x14086cd10]
140744454:  84 C0                    TEST AL,AL
140744456:  74 32                    JZ 0x14074448a
140744458:  48 8D 15 79 7D 1B 00     LEA RDX,[0x1408fc1d8]
14074445f:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
140744466:  FF 15 AC 88 12 00        CALL qword ptr [0x14086cd18]
14074446c:  48 8B D0                 MOV RDX,RAX
14074446f:  33 C0                    XOR EAX,EAX
140744471:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140744476:  41 0F 28 DB              MOVAPS XMM3,XMM11
14074447a:  41 B0 01                 MOV R8B,0x1
14074447d:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
140744484:  FF 15 1E 8E 12 00        CALL qword ptr [0x14086d2a8]
14074448a:  45 33 C0                 XOR R8D,R8D
14074448d:  48 8B D7                 MOV RDX,RDI
140744490:  49 8B CE                 MOV RCX,R14
140744493:  E8 A8 32 C9 FF           CALL 0x1403d7740
140744498:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
14074449d:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
1407444a4:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
1407444ab:  41 0F 28 DB              MOVAPS XMM3,XMM11
1407444af:  41 0F 28 D4              MOVAPS XMM2,XMM12
1407444b3:  41 0F 28 CC              MOVAPS XMM1,XMM12
1407444b7:  48 8D 0D A2 9B 27 00     LEA RCX,[0x1409be060]
1407444be:  FF 15 9C 83 12 00        CALL qword ptr [0x14086c860]
1407444c4:  49 8B 8E A8 B3 00 00     MOV RCX,qword ptr [R14 + 0xb3a8]
1407444cb:  48 8B 01                 MOV RAX,qword ptr [RCX]
1407444ce:  4C 8B 05 33 68 2A 00     MOV R8,qword ptr [0x1409ead08]
1407444d5:  33 D2                    XOR EDX,EDX
1407444d7:  FF 50 70                 CALL qword ptr [RAX + 0x70]
1407444da:  F3 0F 10 1D A6 DB 24 00  MOVSS XMM3,dword ptr [0x140992088]
1407444e2:  0F 28 C3                 MOVAPS XMM0,XMM3
1407444e5:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1407444ea:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
1407444ef:  F3 0F 5C D0              SUBSS XMM2,XMM0
1407444f3:  0F 28 CE                 MOVAPS XMM1,XMM6
1407444f6:  F3 0F 58 4D 84           ADDSS XMM1,dword ptr [RBP + -0x7c]
1407444fb:  0F 28 C3                 MOVAPS XMM0,XMM3
1407444fe:  F3 0F 59 05 26 66 1C 00  MULSS XMM0,dword ptr [0x14090ab2c]
140744506:  F3 0F 58 C8              ADDSS XMM1,XMM0
14074450a:  0F 28 C1                 MOVAPS XMM0,XMM1
14074450d:  0F 14 C2                 UNPCKLPS XMM0,XMM2
140744510:  F2 0F 11 05 D8 9D 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
140744518:  8B 85 98 00 00 00        MOV EAX,dword ptr [RBP + 0x98]
14074451e:  89 05 D4 9D 27 00        MOV dword ptr [0x1409be2f8],EAX
140744524:  F3 0F 59 1D A0 64 1C 00  MULSS XMM3,dword ptr [0x14090a9cc]
14074452c:  F3 0F 11 1D B4 9D 27 00  MOVSS dword ptr [0x1409be2e8],XMM3
140744534:  F3 0F 11 1D B0 9D 27 00  MOVSS dword ptr [0x1409be2ec],XMM3
14074453c:  44 0F 11 05 C8 9D 27 00  MOVUPS xmmword ptr [0x1409be30c],XMM8
140744544:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
14074454b:  41 83 F9 FF              CMP R9D,-0x1
14074454f:  74 07                    JZ 0x140744558
140744551:  45 3B 4C 24 40           CMP R9D,dword ptr [R12 + 0x40]
140744556:  75 08                    JNZ 0x140744560
140744558:  49 8B CC                 MOV RCX,R12
14074455b:  E8 90 1C D0 FF           CALL 0x1404461f0
140744560:  80 7B 20 00              CMP byte ptr [RBX + 0x20],0x0
140744564:  0F 84 F5 00 00 00        JZ 0x14074465f
14074456a:  48 8D 95 D0 00 00 00     LEA RDX,[RBP + 0xd0]
140744571:  48 8D 0D 18 06 31 00     LEA RCX,[0x140a54b90]
140744578:  FF 15 EA 82 12 00        CALL qword ptr [0x14086c868]
14074457e:  48 8B D0                 MOV RDX,RAX
140744581:  41 0F 28 DC              MOVAPS XMM3,XMM12
140744585:  41 0F 28 D4              MOVAPS XMM2,XMM12
140744589:  48 8D 0D D0 9A 27 00     LEA RCX,[0x1409be060]
140744590:  FF 15 BA 82 12 00        CALL qword ptr [0x14086c850]
140744596:  84 C0                    TEST AL,AL
140744598:  0F 84 C1 00 00 00        JZ 0x14074465f
14074459e:  0F 28 05 1B 7F 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
1407445a5:  0F 11 05 60 9D 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
1407445ac:  BA B1 05 00 00           MOV EDX,0x5b1
1407445b1:  48 8D 0D D8 2F 25 00     LEA RCX,[0x140997590]
1407445b8:  FF 15 D2 82 12 00        CALL qword ptr [0x14086c890]
1407445be:  48 8B D0                 MOV RDX,RAX
1407445c1:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
1407445c8:  E8 33 13 8D FF           CALL 0x140015900
1407445cd:  80 3D BD 08 31 00 00     CMP byte ptr [0x140a54e91],0x0
1407445d4:  0F 84 85 00 00 00        JZ 0x14074465f
1407445da:  C6 05 B0 08 31 00 00     MOV byte ptr [0x140a54e91],0x0
1407445e1:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
1407445e6:  75 4A                    JNZ 0x140744632
1407445e8:  48 8D 15 F9 7B 1B 00     LEA RDX,[0x1408fc1e8]
1407445ef:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
1407445f6:  FF 15 14 87 12 00        CALL qword ptr [0x14086cd10]
1407445fc:  84 C0                    TEST AL,AL
1407445fe:  74 32                    JZ 0x140744632
140744600:  48 8D 15 E1 7B 1B 00     LEA RDX,[0x1408fc1e8]
140744607:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
14074460e:  FF 15 04 87 12 00        CALL qword ptr [0x14086cd18]
140744614:  48 8B D0                 MOV RDX,RAX
140744617:  33 C0                    XOR EAX,EAX
140744619:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
14074461e:  41 0F 28 DB              MOVAPS XMM3,XMM11
140744622:  41 B0 01                 MOV R8B,0x1
140744625:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
14074462c:  FF 15 76 8C 12 00        CALL qword ptr [0x14086d2a8]
140744632:  48 8B 45 98              MOV RAX,qword ptr [RBP + -0x68]
140744636:  48 89 44 24 40           MOV qword ptr [RSP + 0x40],RAX
14074463b:  33 C0                    XOR EAX,EAX
14074463d:  48 89 44 24 38           MOV qword ptr [RSP + 0x38],RAX
140744642:  89 44 24 30              MOV dword ptr [RSP + 0x30],EAX
140744646:  89 44 24 28              MOV dword ptr [RSP + 0x28],EAX
14074464a:  89 44 24 20              MOV dword ptr [RSP + 0x20],EAX
14074464e:  45 33 C9                 XOR R9D,R9D
140744651:  4C 8B C7                 MOV R8,RDI
140744654:  8D 50 02                 LEA EDX,[RAX + 0x2]
140744657:  49 8B CE                 MOV RCX,R14
14074465a:  E8 41 9C FA FF           CALL 0x1406ee2a0
14074465f:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140744664:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14074466b:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140744672:  41 0F 28 DB              MOVAPS XMM3,XMM11
140744676:  41 0F 28 D4              MOVAPS XMM2,XMM12
14074467a:  41 0F 28 CC              MOVAPS XMM1,XMM12
14074467e:  48 8D 0D DB 99 27 00     LEA RCX,[0x1409be060]
140744685:  FF 15 D5 81 12 00        CALL qword ptr [0x14086c860]
14074468b:  49 8B 8E 20 B3 00 00     MOV RCX,qword ptr [R14 + 0xb320]
140744692:  48 8B 01                 MOV RAX,qword ptr [RCX]
140744695:  4C 8B 05 6C 66 2A 00     MOV R8,qword ptr [0x1409ead08]
14074469c:  33 D2                    XOR EDX,EDX
14074469e:  FF 50 70                 CALL qword ptr [RAX + 0x70]
1407446a1:  F3 0F 10 1D DF D9 24 00  MOVSS XMM3,dword ptr [0x140992088]
1407446a9:  0F 28 C3                 MOVAPS XMM0,XMM3
1407446ac:  F3 41 0F 59 C5           MULSS XMM0,XMM13
1407446b1:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
1407446b6:  F3 0F 5C D0              SUBSS XMM2,XMM0
1407446ba:  F3 0F 58 75 84           ADDSS XMM6,dword ptr [RBP + -0x7c]
1407446bf:  0F 28 C3                 MOVAPS XMM0,XMM3
1407446c2:  F3 0F 59 05 32 65 1C 00  MULSS XMM0,dword ptr [0x14090abfc]
1407446ca:  F3 0F 58 F0              ADDSS XMM6,XMM0
1407446ce:  0F 28 C6                 MOVAPS XMM0,XMM6
1407446d1:  0F 14 C2                 UNPCKLPS XMM0,XMM2
1407446d4:  F2 0F 11 05 14 9C 27 00  MOVSD qword ptr [0x1409be2f0],XMM0
1407446dc:  8B 45 00                 MOV EAX,dword ptr [RBP]
1407446df:  89 05 13 9C 27 00        MOV dword ptr [0x1409be2f8],EAX
1407446e5:  F3 0F 59 1D DF 62 1C 00  MULSS XMM3,dword ptr [0x14090a9cc]
1407446ed:  F3 0F 11 1D F3 9B 27 00  MOVSS dword ptr [0x1409be2e8],XMM3
1407446f5:  F3 0F 11 1D EF 9B 27 00  MOVSS dword ptr [0x1409be2ec],XMM3
1407446fd:  44 0F 11 0D 07 9C 27 00  MOVUPS xmmword ptr [0x1409be30c],XMM9
140744705:  45 8B 8E A8 AD 00 00     MOV R9D,dword ptr [R14 + 0xada8]
14074470c:  41 83 F9 FF              CMP R9D,-0x1
140744710:  74 07                    JZ 0x140744719
140744712:  45 3B 4C 24 40           CMP R9D,dword ptr [R12 + 0x40]
140744717:  75 0B                    JNZ 0x140744724
140744719:  0F 28 CE                 MOVAPS XMM1,XMM6
14074471c:  49 8B CC                 MOV RCX,R12
14074471f:  E8 CC 1A D0 FF           CALL 0x1404461f0
140744724:  4C 8B 65 98              MOV R12,qword ptr [RBP + -0x68]
140744728:  41 80 7C 24 20 00        CMP byte ptr [R12 + 0x20],0x0
14074472e:  0F 84 EE 00 00 00        JZ 0x140744822
140744734:  48 8D 55 30              LEA RDX,[RBP + 0x30]
140744738:  48 8D 0D 51 04 31 00     LEA RCX,[0x140a54b90]
14074473f:  FF 15 23 81 12 00        CALL qword ptr [0x14086c868]
140744745:  48 8B D0                 MOV RDX,RAX
140744748:  41 0F 28 DC              MOVAPS XMM3,XMM12
14074474c:  41 0F 28 D4              MOVAPS XMM2,XMM12
140744750:  48 8D 0D 09 99 27 00     LEA RCX,[0x1409be060]
140744757:  FF 15 F3 80 12 00        CALL qword ptr [0x14086c850]
14074475d:  84 C0                    TEST AL,AL
14074475f:  0F 84 BD 00 00 00        JZ 0x140744822
140744765:  0F 28 05 54 7D 1C 00     MOVAPS XMM0,xmmword ptr [0x14090c4c0]
14074476c:  0F 11 05 99 9B 27 00     MOVUPS xmmword ptr [0x1409be30c],XMM0
140744773:  BA 0A 08 00 00           MOV EDX,0x80a
140744778:  48 8D 0D 11 2E 25 00     LEA RCX,[0x140997590]
14074477f:  FF 15 0B 81 12 00        CALL qword ptr [0x14086c890]
140744785:  48 8B D0                 MOV RDX,RAX
140744788:  49 8D 8E A0 D5 00 00     LEA RCX,[R14 + 0xd5a0]
14074478f:  E8 6C 11 8D FF           CALL 0x140015900
140744794:  80 3D F6 06 31 00 00     CMP byte ptr [0x140a54e91],0x0
14074479b:  0F 84 81 00 00 00        JZ 0x140744822
1407447a1:  C6 05 E9 06 31 00 00     MOV byte ptr [0x140a54e91],0x0
1407447a8:  41 80 7E 14 00           CMP byte ptr [R14 + 0x14],0x0
1407447ad:  75 4A                    JNZ 0x1407447f9
1407447af:  48 8D 15 D2 79 1B 00     LEA RDX,[0x1408fc188]
1407447b6:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
1407447bd:  FF 15 4D 85 12 00        CALL qword ptr [0x14086cd10]
1407447c3:  84 C0                    TEST AL,AL
1407447c5:  74 32                    JZ 0x1407447f9
1407447c7:  48 8D 15 BA 79 1B 00     LEA RDX,[0x1408fc188]
1407447ce:  49 8D 8E 10 FD 00 00     LEA RCX,[R14 + 0xfd10]
1407447d5:  FF 15 3D 85 12 00        CALL qword ptr [0x14086cd18]
1407447db:  48 8B D0                 MOV RDX,RAX
1407447de:  33 C0                    XOR EAX,EAX
1407447e0:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
1407447e5:  41 0F 28 DB              MOVAPS XMM3,XMM11
1407447e9:  41 B0 01                 MOV R8B,0x1
1407447ec:  49 8B 8E 08 FD 00 00     MOV RCX,qword ptr [R14 + 0xfd08]
1407447f3:  FF 15 AF 8A 12 00        CALL qword ptr [0x14086d2a8]
1407447f9:  4C 8B C7                 MOV R8,RDI
1407447fc:  48 8B D6                 MOV RDX,RSI
1407447ff:  E8 7C 36 A8 FF           CALL 0x1401c7e80
140744804:  45 8B C7                 MOV R8D,R15D
140744807:  48 8B D6                 MOV RDX,RSI
14074480a:  E8 31 E3 A1 FF           CALL 0x140162b40
14074480f:  41 FF CF                 DEC R15D
140744812:  49 FF CD                 DEC R13
140744815:  F3 0F 10 15 0B 61 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
14074481d:  E9 B4 04 00 00           JMP 0x140744cd6
140744822:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140744827:  F3 44 0F 11 64 24 28     MOVSS dword ptr [RSP + 0x28],XMM12
14074482e:  F3 44 0F 11 5C 24 20     MOVSS dword ptr [RSP + 0x20],XMM11
140744835:  41 0F 28 DB              MOVAPS XMM3,XMM11
140744839:  41 0F 28 D4              MOVAPS XMM2,XMM12
14074483d:  41 0F 28 CC              MOVAPS XMM1,XMM12
140744841:  48 8D 0D 18 98 27 00     LEA RCX,[0x1409be060]
140744848:  FF 15 12 80 12 00        CALL qword ptr [0x14086c860]
14074484e:  43 8B 9C AC 10 03 00 00  MOV EBX,dword ptr [R12 + R13*0x4 + 0x310]
140744856:  83 FB 01                 CMP EBX,0x1
140744859:  0F 94 C0                 SETZ AL
14074485c:  F3 0F 10 0D 24 D8 24 00  MOVSS XMM1,dword ptr [0x140992088]
140744864:  0F 28 C1                 MOVAPS XMM0,XMM1
140744867:  F3 41 0F 59 C5           MULSS XMM0,XMM13
14074486c:  F3 0F 10 5D 80           MOVSS XMM3,dword ptr [RBP + -0x80]
140744871:  F3 0F 5C D8              SUBSS XMM3,XMM0
140744875:  F3 0F 59 0D AB 5F 1C 00  MULSS XMM1,dword ptr [0x14090a828]
14074487d:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
140744882:  F3 0F 5C D1              SUBSS XMM2,XMM1
140744886:  F3 0F 10 05 12 61 1C 00  MOVSS XMM0,dword ptr [0x14090a9a0]
14074488e:  F3 0F 11 44 24 28        MOVSS dword ptr [RSP + 0x28],XMM0
140744894:  88 44 24 20              MOV byte ptr [RSP + 0x20],AL
140744898:  49 8B D4                 MOV RDX,R12
14074489b:  49 8B CE                 MOV RCX,R14
14074489e:  E8 2D 01 FB FF           CALL 0x1406f49d0
1407448a3:  45 33 C0                 XOR R8D,R8D
1407448a6:  41 8B C8                 MOV ECX,R8D
1407448a9:  84 C0                    TEST AL,AL
1407448ab:  0F 95 C1                 SETNZ CL
1407448ae:  43 89 8C AC 10 03 00 00  MOV dword ptr [R12 + R13*0x4 + 0x310],ECX
1407448b6:  3B D9                    CMP EBX,ECX
1407448b8:  74 56                    JZ 0x140744910
1407448ba:  44 38 05 EC 02 31 00     CMP byte ptr [0x140a54bad],R8B
1407448c1:  74 4D                    JZ 0x140744910
1407448c3:  41 8B C8                 MOV ECX,R8D
1407448c6:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407448cd:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
1407448d4:  48 C1 F8 03              SAR RAX,0x3
1407448d8:  48 85 C0                 TEST RAX,RAX
1407448db:  74 33                    JZ 0x140744910
1407448dd:  41 8B D0                 MOV EDX,R8D
1407448e0:  44 3B F9                 CMP R15D,ECX
1407448e3:  74 10                    JZ 0x1407448f5
1407448e5:  43 8B 84 AC 10 03 00 00  MOV EAX,dword ptr [R12 + R13*0x4 + 0x310]
1407448ed:  41 89 84 94 10 03 00 00  MOV dword ptr [R12 + RDX*0x4 + 0x310],EAX
1407448f5:  FF C1                    INC ECX
1407448f7:  48 8B 86 40 0D 00 00     MOV RAX,qword ptr [RSI + 0xd40]
1407448fe:  48 2B 86 38 0D 00 00     SUB RAX,qword ptr [RSI + 0xd38]
140744905:  48 C1 F8 03              SAR RAX,0x3
140744909:  8B D1                    MOV EDX,ECX
14074490b:  48 3B D0                 CMP RDX,RAX
14074490e:  72 D0                    JC 0x1407448e0
140744910:  F3 0F 10 05 70 D7 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744918:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
14074491d:  47 39 84 AC 10 03 00 00  CMP dword ptr [R12 + R13*0x4 + 0x310],R8D
140744925:  0F 85 96 03 00 00        JNZ 0x140744cc1
14074492b:  F3 0F 59 05 BD 60 1C 00  MULSS XMM0,dword ptr [0x14090a9f0]
140744933:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744937:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
14074493c:  4C 8B 7D 08              MOV R15,qword ptr [RBP + 0x8]
140744940:  49 8B DF                 MOV RBX,R15
140744943:  48 89 5D 10              MOV qword ptr [RBP + 0x10],RBX
140744947:  48 8B 4D 40              MOV RCX,qword ptr [RBP + 0x40]
14074494b:  48 89 4D 48              MOV qword ptr [RBP + 0x48],RCX
14074494f:  4C 8D 45 40              LEA R8,[RBP + 0x40]
140744953:  48 8B D7                 MOV RDX,RDI
140744956:  49 8B CE                 MOV RCX,R14
140744959:  E8 72 7E A5 FF           CALL 0x14019c7d0
14074495e:  45 33 D2                 XOR R10D,R10D
140744961:  45 8B E2                 MOV R12D,R10D
140744964:  4C 8B 6D 48              MOV R13,qword ptr [RBP + 0x48]
140744968:  48 8B 4D 40              MOV RCX,qword ptr [RBP + 0x40]
14074496c:  4C 2B E9                 SUB R13,RCX
14074496f:  49 C1 FD 03              SAR R13,0x3
140744973:  4D 85 ED                 TEST R13,R13
140744976:  0F 84 CB 00 00 00        JZ 0x140744a47
14074497c:  41 8B C2                 MOV EAX,R10D
14074497f:  45 8D 72 01              LEA R14D,[R10 + 0x1]
140744983:  0F 1F 40 00              NOP dword ptr [RAX]
140744987:  66 0F 1F 84 00 00 00 00 00 NOP word ptr [RAX + RAX*0x1]
140744990:  48 8D 3C C1              LEA RDI,[RCX + RAX*0x8]
140744994:  4C 8B 0F                 MOV R9,qword ptr [RDI]
140744997:  41 8B D2                 MOV EDX,R10D
14074499a:  4C 8B 45 60              MOV R8,qword ptr [RBP + 0x60]
14074499e:  4C 2B 45 58              SUB R8,qword ptr [RBP + 0x58]
1407449a2:  49 C1 F8 03              SAR R8,0x3
1407449a6:  4D 85 C0                 TEST R8,R8
1407449a9:  0F 84 85 00 00 00        JZ 0x140744a34
1407449af:  49 8B CA                 MOV RCX,R10
1407449b2:  48 8B 45 58              MOV RAX,qword ptr [RBP + 0x58]
1407449b6:  4C 3B 0C C8              CMP R9,qword ptr [RAX + RCX*0x8]
1407449ba:  74 0B                    JZ 0x1407449c7
1407449bc:  FF C2                    INC EDX
1407449be:  8B CA                    MOV ECX,EDX
1407449c0:  49 3B C8                 CMP RCX,R8
1407449c3:  72 ED                    JC 0x1407449b2
1407449c5:  EB 69                    JMP 0x140744a30
1407449c7:  48 3B FB                 CMP RDI,RBX
1407449ca:  73 34                    JNC 0x140744a00
1407449cc:  4C 3B FF                 CMP R15,RDI
1407449cf:  77 2F                    JA 0x140744a00
1407449d1:  49 2B FF                 SUB RDI,R15
1407449d4:  48 C1 FF 03              SAR RDI,0x3
1407449d8:  48 3B 5D 18              CMP RBX,qword ptr [RBP + 0x18]
1407449dc:  75 17                    JNZ 0x1407449f5
1407449de:  49 8B D6                 MOV RDX,R14
1407449e1:  48 8D 4D 08              LEA RCX,[RBP + 0x8]
1407449e5:  E8 16 CD 96 FF           CALL 0x1400b1700
1407449ea:  48 8B 5D 10              MOV RBX,qword ptr [RBP + 0x10]
1407449ee:  4C 8B 7D 08              MOV R15,qword ptr [RBP + 0x8]
1407449f2:  45 33 D2                 XOR R10D,R10D
1407449f5:  48 85 DB                 TEST RBX,RBX
1407449f8:  74 2E                    JZ 0x140744a28
1407449fa:  49 8B 04 FF              MOV RAX,qword ptr [R15 + RDI*0x8]
1407449fe:  EB 25                    JMP 0x140744a25
140744a00:  48 3B 5D 18              CMP RBX,qword ptr [RBP + 0x18]
140744a04:  75 17                    JNZ 0x140744a1d
140744a06:  49 8B D6                 MOV RDX,R14
140744a09:  48 8D 4D 08              LEA RCX,[RBP + 0x8]
140744a0d:  E8 EE CC 96 FF           CALL 0x1400b1700
140744a12:  48 8B 5D 10              MOV RBX,qword ptr [RBP + 0x10]
140744a16:  4C 8B 7D 08              MOV R15,qword ptr [RBP + 0x8]
140744a1a:  45 33 D2                 XOR R10D,R10D
140744a1d:  48 85 DB                 TEST RBX,RBX
140744a20:  74 06                    JZ 0x140744a28
140744a22:  48 8B 07                 MOV RAX,qword ptr [RDI]
140744a25:  48 89 03                 MOV qword ptr [RBX],RAX
140744a28:  48 83 C3 08              ADD RBX,0x8
140744a2c:  48 89 5D 10              MOV qword ptr [RBP + 0x10],RBX
140744a30:  48 8B 4D 40              MOV RCX,qword ptr [RBP + 0x40]
140744a34:  41 FF C4                 INC R12D
140744a37:  41 8B C4                 MOV EAX,R12D
140744a3a:  49 3B C5                 CMP RAX,R13
140744a3d:  0F 82 4D FF FF FF        JC 0x140744990
140744a43:  4C 8B 75 20              MOV R14,qword ptr [RBP + 0x20]
140744a47:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744a4e:  4C 8B 6D D0              MOV R13,qword ptr [RBP + -0x30]
140744a52:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744a56:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744a5a:  48 8B 58 10              MOV RBX,qword ptr [RAX + 0x10]
140744a5e:  48 2B 58 08              SUB RBX,qword ptr [RAX + 0x8]
140744a62:  48 C1 FB 04              SAR RBX,0x4
140744a66:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744a6d:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744a71:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744a75:  44 0F B6 38              MOVZX R15D,byte ptr [RAX]
140744a79:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744a80:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744a84:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744a88:  48 8B B8 D8 00 00 00     MOV RDI,qword ptr [RAX + 0xd8]
140744a8f:  48 2B B8 D0 00 00 00     SUB RDI,qword ptr [RAX + 0xd0]
140744a96:  48 C1 FF 04              SAR RDI,0x4
140744a9a:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744aa1:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744aa5:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744aa9:  44 0F B6 A0 C8 00 00 00  MOVZX R12D,byte ptr [RAX + 0xc8]
140744ab1:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744ab8:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744abc:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
140744ac0:  48 83 B8 18 03 00 00 00  CMP qword ptr [RAX + 0x318],0x0
140744ac8:  74 24                    JZ 0x140744aee
140744aca:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744ad1:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744ad5:  48 8B 41 08              MOV RAX,qword ptr [RCX + 0x8]
140744ad9:  48 8B 88 18 03 00 00     MOV RCX,qword ptr [RAX + 0x318]
140744ae0:  83 B9 60 03 00 00 14     CMP dword ptr [RCX + 0x360],0x14
140744ae7:  75 05                    JNZ 0x140744aee
140744ae9:  45 33 C0                 XOR R8D,R8D
140744aec:  EB 03                    JMP 0x140744af1
140744aee:  41 B0 01                 MOV R8B,0x1
140744af1:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744af8:  48 8B C8                 MOV RCX,RAX
140744afb:  4A 8B 04 E8              MOV RAX,qword ptr [RAX + R13*0x8]
140744aff:  48 8B 50 10              MOV RDX,qword ptr [RAX + 0x10]
140744b03:  48 81 C2 C8 00 00 00     ADD RDX,0xc8
140744b0a:  4E 8B 0C E9              MOV R9,qword ptr [RCX + R13*0x8]
140744b0e:  C6 44 24 58 01           MOV byte ptr [RSP + 0x58],0x1
140744b13:  C6 44 24 48 00           MOV byte ptr [RSP + 0x48],0x0
140744b18:  44 88 44 24 40           MOV byte ptr [RSP + 0x40],R8B
140744b1d:  C6 44 24 38 00           MOV byte ptr [RSP + 0x38],0x0
140744b22:  48 8D 45 08              LEA RAX,[RBP + 0x8]
140744b26:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140744b2b:  48 8B 45 98              MOV RAX,qword ptr [RBP + -0x68]
140744b2f:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140744b34:  48 89 54 24 20           MOV qword ptr [RSP + 0x20],RDX
140744b39:  4D 8B 49 10              MOV R9,qword ptr [R9 + 0x10]
140744b3d:  F3 0F 10 55 80           MOVSS XMM2,dword ptr [RBP + -0x80]
140744b42:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140744b47:  49 8B CE                 MOV RCX,R14
140744b4a:  E8 91 74 0A 00           CALL 0x1407ebfe0
140744b4f:  0F 28 D0                 MOVAPS XMM2,XMM0
140744b52:  80 BE 50 0D 00 00 00     CMP byte ptr [RSI + 0xd50],0x0
140744b59:  0F 85 92 00 00 00        JNZ 0x140744bf1
140744b5f:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744b66:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744b6a:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744b6e:  48 8B 48 10              MOV RCX,qword ptr [RAX + 0x10]
140744b72:  48 2B 48 08              SUB RCX,qword ptr [RAX + 0x8]
140744b76:  48 C1 F9 04              SAR RCX,0x4
140744b7a:  48 63 C3                 MOVSXD RAX,EBX
140744b7d:  48 3B C1                 CMP RAX,RCX
140744b80:  75 59                    JNZ 0x140744bdb
140744b82:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744b89:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744b8d:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744b91:  44 3A 38                 CMP R15B,byte ptr [RAX]
140744b94:  75 45                    JNZ 0x140744bdb
140744b96:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744b9d:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744ba1:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744ba5:  48 8B 88 D8 00 00 00     MOV RCX,qword ptr [RAX + 0xd8]
140744bac:  48 2B 88 D0 00 00 00     SUB RCX,qword ptr [RAX + 0xd0]
140744bb3:  48 C1 F9 04              SAR RCX,0x4
140744bb7:  48 63 C7                 MOVSXD RAX,EDI
140744bba:  48 3B C1                 CMP RAX,RCX
140744bbd:  75 1C                    JNZ 0x140744bdb
140744bbf:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744bc6:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744bca:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744bce:  44 3A A0 C8 00 00 00     CMP R12B,byte ptr [RAX + 0xc8]
140744bd5:  75 04                    JNZ 0x140744bdb
140744bd7:  32 C0                    XOR AL,AL
140744bd9:  EB 02                    JMP 0x140744bdd
140744bdb:  B0 01                    MOV AL,0x1
140744bdd:  88 86 50 0D 00 00        MOV byte ptr [RSI + 0xd50],AL
140744be3:  84 C0                    TEST AL,AL
140744be5:  74 0A                    JZ 0x140744bf1
140744be7:  C7 86 C0 0F 00 00 00 00 40 40 MOV dword ptr [RSI + 0xfc0],0x40400000
140744bf1:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744bf8:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744bfc:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744c00:  F3 0F 10 40 04           MOVSS XMM0,dword ptr [RAX + 0x4]
140744c05:  0F 5A C0                 CVTPS2PD XMM0,XMM0
140744c08:  F2 0F 11 45 C0           MOVSD qword ptr [RBP + -0x40],XMM0
140744c0d:  0F 57 C9                 XORPS XMM1,XMM1
140744c10:  66 0F 2F C8              COMISD XMM1,XMM0
140744c14:  76 0A                    JBE 0x140744c20
140744c16:  0F 28 C1                 MOVAPS XMM0,XMM1
140744c19:  F2 0F 11 4D C0           MOVSD qword ptr [RBP + -0x40],XMM1
140744c1e:  EB 10                    JMP 0x140744c30
140744c20:  66 41 0F 2F C6           COMISD XMM0,XMM14
140744c25:  76 09                    JBE 0x140744c30
140744c27:  41 0F 28 C6              MOVAPS XMM0,XMM14
140744c2b:  F2 0F 11 45 C0           MOVSD qword ptr [RBP + -0x40],XMM0
140744c30:  F2 0F 5A C0              CVTSD2SS XMM0,XMM0
140744c34:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744c3b:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744c3f:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744c43:  F3 0F 11 40 04           MOVSS dword ptr [RAX + 0x4],XMM0
140744c48:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744c4f:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744c53:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744c57:  F3 0F 10 80 CC 00 00 00  MOVSS XMM0,dword ptr [RAX + 0xcc]
140744c5f:  0F 5A C0                 CVTPS2PD XMM0,XMM0
140744c62:  F2 0F 11 45 C0           MOVSD qword ptr [RBP + -0x40],XMM0
140744c67:  F2 0F 10 0D D9 53 1C 00  MOVSD XMM1,qword ptr [0x14090a048]
140744c6f:  66 0F 2F C8              COMISD XMM1,XMM0
140744c73:  76 0A                    JBE 0x140744c7f
140744c75:  0F 28 C1                 MOVAPS XMM0,XMM1
140744c78:  F2 0F 11 4D C0           MOVSD qword ptr [RBP + -0x40],XMM1
140744c7d:  EB 10                    JMP 0x140744c8f
140744c7f:  66 41 0F 2F C7           COMISD XMM0,XMM15
140744c84:  76 09                    JBE 0x140744c8f
140744c86:  41 0F 28 C7              MOVAPS XMM0,XMM15
140744c8a:  F2 0F 11 45 C0           MOVSD qword ptr [RBP + -0x40],XMM0
140744c8f:  F2 0F 5A C0              CVTSD2SS XMM0,XMM0
140744c93:  48 8B 86 38 0D 00 00     MOV RAX,qword ptr [RSI + 0xd38]
140744c9a:  4A 8B 0C E8              MOV RCX,qword ptr [RAX + R13*0x8]
140744c9e:  48 8B 41 10              MOV RAX,qword ptr [RCX + 0x10]
140744ca2:  F3 0F 11 80 CC 00 00 00  MOVSS dword ptr [RAX + 0xcc],XMM0
140744caa:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140744caf:  F3 0F 58 CA              ADDSS XMM1,XMM2
140744cb3:  F3 0F 10 15 6D 5C 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
140744cbb:  44 8B 7D 90              MOV R15D,dword ptr [RBP + -0x70]
140744cbf:  EB 10                    JMP 0x140744cd1
140744cc1:  F3 0F 10 15 5F 5C 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
140744cc9:  F3 0F 59 C2              MULSS XMM0,XMM2
140744ccd:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744cd1:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140744cd6:  41 FF C7                 INC R15D
140744cd9:  44 89 7D 90              MOV dword ptr [RBP + -0x70],R15D
140744cdd:  49 FF C5                 INC R13
140744ce0:  4C 89 6D D0              MOV qword ptr [RBP + -0x30],R13
140744ce4:  48 8B 8E 40 0D 00 00     MOV RCX,qword ptr [RSI + 0xd40]
140744ceb:  48 2B 8E 38 0D 00 00     SUB RCX,qword ptr [RSI + 0xd38]
140744cf2:  48 C1 F9 03              SAR RCX,0x3
140744cf6:  49 63 C7                 MOVSXD RAX,R15D
140744cf9:  48 3B C1                 CMP RAX,RCX
140744cfc:  4C 8B 65 B8              MOV R12,qword ptr [RBP + -0x48]
140744d00:  8B 5D 88                 MOV EBX,dword ptr [RBP + -0x78]
140744d03:  0F 82 68 F0 FF FF        JC 0x140743d71
140744d09:  4C 8B 6D 50              MOV R13,qword ptr [RBP + 0x50]
140744d0d:  48 8B 7D 98              MOV RDI,qword ptr [RBP + -0x68]
140744d11:  E9 9B 00 00 00           JMP 0x140744db1
140744d16:  F3 0F 10 15 0A 5C 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
140744d1e:  E9 8E 00 00 00           JMP 0x140744db1
140744d23:  BA 0C 08 00 00           MOV EDX,0x80c
140744d28:  48 8D 0D 61 28 25 00     LEA RCX,[0x140997590]
140744d2f:  FF 15 5B 7B 12 00        CALL qword ptr [0x14086c890]
140744d35:  F3 0F 10 05 4B D3 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744d3d:  0F 28 D8                 MOVAPS XMM3,XMM0
140744d40:  F3 41 0F 59 DD           MULSS XMM3,XMM13
140744d45:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
140744d4a:  F3 41 0F 59 C6           MULSS XMM0,XMM14
140744d4f:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
140744d54:  F3 0F 5C D0              SUBSS XMM2,XMM0
140744d58:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140744d5d:  48 8D 05 34 74 1B 00     LEA RAX,[0x1408fc198]
140744d64:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140744d69:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
140744d71:  66 41 0F 7E D9           MOVD R9D,XMM3
140744d76:  66 41 0F 7E D0           MOVD R8D,XMM2
140744d7b:  48 8B 15 9E 04 25 00     MOV RDX,qword ptr [0x140995220]
140744d82:  48 8D 0D 27 22 25 00     LEA RCX,[0x140996fb0]
140744d89:  FF 15 F1 7A 12 00        CALL qword ptr [0x14086c880]
140744d8f:  F3 0F 10 05 F1 D2 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744d97:  F3 0F 10 15 89 5B 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
140744d9f:  F3 0F 59 C2              MULSS XMM0,XMM2
140744da3:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140744da8:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744dac:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140744db1:  F3 0F 10 05 CF D2 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744db9:  F3 0F 59 C2              MULSS XMM0,XMM2
140744dbd:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140744dc2:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744dc6:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140744dcb:  48 8B 4D 40              MOV RCX,qword ptr [RBP + 0x40]
140744dcf:  48 85 C9                 TEST RCX,RCX
140744dd2:  74 16                    JZ 0x140744dea
140744dd4:  4C 2B E9                 SUB R13,RCX
140744dd7:  49 C1 FD 03              SAR R13,0x3
140744ddb:  41 B8 08 00 00 00        MOV R8D,0x8
140744de1:  49 8B D5                 MOV RDX,R13
140744de4:  E8 67 CC 8B FF           CALL 0x140001a50
140744de9:  90                       NOP
140744dea:  48 8B 4D 08              MOV RCX,qword ptr [RBP + 0x8]
140744dee:  48 85 C9                 TEST RCX,RCX
140744df1:  0F 84 BE 00 00 00        JZ 0x140744eb5
140744df7:  48 8B 55 18              MOV RDX,qword ptr [RBP + 0x18]
140744dfb:  48 2B D1                 SUB RDX,RCX
140744dfe:  48 C1 FA 03              SAR RDX,0x3
140744e02:  41 B8 08 00 00 00        MOV R8D,0x8
140744e08:  E8 43 CC 8B FF           CALL 0x140001a50
140744e0d:  E9 A3 00 00 00           JMP 0x140744eb5
140744e12:  48 8B 9E 40 0D 00 00     MOV RBX,qword ptr [RSI + 0xd40]
140744e19:  48 2B 9E 38 0D 00 00     SUB RBX,qword ptr [RSI + 0xd38]
140744e20:  48 C1 FB 03              SAR RBX,0x3
140744e24:  BA 14 08 00 00           MOV EDX,0x814
140744e29:  48 8D 0D 60 27 25 00     LEA RCX,[0x140997590]
140744e30:  FF 15 5A 7A 12 00        CALL qword ptr [0x14086c890]
140744e36:  F3 0F 10 05 4A D2 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744e3e:  0F 28 D8                 MOVAPS XMM3,XMM0
140744e41:  F3 41 0F 59 DD           MULSS XMM3,XMM13
140744e46:  F3 0F 58 5D 80           ADDSS XMM3,dword ptr [RBP + -0x80]
140744e4b:  F3 0F 59 05 DD 58 1C 00  MULSS XMM0,dword ptr [0x14090a730]
140744e53:  F3 0F 10 55 84           MOVSS XMM2,dword ptr [RBP + -0x7c]
140744e58:  F3 0F 5C D0              SUBSS XMM2,XMM0
140744e5c:  48 89 5C 24 38           MOV qword ptr [RSP + 0x38],RBX
140744e61:  48 89 44 24 30           MOV qword ptr [RSP + 0x30],RAX
140744e66:  48 8D 05 33 73 1B 00     LEA RAX,[0x1408fc1a0]
140744e6d:  48 89 44 24 28           MOV qword ptr [RSP + 0x28],RAX
140744e72:  C7 44 24 20 00 00 99 FF  MOV dword ptr [RSP + 0x20],0xff990000
140744e7a:  66 41 0F 7E D9           MOVD R9D,XMM3
140744e7f:  66 41 0F 7E D0           MOVD R8D,XMM2
140744e84:  48 8B 15 95 03 25 00     MOV RDX,qword ptr [0x140995220]
140744e8b:  48 8D 0D 1E 21 25 00     LEA RCX,[0x140996fb0]
140744e92:  FF 15 E8 79 12 00        CALL qword ptr [0x14086c880]
140744e98:  F3 0F 10 05 E8 D1 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744ea0:  F3 41 0F 59 C7           MULSS XMM0,XMM15
140744ea5:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140744eaa:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744eae:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140744eb3:  EB 04                    JMP 0x140744eb9
140744eb5:  4C 8B 6D E0              MOV R13,qword ptr [RBP + -0x20]
140744eb9:  F3 0F 10 05 C7 D1 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744ec1:  F3 0F 59 05 37 59 1C 00  MULSS XMM0,dword ptr [0x14090a800]
140744ec9:  F3 0F 10 4D 84           MOVSS XMM1,dword ptr [RBP + -0x7c]
140744ece:  F3 0F 5C C8              SUBSS XMM1,XMM0
140744ed2:  F3 0F 11 4D 84           MOVSS dword ptr [RBP + -0x7c],XMM1
140744ed7:  41 83 BE B0 05 00 00 02  CMP dword ptr [R14 + 0x5b0],0x2
140744edf:  0F 85 6D 01 00 00        JNZ 0x140745052
140744ee5:  48 8B 8E 78 09 00 00     MOV RCX,qword ptr [RSI + 0x978]
140744eec:  48 2B 8E 70 09 00 00     SUB RCX,qword ptr [RSI + 0x970]
140744ef3:  48 B8 25 49 92 24 49 92 24 49 MOV RAX,0x4924924924924925
140744efd:  48 F7 E9                 IMUL RCX
140744f00:  48 C1 FA 06              SAR RDX,0x6
140744f04:  4C 8B CA                 MOV R9,RDX
140744f07:  49 C1 E9 3F              SHR R9,0x3f
140744f0b:  4C 03 CA                 ADD R9,RDX
140744f0e:  41 83 E9 01              SUB R9D,0x1
140744f12:  0F 88 3A 01 00 00        JS 0x140745052
140744f18:  4D 63 C1                 MOVSXD R8,R9D
140744f1b:  49 69 D0 E0 00 00 00     IMUL RDX,R8,0xe0
140744f22:  0F 1F 40 00              NOP dword ptr [RAX]
140744f26:  66 66 0F 1F 84 00 00 00 00 00 NOP dword ptr [RAX + RAX*0x1]
140744f30:  48 8B 86 70 09 00 00     MOV RAX,qword ptr [RSI + 0x970]
140744f37:  48 8B 4C 02 08           MOV RCX,qword ptr [RDX + RAX*0x1 + 0x8]
140744f3c:  48 2B 0C 02              SUB RCX,qword ptr [RDX + RAX*0x1]
140744f40:  48 C1 F9 04              SAR RCX,0x4
140744f44:  48 85 C9                 TEST RCX,RCX
140744f47:  74 51                    JZ 0x140744f9a
140744f49:  48 8B 0C 02              MOV RCX,qword ptr [RDX + RAX*0x1]
140744f4d:  49 8B 86 E0 C2 00 00     MOV RAX,qword ptr [R14 + 0xc2e0]
140744f54:  48 39 01                 CMP qword ptr [RCX],RAX
140744f57:  75 15                    JNZ 0x140744f6e
140744f59:  48 8B 86 18 03 00 00     MOV RAX,qword ptr [RSI + 0x318]
140744f60:  48 85 C0                 TEST RAX,RAX
140744f63:  74 09                    JZ 0x140744f6e
140744f65:  80 B8 4B 07 00 00 00     CMP byte ptr [RAX + 0x74b],0x0
140744f6c:  74 42                    JZ 0x140744fb0
140744f6e:  48 8B 86 70 09 00 00     MOV RAX,qword ptr [RSI + 0x970]
140744f75:  48 8B 0C 02              MOV RCX,qword ptr [RDX + RAX*0x1]
140744f79:  49 8B 86 F8 C2 00 00     MOV RAX,qword ptr [R14 + 0xc2f8]
140744f80:  48 39 01                 CMP qword ptr [RCX],RAX
140744f83:  75 15                    JNZ 0x140744f9a
140744f85:  48 8B 86 18 03 00 00     MOV RAX,qword ptr [RSI + 0x318]
140744f8c:  48 85 C0                 TEST RAX,RAX
140744f8f:  74 09                    JZ 0x140744f9a
140744f91:  80 B8 4B 07 00 00 01     CMP byte ptr [RAX + 0x74b],0x1
140744f98:  74 16                    JZ 0x140744fb0
140744f9a:  41 FF C9                 DEC R9D
140744f9d:  48 81 EA E0 00 00 00     SUB RDX,0xe0
140744fa4:  49 83 E8 01              SUB R8,0x1
140744fa8:  0F 88 A4 00 00 00        JS 0x140745052
140744fae:  EB 80                    JMP 0x140744f30
140744fb0:  49 63 C1                 MOVSXD RAX,R9D
140744fb3:  4C 69 C0 E0 00 00 00     IMUL R8,RAX,0xe0
140744fba:  4C 03 86 70 09 00 00     ADD R8,qword ptr [RSI + 0x970]
140744fc1:  0F 84 8B 00 00 00        JZ 0x140745052
140744fc7:  F3 0F 10 05 B9 D0 24 00  MOVSS XMM0,dword ptr [0x140992088]
140744fcf:  F3 0F 10 15 51 59 1C 00  MOVSS XMM2,dword ptr [0x14090a928]
140744fd7:  F3 0F 59 C2              MULSS XMM0,XMM2
140744fdb:  F3 0F 10 4D 80           MOVSS XMM1,dword ptr [RBP + -0x80]
140744fe0:  F3 0F 58 C8              ADDSS XMM1,XMM0
140744fe4:  F3 0F 11 4D 80           MOVSS dword ptr [RBP + -0x80],XMM1
140744fe9:  C6 44 24 78 00           MOV byte ptr [RSP + 0x78],0x0
140744fee:  C6 44 24 70 00           MOV byte ptr [RSP + 0x70],0x0
140744ff3:  F3 0F 11 54 24 68        MOVSS dword ptr [RSP + 0x68],XMM2
140744ff9:  F3 0F 10 05 5F 59 1C 00  MOVSS XMM0,dword ptr [0x14090a960]
140745001:  F3 0F 11 44 24 60        MOVSS dword ptr [RSP + 0x60],XMM0
140745007:  F3 44 0F 11 64 24 58     MOVSS dword ptr [RSP + 0x58],XMM12
14074500e:  C7 44 24 50 09 00 00 00  MOV dword ptr [RSP + 0x50],0x9
140745016:  8B 86 E8 09 00 00        MOV EAX,dword ptr [RSI + 0x9e8]
14074501c:  89 44 24 48              MOV dword ptr [RSP + 0x48],EAX
140745020:  C7 44 24 40 FF FF FF FF  MOV dword ptr [RSP + 0x40],0xffffffff
140745028:  F3 44 0F 11 64 24 38     MOVSS dword ptr [RSP + 0x38],XMM12
14074502f:  C6 44 24 30 01           MOV byte ptr [RSP + 0x30],0x1
140745034:  33 C0                    XOR EAX,EAX
140745036:  89 44 24 28              MOV dword ptr [RSP + 0x28],EAX
14074503a:  48 8D 45 80              LEA RAX,[RBP + -0x80]
14074503e:  48 89 44 24 20           MOV qword ptr [RSP + 0x20],RAX
140745043:  4C 8D 4D 84              LEA R9,[RBP + -0x7c]
140745047:  48 8B D7                 MOV RDX,RDI
14074504a:  49 8B CE                 MOV RCX,R14
14074504d:  E8 AE 1B FC FF           CALL 0x140706c00
140745052:  4C 8D 4D 80              LEA R9,[RBP + -0x80]
140745056:  4C 8D 45 84              LEA R8,[RBP + -0x7c]
14074505a:  48 8B D7                 MOV RDX,RDI
14074505d:  49 8B CE                 MOV RCX,R14
140745060:  E8 DB FA FA FF           CALL 0x1406f4b40
140745065:  F3 0F 10 45 80           MOVSS XMM0,dword ptr [RBP + -0x80]
14074506a:  F3 0F 11 87 50 02 00 00  MOVSS dword ptr [RDI + 0x250],XMM0
140745072:  48 8B 45 70              MOV RAX,qword ptr [RBP + 0x70]
140745076:  48 85 C0                 TEST RAX,RAX
140745079:  74 1D                    JZ 0x140745098
14074507b:  48 8B 95 80 00 00 00     MOV RDX,qword ptr [RBP + 0x80]
140745082:  48 2B D0                 SUB RDX,RAX
140745085:  48 C1 FA 02              SAR RDX,0x2
140745089:  41 B8 04 00 00 00        MOV R8D,0x4
14074508f:  48 8B C8                 MOV RCX,RAX
140745092:  E8 B9 C9 8B FF           CALL 0x140001a50
140745097:  90                       NOP
140745098:  4D 85 ED                 TEST R13,R13
14074509b:  74 1A                    JZ 0x1407450b7
14074509d:  48 8B 55 D8              MOV RDX,qword ptr [RBP + -0x28]
1407450a1:  49 2B D5                 SUB RDX,R13
1407450a4:  48 C1 FA 02              SAR RDX,0x2
1407450a8:  41 B8 04 00 00 00        MOV R8D,0x4
1407450ae:  49 8B CD                 MOV RCX,R13
1407450b1:  E8 9A C9 8B FF           CALL 0x140001a50
1407450b6:  90                       NOP
1407450b7:  48 8B 4D 58              MOV RCX,qword ptr [RBP + 0x58]
1407450bb:  48 85 C9                 TEST RCX,RCX
1407450be:  74 16                    JZ 0x1407450d6
1407450c0:  48 8B 55 68              MOV RDX,qword ptr [RBP + 0x68]
1407450c4:  48 2B D1                 SUB RDX,RCX
1407450c7:  48 C1 FA 03              SAR RDX,0x3
1407450cb:  41 B8 08 00 00 00        MOV R8D,0x8
1407450d1:  E8 7A C9 8B FF           CALL 0x140001a50
1407450d6:  48 8B 8D 60 0D 00 00     MOV RCX,qword ptr [RBP + 0xd60]
1407450dd:  48 33 CC                 XOR RCX,RSP
1407450e0:  E8 BB 8D 10 00           CALL 0x14084dea0
1407450e5:  4C 8D 9C 24 20 0F 00 00  LEA R11,[RSP + 0xf20]
1407450ed:  49 8B 5B 50              MOV RBX,qword ptr [R11 + 0x50]
1407450f1:  41 0F 28 73 F0           MOVAPS XMM6,xmmword ptr [R11 + -0x10]
1407450f6:  41 0F 28 7B E0           MOVAPS XMM7,xmmword ptr [R11 + -0x20]
1407450fb:  45 0F 28 43 D0           MOVAPS XMM8,xmmword ptr [R11 + -0x30]
140745100:  45 0F 28 4B C0           MOVAPS XMM9,xmmword ptr [R11 + -0x40]

```

---

## 3. Decompilation of FUN_14001E440 (0x14001E440)

```c
// ============================================================
// Function: FUN_14001e440 at 14001e440
// Target address checked: 0x14001E440
// ============================================================

void FUN_14001e440(longlong *param_1,ulonglong param_2)

{
  code *pcVar1;
  ulonglong uVar2;
  longlong lVar3;
  ulonglong uVar4;
  
  if (param_2 <= (ulonglong)(param_1[2] - param_1[1] >> 2)) {
    return;
  }
  lVar3 = param_1[1] - *param_1 >> 2;
  if (0x3fffffffffffffffU - lVar3 < param_2) {
    std::_Xlength_error("vector<T> too long");
    pcVar1 = (code *)swi(3);
    (*pcVar1)();
    return;
  }
  param_2 = param_2 + lVar3;
  uVar4 = param_1[2] - *param_1 >> 2;
  uVar2 = 0;
  if (uVar4 <= 0x3fffffffffffffff - (uVar4 >> 1)) {
    uVar2 = (uVar4 >> 1) + uVar4;
  }
  if (param_2 <= uVar2) {
    param_2 = uVar2;
  }
  FUN_14001e680(param_1,param_2);
  return;
}



```
