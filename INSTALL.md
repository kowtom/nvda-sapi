# Installation Guide

This guide walks you through installing and configuring the NVDA SAPI Bridge.

## Prerequisites

Before installation, ensure you have:

1. **Windows 7 or later** (64-bit or 32-bit)
2. **Python 3.7 or later** installed
   - Download from: https://www.python.org/downloads/
   - During installation, check "Add Python to PATH"
3. **NVDA screen reader** installed and working
   - Download from: https://www.nvaccess.org/download/

## Installation Steps

### Step 1: Install Python

If you don't have Python installed:

1. Download Python from https://www.python.org/downloads/
2. Run the installer
3. **Important**: Check "Add Python to PATH" during installation
4. Click "Install Now"
5. Verify installation by opening Command Prompt and running:
   ```bash
   python --version
   ```
   You should see something like "Python 3.11.x"

### Step 2: Install Python Dependencies

Open Command Prompt and navigate to the project directory:

```bash
cd path\to\nvda-sapi
```

Install dependencies:

```bash
pip install -r requirements.txt
```

Or install manually:

```bash
pip install pywin32 comtypes
```

**Note**: If you get permission errors, try:
```bash
pip install --user pywin32 comtypes
```

### Step 3: Post-install for pywin32

After installing pywin32, you may need to run the post-install script:

