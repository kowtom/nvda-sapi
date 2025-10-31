# Application-Specific Usage Guide

This guide provides step-by-step instructions for configuring popular SAPI5-compatible applications to use NVDA Bridge.

## Common SAPI5 Applications

### Balabolka

Balabolka is a free text-to-speech reader.

**Installation:**
1. Download from: http://www.cross-plus-a.com/balabolka.htm
2. Install and launch Balabolka

**Configuration:**
1. Open Balabolka
2. Click `Options` menu → `Voice...`
3. In the "Voice" dropdown, select "NVDA Bridge"
4. Adjust rate, pitch, and volume if needed (these may not affect NVDA)
5. Click `OK`

**Usage:**
- Open a text file or paste text
- Click `Reading` → `Read Aloud` (or press F5)
- Text will be spoken through NVDA

### Natural Reader

**Configuration:**
1. Open Natural Reader
2. Go to `Settings` or `Preferences`
3. Find "Voice" settings
4. Select "NVDA Bridge" from the available voices
5. Apply settings

### Read Aloud (Text to Voice)

**Configuration:**
1. Open the application
2. Go to `Settings` → `Voice Settings`
3. Select "NVDA Bridge" from the voice list
4. Save settings

### Microsoft Word (with SAPI5 Add-in)

Some Word add-ins use SAPI5 for text-to-speech:

**Configuration:**
1. Open Word
2. Find the TTS add-in settings (varies by add-in)
3. Select "NVDA Bridge" as the voice
4. Apply changes

### Adobe Reader (with Read Out Loud)

Adobe Reader can use SAPI5 voices for its Read Out Loud feature:

**Configuration:**
1. Open Adobe Reader
2. Go to `Edit` → `Preferences`
3. Select `Reading` category
4. Under "Read Out Loud Options", click "Screen Reader Options..."
5. Select "NVDA Bridge" if available
6. Click OK

**Note:** Adobe Reader may prefer AT APIs over SAPI5. If NVDA Bridge doesn't appear, ensure NVDA is running and try toggling the Read Out Loud settings.

### Windows Narrator

**Note:** Windows Narrator can use SAPI5 voices, but it's recommended to use NVDA directly instead of Narrator. However, to test:

**Configuration:**
1. Open Control Panel
2. Go to `Speech Recognition` → `Text to Speech`
3. Select "NVDA Bridge" from the voice dropdown
4. Open Narrator (Windows key + Ctrl + Enter)

## Programming and Development Tools

### Eclipse IDE (with TTS Plugin)

If you have a TTS plugin installed:

**Configuration:**
1. Go to `Window` → `Preferences`
2. Find the TTS plugin settings
3. Select "NVDA Bridge" as the voice
4. Apply settings

### Visual Studio (with Productivity Power Tools or similar)

Some VS extensions support SAPI5:

**Configuration:**
1. Go to `Tools` → `Options`
2. Find the TTS extension settings
3. Select "NVDA Bridge"
4. Apply

## Educational Software

Many educational applications use SAPI5 for speech feedback.

### General Configuration Steps:

1. Open the educational application
2. Look for:
   - Settings/Preferences
   - Accessibility Settings
   - Voice Settings
   - Audio/Speech Settings
3. Find the voice selection dropdown
4. Choose "NVDA Bridge"
5. Save/Apply changes

## E-Book Readers

### Calibre E-Book Reader (with TTS Plugin)

**Configuration:**
1. Install the TTS plugin for Calibre
2. In Calibre, go to Preferences → Plugins → Text to Speech
3. Configure to use "NVDA Bridge"
4. Open an e-book and enable read-aloud

## Communication Software

### Text-to-Speech Chat Readers

Some accessibility-focused chat applications use SAPI5:

**Configuration:**
1. Open the chat application settings
2. Find "Text-to-Speech" or "Accessibility" settings
3. Select "NVDA Bridge" as the voice
4. Configure when to speak (new messages, mentions, etc.)

## Custom Applications

For custom or proprietary applications using SAPI5:

### Finding Voice Settings

Voice settings are commonly found in:
- `Settings` → `Accessibility`
- `Preferences` → `Speech` or `Audio`
- `Options` → `Text-to-Speech`
- `Tools` → `Voice Settings`

### If Voice Settings Aren't Obvious:

1. Check the application's help documentation
2. Look for accessibility-related menu items
3. Contact the application developer for guidance

### Testing if an Application Uses SAPI5:

1. Open Windows Speech Settings (Control Panel → Speech Recognition → Text to Speech)
2. Set a distinctive voice as default (e.g., one with a very different accent)
3. Use the application's speech feature
4. If you hear the default voice, the application uses SAPI5
5. Return to Windows Speech Settings and select "NVDA Bridge"

## System-Wide Default Voice

To make NVDA Bridge the default for all SAPI5 applications:

**Configuration:**
1. Open Control Panel
2. Navigate to `Speech Recognition` → `Text to Speech`
3. Select "NVDA Bridge" from the voice dropdown
4. Click `Apply`

**Note:** This affects all applications that use the system default SAPI5 voice.

## Troubleshooting Application-Specific Issues

### Application Doesn't List NVDA Bridge

**Possible Causes:**
- Application caches voice list
- Application needs restart after bridge installation
- Application uses a specific SAPI version

**Solutions:**
1. Restart the application
2. Log out and log back in to Windows
3. Reboot the computer
4. Check if application has a "Refresh" or "Reload" voices button

### Voice Selected But No Speech

**Possible Causes:**
- NVDA not running
- Application volume muted
- NVDA speech mode disabled

**Solutions:**
1. Ensure NVDA is running (press NVDA+T to test)
2. Check application volume settings
3. Press NVDA+S to cycle through speech modes (ensure not set to "off")
4. Check NVDA's synthesizer settings

### Application Uses Default Voice Instead

**Some Applications Ignore Voice Selection:**

If an application ignores your voice selection:
1. Try setting NVDA Bridge as the system default
2. Check for application-specific configuration files
3. Consult application documentation

### Application Crashes

**If an application crashes when using NVDA Bridge:**
1. Try a different SAPI5 voice to verify the application works
2. Check Windows Event Viewer for error details
3. Update the application to the latest version
4. Report the issue on the NVDA Bridge GitHub with:
   - Application name and version
   - Error messages
   - Steps to reproduce

## Tips for Best Experience

### NVDA Configuration

For best results with NVDA Bridge:

1. Configure NVDA's synthesizer settings to your preference
2. Adjust NVDA's speech rate, pitch, and volume
3. These settings will apply to speech from SAPI5 applications via the bridge

### Application Settings

In SAPI5 applications:

1. Rate, pitch, and volume settings may not affect NVDA
2. Use NVDA's own settings instead
3. Disable any echo or repeat features in the application
4. NVDA will handle speech output

### Multiple Applications

You can use NVDA Bridge with multiple applications simultaneously:

1. All speech goes through NVDA
2. NVDA may interrupt previous speech when new speech arrives
3. Configure NVDA's interruption behavior in NVDA settings

## Providing Feedback

If you successfully configure an application not listed here:

1. Consider contributing to this guide
2. Open a pull request or issue on GitHub
3. Include:
   - Application name and version
   - Configuration steps
   - Any special considerations

## Additional Resources

- NVDA User Guide: https://www.nvaccess.org/files/nvda/documentation/userGuide.html
- SAPI5 Documentation: https://docs.microsoft.com/en-us/previous-versions/windows/desktop/ms723627(v=vs.85)
- Application-specific forums and documentation
