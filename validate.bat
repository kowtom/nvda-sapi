@echo off
REM Validation script to verify NVDA SAPI Bridge builds

echo NVDA SAPI Bridge - Build Validation
echo ====================================
echo.

set ERRORS=0

REM Check if build directories exist
if not exist "build\x86\bin" (
    echo [ERROR] x86 build directory not found
    set /a ERRORS+=1
) else (
    echo [OK] x86 build directory exists
)

if not exist "build\x64\bin" (
    echo [ERROR] x64 build directory not found
    set /a ERRORS+=1
) else (
    echo [OK] x64 build directory exists
)

REM Check if DLLs exist
if not exist "build\x86\bin\Release\nvda_sapi32.dll" (
    if not exist "build\x86\bin\nvda_sapi32.dll" (
        echo [ERROR] nvda_sapi32.dll not found
        set /a ERRORS+=1
    ) else (
        echo [OK] nvda_sapi32.dll found
    )
) else (
    echo [OK] nvda_sapi32.dll found
)

if not exist "build\x64\bin\Release\nvda_sapi64.dll" (
    if not exist "build\x64\bin\nvda_sapi64.dll" (
        echo [ERROR] nvda_sapi64.dll not found
        set /a ERRORS+=1
    ) else (
        echo [OK] nvda_sapi64.dll found
    )
) else (
    echo [OK] nvda_sapi64.dll found
)

REM Check DLL exports (requires dumpbin)
where dumpbin >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo.
    echo Checking DLL exports...
    
    for %%f in (build\x86\bin\Release\nvda_sapi32.dll build\x86\bin\nvda_sapi32.dll) do (
        if exist "%%f" (
            echo Checking %%f
            dumpbin /exports "%%f" | findstr /C:"DllGetClassObject" >nul
            if %ERRORLEVEL% NEQ 0 (
                echo [ERROR] DllGetClassObject not exported
                set /a ERRORS+=1
            ) else (
                echo [OK] DllGetClassObject exported
            )
            goto :check_x64
        )
    )
    
    :check_x64
    for %%f in (build\x64\bin\Release\nvda_sapi64.dll build\x64\bin\nvda_sapi64.dll) do (
        if exist "%%f" (
            echo Checking %%f
            dumpbin /exports "%%f" | findstr /C:"DllGetClassObject" >nul
            if %ERRORLEVEL% NEQ 0 (
                echo [ERROR] DllGetClassObject not exported  
                set /a ERRORS+=1
            ) else (
                echo [OK] DllGetClassObject exported
            )
            goto :done_exports
        )
    )
    :done_exports
) else (
    echo [INFO] dumpbin not found - skipping export checks
)

echo.
echo ====================================
if %ERRORS% EQU 0 (
    echo Validation PASSED - All checks successful!
    echo.
    echo You can now register the DLLs with:
    echo   regsvr32 build\x86\bin\Release\nvda_sapi32.dll
    echo   regsvr32 build\x64\bin\Release\nvda_sapi64.dll
    exit /b 0
) else (
    echo Validation FAILED - %ERRORS% error(s) found
    echo Please rebuild the project
    exit /b 1
)
