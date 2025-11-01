/**
 * SAPI5 to NVDA Bridge - TTS Engine Implementation
 * 
 * This implements a SAPI5 Text-to-Speech engine that redirects speech
 * output to the NVDA screen reader using the NVDA Controller Client API.
 * 
 * License: GPL v2 or later
 */

#include <windows.h>
#include <sapi.h>
#include <sphelper.h>
#include <stdio.h>

// NVDA Controller Client API
extern "C" {
    __declspec(dllimport) int __stdcall nvdaController_testIfRunning();
    __declspec(dllimport) int __stdcall nvdaController_speakText(const wchar_t* text);
    __declspec(dllimport) int __stdcall nvdaController_cancelSpeech();
}

// {2F9DE777-6B8D-4ECE-AA3A-1D7F5B2A7E2E}
static const GUID CLSID_NVDASAPIBridge = 
{ 0x2f9de777, 0x6b8d, 0x4ece, { 0xaa, 0x3a, 0x1d, 0x7f, 0x5b, 0x2a, 0x7e, 0x2e } };

class CNVDATTSEngine : public ISpTTSEngine, public ISpObjectWithToken
{
private:
    LONG m_refCount;
    ISpObjectToken* m_token;
    CRITICAL_SECTION m_critSec;

public:
    CNVDATTSEngine() : m_refCount(1), m_token(NULL)
    {
        InitializeCriticalSection(&m_critSec);
    }

    ~CNVDATTSEngine()
    {
        if (m_token) {
            m_token->Release();
        }
        DeleteCriticalSection(&m_critSec);
    }

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
    {
        if (riid == IID_IUnknown || riid == IID_ISpTTSEngine) {
            *ppvObject = static_cast<ISpTTSEngine*>(this);
        } else if (riid == IID_ISpObjectWithToken) {
            *ppvObject = static_cast<ISpObjectWithToken*>(this);
        } else {
            *ppvObject = NULL;
            return E_NOINTERFACE;
        }
        AddRef();
        return S_OK;
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&m_refCount);
    }

    STDMETHOD_(ULONG, Release)()
    {
        LONG refs = InterlockedDecrement(&m_refCount);
        if (refs == 0) {
            delete this;
        }
        return refs;
    }

    // ISpObjectWithToken methods
    STDMETHOD(SetObjectToken)(ISpObjectToken* pToken)
    {
        EnterCriticalSection(&m_critSec);
        if (m_token) {
            m_token->Release();
        }
        m_token = pToken;
        if (m_token) {
            m_token->AddRef();
        }
        LeaveCriticalSection(&m_critSec);
        return S_OK;
    }

    STDMETHOD(GetObjectToken)(ISpObjectToken** ppToken)
    {
        if (!ppToken) return E_POINTER;
        
        EnterCriticalSection(&m_critSec);
        *ppToken = m_token;
        if (m_token) {
            m_token->AddRef();
        }
        LeaveCriticalSection(&m_critSec);
        return m_token ? S_OK : S_FALSE;
    }

    // ISpTTSEngine methods
    STDMETHOD(Speak)(DWORD dwSpeakFlags, REFGUID rguidFormatId, const WAVEFORMATEX* pWaveFormatEx,
                     const SPVTEXTFRAG* pTextFragList, ISpTTSEngineSite* pOutputSite)
    {
        if (!pTextFragList) return E_INVALIDARG;

        // Check if NVDA is running
        if (nvdaController_testIfRunning() != 0) {
            // NVDA is not running, silently fail
            return S_OK;
        }

        // Build the text string from fragments
        WCHAR textBuffer[4096] = {0};
        size_t bufferPos = 0;
        
        for (const SPVTEXTFRAG* pFrag = pTextFragList; pFrag; pFrag = pFrag->pNext) {
            if (pFrag->pTextStart && pFrag->ulTextLen > 0) {
                size_t remaining = 4095 - bufferPos;
                size_t toCopy = min(pFrag->ulTextLen, remaining);
                
                if (toCopy > 0) {
                    wcsncpy_s(textBuffer + bufferPos, remaining + 1, 
                             pFrag->pTextStart, toCopy);
                    bufferPos += toCopy;
                }
                
                if (bufferPos >= 4095) break;
            }
        }

        // Send to NVDA
        if (bufferPos > 0) {
            nvdaController_speakText(textBuffer);
        }

        // Notify completion
        if (pOutputSite) {
            pOutputSite->CompleteSkip(0);
        }

        return S_OK;
    }

    STDMETHOD(GetOutputFormat)(const GUID* pTargetFormatId, const WAVEFORMATEX* pTargetWaveFormatEx,
                               GUID* pDesiredFormatId, WAVEFORMATEX** ppCoMemDesiredWaveFormatEx)
    {
        // We don't actually output audio, but we need to return a format
        *pDesiredFormatId = SPDFID_WaveFormatEx;
        
        WAVEFORMATEX* pFormat = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
        if (!pFormat) return E_OUTOFMEMORY;
        
        pFormat->wFormatTag = WAVE_FORMAT_PCM;
        pFormat->nChannels = 1;
        pFormat->nSamplesPerSec = 22050;
        pFormat->wBitsPerSample = 16;
        pFormat->nBlockAlign = pFormat->nChannels * pFormat->wBitsPerSample / 8;
        pFormat->nAvgBytesPerSec = pFormat->nSamplesPerSec * pFormat->nBlockAlign;
        pFormat->cbSize = 0;
        
        *ppCoMemDesiredWaveFormatEx = pFormat;
        return S_OK;
    }
};

// Class factory
class CClassFactory : public IClassFactory
{
private:
    LONG m_refCount;

public:
    CClassFactory() : m_refCount(1) {}

    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject)
    {
        if (riid == IID_IUnknown || riid == IID_IClassFactory) {
            *ppvObject = this;
            AddRef();
            return S_OK;
        }
        *ppvObject = NULL;
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&m_refCount);
    }

    STDMETHOD_(ULONG, Release)()
    {
        LONG refs = InterlockedDecrement(&m_refCount);
        if (refs == 0) {
            delete this;
        }
        return refs;
    }

    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppvObject)
    {
        if (pUnkOuter) return CLASS_E_NOAGGREGATION;
        
        CNVDATTSEngine* pEngine = new CNVDATTSEngine();
        if (!pEngine) return E_OUTOFMEMORY;
        
        HRESULT hr = pEngine->QueryInterface(riid, ppvObject);
        pEngine->Release();
        return hr;
    }

    STDMETHOD(LockServer)(BOOL fLock)
    {
        return S_OK;
    }
};

// DLL exports
static HINSTANCE g_hInstance = NULL;

BOOL APIENTRY DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH) {
        g_hInstance = hInstance;
        DisableThreadLibraryCalls(hInstance);
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    if (rclsid != CLSID_NVDASAPIBridge) {
        return CLASS_E_CLASSNOTAVAILABLE;
    }
    
    CClassFactory* pFactory = new CClassFactory();
    if (!pFactory) return E_OUTOFMEMORY;
    
    HRESULT hr = pFactory->QueryInterface(riid, ppv);
    pFactory->Release();
    return hr;
}

STDAPI DllCanUnloadNow()
{
    return S_OK;
}

STDAPI DllRegisterServer()
{
    return S_OK; // Registration handled by external script
}

STDAPI DllUnregisterServer()
{
    return S_OK; // Unregistration handled by external script
}
