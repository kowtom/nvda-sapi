# NVDA SAPI Bridge - Architecture Documentation

## Overview

The NVDA SAPI Bridge is a COM DLL that acts as a SAPI5 Text-to-Speech engine, forwarding speech requests to the NVDA screen reader. This document describes the technical architecture and design decisions.

## System Architecture

```
┌─────────────────────────┐
│   Legacy Application    │
│  (32-bit or 64-bit)     │
└───────────┬─────────────┘
            │ SAPI5 Interface
            │ (ISpVoice)
            ▼
┌─────────────────────────┐
│   Windows SAPI5 Core    │
│  (sapicpl.dll, sapi.dll)│
└───────────┬─────────────┘
            │ ISpTTSEngine
            │
            ▼
┌─────────────────────────┐
│  NVDA SAPI Bridge DLL   │
│   (nvda_sapi32/64.dll)  │
│                         │
│  ┌──────────────────┐   │
│  │   SAPIVoice      │   │
│  │  (ISpTTSEngine)  │   │
│  └────────┬─────────┘   │
│           │             │
│  ┌────────▼─────────┐   │
│  │   NVDAClient     │   │
│  │ (Controller API) │   │
│  └──────────────────┘   │
└───────────┬─────────────┘
            │ nvdaControllerClient API
            │
            ▼
┌─────────────────────────┐
│   NVDA Screen Reader    │
│   (nvda.exe)            │
└─────────────────────────┘
```

## Component Design

### 1. COM Bridge Layer (`nvda_sapi_bridge.cpp/h`)

**Responsibilities:**
- DLL entry point and initialization
- COM server registration/unregistration
- Class factory implementation for creating SAPI voice instances
- Server lock management

**Key Classes:**
- `SAPIVoiceFactory`: IClassFactory implementation for creating voice instances

**Key Functions:**
- `DllMain`: DLL initialization
- `DllGetClassObject`: COM class factory retrieval
- `DllCanUnloadNow`: Determines if DLL can be unloaded
- `DllRegisterServer`: Registers COM server in Windows registry
- `DllUnregisterServer`: Removes COM registration

**Design Decisions:**
- Uses a unique CLSID to avoid conflicts with other SAPI engines
- Implements standard COM reference counting
- Supports apartment threading model for compatibility

### 2. SAPI Voice Engine (`sapi_voice.cpp/h`)

**Responsibilities:**
- Implements SAPI5 TTS engine interface (`ISpTTSEngine`)
- Processes text fragments from SAPI
- Manages NVDA client instance

**Key Classes:**
- `SAPIVoice`: Main TTS engine implementation

