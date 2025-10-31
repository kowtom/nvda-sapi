/*
 * DLL entry point and COM registration for NVDA-SAPI Bridge
 */

#include <windows.h>
#include "NvdaSapiEngine.h"

HMODULE g_hModule = nullptr;
LONG g_lLocks = 0;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            g_hModule = hModule;
            DisableThreadLibraryCalls(hModule);
            break;
            
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    if (rclsid == CLSID_NvdaSapiEngine)
    {
        CNvdaSapiEngineFactory* pFactory = new CNvdaSapiEngineFactory();
        if (!pFactory)
        {
            return E_OUTOFMEMORY;
        }

        HRESULT hr = pFactory->QueryInterface(riid, ppv);
        pFactory->Release();
        
        return hr;
    }
    
    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllCanUnloadNow()
{
    return (g_lLocks == 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer()
{
    // Registration is now handled by .reg file or registration script
    return S_OK;
}

STDAPI DllUnregisterServer()
{
    // Unregistration is now handled by .reg file or registration script
    return S_OK;
}
