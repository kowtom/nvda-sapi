#pragma once

#include <windows.h>
#include <unknwn.h>
#include <olectl.h>

// DLL main functions
extern "C" {
    BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);
    HRESULT WINAPI DllRegisterServer();
    HRESULT WINAPI DllUnregisterServer();
    HRESULT WINAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv);
    HRESULT WINAPI DllCanUnloadNow();
}

// Class factory for creating SAPI voice instances
class SAPIVoiceFactory : public IClassFactory {
public:
    SAPIVoiceFactory();
    virtual ~SAPIVoiceFactory();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // IClassFactory methods
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
    STDMETHOD(LockServer)(BOOL fLock) override;

private:
    LONG m_refCount;
};

// Global module handle
extern HMODULE g_hModule;
extern LONG g_serverLocks;
