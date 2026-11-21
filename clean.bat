@echo off
del /s /q *.obj *.exp *.lib *.ilk *.pdb >nul 2>nul
if exist build\obj rmdir /s /q build\obj >nul 2>nul
if exist build\wrsre_obj rmdir /s /q build\wrsre_obj >nul 2>nul
echo Workspace cleaned successfully.
