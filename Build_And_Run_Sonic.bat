@echo off
title Sonic 1 PC Port - Build & Run
cd /d "C:\Users\Admin\Desktop\Mohammed Desktop\sonic1_pc"

:: Opens a visible MSYS2 terminal window, builds the latest code, and runs the game
C:\msys64\msys2_shell.cmd -ucrt64 -defterm -where "%CD%" -c "echo Building latest code... && cmake --build build && echo Launching Game... && ./build/sonic1.exe"
