@echo off
REM Registration script for NVDA SAPI Bridge
REM Must be run as administrator

echo NVDA SAPI Bridge Registration
echo ==============================
echo.

REM Check if Python is installed
python --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo Error: Python is not installed or not in PATH!
    echo Please install Python 3.7+ from https://www.python.org
    echo.
    pause
    exit /b 1
)

echo Installing Python dependencies...
python -m pip install -r "%~dp0..\requirements.txt"

if %ERRORLEVEL% NEQ 0 (
    echo Error: Failed to install dependencies!
    echo.
    pause
    exit /b 1
)

echo.
echo Registering NVDA SAPI Bridge...
echo.

python "%~dp0..\nvda_sapi_bridge.py" --register

echo.
echo The "NVDA Bridge" voice should now be available in SAPI5 applications.
echo Make sure NVDA is running when using legacy applications.
echo.
pause
