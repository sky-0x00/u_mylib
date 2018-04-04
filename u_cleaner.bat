@echo off
echo ВНИМАНИЕ! После продолжения будут удалены все временные файлы проектов среды Visual Studio, кроме исполняемых (.exe и .dll) модулей!
echo Для продолжения нажмите любую клавишу...
pause>nul

rd "%~dp0Debug" /S /Q
rd "%~dp0Release" /S /Q
rd "%~dp0x64" /S /Q
rd "%~dp0Win32" /S /Q
rd "%~dp0ipch" /S /Q

del "%~dp0*.obj" /S /Q
del "%~dp0*.sdf" /S /Q
del "%~dp0*.tlog" /S /Q
del "%~dp0*.ilk" /S /Q
del "%~dp0*.pdb" /S /Q
del "%~dp0*.idb" /S /Q
del "%~dp0*.pch" /S /Q
del "%~dp0*.ipch" /S /Q

rem echo Для продолжения нажмите любую клавишу...
rem pause>nul