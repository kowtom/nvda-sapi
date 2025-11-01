@echo off
REM Installation wrapper for NVDA SAPI Bridge
REM This batch file launches the PowerShell installer with administrator privileges

echo NVDA SAPI Bridge Installer
echo ==========================
echo.
echo This will install the NVDA SAPI Bridge on your system.
echo Administrator privileges are required.
echo.
pause

powershell -ExecutionPolicy Bypass -File "%~dp0install.ps1"

echo.
echo Press any key to exit...
pause >nul
