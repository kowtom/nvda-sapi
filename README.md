# NVDA-SAPI Bridge

A SAPI5 Text-to-Speech engine that bridges legacy Windows applications to the NVDA screen reader.

## Overview

This project provides a solution for totally blind users who want to use legacy Windows applications that support SAPI5 speech output with their NVDA screen reader. The bridge acts as a SAPI5 TTS engine that forwards all speech requests to NVDA.

## Features

- Acts as a standard SAPI5 TTS engine
- Forwards all speech output to NVDA
- Compatible with any Windows application that uses SAPI5 for speech
- Simple installation and configuration
- Minimal resource usage

## Requirements

- Windows 7 or later
- NVDA screen reader installed and running
- Visual Studio 2017 or later (for building from source)
- CMake 3.15 or later (for building from source)

## Installation

### Using Pre-built Binaries

1. Download the latest release from the releases page
2. Extract the archive to a folder
3. Right-click `register.bat` and select "Run as Administrator"
4. NVDA Bridge will now appear as a voice option in SAPI5 applications

### Building from Source

1. Clone this repository:
   ```
   git clone https://github.com/kowtom/nvda-sapi.git
   cd nvda-sapi
   ```

2. Obtain the NVDA Controller Client library:
   - Download `nvdaControllerClient.lib` from the NVDA source repository
   - Place it in the `nvdaControllerClient/` directory
   - See `nvdaControllerClient/README.md` for details

3. Build using CMake:
   ```
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019" -A Win32
   cmake --build . --config Release
   ```

4. Register the DLL:
   - Navigate to the `build/Release` directory
   - Copy `nvda-sapi.dll` to the `scripts/` folder
   - Run `scripts/register.bat` as Administrator

## Usage

1. Ensure NVDA is running
2. Open a SAPI5-compatible application
3. Select "NVDA Bridge" as the voice in the application's speech settings
4. Speech output will now go through NVDA

### Supported Applications

Any application that uses SAPI5 for text-to-speech, including:
- Adobe Reader (older versions)
- Balabolka
- Text Aloud
- Various accessibility tools
- Legacy business applications

## How It Works

The NVDA-SAPI Bridge implements the SAPI5 `ISpTTSEngine` interface. When an application requests speech:

1. The application calls SAPI5 with text to speak
2. SAPI5 routes the request to NVDA-SAPI Bridge
3. The bridge extracts the text and forwards it to NVDA using the NVDA Controller Client API
4. NVDA speaks the text using its configured speech synthesizer

## Uninstallation

1. Right-click `unregister.bat` in the scripts folder
2. Select "Run as Administrator"
3. Delete the installation folder

## Troubleshooting

**NVDA doesn't speak:**
- Ensure NVDA is running before starting the SAPI5 application
- Check that NVDA Bridge is selected as the voice in your application

**Registration fails:**
- Make sure you run the registration script as Administrator
- Check that the DLL is in the same folder as the registration scripts

**Voice doesn't appear:**
- Re-run the registration script
- Check Windows Event Viewer for COM registration errors

## Development

### Project Structure

```
nvda-sapi/
├── CMakeLists.txt              # Build configuration
├── src/
│   ├── NvdaSapiEngine.cpp     # Main TTS engine implementation
│   ├── NvdaSapiEngine.def     # DLL exports definition
│   └── dllmain.cpp            # DLL entry point and COM registration
├── include/
│   └── NvdaSapiEngine.h       # Header file
├── nvdaControllerClient/
│   ├── nvdaControllerClient.h # NVDA Controller Client API header
│   └── README.md              # Instructions for obtaining the library
└── scripts/
    ├── register.bat           # Registration script
    ├── unregister.bat         # Unregistration script
    ├── nvda-sapi-register.reg # Registry entries for registration
    └── nvda-sapi-unregister.reg # Registry entries for unregistration
```

### Technical Details

- **COM CLSID:** `{E6F6B6F0-8F3A-4B3E-9F3E-3F3E3F3E3F3E}`
- **Interface:** Implements `ISpTTSEngine` and `ISpObjectWithToken`
- **Threading Model:** Both (apartment and free-threaded)

## License

This project is licensed under the GNU General Public License v2.0, compatible with NVDA's license.

## Contributing

Contributions are welcome! Please feel free to submit issues or pull requests.

## Acknowledgments

- NVDA development team for the NVDA Controller Client API
- Microsoft for the SAPI5 API documentation

## Contact

For issues and questions, please use the GitHub issue tracker.