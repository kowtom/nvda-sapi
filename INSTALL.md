# NVDA SAPI Bridge - Installation Guide

## Prerequisites

Before installing the NVDA SAPI Bridge, ensure you have:

1. **NVDA Screen Reader**: Download and install from https://www.nvaccess.org/download/
2. **Administrator Access**: Required for COM DLL registration

## Installation Steps

### Step 1: Build the Bridge

Open a Command Prompt and navigate to the project directory:

```batch
cd nvda-sapi
build.bat
```

This will create two DLL files:
- `build\x86\bin\Release\nvda_sapi32.dll` (for 32-bit applications)
- `build\x64\bin\Release\nvda_sapi64.dll` (for 64-bit applications)

### Step 2: Register the COM DLL

You need to register the appropriate DLL based on your application's architecture.

#### For 32-bit Legacy Applications

Open Command Prompt as Administrator and run:
```batch
regsvr32 "C:\path\to\nvda-sapi\build\x86\bin\Release\nvda_sapi32.dll"
```

#### For 64-bit Applications

Open Command Prompt as Administrator and run:
```batch
regsvr32 "C:\path\to\nvda-sapi\build\x64\bin\Release\nvda_sapi64.dll"
```

**Note**: If you're unsure which version your application uses, register both.

### Step 3: Configure Your Application

1. Launch your legacy application
2. Navigate to its speech or accessibility settings
3. Look for a voice selection option - you should see "NVDA" in the list of available voices
4. Select "NVDA Screen Reader Voice" or "NVDA"
5. The application should now speak through NVDA

**Important**: After registering the DLL, you may need to restart your application for it to detect the new NVDA voice.

## Verification

To verify the installation:

1. Ensure NVDA is running
2. Open your legacy application
3. Check the voice selection menu - "NVDA" should appear in the list
4. Select NVDA as the voice
5. Trigger a speech event in the application
6. You should hear the speech through NVDA

1. Ensure NVDA is running
2. Launch your legacy application
3. Navigate to voice settings and look for "NVDA" in the voices list
4. Select the NVDA voice
5. Trigger a speech event in the application
6. You should hear the speech through NVDA

If "NVDA" doesn't appear in the voice list:
- Make sure you registered the correct DLL (32-bit vs 64-bit)
- Restart the application after registration
- Check Windows Event Viewer for registration errors

## Troubleshooting

### "Module not found" Error

This usually means the DLL dependencies are missing. Ensure:
- Visual C++ Redistributables are installed
- The DLL is in an accessible location

### Registration Fails

- Ensure you're running Command Prompt as Administrator
- Check the full path to the DLL is correct
- Verify the DLL was built successfully

### No Speech Output

1. Verify NVDA is running: Press `NVDA+N` to open NVDA menu
2. Check NVDA is not muted: Press `NVDA+S` to toggle speech
3. Test NVDA directly: Press `NVDA+T` for time announcement
4. Ensure you selected "NVDA" voice in your application's voice settings
5. Restart the application after changing the voice selection
6. Ensure the correct architecture DLL is registered

### Voice Not Appearing in Application

1. Verify successful registration - you should see "DllRegisterServer in [DLL path] succeeded" message
2. Restart the application - some apps only enumerate voices at startup
3. Check if you registered the correct DLL architecture (32-bit app needs 32-bit DLL)
4. Try registering both 32-bit and 64-bit DLLs if unsure
5. Check Windows Registry:
   - Open Registry Editor (regedit)
   - Navigate to `HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens`
   - Look for "NVDA" key - it should exist after successful registration

## Uninstallation

To remove the NVDA SAPI Bridge:

1. Unregister the DLLs (as Administrator):
   ```batch
   regsvr32 /u "C:\path\to\nvda-sapi\build\x86\bin\Release\nvda_sapi32.dll"
   regsvr32 /u "C:\path\to\nvda-sapi\build\x64\bin\Release\nvda_sapi64.dll"
   ```

2. Delete the build directory:
   ```batch
   rmdir /s /q build
   ```

## Advanced Configuration

### Custom Installation Path

If you want to install the DLLs to a specific location:

1. Copy the DLL to your desired location (e.g., `C:\Program Files\NVDA-SAPI\`)
2. Register from that location:
   ```batch
   regsvr32 "C:\Program Files\NVDA-SAPI\nvda_sapi32.dll"
   ```

### Multiple Applications

You can use the bridge with multiple applications simultaneously. Each application will independently send speech to NVDA, which will queue the speech appropriately.

## Support

For issues and questions:
- Check the main README.md for troubleshooting tips
- Review NVDA documentation at https://www.nvaccess.org/documentation/
- File issues on the project repository
