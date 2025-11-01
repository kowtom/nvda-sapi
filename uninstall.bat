@echo off
REM Uninstallation wrapper for NVDA SAPI Bridge

echo NVDA SAPI Bridge Uninstaller
echo =============================
echo.
echo This will remove the NVDA SAPI Bridge from your system.
echo Administrator privileges are required.
echo.
pause

powershell -ExecutionPolicy Bypass -File "%~dp0install.ps1" -Uninstall

echo.
echo Press any key to exit...
pause >nul
