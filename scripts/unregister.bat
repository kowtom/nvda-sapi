@echo off
REM Unregistration script for NVDA-SAPI Bridge
REM Must be run as Administrator

echo Unregistering NVDA-SAPI Bridge...

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0

REM Remove registry settings
reg import "%SCRIPT_DIR%nvda-sapi-unregister.reg"

REM Unregister the DLL
regsvr32 /u /s "%SCRIPT_DIR%nvda-sapi.dll"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo NVDA-SAPI Bridge unregistered successfully!
) else (
    echo.
    echo ERROR: Failed to unregister DLL.
    echo Please run this script as Administrator.
)

pause
