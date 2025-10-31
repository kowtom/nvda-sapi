@echo off
REM Test script for NVDA SAPI Bridge

echo NVDA SAPI Bridge Test
echo =====================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: Python is not installed or not in PATH!
    echo.
    pause
    exit /b 1
)

echo Running test script...
echo.

python "%~dp0..\test_sapi_bridge.py"

echo.
pause
