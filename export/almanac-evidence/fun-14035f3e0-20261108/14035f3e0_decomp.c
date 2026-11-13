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
  uRam00000001409be314 = _UNK_14090c4c8;
  do {
    iVar16 = iVar15 + 0x68b;
    if (iVar15 == 5) {
      iVar16 = 0x2b6;
    }
    lVar10 = *DAT_140995220;
    C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar16);
    piVar8 = (int *)(**(code **)(lVar10 + 0x28))(DAT_140995220,local_f0,fVar24,fVar27);
    fVar23 = DAT_140992088;
    iVar17 = (int)((float)*piVar8 - DAT_140992088);
    _local_128 = CONCAT44((int)((float)piVar8[1] - DAT_140992088),iVar17);
    iVar18 = (int)((float)piVar8[2] + DAT_140992088);
    _iStack_120 = CONCAT44((int)((float)piVar8[3] + DAT_140992088),iVar18);
    cVar4 = FUN_1402fd840(param_1,iVar15,1);
    if (cVar4 != '\0') {
      C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar16);
      puVar28 = &DAT_1408b8638;
      fVar19 = -1.7014118e+38;
    }
    else {
      C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,iVar16);
      puVar28 = &DAT_1408b8640;
      fVar19 = -6.617445e-24;
    }
    bVar6 = cVar4 == '\0';
    C3D_FONTMANAGER::PrintLeftUnicode
              ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,fVar24 - fVar23 * 0.0,
               fVar19,(ulong)puVar28,(wchar_t *)(ulonglong)(uint)(fVar27 - fVar23 * 0.0));
    if ((iVar15 == 4) && (*(int *)(param_1 + 0x10f20) == 4)) {
      bVar6 = false;
LAB_14035f745:
      plVar14 = *(longlong **)(param_1 + 0xb028);
    }
    else {
      if (!bVar6) goto LAB_14035f745;
      plVar14 = *(longlong **)(param_1 + 0xb038);
    }
    (**(code **)(*plVar14 + 0x70))(plVar14,0,DAT_1409ead08);
    _DAT_1409be30c = uVar13;
    uRam00000001409be314 = uVar3;
    if (iVar15 == *(int *)(param_1 + 0x10f20)) {
      _DAT_1409be30c = _DAT_14090c4c0;
      uRam00000001409be314 = _UNK_14090c4c8;
    }
    FUN_1404461f0(puVar1,(float)(iVar18 + iVar17) * DAT_140909ddc,DAT_140992088 * fVar25 + fVar27,6)
    ;
    pfVar9 = (float *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    if (((float)iVar17 < *pfVar9) &&
       (pfVar9 = (float *)C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90),
       *pfVar9 <= (float)iVar18 && (float)iVar18 != *pfVar9)) {
      lVar10 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
      if ((float)iStack_124 < *(float *)(lVar10 + 4)) {
        lVar10 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
        if ((*(float *)(lVar10 + 4) <= (float)iStack_11c &&
             (float)iStack_11c != *(float *)(lVar10 + 4)) && (!bVar6)) {
          _DAT_1409be30c = _DAT_14090c4c0;
          uRam00000001409be314 = _UNK_14090c4c8;
          if (DAT_140a54e91 != '\0') {
            FUN_1403ffad0(param_1,"gui_click",1);
            FUN_1402fd840(param_1,iVar15,0);
          }
        }
      }
    }
    fVar24 = DAT_140992088 * fVar21;
    C3D_PANEL2D::DrawFrom9Patch((C3D_PANEL2D *)&DAT_1409be060,&local_128,fVar24,fVar24);
    fVar19 = DAT_140992088;
    fVar23 = DAT_14090a9f0;
    iVar15 = iVar15 + 1;
    fVar24 = DAT_140992088 * fVar25 + (float)iVar18;
  } while (iVar15 < 6);
  fVar24 = DAT_140992088 * DAT_14090aab8;
  _DAT_1409be30c = _DAT_14090c4c0;
  uRam00000001409be314 = _UNK_14090c4c8;
  *(undefined4 *)(param_1 + 0x1132c) = *(undefined4 *)(param_1 + 0x594);
  fVar27 = fVar27 + fVar24;
  fVar24 = fVar19 * DAT_14090ab14;
  *(undefined4 *)(param_1 + 0x11328) = *(undefined4 *)(param_1 + 0x590);
  lVar10 = *(longlong *)(param_1 + 0x11728);
  lVar11 = (longlong)*(int *)(param_1 + 0x10f18) * 0x400;
  iVar15 = *(int *)(lVar11 + 4 + lVar10);
  iVar16 = *(int *)(lVar11 + lVar10) + 1;
  if (iVar16 == 0x16d) {
    iVar16 = 0;
    iVar15 = iVar15 + 1;
  }
  iVar17 = -1;
  local_118 = -1;
  if (*(int *)(param_1 + 0x10f1c) !=
      (int)(*(longlong *)(param_1 + 0x11730) - *(longlong *)(param_1 + 0x11728) >> 10)) {
    lVar11 = (longlong)*(int *)(param_1 + 0x10f1c) * 0x400;
    local_118 = *(int *)(lVar11 + 4 + lVar10);
    iVar17 = *(int *)(lVar11 + lVar10);
  }
  fVar25 = fVar19;
  cVar4 = '\0';
  if ((*(char *)(param_1 + 0x5e6) != '\0') &&
     (cVar5 = FUN_140335bc0(), fVar25 = DAT_140992088, cVar4 = '\0', cVar5 != '\0')) {
    cVar4 = '\x01';
  }
  fVar26 = DAT_14090a828;
  fVar25 = fVar19 * fVar23 + fVar24 + fVar25 * 0.0;
  FUN_1404461f0(puVar1);
  cVar5 = FUN_140335500(param_1);
  fVar24 = DAT_14090a9cc;
  if ((cVar5 != '\0') && (DAT_140a54e91 != '\0')) {
    DAT_140a54e91 = '\0';
    uVar7 = FUN_140335b10(iVar16);
    fVar19 = DAT_140992088 * fVar24;
    *(undefined4 *)(param_1 + 0xc814) = uVar7;
    *(float *)(param_1 + 0x5e8) = fVar25;
    *(int *)(param_1 + 0xc818) = iVar15;
    *(float *)(param_1 + 0x5ec) = fVar19 + fVar27;
    if ((*(char *)(param_1 + 0x14) == '\0') &&
       (bVar6 = C3D_CONFIGURATOR::GetValueExist((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"),
       bVar6)) {
      pcVar12 = C3D_CONFIGURATOR::GetValueString((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click")
      ;
      C3D_SOUNDMANAGER::PlaySound2D
                (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
    }
    *(int **)(param_1 + 0x5f0) = (int *)(param_1 + 0x10f18);
    *(bool *)(param_1 + 0x5e6) = *(char *)(param_1 + 0x5e6) == '\0';
    FUN_140450b50(*puVar1,*(undefined8 *)(param_1 + 0xadb8));
    *(undefined8 *)(param_1 + 0xadb8) = *puVar1;
    *(undefined8 *)(param_1 + 0xadc8) = 0;
  }
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x675);
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             fVar25 - DAT_140992088 * fVar26,-1.7014118e+38,0x1408b85b0,
             (wchar_t *)(ulonglong)(uint)(fVar27 - DAT_140992088 * fVar23));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  fVar19 = DAT_14090ad74;
  _DAT_1409be2e8 = DAT_140992088 * fVar21;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 = CONCAT44(fVar27 - DAT_140992088 * 0.0,DAT_140992088 * DAT_14090ad74 + fVar25);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14035fcfc;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xffffffff,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_14035fcfc:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  fVar26 = DAT_14090adc8;
  fVar21 = DAT_14090a9a0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9a0;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 = CONCAT44(fVar27 - DAT_140992088 * 0.0,DAT_140992088 * DAT_14090adc8 + fVar25);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14035fea2;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,1,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_14035fea2:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0xdf11);
  fVar22 = DAT_14090a900;
  C3D_FONTMANAGER::PrintCenterUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             (DAT_140992088 * DAT_14090aa1c + fVar25) - (float)DAT_14099528c * DAT_140909ddc,
             -1.7014118e+38,0x1408b8588,
             (wchar_t *)(ulonglong)(uint)(DAT_140992088 * DAT_14090a900 + fVar27));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar21;
  DAT_1409be2f8 = 0;
  fVar20 = fVar25 - DAT_140992088 * DAT_14090a86c;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar20);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar20,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_1403600b3;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xfffffffe,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_1403600b3:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  fVar20 = DAT_14090ab90;
  _DAT_1409be2e8 = DAT_140992088 * fVar21;
  DAT_1409be2f8 = 0;
  fVar21 = DAT_140992088 * DAT_14090ab90 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar21);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar21,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14036023d;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,2,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_14036023d:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0xdf12);
  C3D_FONTMANAGER::PrintCenterUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             (DAT_140992088 * DAT_14090ad0c + fVar25) - (float)DAT_14099528c * DAT_140909ddc,
             -1.7014118e+38,0x1408b85d0,
             (wchar_t *)(ulonglong)(uint)(DAT_140992088 * fVar22 + fVar27));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  fVar21 = DAT_14090a9a0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9a0;
  DAT_1409be2f8 = 0;
  fVar22 = DAT_140992088 * DAT_14090abf8 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar22);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar22,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14036044b;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xfffffffd,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_14036044b:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar21;
  DAT_1409be2f8 = 0;
  fVar21 = DAT_140992088 * fVar26 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar21);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar21,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_1403605cc;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,3,1,local_148);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_1403605cc:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  fVar21 = DAT_14090a928;
  fVar26 = DAT_140992088 * DAT_14090ae04;
  *(undefined4 *)(param_1 + 0xada8) = 6;
  cVar5 = FUN_140395a70(param_1);
  if (cVar5 != '\0') {
    *(bool *)(param_1 + 0xc808) = *(char *)(param_1 + 0xc808) == '\0';
  }
  (**(code **)(**(longlong **)(param_1 + 0xb450) + 0x70))
            (*(longlong **)(param_1 + 0xb450),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar21;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 = CONCAT44(DAT_140992088 * DAT_14090a800 + fVar27,_DAT_1409be2e8 + fVar26 + fVar25)
  ;
  _DAT_1409be2ec = _DAT_1409be2e8;
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  fVar21 = DAT_14090a828;
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  fVar25 = fVar25 + DAT_140992088 * _DAT_14090ae6c;
  FUN_1404461f0(puVar1,DAT_140992088 * DAT_14090a828 + fVar25);
  iVar15 = local_118;
  cVar5 = FUN_140335500(param_1);
  if ((cVar5 != '\0') && (DAT_140a54e91 != '\0')) {
    DAT_140a54e91 = '\0';
    if (iVar17 < 1) {
      iVar17 = *(int *)(param_1 + 0x590);
    }
    uVar7 = FUN_140335b10(iVar17);
    *(undefined4 *)(param_1 + 0xc814) = uVar7;
    if (iVar15 < 1) {
      *(undefined4 *)(param_1 + 0xc818) = *(undefined4 *)(param_1 + 0x594);
    }
    else {
      *(int *)(param_1 + 0xc818) = iVar15;
    }
    fVar24 = DAT_140992088 * fVar24;
    *(float *)(param_1 + 0x5e8) = fVar25;
    *(float *)(param_1 + 0x5ec) = fVar24 + fVar27;
    if ((*(char *)(param_1 + 0x14) == '\0') &&
       (bVar6 = C3D_CONFIGURATOR::GetValueExist((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"),
       bVar6)) {
      pcVar12 = C3D_CONFIGURATOR::GetValueString((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click")
      ;
      C3D_SOUNDMANAGER::PlaySound2D
                (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
    }
    *(bool *)(param_1 + 0x5e6) = *(char *)(param_1 + 0x5e6) == '\0';
    *(longlong *)(param_1 + 0x5f0) = param_1 + 0x10f1c;
    FUN_140450b50(*puVar1,*(undefined8 *)(param_1 + 0xadb8));
    *(undefined8 *)(param_1 + 0xadb8) = *puVar1;
    *(undefined8 *)(param_1 + 0xadc8) = 0;
  }
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0x676);
  C3D_FONTMANAGER::PrintLeftUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             fVar25 - DAT_140992088 * fVar21,-1.7014118e+38,0x1408b86c0,
             (wchar_t *)(ulonglong)(uint)(fVar27 - DAT_140992088 * fVar23));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  fVar24 = DAT_14090a9a0;
  _DAT_1409be2e8 = DAT_140992088 * DAT_14090a9a0;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 = CONCAT44(fVar27 - DAT_140992088 * 0.0,DAT_140992088 * fVar19 + fVar25);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
  bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
  if (bVar6) {
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = '\0';
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xffffffff,local_148,1);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  fVar21 = DAT_14090adc8;
  _DAT_1409be2e8 = DAT_140992088 * fVar24;
  DAT_1409be2f8 = 0;
  _DAT_1409be2f0 = CONCAT44(fVar27 - DAT_140992088 * 0.0,DAT_140992088 * DAT_14090adc8 + fVar25);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_140360bc5;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,1,local_148,1);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_140360bc5:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0xdf11);
  fVar19 = DAT_14090a900;
  fVar23 = DAT_140909ddc;
  C3D_FONTMANAGER::PrintCenterUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             (DAT_140992088 * DAT_14090aa1c + fVar25) - (float)DAT_14099528c * DAT_140909ddc,
             -1.7014118e+38,0x1408b8710,
             (wchar_t *)(ulonglong)(uint)(DAT_140992088 * DAT_14090a900 + fVar27));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar24;
  DAT_1409be2f8 = 0;
  fVar26 = fVar25 - DAT_140992088 * DAT_14090a86c;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar26);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar26,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_140360ddc;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xfffffffe,local_148,1);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_140360ddc:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar24;
  DAT_1409be2f8 = 0;
  fVar26 = DAT_140992088 * fVar20 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar26);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar26,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_140360f5d;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,2,local_148,1);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_140360f5d:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  C3D_LANGUAGE::GetString((C3D_LANGUAGE *)&DAT_140997590,0xdf12);
  C3D_FONTMANAGER::PrintCenterUnicode
            ((C3D_FONTMANAGER *)&DAT_140996fb0,(C3D_FONT *)DAT_140995220,
             (DAT_140992088 * DAT_14090ad0c + fVar25) - (float)DAT_14099528c * fVar23,-1.7014118e+38
             ,0x1408b8670,(wchar_t *)(ulonglong)(uint)(DAT_140992088 * fVar19 + fVar27));
  (**(code **)(**(longlong **)(param_1 + 0xb4a0) + 0x70))
            (*(longlong **)(param_1 + 0xb4a0),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar24;
  DAT_1409be2f8 = 0;
  fVar23 = DAT_140992088 * DAT_14090abf8 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar23);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar23,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (!bVar6) goto LAB_14036115f;
    _DAT_1409be30c = _DAT_14090c4c0;
    uRam00000001409be314 = _UNK_14090c4c8;
    if (DAT_140a54e91 != '\0') {
      DAT_140a54e91 = cVar4;
      if ((*(char *)(param_1 + 0x14) == '\0') &&
         (bVar6 = C3D_CONFIGURATOR::GetValueExist
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
        pcVar12 = C3D_CONFIGURATOR::GetValueString
                            ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
        C3D_SOUNDMANAGER::PlaySound2D
                  (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
      }
      FUN_1403613b0(param_1,0xfffffffd,local_148,1);
      *(undefined4 *)(param_1 + 0x10f20) = 4;
    }
  }
  else {
LAB_14036115f:
    _DAT_1409be30c = _DAT_14090c100;
    uRam00000001409be314 = _UNK_14090c108;
  }
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  (**(code **)(**(longlong **)(param_1 + 0xb498) + 0x70))
            (*(longlong **)(param_1 + 0xb498),0,DAT_1409ead08);
  _DAT_1409be2e8 = DAT_140992088 * fVar24;
  DAT_1409be2f8 = 0;
  fVar25 = DAT_140992088 * fVar21 + fVar25;
  _DAT_1409be2f0 = CONCAT44(_DAT_1409be2e8 + fVar27,fVar25);
  _DAT_1409be2ec = _DAT_1409be2e8;
  FUN_1404461f0(puVar1,fVar25,_DAT_1409be2e8 + fVar27,6);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  if (cVar4 == '\0') {
    uVar13 = C3D_INPUT::GetMouseSolid((C3D_INPUT *)&DAT_140a54b90);
    bVar6 = C3D_PANEL2D::Collision((C3D_PANEL2D *)&DAT_1409be060,uVar13);
    if (bVar6) {
      _DAT_1409be30c = _DAT_14090c4c0;
      uRam00000001409be314 = _UNK_14090c4c8;
      if (DAT_140a54e91 != '\0') {
        DAT_140a54e91 = cVar4;
        if ((*(char *)(param_1 + 0x14) == '\0') &&
           (bVar6 = C3D_CONFIGURATOR::GetValueExist
                              ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click"), bVar6)) {
          pcVar12 = C3D_CONFIGURATOR::GetValueString
                              ((C3D_CONFIGURATOR *)(param_1 + 0xfd10),"gui_click");
          C3D_SOUNDMANAGER::PlaySound2D
                    (*(C3D_SOUNDMANAGER **)(param_1 + 0xfd08),pcVar12,true,fVar2,(char *)0x0);
        }
        FUN_1403613b0(param_1,3,local_148,1);
        *(undefined4 *)(param_1 + 0x10f20) = 4;
      }
      goto LAB_1403612ee;
    }
  }
  _DAT_1409be30c = _DAT_14090c100;
  uRam00000001409be314 = _UNK_14090c108;
LAB_1403612ee:
  C3D_PANEL2D::Draw((C3D_PANEL2D *)&DAT_1409be060,0.0,0.0,fVar2,fVar2,0.0,true);
  _DAT_1409be30c = _DAT_14090c540;
  uRam00000001409be314 = _UNK_14090c548;
  *local_110 = DAT_140992088 * DAT_14090aa28 + fVar27;
  return cVar4;
}



