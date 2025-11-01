#!/bin/bash
# Build script for both x86 and x64 architectures using MinGW

echo "Building NVDA SAPI Bridge..."

# Create build directories
mkdir -p build/x86
mkdir -p build/x64

# Build x86 (32-bit)
echo ""
echo "========================================"
echo "Building x86 (32-bit) version..."
echo "========================================"
cd build/x86
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../toolchain-mingw32.cmake ../..
cmake --build . --config Release
cd ../..

# Build x64 (64-bit)
echo ""
echo "========================================"
echo "Building x64 (64-bit) version..."
echo "========================================"
cd build/x64
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../toolchain-mingw64.cmake ../..
cmake --build . --config Release
cd ../..

echo ""
echo "========================================"
echo "Build completed!"
echo "========================================"
echo "x86 DLL: build/x86/bin/nvda_sapi32.dll"
echo "x64 DLL: build/x64/bin/nvda_sapi64.dll"
echo ""
echo "To register the DLLs on Windows, run:"
echo "  regsvr32 build/x86/bin/nvda_sapi32.dll"
echo "  regsvr32 build/x64/bin/nvda_sapi64.dll"
echo ""
