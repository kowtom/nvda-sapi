#include "nvda_sapi_bridge.h"
#include "sapi_voice.h"
#include <strsafe.h>
#include <olectl.h>

// Define SELFREG error codes if not available
#ifndef SELFREG_E_TYPELIB
#define SELFREG_E_TYPELIB _HRESULT_TYPEDEF_(0x80029C4AL)
#endif

#ifndef SELFREG_E_CLASS
#define SELFREG_E_CLASS _HRESULT_TYPEDEF_(0x80029C45L)
#endif

// {A65F3370-547A-4E90-90B1-F5DF86FB7815} - NVDA SAPI Bridge CLSID
// This GUID uniquely identifies our SAPI voice engine
static const CLSID CLSID_NVDASAPIBridge = 
    {0xA65F3370, 0x547A, 0x4E90, {0x90, 0xB1, 0xF5, 0xDF, 0x86, 0xFB, 0x78, 0x15}};

// Voice token name - this is what appears in SAPI applications
static const wchar_t* VOICE_TOKEN_NAME = L"NVDA";
static const wchar_t* VOICE_DESCRIPTION = L"NVDA Screen Reader Voice";

// Global variables
HMODULE g_hModule = nullptr;
LONG g_serverLocks = 0;

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

// SAPIVoiceFactory implementation
SAPIVoiceFactory::SAPIVoiceFactory()
    : m_refCount(1) {
}

SAPIVoiceFactory::~SAPIVoiceFactory() {
}

STDMETHODIMP SAPIVoiceFactory::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_IClassFactory) {
        *ppvObject = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) SAPIVoiceFactory::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) SAPIVoiceFactory::Release() {
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0) {
        delete this;
    }
    return refCount;
}

