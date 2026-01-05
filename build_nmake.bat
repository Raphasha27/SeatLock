@echo off
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" -arch=x64 > nul 2>&1

echo CHECKING EXTENDED TOOLLS:
where cl
where cmake

if not exist build mkdir build
cd build

echo CONFIGURING (NMake):
cmake -G "NMake Makefiles" ..
if %errorlevel% neq 0 (
    echo CONFIG ERROR
    exit /b 1
)

echo BUILDING:
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo BUILD ERROR
    exit /b 1
)

echo BENCHMARK:
.\SeatLockBench.exe
