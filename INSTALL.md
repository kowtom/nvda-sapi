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
3. Select SAPI5 as the speech engine (if options are available)
4. The application should now speak through NVDA

## Verification

To verify the installation:

1. Ensure NVDA is running
2. Launch your legacy application
3. Trigger a speech event in the application
4. You should hear the speech through NVDA

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
4. Ensure the correct architecture DLL is registered

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
