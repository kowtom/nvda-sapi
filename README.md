# NVDA SAPI Bridge

A SAPI5 Text-to-Speech engine that bridges legacy Windows applications to the NVDA screen reader.

## Overview

This project provides a SAPI5 (Speech API 5) TTS engine implementation that redirects speech output to NVDA (NonVisual Desktop Access) screen reader. This allows totally blind users to use legacy Windows applications that support SAPI5 speech output with their preferred NVDA screen reader.

### Features

- Full SAPI5 TTS engine implementation
- Seamless integration with NVDA via Controller Client API
- Support for both 32-bit and 64-bit applications
- Built with open-source tools (MinGW-w64)
- Easy installation and uninstallation scripts
- No dependencies on proprietary toolchains

## Requirements

### For Building

- **MinGW-w64** toolchain (both 32-bit and 64-bit compilers)
  - `i686-w64-mingw32-g++` for 32-bit builds
  - `x86_64-w64-mingw32-g++` for 64-bit builds
- **Make** utility
- Windows SDK headers (for SAPI5 interfaces)

On Debian/Ubuntu-based systems:
```bash
sudo apt-get install mingw-w64 make
```

### For Running

- **NVDA** screen reader installed and running
  - Download from: https://www.nvaccess.org/download/
- Windows Vista or later (both 32-bit and 64-bit supported)

## Building

### Quick Build

To build both 32-bit and 64-bit versions:

```bash
make
```

This will create:
- `build/x86/nvda_sapi_bridge.dll` (32-bit)
- `build/x64/nvda_sapi_bridge.dll` (64-bit)

### Build Individual Architectures

Build only 32-bit:
```bash
make x86
```

Build only 64-bit:
```bash
make x64
```

### Clean Build Files

```bash
make clean
```

## Installation

### Using the Installer (Recommended)

1. Build the project (see above)
2. Run `install.bat` as Administrator
3. Follow the on-screen instructions

The installer will:
- Copy the DLLs to the installation directory
- Register the COM objects
- Create SAPI voice tokens
- Set up both 32-bit and 64-bit voices on 64-bit systems

### Manual Installation

If you prefer to install manually, you can run the PowerShell script directly:

```powershell
# Run as Administrator
powershell -ExecutionPolicy Bypass -File install.ps1
```

## Uninstallation

### Using the Uninstaller

Run `uninstall.bat` as Administrator.

### Manual Uninstallation

```powershell
# Run as Administrator
powershell -ExecutionPolicy Bypass -File install.ps1 -Uninstall
```

## Usage

1. **Start NVDA** - Make sure NVDA is running before using legacy applications
2. **Configure Your Application** - In your SAPI5-compatible application, select "NVDA Screen Reader" or "NVDA" as the voice
3. **Use the Application** - Speech output will now be routed through NVDA

### Supported Applications

Any Windows application that uses SAPI5 for speech output, including:
- Legacy screen readers
- Educational software
- Accessibility tools
- Custom enterprise applications
- Development tools with SAPI5 speech support

## How It Works

The NVDA SAPI Bridge implements a standard SAPI5 TTS engine that:

1. Registers as a COM server providing the `ISpTTSEngine` interface
2. Receives speech requests from SAPI5 applications
3. Forwards the text to NVDA using the NVDA Controller Client API
4. Returns immediately, allowing NVDA to handle the actual speech synthesis

This approach ensures compatibility with existing applications while leveraging NVDA's superior speech capabilities and user customization options.

## Architecture

```
┌─────────────────────────┐
│  Legacy Application     │
│  (uses SAPI5)          │
└───────────┬─────────────┘
            │
            │ SAPI5 Interface
            │
┌───────────▼─────────────┐
│  NVDA SAPI Bridge       │
│  (COM Server)           │
└───────────┬─────────────┘
            │
            │ Controller Client API
            │
┌───────────▼─────────────┐
│  NVDA Screen Reader     │
│  (Speech Synthesis)     │
└─────────────────────────┘
```

## Technical Details

### COM Interface Implementation

The bridge implements the following COM interfaces:
- `IUnknown` - Basic COM object interface
- `ISpTTSEngine` - SAPI5 TTS engine interface
- `ISpObjectWithToken` - SAPI voice token interface

### NVDA Controller Client API

The bridge uses NVDA's Controller Client API functions:
- `nvdaController_testIfRunning()` - Check if NVDA is active
- `nvdaController_speakText()` - Send text to NVDA for speech
- `nvdaController_cancelSpeech()` - Cancel current speech

### CLSID

The COM object is registered with the following CLSID:
```
{2F9DE777-6B8D-4ECE-AA3A-1D7F5B2A7E2E}
```

## Building from Source on Linux

You can cross-compile for Windows from Linux using MinGW-w64:

```bash
# Install MinGW-w64
sudo apt-get install mingw-w64 make

# Clone the repository
git clone https://github.com/kowtom/nvda-sapi.git
cd nvda-sapi

# Build
make
```

The built DLLs can then be transferred to a Windows system for installation.

## Troubleshooting

### Voice Not Appearing in Application

1. Verify installation completed successfully
2. Restart the application
3. Check that you have administrator privileges during installation
4. For 32-bit applications on 64-bit Windows, ensure the 32-bit DLL was registered

### No Speech Output

1. Ensure NVDA is running
2. Test NVDA speech with NVDA+T (read title)
3. Check NVDA's speech settings
4. Verify the application is using the NVDA voice

### Build Errors

1. Ensure MinGW-w64 is properly installed
2. Verify both compilers are in your PATH:
   ```bash
   i686-w64-mingw32-g++ --version
   x86_64-w64-mingw32-g++ --version
   ```
3. Make sure you have Windows SDK headers available

## License

This project is licensed under the GNU General Public License v2.0 or later.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Credits

- **NVDA** - NonVisual Desktop Access (https://www.nvaccess.org/)
- **Microsoft SAPI** - Speech API specification
- **MinGW-w64** - Open-source Windows cross-compiler

## See Also

- [NVDA Official Website](https://www.nvaccess.org/)
- [NVDA Controller Client Documentation](https://www.nvaccess.org/files/nvda/documentation/controllerClient.html)
- [Microsoft SAPI 5 Documentation](https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms723627(v=vs.85))

## Support

For issues and questions:
- Open an issue on GitHub
- Check existing documentation
- Contact the NVDA community