STDMETHODIMP SAPIVoiceFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) {
    if (!ppvObject) {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if (pUnkOuter) {
        return CLASS_E_NOAGGREGATION;
    }

    IUnknown* pUnknown = nullptr;
    HRESULT hr = CreateSAPIVoice(&pUnknown);
    if (FAILED(hr)) {
        return hr;
    }

    hr = pUnknown->QueryInterface(riid, ppvObject);
    pUnknown->Release();

    return hr;
}

STDMETHODIMP SAPIVoiceFactory::LockServer(BOOL fLock) {
    if (fLock) {
        InterlockedIncrement(&g_serverLocks);
    } else {
        InterlockedDecrement(&g_serverLocks);
    }
    return S_OK;
}

// DLL exports
STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv) {
    if (!ppv) {
        return E_POINTER;
    }

    *ppv = nullptr;

    if (rclsid != CLSID_NVDASAPIBridge) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    SAPIVoiceFactory* pFactory = new (std::nothrow) SAPIVoiceFactory();
    if (!pFactory) {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();

    return hr;
}

STDAPI DllCanUnloadNow() {
    return (g_serverLocks == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer() {
    HRESULT hr = S_OK;
    
    // Get DLL path
    wchar_t dllPath[MAX_PATH];
    if (!GetModuleFileNameW(g_hModule, dllPath, MAX_PATH)) {
        return SELFREG_E_TYPELIB;
    }

    // Convert CLSID to string
    LPOLESTR clsidStr;
    StringFromCLSID(CLSID_NVDASAPIBridge, &clsidStr);

    // 1. Register the COM server class
    wchar_t keyPath[512];
    StringCchPrintfW(keyPath, 512, L"CLSID\\%s\\InProcServer32", clsidStr);

    HKEY hKey;
    LONG result = RegCreateKeyExW(HKEY_CLASSES_ROOT, keyPath, 0, nullptr,
                                   REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr);
    
    if (result == ERROR_SUCCESS) {
        RegSetValueExW(hKey, nullptr, 0, REG_SZ, 
                      reinterpret_cast<const BYTE*>(dllPath),
                      static_cast<DWORD>((wcslen(dllPath) + 1) * sizeof(wchar_t)));
        
        const wchar_t* threadingModel = L"Apartment";
        RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ,
                      reinterpret_cast<const BYTE*>(threadingModel),
                      static_cast<DWORD>((wcslen(threadingModel) + 1) * sizeof(wchar_t)));
        
        RegCloseKey(hKey);
    } else {
        CoTaskMemFree(clsidStr);
        return SELFREG_E_CLASS;
    }

    // 2. Register the SAPI voice token
    // Voice tokens are registered under HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Speech\Voices\Tokens
    StringCchPrintfW(keyPath, 512, L"SOFTWARE\\Microsoft\\Speech\\Voices\\Tokens\\%s", VOICE_TOKEN_NAME);
    
    result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, nullptr,
                            REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr);
    
    if (result == ERROR_SUCCESS) {
        // Set the default value (voice description)
        RegSetValueExW(hKey, nullptr, 0, REG_SZ,
                      reinterpret_cast<const BYTE*>(VOICE_DESCRIPTION),
                      static_cast<DWORD>((wcslen(VOICE_DESCRIPTION) + 1) * sizeof(wchar_t)));
        
        // Set the CLSID value to point to our TTS engine
        RegSetValueExW(hKey, L"CLSID", 0, REG_SZ,
                      reinterpret_cast<const BYTE*>(clsidStr),
                      static_cast<DWORD>((wcslen(clsidStr) + 1) * sizeof(wchar_t)));
        
        // Set language (409 = US English, 0 = gender neutral)
        const wchar_t* langId = L"409";
        RegSetValueExW(hKey, L"LangDataPath", 0, REG_SZ,
                      reinterpret_cast<const BYTE*>(langId),
                      static_cast<DWORD>((wcslen(langId) + 1) * sizeof(wchar_t)));
        
        RegCloseKey(hKey);
        
        // Create Attributes subkey
        StringCchPrintfW(keyPath, 512, L"SOFTWARE\\Microsoft\\Speech\\Voices\\Tokens\\%s\\Attributes", VOICE_TOKEN_NAME);
        result = RegCreateKeyExW(HKEY_LOCAL_MACHINE, keyPath, 0, nullptr,
                                REG_OPTION_NON_VOLATILE, KEY_WRITE, nullptr, &hKey, nullptr);
        
        if (result == ERROR_SUCCESS) {
            // Set voice attributes
            const wchar_t* language = L"409";  // US English
            RegSetValueExW(hKey, L"Language", 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(language),
                          static_cast<DWORD>((wcslen(language) + 1) * sizeof(wchar_t)));
            
            const wchar_t* gender = L"Neutral";
            RegSetValueExW(hKey, L"Gender", 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(gender),
                          static_cast<DWORD>((wcslen(gender) + 1) * sizeof(wchar_t)));
            
            const wchar_t* age = L"Adult";
            RegSetValueExW(hKey, L"Age", 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(age),
                          static_cast<DWORD>((wcslen(age) + 1) * sizeof(wchar_t)));
            
            const wchar_t* vendor = L"NVDA";
            RegSetValueExW(hKey, L"Vendor", 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(vendor),
                          static_cast<DWORD>((wcslen(vendor) + 1) * sizeof(wchar_t)));
            
            const wchar_t* name = L"NVDA";
            RegSetValueExW(hKey, L"Name", 0, REG_SZ,
                          reinterpret_cast<const BYTE*>(name),
                          static_cast<DWORD>((wcslen(name) + 1) * sizeof(wchar_t)));
            
            RegCloseKey(hKey);
        }
    } else {
        CoTaskMemFree(clsidStr);
        return SELFREG_E_CLASS;
    }

    CoTaskMemFree(clsidStr);

    return hr;
}

STDAPI DllUnregisterServer() {
    HRESULT hr = S_OK;
    
    // Convert CLSID to string
    LPOLESTR clsidStr;
    StringFromCLSID(CLSID_NVDASAPIBridge, &clsidStr);

    // 1. Unregister the SAPI voice token
    wchar_t keyPath[512];
    StringCchPrintfW(keyPath, 512, L"SOFTWARE\\Microsoft\\Speech\\Voices\\Tokens\\%s", VOICE_TOKEN_NAME);
    LONG result = RegDeleteTreeW(HKEY_LOCAL_MACHINE, keyPath);

    // 2. Unregister the COM server class
    StringCchPrintfW(keyPath, 512, L"CLSID\\%s", clsidStr);
    result = RegDeleteTreeW(HKEY_CLASSES_ROOT, keyPath);

    CoTaskMemFree(clsidStr);

    return (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND) ? S_OK : SELFREG_E_CLASS;
}
