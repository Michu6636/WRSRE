#!/bin/bash
# Regenerates bounded excerpts for export/almanac-row-item-analysis-20261110 (static only)
set -e
R=/home/user/WRSRE
E=$R/export/almanac-row-item-analysis-20261110
PY=$R/scratch/venv/bin/python
ANN=$R/scratch/rep/annot.py
DLLSHA=fd7d94590b98c7eb80fa14ef95f2bbdf4b5e058f68d07dfa97d0db8dcbfbcb6a
hdr(){ printf '%s\n' "# $1" "# Source: Mods for analysis/Diplomacy/tesmioloader/the build/plugins/reputation.dll (SHA-256 $DLLSHA)" "# Range: $2" "# Static objdump excerpt, annotated by annot.py (rip-relative targets, .rdata strings). Nothing executed." "# Purpose: $3" ""; }
dll(){ out=$1; title=$2; lo=$3; hi=$4; purpose=$5; { hdr "$title" "$lo-$hi" "$purpose"; (cd $R/scratch/rep && $PY $ANN $lo $hi); } > $E/$out; echo "$out $(wc -l < $E/$out) lines"; }
dll D01_row_insert_gates_0x180057e40.txt "D01 trade-row insertion gates and retry" 180057e40 180057ec7 "build/sidebar_item gates, retry counter 0x180287b50"
dll D02_row_insert_subvector_check_0x180057f50.txt "D02 trade record search and sub-vector size check" 180057f50 180057fe9 "record+0x48 header, exact 0x50 multiple check"
dll D03_row_insert_entry_build_push_0x180058043.txt "D03 entry build (name, string id) and native push" 180058043 1800580e0 "name copy, string id from 0x1800c3c58, call exe+0x4492f0"
dll D04_row_insert_texture_index_0x180058100.txt "D04 icon load, texture check, rollback, index store" 180058100 180058260 "editor/%s_%s.png, texture at last entry +0x48, rollback end-=0x50, index to 0x1800c3c5c"
dll D05_selection_handler_0x1800575c9.txt "D05 selection handler" 1800575c9 180057780 "calls to 0x180057e40, 0x180058310, 0x180057d50, 0x1800593b0, 0x18002f1e0"
dll D06_trade_row_predicate_0x180058310.txt "D06 trade-row selected predicate" 180058310 1800583d7 "selected record named trade, selected index == stored item index"
dll D07_click_case_loop_0x180057600.txt "D07 case loop and row-click dispatch call" 180057600 180057623 "stores case index to 0x180411bcc only when a case test passes"
dll D08_click_dispatch_0x180057670.txt "D08 dispatch call guard" 180057670 18005767d "dispatch only when all seven DLL flags are zero"
dll D09_case_test_0x18002f120.txt "D09 eight-way case test" 18002f120 18002f1b9 "case k returns true when its DLL state flag is set"
dll D10_dispatch_entry_0x18002f1e0.txt "D10 dispatcher entry and range check" 18002f1e0 18002f1f6 "cmp ecx,7 / ja tail: index 8 leaves the switch"
dll D11_dispatch_tail_0x18002f327.txt "D11 dispatcher common tail" 18002f327 18002f3e0 "common tail reached for index 8"
dll D12_window_flag_predicate_0x180001c80.txt "D12 any-DLL-window-open predicate" 180001c80 180001cc7 "returns 1 when any of seven DLL flags is nonzero"
dll D13_setdim_setpos_detours_0x180067fa0.txt "D13 SetDimensions and SetPosition detours" 180067fa0 180068140 "caller return-address gates exe+0x31E478/+0x31E4B2/+0x31E509; raw and gated stores"
dll D14_clamp_writes_AE54_AE58_0x180028100.txt "D14 clamp writing AE54/AE58" 180028100 180028183 "exe+0x9DFD64 and exe+0x9DFD68 writes"
dll D15_sizer_writes_AE54_AE58_0x180058b00.txt "D15 sizer AE54/AE58 writes" 180058b00 180058b30 "exe+0x9DFD64/68 writes from the sidebar-panel sizer"
dll D16_sidebar_entry_append_0x180056ea0.txt "D16 main sidebar entry append (copy constructor exe+0x459530)" 180056ea0 180056ef0 "exe+0x9E1520 += 0x68 after copy"
