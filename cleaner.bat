@echo off
echo ��������! ��᫥ �த������� ���� 㤠���� �� �६���� 䠩�� �஥�⮢ �।� Visual Studio, �஬� �ᯮ��塞�� (.exe � .dll) ���㫥�!
echo ��� �த������� ������ ���� �������...
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

rem echo ��� �த������� ������ ���� �������...
rem pause>nul