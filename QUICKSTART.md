# Quick Start Guide

Get up and running with NVDA SAPI Bridge in 5 minutes!

## Prerequisites

- Windows 7 or later
- Python 3.7+ installed ([Download Python](https://www.python.org/downloads/))
- NVDA screen reader installed ([Download NVDA](https://www.nvaccess.org/download/))

## Installation (3 Steps)

### 1. Install Dependencies

Open Command Prompt and run:

```bash
pip install pywin32 comtypes
```

### 2. Register the Bridge

Right-click on `scripts\register.bat` and select **"Run as administrator"**

Or manually:
```bash
python nvda_sapi_bridge.py --register
```

### 3. Test It

Run the test script:
```bash
python test_sapi_bridge.py
```

You should see "NVDA Bridge" listed as an available voice!

## Using with Applications

### Example: Balabolka

1. Download Balabolka from http://www.cross-plus-a.com/balabolka.htm
2. Open Balabolka
3. Go to `Options` → `Voice...`
4. Select **"NVDA Bridge"** from the voice dropdown
5. Click `OK`
6. Type or paste some text
7. Press `F5` to read aloud
8. **The text will be spoken through NVDA!**

### Other Applications

Most SAPI5 applications have similar settings:
- Look for "Voice" or "Speech" settings
- Select "NVDA Bridge"
- Start using the application with NVDA speech!

## Troubleshooting

### Voice Not Appearing?

1. Ensure you ran registration as Administrator
2. Restart your application
3. Verify NVDA is running

### No Speech?

1. Press `NVDA+T` to verify NVDA is working
2. Check that "NVDA Bridge" is selected in your application
3. Ensure NVDA speech is not muted (press `NVDA+S` to cycle speech modes)

## Need Help?

- See [README.md](README.md) for detailed information
- See [INSTALL.md](INSTALL.md) for troubleshooting
- See [APPLICATIONS.md](APPLICATIONS.md) for app-specific guides

## Uninstalling

Right-click on `scripts\unregister.bat` and select **"Run as administrator"**

---

**That's it!** You can now use legacy SAPI5 applications with NVDA. 🎉
