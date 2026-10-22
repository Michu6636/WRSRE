@echo off
del /s /q *.obj *.exp *.lib *.ilk *.pdb >nul 2>nul
if not errorlevel 1 echo Workspace cleaned successfully.
