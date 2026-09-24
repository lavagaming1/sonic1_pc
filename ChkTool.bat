@echo off
title Sonic 1 PC Port - Smart Build & Run
cd /d "%~dp0"

:: Step 1: Check if MSYS2 exists
if not exist "C:\msys64\msys2_shell.cmd" (
    echo [!] MSYS2 was not found at C:\msys64.
    echo Please install MSYS2 from https://www.msys2.org/ to build this project.
    pause
    exit /b 1
)

echo ===================================================
echo   Checking dependencies & building latest code...
echo ===================================================

:: Step 2: Auto-install missing build tools (if needed), build, and run
C:\msys64\msys2_shell.cmd -ucrt64 -defterm -where "%CD%" -c "pacman -Sy --noconfirm --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-sdl2 && cmake -B build -G Ninja && cmake --build build && ./build/sonic1.exe"

if %errorlevel% neq 0 (
    echo.
    echo [!] Build failed. Check the error messages above.
    pause
)