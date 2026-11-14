/* Excerpt: 14035f3e0_decomp.c lines 1-80 */
// ============================================================
// Function: FUN_14035f3e0 at 14035f3e0
// Target address checked: 0x14035F3E0
// ============================================================

/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char FUN_14035f3e0(longlong param_1,float *param_2,float *param_3)

{
  undefined8 *puVar1;
  float fVar2;
  undefined8 uVar3;
  char cVar4;
  char cVar5;
  bool bVar6;
  undefined4 uVar7;
  int *piVar8;
  float *pfVar9;
  longlong lVar10;
  longlong lVar11;
  char *pcVar12;
  undefined8 uVar13;
  longlong *plVar14;
  int iVar15;
  int iVar16;
  int iVar17;
  int iVar18;
  float fVar19;
  float fVar20;
  float fVar21;
  float fVar22;
  float fVar23;
  float fVar24;
  float fVar25;
  float fVar26;
  float fVar27;
  undefined1 auStackY_188 [32];
  undefined *puVar28;
  char local_148;
  int local_128;
  int iStack_124;
  int iStack_120;
  int iStack_11c;
  int local_118;
  float *local_110;
  undefined1 local_f0 [16];
  ulonglong local_e0;
  
  fVar25 = DAT_14090a8cc;
  local_e0 = DAT_140992048 ^ (ulonglong)auStackY_188;
  fVar24 = *param_2;
  fVar27 = *param_3;
  local_148 = *(char *)(param_1 + 0xc808);
  if (DAT_140a54bba != '\0') {
    local_148 = local_148 == '\0';
  }
  iVar18 = (int)(fVar24 - DAT_140992088 * DAT_14090a8cc);
  iVar16 = (int)(DAT_140992088 * DAT_14090afd4 + fVar24);
  iVar17 = (int)(fVar27 - DAT_140992088 * DAT_14090a828);
  iVar15 = (int)(DAT_140992088 * DAT_14090aba0 + fVar27);
  local_110 = param_3;
  (**(code **)(**(longlong **)(param_1 + 0xb048) + 0x70))
            (*(longlong **)(param_1 + 0xb048),0,DAT_1409ead08);
  fVar2 = DAT_140909f58;
  _DAT_1409be30c = _DAT_14090bf70;
  uRam00000001409be314 = _UNK_14090bf78;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 =
       CONCAT44((float)(iVar15 + iVar17) * DAT_140909ddc,(float)(iVar16 + iVar18) * DAT_140909ddc);
  _DAT_1409be2e8 = (float)(iVar16 - iVar18);
  _DAT_1409be2ec = (float)(iVar15 - iVar17);
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,DAT_140909f58,DAT_140909f58,0.0,true);
  uVar3 = _UNK_14090c108;
  uVar13 = _DAT_14090c100;
  fVar21 = DAT_14090a9a0;
  puVar1 = (undefined8 *)(param_1 + 0xadb0);
  iVar15 = 0;
  _DAT_1409be30c = _DAT_14090c4c0;
