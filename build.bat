@echo off
setlocal enabledelayedexpansion

REM -----------------------------------------------------------------------------
REM Build plugins for tesmioloader.
REM
REM By default this builds every plugin in plugins\ into build\plugins. The
REM upstream tesmioloader.dll is NOT built by default - it is opt-in via
REM `build.bat loader`. Anything the Almanac ImGui panel needs is fetched and
REM built by the same run; there is no second command.
REM
REM `build.bat` and `build.bat wrsre` share build\plugins. Stage one or the
REM other, never both, or every plugin ships twice (monolithic + standalone).
REM
REM Requires Visual Studio Build Tools 2022 with "Desktop development with
REM C++". Free download: https://visualstudio.microsoft.com/downloads/
REM
REM The script locates vcvars64.bat through vswhere, sources it, and then
REM invokes cl.exe. vcvars64.bat must run in the same cmd session as
REM cl.exe, which is why the whole build lives in one file.
REM
REM /MT statically links the C runtime into each DLL. That is not a
REM preference - the contract in src/tesmio_api.h says nothing may be
REM freed across the loader/plugin boundary. Do not change it.
REM
REM Usage:
REM   build.bat              build every plugin into build\plugins
REM   build.bat wrsre        build the single-DLL wrsre.dll + all INIs
REM   build.bat clean        remove build\plugins\ first
REM   build.bat <name>       build one plugin, e.g. `build.bat sim_clock`
REM   build.bat almanac_overlay_dev
REM                          build only the Almanac ImGui panel (and its pinned
REM                          ImGui, fetched into .cache on first run; set
REM                          ALMANAC_IMGUI_DIR to reuse a checkout)
REM   build.bat almanac_panel
REM                          same as almanac_overlay_dev
REM   build.bat almanac_overlay_dev_cmake
REM                          the older isolated CMake prototype (not the panel)
REM   build.bat loader       build tesmioloader.dll alone
REM -----------------------------------------------------------------------------

set "ROOT=%~dp0"
pushd "%ROOT%"

REM --- Optional: clean ---
if /i "%~1"=="clean" (
    if exist build\plugins (
        rmdir /s /q build\plugins
    )
    shift
)

REM --- Locate MSVC ---
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" goto :no_vs

