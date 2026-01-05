@echo off
echo ==========================================
echo      SeatLock High-Performance Build
echo ==========================================

REM 1. Try to setup VS Environment
if exist "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat" (
    echo [INFO] Found VS2026 BuildTools, initializing...
    call "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\VsDevCmd.bat"
) else (
    echo [WARNING] Could not find VsDevCmd.bat at expected location.
    echo Assuming you are running this from a Developer Command Prompt...
)

REM 2. Check for CMake
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] CMake not found! 
    echo Please rerun the Visual Studio Installer and ensure "C++ CMake tools for Windows" is checked.
    pause
    exit /b 1
)

REM 3. Create Build Dir
if not exist build mkdir build
cd build

REM 4. Generate Build Files
echo [INFO] Generating project files...
cmake ..
if %errorlevel% neq 0 (
    echo [ERROR] CMake configuration failed.
    pause
    exit /b 1
)

REM 5. Build Release
echo [INFO] Building Release configuration...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo [ERROR] Build failed.
    pause
    exit /b 1
)

REM 6. Run Benchmark
echo ==========================================
echo        RUNNING BENCHMARK
echo ==========================================
.\Release\SeatLockBench.exe

echo.
echo [SUCCESS] Build and Benchmark Complete.
pause
