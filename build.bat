@echo off
REM Build script for both x86 and x64 architectures

echo Building NVDA SAPI Bridge...

REM Create build directories
if not exist build\x86 mkdir build\x86
if not exist build\x64 mkdir build\x64

REM Build x86 (32-bit)
echo.
echo ========================================
echo Building x86 (32-bit) version...
echo ========================================
cd build\x86
cmake -G "Visual Studio 17 2022" -A Win32 ..\..
cmake --build . --config Release
cd ..\..

REM Build x64 (64-bit)
echo.
echo ========================================
echo Building x64 (64-bit) version...
echo ========================================
cd build\x64
cmake -G "Visual Studio 17 2022" -A x64 ..\..
cmake --build . --config Release
cd ..\..

echo.
echo ========================================
echo Build completed!
echo ========================================
echo x86 DLL: build\x86\bin\Release\nvda_sapi32.dll
echo x64 DLL: build\x64\bin\Release\nvda_sapi64.dll
echo.
echo To register the DLLs, run:
echo   regsvr32 build\x86\bin\Release\nvda_sapi32.dll
echo   regsvr32 build\x64\bin\Release\nvda_sapi64.dll
echo.