set "VSPATH="
for /f "usebackq tokens=*" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath 2^>nul`) do set "VSPATH=%%i"
if not defined VSPATH goto :no_vs

call "%VSPATH%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo ERROR: vcvars64.bat failed. Try running it by hand to see why.
    popd
    exit /b 1
)

REM --- Directories ---
if not exist build          mkdir build
if not exist build\plugins  mkdir build\plugins

REM --- Dear ImGui (optional) ---

REM The Almanac panel is the only plugin that links Dear ImGui, and it is the
REM only reason this script needs a dependency at all. Resolution is shared by
REM the default build and `build.bat almanac_overlay_dev`, so a full build really
REM is one command.
REM
REM   ALMANAC_IMGUI_DIR set      use that checkout as-is
REM   .cache\almanac-imgui-host-tests\imgui.cpp exists   reuse it
REM   otherwise                  clone the pinned tag, ONCE
REM
REM If the clone cannot happen (no git, no network), IMGUI_OK stays 0: the
REM Almanac panel is skipped with a comment, everything else still builds, and
REM this script still exits successfully. A missing dev tool must not fail a
REM build of the plugins the game actually plays with.
set "IMGUI_OK=0"
set "IMGUI_DIR=%ALMANAC_IMGUI_DIR%"
if not defined IMGUI_DIR set "IMGUI_DIR=%ROOT%.cache\almanac-imgui-host-tests"

if not exist "!IMGUI_DIR!\imgui.cpp" (
    where git >nul 2>nul
    if errorlevel 1 (
        set "IMGUI_NO_GIT=1"
    ) else (
        if not exist ".cache" mkdir ".cache"
        echo [dependency] fetching Dear ImGui v1.91.9b into !IMGUI_DIR!
        git -c core.autocrlf=false clone --depth 1 --branch v1.91.9b https://github.com/ocornut/imgui.git "!IMGUI_DIR!"
    )
)

if exist "!IMGUI_DIR!\imgui.cpp" (
    if exist "!IMGUI_DIR!\backends\imgui_impl_dx11.cpp" (
        for %%I in ("!IMGUI_DIR!") do set "IMGUI_DIR=%%~fI"
        set "IMGUI_OK=1"
    )
)

REM --- Common compiler flags ---
REM
REM   /nologo                 no banner
REM   /MT                     static CRT, per the API contract
REM   /O2                     release optimisation
REM   /W3                     warning level 3
REM   /std:c++17              the standard the project targets
REM   /EHsc                   standard C++ exception semantics; __try /
REM                           __except is SEH and works regardless
REM   /D_CRT_*                silence a handful of deliberate uses of the
REM                           classic string functions
REM   /I"%ROOT%src"           so a plugin can write #include "game_world.h"
REM                           without a path, even though most of them use
REM                           relative includes
set "COMMON=/nologo /MT /O2 /W3 /std:c++17 /EHsc /D_CRT_SECURE_NO_WARNINGS /D_CRT_NONSTDC_NO_DEPRECATE /I"%ROOT%src""

set "WHICH=%~1"

REM --- Counters: defined before any branch so :done can always test them ---
set "BUILT=0"
set "SKIPPED=0"
set "FAILED=0"
set "MATCHED=0"

REM --- Loader (explicit opt-in only: `build.bat loader`) ---
if /i "%WHICH%"=="loader" goto :build_loader
goto :after_loader

:build_loader
if not exist "src\tesmioloader.cpp" (
    echo [skip]  tesmioloader.dll - src\tesmioloader.cpp not found
    if not "%WHICH%"=="" goto :failed
    goto :after_loader
)
echo [build] tesmioloader.dll
cl %COMMON% /LD "src\tesmioloader.cpp" /link /OUT:build\plugins\tesmioloader.dll user32.lib
if errorlevel 1 (
    echo [FAIL]  tesmioloader
    set /a FAILED+=1
    if not "%WHICH%"=="" goto :failed
    goto :after_loader
)
REM The loader was asked for by name: do not build plugins as well.
if /i "%WHICH%"=="loader" goto :done

:after_loader

REM Default build and named plugin builds fall through to the loop. The two
REM Almanac targets branch away here; anything else must not reach the label
REM below, which would make a plain `build.bat` build only the panel.
if /i "%WHICH%"=="almanac_overlay_dev_cmake" goto :build_almanac_overlay_dev
if /i "%WHICH%"=="almanac_overlay_dev" goto :build_almanac_panel
if /i "%WHICH%"=="almanac_panel"       goto :build_almanac_panel
if /i "%WHICH%"=="wrsre"               goto :build_wrsre_monolithic
goto :plugins

:build_wrsre_monolithic
echo [build] wrsre.dll (single-DLL monolithic build)
if not exist "build\wrsre_obj" mkdir "build\wrsre_obj"
set "WRSRE_MODS=sim_clock persistence currency resource_pricing enterprise_base republic_bank taxes state_ledger citizen_labor citizen_retail private_shops private_logistics private_construction ui_panels ui_overlay"
set "WRSRE_OBJS="
for %%m in (%WRSRE_MODS%) do (
    echo   [cc]  plugins\%%m\%%m.cpp
    cl %COMMON% /c /DWRSRE_MONOLITHIC_MODULE=%%m /Fo"build\wrsre_obj\%%m.obj" "plugins\%%m\%%m.cpp"
    if errorlevel 1 (
        echo [FAIL]  %%m
        set /a FAILED+=1
        goto :failed
    )
    set "WRSRE_OBJS=!WRSRE_OBJS! build\wrsre_obj\%%m.obj"
    if exist "plugins\%%m\%%m.ini" (
        copy /Y "plugins\%%m\%%m.ini" "build\plugins\%%m.ini" >nul
    )
)
echo   [cc]  src\wrsre_entry.cpp
cl %COMMON% /c /Fo"build\wrsre_obj\wrsre_entry.obj" "src\wrsre_entry.cpp"
if errorlevel 1 (
    echo [FAIL]  wrsre_entry
    set /a FAILED+=1
    goto :failed
)
echo   [link] build\plugins\wrsre.dll
link /NOLOGO /DLL /OUT:build\plugins\wrsre.dll build\wrsre_obj\wrsre_entry.obj !WRSRE_OBJS! user32.lib
if errorlevel 1 (
    echo [FAIL]  wrsre.dll
    set /a FAILED+=1
    goto :failed
)
set /a BUILT+=1
REM The monolith and the per-plugin DLLs must never be staged together.
REM Remove only the stale standalone DLLs. The INIs stay: wrsre.dll reads
REM them too, so deleting them would leave an incomplete staging set.
for %%m in (!WRSRE_MODS!) do (
    if exist "build\plugins\%%m.dll" del /q "build\plugins\%%m.dll" >nul 2>&1
)
echo [build] wrsre.dll ready in build\plugins (stale standalone plugin DLLs removed)
goto :done

:build_almanac_panel
echo [build] plugins\almanac_overlay_dev (ImGui from !IMGUI_DIR!)
if "!IMGUI_OK!"=="0" (
    echo ERROR: Dear ImGui v1.91.9b is not available.
    if defined IMGUI_NO_GIT echo        git was not found on PATH; it is needed for the one-time fetch.
    echo        Set ALMANAC_IMGUI_DIR to an existing checkout of the v1.91.9b tag
    echo        ^(the folder containing imgui.cpp^) and run this again.
    goto :failed
)
REM Version warning only: a different checkout still compiles, but the host
REM drawing checks were written against this one.
findstr /r /c:"#define IMGUI_VERSION_NUM  *19191" "!IMGUI_DIR!\imgui.h" >nul 2>nul
if errorlevel 1 echo [build] WARNING: IMGUI_DIR is not v1.91.9b (IMGUI_VERSION_NUM 19191)
call :compile_almanac_panel
if errorlevel 1 goto :failed
goto :done

REM LEGACY: the isolated CMake prototype under prototypes\almanac_overlay\native.
REM It is NOT the panel: it refuses to install unless the game files match one
REM exact build. `build.bat almanac_overlay_dev` builds the real plugin above;
REM this target is kept only for the standalone WARP diagnostic window.
:build_almanac_overlay_dev
where cmake >nul 2>nul
if errorlevel 1 (
    echo ERROR: CMake 3.24 or newer is required for almanac_overlay_dev.
    goto :failed
)

set "IMGUI_DIR=%ALMANAC_IMGUI_DIR%"
if not defined IMGUI_DIR (
    set "IMGUI_DIR=%ROOT%.cache\almanac-imgui-host-tests"
    if not exist "!IMGUI_DIR!\imgui.cpp" (
        where git >nul 2>nul
        if errorlevel 1 (
            echo ERROR: Git is required to fetch pinned Dear ImGui v1.91.9b.
            goto :failed
        )
        if not exist ".cache" mkdir ".cache"
        echo [dependency] fetching Dear ImGui v1.91.9b into !IMGUI_DIR!
        git -c core.autocrlf=false clone --depth 1 --branch v1.91.9b https://github.com/ocornut/imgui.git "!IMGUI_DIR!"
        if errorlevel 1 goto :failed
    )
) else (
    if not exist "!IMGUI_DIR!\imgui.cpp" (
        echo ERROR: ALMANAC_IMGUI_DIR does not contain imgui.cpp: !IMGUI_DIR!
        goto :failed
    )
)
for %%I in ("!IMGUI_DIR!") do set "IMGUI_DIR=%%~fI"

cmake -S prototypes\almanac_overlay\native -B build\almanac_native -G "Visual Studio 17 2022" -A x64 "-DALMANAC_IMGUI_DIR=!IMGUI_DIR!" -DALMANAC_BUILD_DEV_PLUGIN=ON
if errorlevel 1 goto :failed
cmake --build build\almanac_native --config Debug --target almanac_overlay_dev
if errorlevel 1 goto :failed

set "DEV_DLL=build\almanac_native\Debug\almanac_overlay_dev.dll"
if not exist "!DEV_DLL!" (
    echo ERROR: expected DLL was not produced: !DEV_DLL!
    goto :failed
)
if not exist "build\plugins" mkdir "build\plugins"
copy /Y "!DEV_DLL!" "build\plugins\almanac_overlay_dev.dll" >nul
if errorlevel 1 goto :failed
copy /Y "prototypes\almanac_overlay\native\almanac_overlay_dev.ini" "build\plugins\almanac_overlay_dev.ini" >nul
if errorlevel 1 goto :failed
echo [build] almanac_overlay_dev.dll and config staged in build\plugins
echo [build] copy only into a disposable Tesmio runtime before testing
goto :done

:compile_almanac_panel
REM Shared by the default build loop and by `build.bat almanac_overlay_dev`.
REM Expects IMGUI_DIR (absolute) and IMGUI_OK=1 to be set. Returns errorlevel 1
REM on failure so the caller decides whether that is fatal. Writes the DLL and
REM its INI into build\plugins, exactly like a one-file plugin.
echo [build] plugins\almanac_overlay_dev.dll
cl %COMMON% /LD /I"!IMGUI_DIR!" /I"!IMGUI_DIR!\backends" ^
   "plugins\almanac_overlay_dev\almanac_overlay_dev.cpp" ^
   "plugins\almanac_overlay_dev\graphics_hook.cpp" ^
   "plugins\almanac_overlay_dev\almanac_panel.cpp" ^
   "plugins\almanac_overlay_dev\almanac_panel_view.cpp" ^
   "!IMGUI_DIR!\imgui.cpp" "!IMGUI_DIR!\imgui_draw.cpp" ^
   "!IMGUI_DIR!\imgui_tables.cpp" "!IMGUI_DIR!\imgui_widgets.cpp" ^
   "!IMGUI_DIR!\backends\imgui_impl_dx11.cpp" ^
   /link /OUT:build\plugins\almanac_overlay_dev.dll
if errorlevel 1 (
    echo [FAIL]  almanac_overlay_dev
    exit /b 1
)
if exist "build\plugins\almanac_overlay_dev.ini" del /q "build\plugins\almanac_overlay_dev.ini" >nul 2>&1
copy /Y "plugins\almanac_overlay_dev\almanac_overlay_dev.ini" "build\plugins\almanac_overlay_dev.ini" >nul
if errorlevel 1 (
    echo [FAIL]  almanac_overlay_dev.ini could not be copied
    exit /b 1
)
del /q "plugins\almanac_overlay_dev\*.obj" "plugins\almanac_overlay_dev\*.exp" "plugins\almanac_overlay_dev\*.lib" "plugins\almanac_overlay_dev\*.ilk" "plugins\almanac_overlay_dev\*.pdb" >nul 2>nul
exit /b 0


REM --- Staging guard: do not leave wrsre.dll beside per-plugin DLLs ---
if exists "build\plugins\wrsre.dll" (
    echo [build] NOTE: removing stale wrsre.dll - this target produces separate
    echo         plugin DLLs. Use "build.bat wrsre" for the monolith instead.
    del /q "build\plugins\wrsre.dll" >nul 2>&1
)
:plugins

REM --- Plugins ---

for /d %%d in (plugins\*) do (
    set "NAME=%%~nxd"
    set "MATCH=0"

    if "%WHICH%"==""        set "MATCH=1"
    if /i "!NAME!"=="%WHICH%" set "MATCH=1"
    if not "%WHICH%"=="" if /i "!NAME!"=="%WHICH%" set "MATCHED=1"

    if "!MATCH!"=="1" (
        if /i "!NAME!"=="almanac_overlay_dev" (
            REM The one plugin that is not a single translation unit: it links
            REM the pinned Dear ImGui core and its DX11 backend as well. Same
            REM /MT flags, same output folder, discovered like any other.
            if "!IMGUI_OK!"=="1" (
                call :compile_almanac_panel
                if errorlevel 1 (
                    set /a FAILED+=1
                ) else (
                    set /a BUILT+=1
                    set "ALMANAC_BUILT=1"
                )
            ) else (
                REM Deliberately not a failure: a missing dev dependency must
                REM not break a build of the plugins the game plays with.
                echo [skip]  plugins\!NAME! - Dear ImGui not available
                echo         One-time fetch needs git and network access, or set
                echo         ALMANAC_IMGUI_DIR to a v1.91.9b checkout and rebuild.
                set /a SKIPPED+=1
            )
        ) else if exist "%%d\!NAME!.cpp" (
            echo [build] plugins\!NAME!.dll
            cl %COMMON% /LD "%%d\!NAME!.cpp" /link /OUT:build\plugins\!NAME!.dll
            if errorlevel 1 (
                set /a FAILED+=1
                echo [FAIL]  !NAME!
            ) else (
                set /a BUILT+=1
                if exist "%%d\!NAME!.ini" (
                    if exist "build\plugins\!NAME!.ini" del /q "build\plugins\!NAME!.ini" >nul 2>&1
                    copy /Y "%%d\!NAME!.ini" "build\plugins\!NAME!.ini" >nul
                )
                REM Remove intermediate compiler files from the source directory
                del /q "%%d\!NAME!.obj" "%%d\!NAME!.exp" "%%d\!NAME!.lib" "%%d\!NAME!.ilk" "%%d\!NAME!.pdb" >nul 2>nul
            )
        ) else (
            echo [skip]  plugins\!NAME! - no !NAME!.cpp
            set /a SKIPPED+=1
        )
    )
)

REM --- Explicit target that matched nothing / built nothing ---
if not "%WHICH%"=="" if "!MATCHED!"=="0" (
    echo ERROR: unknown target "%WHICH%" - no plugins\%WHICH% directory
    set /a FAILED+=1
)
if not "%WHICH%"=="" if "!BUILT!"=="0" if "!FAILED!"=="0" (
    echo ERROR: target "%WHICH%" was not built
    set /a FAILED+=1
)

REM --- Report ---
echo.
echo -----------------------------------------------------------------------------
echo   built:   !BUILT!
echo   skipped: !SKIPPED!
if !FAILED! gtr 0 (
    echo   FAILED:  !FAILED!
)
echo -----------------------------------------------------------------------------
if "%WHICH%"=="" (
    echo   Copy the DLLs and INIs into the game\tesmioloader\plugins folder.
    echo   tesmioloader.dll is NOT built here - use "build.bat loader" for it.
    echo   Do not mix these per-plugin DLLs with wrsre.dll from "build.bat wrsre".
    echo   NOTE: almanac_button.dll is a disposable test build that ends the
    echo         game process if its guard fails. Do not ship it normally.
    if not defined ALMANAC_BUILT echo   NOTE: almanac_overlay_dev was skipped - see the line above.
    if defined ALMANAC_BUILT echo   Includes the Almanac panel: press F9 in game to open it.
)
echo.

REM --- Auto-clean intermediate build files at the end ---
call "%ROOT%clean.bat"

:done
popd
if !FAILED! gtr 0 exit /b 1
exit /b 0

:no_vs
echo.
echo Visual Studio with C++ build tools not found.
echo.
echo Install Visual Studio Build Tools 2022 (free):
echo   https://visualstudio.microsoft.com/downloads/
echo Select "Desktop development with C++" during installation.
echo.
popd
exit /b 1

:failed
echo.
echo BUILD FAILED.
popd
exit /b 1