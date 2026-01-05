@echo off
echo ==========================================
echo      SeatLock High-Performance Build
echo ==========================================

REM Initialize VS Environment
call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" > init_log.txt 2>&1

REM Create Build Dir
if not exist build mkdir build
cd build

REM CMake Configure
echo [INFO] Generating project files...
cmake .. > config_log.txt 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed. Check config_log.txt
    exit /b 1
)

REM Build
echo [INFO] Building...
cmake --build . --config Release > build_log.txt 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] Build failed. Check build_log.txt
    exit /b 1
)

REM Run
echo [INFO] Running Benchmark...
.\Release\SeatLockBench.exe > bench_results.txt 2>&1
type bench_results.txt
