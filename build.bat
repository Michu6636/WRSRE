@echo off
setlocal enabledelayedexpansion

REM -----------------------------------------------------------------------------
REM Build plugins for tesmioloader.
REM
REM By default this builds every plugin in plugins\ and nothing else. The
REM loader (tesmioloader.dll) is not built here because it is already in
REM the game; if you do want to rebuild it, pass "loader" as the first
REM argument and the script will look for src\tesmioloader.cpp.
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
REM   build.bat              build every plugin in plugins\
REM   build.bat clean        remove build\plugins\ first
REM   build.bat <name>       build one plugin, e.g. `build.bat sim_clock`
REM   build.bat loader       also build tesmioloader.dll (needs
REM                          src\tesmioloader.cpp)
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

REM --- Loader: opt-in only ---
if /i "%WHICH%"=="loader" (
    if not exist "src\tesmioloader.cpp" (
        echo ERROR: src\tesmioloader.cpp not found - loader not built
        goto :failed
    )
    echo [build] tesmioloader.dll
    cl %COMMON% /LD "src\tesmioloader.cpp" /link /OUT:build\tesmioloader.dll user32.lib
    if errorlevel 1 goto :failed
    REM If the user asked for loader explicitly, do not build plugins too.
    goto :done
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
        if exist "%%d\!NAME!.cpp" (
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