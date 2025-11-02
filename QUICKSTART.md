# NVDA SAPI Bridge - Quick Start Guide

This guide will help you quickly get started with the NVDA SAPI Bridge.

## What is this?

The NVDA SAPI Bridge is a small DLL that makes legacy applications that only support Microsoft SAPI5 (Speech API) work with NVDA screen reader. This is useful for blind users who prefer NVDA but need to use old software that doesn't natively support it.

## Quick Start

### 1. Prerequisites

- **NVDA Screen Reader**: Download from https://www.nvaccess.org/
- **Build Tools** (only if building from source):
  - Windows: Visual Studio 2022 or MinGW-w64
  - CMake 3.15+

### 2. Building (Windows)

```batch
# Open Command Prompt
cd nvda-sapi
build.bat
```

This creates two DLL files:
- `build\x86\bin\Release\nvda_sapi32.dll` for 32-bit apps
- `build\x64\bin\Release\nvda_sapi64.dll` for 64-bit apps

### 3. Installing

Open Command Prompt **as Administrator** and run:

```batch
# For 32-bit applications
regsvr32 "C:\full\path\to\nvda_sapi32.dll"

# For 64-bit applications  
regsvr32 "C:\full\path\to\nvda_sapi64.dll"
```

You should see a success message: "DllRegisterServer in [path] succeeded"

### 4. Select the NVDA Voice

1. Start NVDA
2. Launch your legacy application
3. Open the application's voice or speech settings
4. Look for "NVDA Screen Reader Voice" or "NVDA" in the voice list
5. Select it as the active voice
6. The application should now speak through NVDA!

**Tip**: If the NVDA voice doesn't appear, restart the application - some apps only check for voices at startup.

## Example Applications

This works with applications like:
- Old screen reader configurations
- Text-to-speech enabled applications
- Accessibility tools that use SAPI5
- Educational software with speech support
- Many legacy Windows applications

## Troubleshooting

**NVDA voice not in the list:**
- Make sure you registered the DLL (you should have seen a success message)
- Check you registered the correct version (32-bit app = 32-bit DLL)
- Restart the application - it may only check for voices at startup
- Run Command Prompt as Administrator when registering

**No speech:**
- Make sure NVDA is running
- Verify you selected "NVDA" as the voice in the application
- Check that the correct DLL (32-bit vs 64-bit) is registered
- Restart the application after registering

**Registration failed:**
- Run Command Prompt as Administrator
- Check the DLL path is correct
- Use full path, not relative path

**Still not working:**
- Check Windows Event Viewer for errors
- Verify NVDA controller client is installed
- Try the other architecture DLL

## Uninstalling

```batch
# As Administrator
regsvr32 /u "C:\full\path\to\nvda_sapi32.dll"
regsvr32 /u "C:\full\path\to\nvda_sapi64.dll"
```

## Need More Help?

- Full documentation: See [README.md](README.md)
- Installation guide: See [INSTALL.md](INSTALL.md)
- Technical details: See [ARCHITECTURE.md](ARCHITECTURE.md)

## For Developers

The code is modern C++17 with:
- COM implementation for SAPI5
- NVDA controller client integration  
- Cross-platform build system
- Comprehensive documentation

Build with:
```bash
# Linux (cross-compile)
./build.sh

# Windows
build.bat
```

See the architecture documentation for technical details.
