@echo off
setlocal
cd /d "%~dp0"
where gcc >nul 2>nul
if errorlevel 1 (
  echo Please install MinGW-w64 GCC and add its bin folder to PATH.
  exit /b 1
)
if not exist "..\dist" mkdir "..\dist"
gcc calculator.c -std=c11 -O2 -Wall -Wextra -municode -mwindows -static -s -o "..\dist\Calculator.exe" -luser32 -lgdi32
if errorlevel 1 exit /b 1
echo Built dist\Calculator.exe
