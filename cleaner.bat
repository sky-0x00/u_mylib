@echo off
echo ВНИМАНИЕ! После продолжения будут удалены все временные файлы проектов среды Visual Studio 2012, кроме исполняемых (.exe и .dll) модулей!
echo Для продолжения нажмите любую клавишу...
pause>nul
rem pause
del *.obj /S /Q
del *.sdf /S /Q
del *.tlog /S /Q
del *.ilk /S /Q
del *.pdb /S /Q
del *.idb /S /Q
del *.pch /S /Q
del *.ipch /S /Q
rem pause
echo Для продолжения нажмите любую клавишу...
pause>nul