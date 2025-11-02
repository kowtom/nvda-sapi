#!/bin/bash
# Validation script to verify NVDA SAPI Bridge builds

echo "NVDA SAPI Bridge - Build Validation"
echo "===================================="
echo ""

ERRORS=0

# Minimum expected DLL size in bytes (50KB)
# DLLs smaller than this likely failed to build correctly
MIN_DLL_SIZE=50000

# Check if build directories exist
if [ ! -d "build/x86/bin" ]; then
    echo "[ERROR] x86 build directory not found"
    ((ERRORS++))
else
    echo "[OK] x86 build directory exists"
fi

if [ ! -d "build/x64/bin" ]; then
    echo "[ERROR] x64 build directory not found"
    ((ERRORS++))
else
    echo "[OK] x64 build directory exists"
fi

# Check if DLLs exist
if [ ! -f "build/x86/bin/nvda_sapi32.dll" ]; then
    echo "[ERROR] nvda_sapi32.dll not found"
    ((ERRORS++))
else
    echo "[OK] nvda_sapi32.dll found"
    
    # Check file size (should be at least MIN_DLL_SIZE)
    SIZE=$(stat -f%z "build/x86/bin/nvda_sapi32.dll" 2>/dev/null || stat -c%s "build/x86/bin/nvda_sapi32.dll" 2>/dev/null)
    if [ "$SIZE" -lt "$MIN_DLL_SIZE" ]; then
        echo "[WARNING] nvda_sapi32.dll seems too small ($SIZE bytes, expected >$MIN_DLL_SIZE)"
    else
        echo "[OK] nvda_sapi32.dll size: $SIZE bytes"
    fi
fi

if [ ! -f "build/x64/bin/nvda_sapi64.dll" ]; then
    echo "[ERROR] nvda_sapi64.dll not found"
    ((ERRORS++))
else
    echo "[OK] nvda_sapi64.dll found"
    
    # Check file size
    SIZE=$(stat -f%z "build/x64/bin/nvda_sapi64.dll" 2>/dev/null || stat -c%s "build/x64/bin/nvda_sapi64.dll" 2>/dev/null)
    if [ "$SIZE" -lt "$MIN_DLL_SIZE" ]; then
        echo "[WARNING] nvda_sapi64.dll seems too small ($SIZE bytes, expected >$MIN_DLL_SIZE)"
    else
        echo "[OK] nvda_sapi64.dll size: $SIZE bytes"
    fi
fi

# Check file format with 'file' command
if command -v file &> /dev/null; then
    echo ""
    echo "Checking DLL formats..."
    
    if [ -f "build/x86/bin/nvda_sapi32.dll" ]; then
        FORMAT=$(file "build/x86/bin/nvda_sapi32.dll")
        if echo "$FORMAT" | grep -q "PE32.*Intel 80386"; then
            echo "[OK] nvda_sapi32.dll is 32-bit PE"
        else
            echo "[ERROR] nvda_sapi32.dll is not 32-bit PE"
            echo "       $FORMAT"
            ((ERRORS++))
        fi
    fi
    
    if [ -f "build/x64/bin/nvda_sapi64.dll" ]; then
        FORMAT=$(file "build/x64/bin/nvda_sapi64.dll")
        if echo "$FORMAT" | grep -q "PE32+.*x86-64"; then
            echo "[OK] nvda_sapi64.dll is 64-bit PE"
        else
            echo "[ERROR] nvda_sapi64.dll is not 64-bit PE"
            echo "       $FORMAT"
            ((ERRORS++))
        fi
    fi
fi

# Check exports with objdump
if command -v x86_64-w64-mingw32-objdump &> /dev/null; then
    echo ""
    echo "Checking DLL exports..."
    
    if [ -f "build/x64/bin/nvda_sapi64.dll" ]; then
        if x86_64-w64-mingw32-objdump -p "build/x64/bin/nvda_sapi64.dll" | grep -q "DllGetClassObject"; then
            echo "[OK] DllGetClassObject exported from nvda_sapi64.dll"
        else
            echo "[ERROR] DllGetClassObject not exported from nvda_sapi64.dll"
            ((ERRORS++))
        fi
    fi
fi

if command -v i686-w64-mingw32-objdump &> /dev/null; then
    if [ -f "build/x86/bin/nvda_sapi32.dll" ]; then
        if i686-w64-mingw32-objdump -p "build/x86/bin/nvda_sapi32.dll" | grep -q "DllGetClassObject"; then
            echo "[OK] DllGetClassObject exported from nvda_sapi32.dll"
        else
            echo "[ERROR] DllGetClassObject not exported from nvda_sapi32.dll"
            ((ERRORS++))
        fi
    fi
fi

echo ""
echo "===================================="
if [ $ERRORS -eq 0 ]; then
    echo "Validation PASSED - All checks successful!"
    echo ""
    echo "DLLs are ready for deployment to Windows."
    echo "To register on Windows, run:"
    echo "  regsvr32 build\\x86\\bin\\nvda_sapi32.dll"
    echo "  regsvr32 build\\x64\\bin\\nvda_sapi64.dll"
    exit 0
else
    echo "Validation FAILED - $ERRORS error(s) found"
    echo "Please rebuild the project"
    exit 1
fi
