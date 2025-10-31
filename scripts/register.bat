@echo off
REM Registration script for NVDA-SAPI Bridge
REM Must be run as Administrator

echo Registering NVDA-SAPI Bridge...

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0

REM Register the DLL
regsvr32 /s "%SCRIPT_DIR%nvda-sapi.dll"

if %ERRORLEVEL% EQU 0 (
    echo.
    echo Registering SAPI5 token...
    
    REM Import registry settings
    reg import "%SCRIPT_DIR%nvda-sapi-register.reg"
    
    if %ERRORLEVEL% EQU 0 (
        echo.
        echo NVDA-SAPI Bridge registered successfully!
        echo You can now select "NVDA Bridge" as a voice in SAPI5 applications.
    ) else (
        echo.
        echo ERROR: Failed to register SAPI5 token.
        echo Please run this script as Administrator.
    )
) else (
    echo.
    echo ERROR: Failed to register DLL.
    echo Please run this script as Administrator.
)

pause
