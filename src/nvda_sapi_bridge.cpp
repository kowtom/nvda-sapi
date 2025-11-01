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

// {E6F8A7F0-8B1E-4F9A-9C3D-1234567890AB} - NVDA SAPI Bridge CLSID
// This GUID uniquely identifies our SAPI voice engine
static const CLSID CLSID_NVDASAPIBridge = 
    {0xE6F8A7F0, 0x8B1E, 0x4F9A, {0x9C, 0x3D, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB}};

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
    // Get DLL path
    wchar_t dllPath[MAX_PATH];
    if (!GetModuleFileNameW(g_hModule, dllPath, MAX_PATH)) {
        return SELFREG_E_TYPELIB;
    }

    // Convert CLSID to string
    LPOLESTR clsidStr;
    StringFromCLSID(CLSID_NVDASAPIBridge, &clsidStr);

    // Create registry key path
    wchar_t keyPath[512];
    StringCchPrintfW(keyPath, 512, L"CLSID\\%s\\InProcServer32", clsidStr);

    // Register the COM server
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
    }

    CoTaskMemFree(clsidStr);

    return (result == ERROR_SUCCESS) ? S_OK : SELFREG_E_CLASS;
}

STDAPI DllUnregisterServer() {
    // Convert CLSID to string
    LPOLESTR clsidStr;
    StringFromCLSID(CLSID_NVDASAPIBridge, &clsidStr);

    // Create registry key path
    wchar_t keyPath[512];
    StringCchPrintfW(keyPath, 512, L"CLSID\\%s", clsidStr);

    // Remove the registry key
    LONG result = RegDeleteTreeW(HKEY_CLASSES_ROOT, keyPath);

    CoTaskMemFree(clsidStr);

    return (result == ERROR_SUCCESS || result == ERROR_FILE_NOT_FOUND) ? S_OK : SELFREG_E_CLASS;
}
