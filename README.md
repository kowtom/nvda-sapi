# NVDA SAPI Bridge

A SAPI5 Text-to-Speech bridge that routes speech from legacy Windows applications to the NVDA screen reader.

## Overview

This project provides a solution for totally blind users who want to use legacy Windows applications that support SAPI5 (Speech API 5) speech output with the NVDA (NonVisual Desktop Access) screen reader.

The NVDA SAPI Bridge registers as a SAPI5 TTS voice. When legacy applications use SAPI5 to speak, the bridge intercepts the text and forwards it to NVDA using its controller client API.

## Features

- **SAPI5 Compatible**: Works with any application that uses SAPI5 for speech output
- **NVDA Integration**: Seamlessly forwards speech to NVDA screen reader
- **Open Source**: Built with Python and open source libraries
- **Easy Installation**: Simple installation with Python package manager
- **Lightweight**: Minimal overhead, efficient implementation
- **Cross-platform Friendly**: Python-based, easier to maintain and extend

## Requirements

### Runtime Requirements
- Windows 7 or later (64-bit or 32-bit)
- Python 3.7 or later
- NVDA screen reader installed and running
- Legacy application that supports SAPI5 speech output

### Python Dependencies
- pywin32 (Windows Python extensions)
- comtypes (COM automation)

## Installation

### Step 1: Install Python

1. Download Python from https://www.python.org/downloads/
2. During installation, make sure to check "Add Python to PATH"
3. Complete the installation

### Step 2: Install Dependencies

Open Command Prompt and run:

```bash
pip install -r requirements.txt
```

Or install manually:

```bash
pip install pywin32 comtypes
```

### Step 3: Register the Bridge

1. Open Command Prompt **as Administrator**
2. Navigate to the project directory
3. Run:

```bash
python nvda_sapi_bridge.py --register
```

Or simply run `scripts\register.bat` as Administrator.

## Usage

1. **Start NVDA** screen reader

2. **Configure your legacy application** to use the "NVDA Bridge" voice
   - In most SAPI5 applications, go to speech settings
   - Select "NVDA Bridge" from the list of available voices

3. **Use the application normally**
   - All speech output will now be routed through NVDA
   - NVDA's configured voice and settings will be used

## Testing

Test the installation with the provided test script:

```bash
python test_sapi_bridge.py
```

This will:
- List all available SAPI5 voices
- Attempt to speak a test message through NVDA Bridge
- Verify that the bridge is working correctly

You can also test with any SAPI5 application, such as:
- Balabolka (free SAPI5 text reader)
- Natural Reader
- Many educational and accessibility applications

## Uninstallation

### Quick Uninstall

Run `scripts\unregister.bat` as Administrator.

### Manual Uninstall

```bash
python nvda_sapi_bridge.py --unregister
```

## Architecture

```
Legacy Application (SAPI5)
         ↓
NVDA SAPI Bridge (Python COM Server)
         ↓
NVDA Controller Client API
         ↓
NVDA Screen Reader
         ↓
Speech Output (configured TTS engine)
```

## Technical Details

- **Language**: Python 3
- **COM Framework**: pywin32 and comtypes
- **SAPI Version**: SAPI 5.x
- **NVDA Integration**: nvdaControllerClient API

## Troubleshooting

### "NVDA Bridge" voice not appearing

- Ensure the bridge is registered: Run `python nvda_sapi_bridge.py --register` as Administrator
- Restart the application that uses SAPI5
- Check that Python is installed correctly and in PATH

### No speech output

- Ensure NVDA is running (press NVDA+Ctrl+T to test)
- Check that NVDA controller client DLLs are available (installed with NVDA)
- Verify the legacy application is configured to use "NVDA Bridge" voice
- Check NVDA speech settings

### Python errors during registration

- Ensure you're running as Administrator
- Check that all dependencies are installed: `pip install -r requirements.txt`
- Verify Python version is 3.7 or later: `python --version`

### Application crashes

- Check Windows Event Viewer for error details
- Ensure pywin32 is properly installed: `pip install --upgrade pywin32`
- After installing/upgrading pywin32, run: `python Scripts/pywin32_postinstall.py -install` (from Python directory)

## Development

### Project Structure

```
nvda-sapi/
├── nvda_sapi_bridge.py      # Main bridge implementation
├── test_sapi_bridge.py      # Test script
├── requirements.txt         # Python dependencies
├── scripts/
│   ├── register.bat         # Windows registration script
│   └── unregister.bat       # Windows unregistration script
├── README.md
├── INSTALL.md
└── LICENSE
```

### Extending the Bridge

The bridge can be extended to support additional features:
- Custom voice parameters (rate, volume, pitch)
- Multiple NVDA profiles
- Logging and debugging
- Advanced text processing

See the source code comments for details.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## Credits

- NVDA team for the excellent screen reader and controller client API
- Microsoft for SAPI5 specification
- Python pywin32 and comtypes developers

## Support

For issues and questions:
- Open an issue on GitHub
- Consult NVDA community forums at https://nvda.groups.io/
- Check SAPI5 documentation for application-specific questions

## Alternatives

If this solution doesn't work for your needs, consider:
- Using NVDA directly (many modern applications support screen readers directly)
- Checking if your legacy application has NVDA support plugins
- Using Windows built-in accessibility features