**Key Methods:**
- `Speak()`: Processes SAPI text fragments and forwards to NVDA
- `GetOutputFormat()`: Returns audio format (returns text format since we don't generate audio)

**Design Decisions:**
- Uses `std::unique_ptr` for NVDA client (RAII)
- Aggregates text fragments before sending to NVDA
- Returns success even if NVDA is unavailable to avoid breaking applications
- No audio generation (text-only processing)

### 3. NVDA Client Layer (`nvda_client.cpp/h`)

**Responsibilities:**
- Loads NVDA controller client DLL
- Manages function pointers to NVDA API
- Provides simplified interface for speech operations

**Key Classes:**
- `NVDAClient`: Wrapper for NVDA controller client API

**Key Methods:**
- `Initialize()`: Loads controller client and resolves functions
- `Speak()`: Sends text to NVDA for speech
- `CancelSpeech()`: Cancels ongoing speech
- `IsNVDARunning()`: Checks NVDA availability

**Design Decisions:**
- Dynamically loads NVDA controller client (no static linking)
- Tries both 32-bit and 64-bit controller client DLLs
- Gracefully handles NVDA not being available
- Uses RAII for DLL handle management

## Data Flow

### Speech Request Flow

1. **Application initiates speech:**
   - Legacy app calls `ISpVoice::Speak()`
   - Windows SAPI core routes to registered TTS engine

2. **SAPI engine receives request:**
   - `SAPIVoice::Speak()` is called with text fragments
   - Text fragments are parsed and concatenated

3. **Text forwarding:**
   - Complete text is passed to `NVDAClient::Speak()`
   - NVDA client checks if NVDA is running

4. **NVDA speech:**
   - Text is sent via `nvdaController_speakText()`
   - NVDA queues and speaks the text

5. **Completion:**
   - `CompleteSkip()` notifies SAPI of completion
   - Control returns to application

## Threading Model

- **COM Apartment Threading**: Each instance runs in its own apartment
- **Thread Safety**: COM handles synchronization
- **NVDA Client**: Thread-safe through COM serialization

## Error Handling

### Strategy
1. **Graceful Degradation**: Returns success even if NVDA unavailable
2. **Null Checks**: All pointer parameters validated
3. **COM Error Codes**: Standard HRESULT returns
4. **Resource Cleanup**: RAII ensures proper cleanup

### Error Scenarios

| Scenario | Handling |
|----------|----------|
| NVDA not running | Return S_OK (silent failure) |
| Invalid parameters | Return E_INVALIDARG/E_POINTER |
| Out of memory | Return E_OUTOFMEMORY |
| Controller client missing | Initialization fails gracefully |

## Memory Management

- **Reference Counting**: COM objects use `AddRef()`/`Release()`
- **Smart Pointers**: `std::unique_ptr` for owned objects
- **No Memory Leaks**: All resources cleaned up in destructors
- **CoTaskMemAlloc**: Used for COM string allocations

## Build System

### CMake Configuration
- **Multi-Architecture**: Supports both x86 and x64
- **Modular**: Separate CMakeLists for source
- **Dependencies**: Minimal (Windows SDK only)

### Compilation
- **C++17 Standard**: Modern C++ features
- **Unicode**: Full Unicode support (UNICODE/_UNICODE)
- **Optimization**: Release builds optimized

## Registry Structure

When registered, the bridge creates two sets of registry entries:

### 1. COM Server Registration

```
HKEY_CLASSES_ROOT\
  CLSID\
    {A65F3370-547A-4E90-90B1-F5DF86FB7815}\
      InProcServer32\
        (Default) = "C:\path\to\nvda_sapi32.dll"
        ThreadingModel = "Apartment"
```

### 2. SAPI Voice Token Registration

This is critical for voice discovery by SAPI applications:

```
HKEY_LOCAL_MACHINE\
  SOFTWARE\
    Microsoft\
      Speech\
        Voices\
          Tokens\
            NVDA\
              (Default) = "NVDA Screen Reader Voice"
              CLSID = "{A65F3370-547A-4E90-90B1-F5DF86FB7815}"
              LangDataPath = "409"
              Attributes\
                Language = "409"
                Gender = "Neutral"
                Age = "Adult"
                Vendor = "NVDA"
                Name = "NVDA"
```

The voice token registration is what makes the NVDA voice appear in application voice selection lists. Without this, applications cannot discover the TTS engine even if the COM server is properly registered.

**Key Points:**
- Voice tokens are stored in `HKEY_LOCAL_MACHINE` (requires admin rights)
- The CLSID links the voice token to the TTS engine COM class
- Attributes help applications filter and display voices appropriately
- Language code 409 = US English (can be extended for other languages)

## Performance Considerations

- **Minimal Overhead**: Direct text forwarding
- **No Audio Processing**: No DSP or encoding
- **Lazy Loading**: NVDA client loaded on first use
- **Efficient Text Processing**: Single pass fragment concatenation

## Security Considerations

- **Input Validation**: All SAPI inputs validated
- **No Buffer Overflows**: Uses safe string functions
- **DLL Hijacking Protection**: Uses full paths for NVDA client
- **COM Security**: Standard COM security model

## Limitations

1. **No Audio Generation**: Cannot be used with applications expecting audio output
2. **NVDA Required**: Requires NVDA to be running
3. **Windows Only**: Platform-specific (Windows COM)
4. **Text Only**: No support for SSML or phonetic pronunciation from SAPI

## Future Enhancements

Potential improvements:
- SSML support for advanced speech control
- Configuration UI for voice parameters
- Logging and diagnostics
- Support for SAPI events (word boundaries, phonemes)
- Multiple voice selection

## Testing Strategy

Recommended testing:
1. **Unit Tests**: Test individual components in isolation
2. **Integration Tests**: Test with actual SAPI applications
3. **Architecture Tests**: Verify both x86 and x64 builds
4. **NVDA Tests**: Test with NVDA running and stopped
5. **Stress Tests**: Multiple simultaneous speech requests

## Dependencies

### Build Time
- CMake 3.15+
- Windows SDK (for SAPI headers)
- C++17 compiler

### Runtime
- Windows 7+
- NVDA with controller client DLL
- Visual C++ Runtime (typically pre-installed)

## Versioning

Version scheme: MAJOR.MINOR.PATCH
- **MAJOR**: Breaking API changes
- **MINOR**: New features
- **PATCH**: Bug fixes