1. Find your Python installation directory (usually `C:\Python3x\` or `C:\Users\YourName\AppData\Local\Programs\Python\Python3x\`)
2. Run:
   ```bash
   python Scripts\pywin32_postinstall.py -install
   ```

### Step 4: Register the NVDA SAPI Bridge

**Option A: Using the Batch Script (Recommended)**

1. Right-click on `scripts\register.bat`
2. Select "Run as administrator"
3. Follow the on-screen prompts
4. Wait for "Registration successful!" message

**Option B: Manual Registration via Python**

1. Open Command Prompt **as Administrator**
2. Navigate to the project directory
3. Run:
   ```bash
   python nvda_sapi_bridge.py --register
   ```

### Step 5: Verify Installation

1. Start NVDA (if not already running)
2. Open Command Prompt and run:
   ```bash
   python test_sapi_bridge.py
   ```
3. You should see "NVDA Bridge" listed among available voices
4. If the test speaks a message through NVDA, the installation is successful!

## Testing the Installation

### Automated Test

Run the test script:

```bash
python test_sapi_bridge.py
```

This will:
- List all SAPI5 voices (including NVDA Bridge)
- Attempt to speak a test message
- Verify the bridge is working

### Manual Test with PowerShell

You can also test with this PowerShell one-liner:

```powershell
Add-Type -AssemblyName System.Speech; $s = New-Object System.Speech.Synthesis.SpeechSynthesizer; $s.SelectVoice("NVDA Bridge"); $s.Speak("Testing NVDA Bridge")
```

### Test with Third-Party Applications

Try configuring these free applications to use NVDA Bridge:

- **Balabolka**: Free text-to-speech reader
  - Download: http://www.cross-plus-a.com/balabolka.htm
  - Settings → Voice → Select "NVDA Bridge"

- **@Voice**: Text reader
  - Settings → TTS → Select "NVDA Bridge"

## Configuration in Applications

### General SAPI5 Application Configuration

Most applications with SAPI5 support have similar settings:

1. Open the application
2. Find Settings/Preferences/Options
3. Look for:
   - "Text to Speech"
   - "Voice Settings"
   - "Speech"
   - "Accessibility"
4. Select "NVDA Bridge" from the voice dropdown
5. Apply and save

### Specific Application Examples

#### Example: Configuring in Balabolka

1. Open Balabolka
2. Click "Options" → "Voice"
3. In the "Voice" dropdown, select "NVDA Bridge"
4. Click "OK"

#### Example: Configuring in Windows Speech Settings

To set NVDA Bridge as the default system voice:

1. Open Control Panel
2. Go to "Speech Recognition"
3. Click "Text to Speech" tab
4. Select "NVDA Bridge" from voice dropdown
5. Click "Apply"

**Note**: This affects all SAPI5 applications that don't explicitly choose a voice.

## Troubleshooting

### Issue: Python not found

**Symptoms:**
- `'python' is not recognized as an internal or external command`

**Solution:**
- Reinstall Python and check "Add Python to PATH"
- Or add Python manually to PATH:
  1. Find Python installation directory
  2. Add to PATH environment variable

### Issue: pip install fails

**Symptoms:**
- Permission errors
- Network errors

**Solutions:**
- Use `pip install --user pywin32 comtypes`
- Check internet connection
- Try using a different network or VPN if behind corporate firewall

### Issue: "NVDA Bridge" doesn't appear

**Symptoms:**
- Voice not in application's voice list
- Registration appears successful but voice missing

**Solutions:**
1. Restart the application after registration
2. Re-register the bridge as Administrator:
   ```bash
   python nvda_sapi_bridge.py --unregister
   python nvda_sapi_bridge.py --register
   ```
3. Check registration in Registry Editor:
   - Press Win+R, type `regedit`
   - Navigate to: `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens\`
   - Look for "NVDABridge" key

### Issue: No speech output

**Symptoms:**
- Voice selected but nothing is spoken
- No errors shown

**Solutions:**
1. **Verify NVDA is running:**
   - Press NVDA+T to make NVDA speak the window title
   - If nothing happens, start NVDA

2. **Check NVDA Controller Client:**
   - The DLLs should be in NVDA installation directory
   - Usually `C:\Program Files (x86)\NVDA\` or `C:\Program Files\NVDA\`

3. **Test NVDA directly:**
   - Press NVDA+Ctrl+T to speak the title
   - If this works, NVDA is functioning

4. **Check application settings:**
   - Verify volume is not muted
   - Check rate/speed settings are reasonable

### Issue: Registration fails with COM errors

**Symptoms:**
- Error during registration
- COM or pywin32 related errors

**Solutions:**
1. Run post-install for pywin32:
   ```bash
   python Scripts\pywin32_postinstall.py -install
   ```

2. Reinstall pywin32:
   ```bash
   pip uninstall pywin32
   pip install pywin32
   ```

3. Ensure running as Administrator

### Issue: Application crashes

**Symptoms:**
- Application closes unexpectedly when using NVDA Bridge

**Solutions:**
1. Check Windows Event Viewer for details
2. Try a different SAPI5 voice to verify the application works
3. Update Python and dependencies:
   ```bash
   pip install --upgrade pywin32 comtypes
   ```

4. Report the issue with:
   - Application name and version
   - Python version
   - Error messages from Event Viewer

## Advanced Configuration

### Running as a Windows Service

For applications that start before user login, you may need to register the bridge for all users:

1. Register as SYSTEM user using PsExec:
   ```bash
   psexec -i -s python nvda_sapi_bridge.py --register
   ```

### Multiple Python Versions

If you have multiple Python versions:

1. Use full path to desired Python:
   ```bash
   C:\Python39\python.exe nvda_sapi_bridge.py --register
   ```

2. Or use py launcher:
   ```bash
   py -3.9 nvda_sapi_bridge.py --register
   ```

## Uninstallation

### Quick Uninstall

1. Right-click `scripts\unregister.bat`
2. Select "Run as administrator"

### Manual Uninstall

```bash
python nvda_sapi_bridge.py --unregister
```

### Complete Removal

To remove all traces:

1. Unregister the bridge (see above)
2. Uninstall Python dependencies (optional):
   ```bash
   pip uninstall pywin32 comtypes
   ```
3. Delete the project directory
4. Remove Python (optional, if not needed for other applications)

## Getting Help

If you encounter issues not covered here:

1. Check the troubleshooting section above
2. Review the README.md file
3. Run the test script for diagnostic information:
   ```bash
   python test_sapi_bridge.py
   ```
4. Open an issue on GitHub with:
   - Python version (`python --version`)
   - NVDA version
   - Error messages
   - Steps to reproduce

## Next Steps

Once installed and verified:

1. Configure your legacy applications to use NVDA Bridge
2. Customize NVDA voice settings for optimal experience
3. Share with other NVDA users who need SAPI5 compatibility
4. Consider contributing improvements to the project!
