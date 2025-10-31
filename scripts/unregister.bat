@echo off
REM Unregistration script for NVDA SAPI Bridge
REM Must be run as administrator

echo NVDA SAPI Bridge Unregistration
echo ================================
echo.

echo Unregistering NVDA SAPI Bridge...
echo.

python "%~dp0..\nvda_sapi_bridge.py" --unregister

echo.
pause
