@echo off
call "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
cl /EHsc /std:c++17 /Fe:renderer.exe file_renderer.cpp user32.lib gdi32.lib
if %ERRORLEVEL% NEQ 0 echo BUILD FAILED
if %ERRORLEVEL% EQU 0 echo BUILD OK
