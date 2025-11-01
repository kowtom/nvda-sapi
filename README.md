# NVDA SAPI Bridge

A SAPI5 (Speech API 5) bridge that enables legacy Windows applications to use NVDA screen reader for speech output.

## Overview

This project provides a COM DLL that implements the SAPI5 TTS (Text-To-Speech) interface and forwards speech requests to NVDA. This allows blind users to use legacy Windows applications that only support SAPI5 with their preferred NVDA screen reader.

## Features

- ✅ Full SAPI5 TTS engine implementation
- ✅ Seamless integration with NVDA screen reader
- ✅ Support for both 32-bit and 64-bit architectures
- ✅ Modern C++17 codebase with best practices
- ✅ Simple, clean design
- ✅ Easy command-line building

## Requirements

### Build Requirements
- CMake 3.15 or later
- Visual Studio 2022 (or compatible C++ compiler with Windows SDK)
- Windows SDK with SAPI5 headers

### Runtime Requirements
- Windows 7 or later
- NVDA screen reader installed and running
- NVDA Controller Client DLL (usually included with NVDA)

## Building

### From Windows Command Line

Using the provided batch script:
```batch
build.bat
```

Or manually:
```batch
# Create build directory
mkdir build
cd build

# Configure for x86 (32-bit)
cmake -G "Visual Studio 17 2022" -A Win32 ..
cmake --build . --config Release

# Configure for x64 (64-bit)
cmake -G "Visual Studio 17 2022" -A x64 ..
cmake --build . --config Release
```

### From Unix-like Shell (with MinGW)

```bash
chmod +x build.sh
./build.sh
```

## Installation

After building, you need to register the COM DLL:

### For 32-bit applications
```batch
regsvr32 build\x86\bin\Release\nvda_sapi32.dll
```

### For 64-bit applications
```batch
regsvr32 build\x64\bin\Release\nvda_sapi64.dll
```

**Note:** You may need administrator privileges to register the DLL.

## Usage

1. Ensure NVDA is installed and running
2. Build and register the appropriate DLL (32-bit or 64-bit based on your application)
3. Configure your legacy application to use SAPI5 for speech output
4. The application's speech will now be routed through NVDA

## Uninstallation

To unregister the COM DLL:

```batch
regsvr32 /u build\x86\bin\Release\nvda_sapi32.dll
regsvr32 /u build\x64\bin\Release\nvda_sapi64.dll
```

## Architecture

The bridge consists of three main components:

1. **SAPI Voice Engine** (`sapi_voice.cpp/h`): Implements the `ISpTTSEngine` interface required by SAPI5
2. **NVDA Client** (`nvda_client.cpp/h`): Handles communication with NVDA using the controller client API
3. **COM Bridge** (`nvda_sapi_bridge.cpp/h`): Provides COM registration and class factory

### How It Works

1. Legacy application requests speech via SAPI5 interface
2. Our bridge receives the speech request through `ISpTTSEngine::Speak()`
3. Text is extracted from SAPI text fragments
4. Text is forwarded to NVDA using `nvdaController_speakText()`
5. NVDA speaks the text using its configured voice

## Development

### Code Structure
```
nvda-sapi/
├── CMakeLists.txt          # Main CMake configuration
├── build.bat               # Windows build script
├── build.sh                # Unix build script
├── README.md               # This file
└── src/
    ├── CMakeLists.txt      # Source CMake configuration
    ├── nvda_sapi_bridge.cpp/h    # COM bridge and registration
    ├── sapi_voice.cpp/h           # SAPI5 TTS engine implementation
    ├── nvda_client.cpp/h          # NVDA communication layer
    └── nvda_sapi_bridge.def       # DLL exports definition
```

### Coding Standards

- Modern C++17
- RAII principles for resource management
- Clear separation of concerns
- Comprehensive error handling
- Minimal external dependencies

## Troubleshooting

### Speech not working
- Verify NVDA is running
- Check that the correct DLL (32/64-bit) is registered for your application
- Ensure NVDA controller client DLL is accessible

### Registration fails
- Run command prompt as Administrator
- Verify the DLL path is correct
- Check Windows Event Viewer for detailed error messages

### Build errors
- Ensure you have Visual Studio 2022 or compatible compiler
- Verify Windows SDK is installed with SAPI5 headers
- Check CMake version is 3.15 or later

## License

This project is provided as-is for use by the blind and visually impaired community.

## Contributing

Contributions are welcome! Please ensure code follows the existing style and standards.

## Acknowledgments

- NVDA development team for the excellent screen reader
- Microsoft for SAPI5 specification