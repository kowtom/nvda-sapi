/*
 * NVDA-SAPI Bridge TTS Engine Implementation
 */

#include "NvdaSapiEngine.h"
#include "../nvdaControllerClient/nvdaControllerClient.h"
#include <sstream>

// Constructor
CNvdaSapiEngine::CNvdaSapiEngine() : m_refCount(1), m_pToken(nullptr)
{
}

// Destructor
CNvdaSapiEngine::~CNvdaSapiEngine()
{
    if (m_pToken)
    {
        m_pToken->Release();
    }
}

// IUnknown implementation
STDMETHODIMP CNvdaSapiEngine::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_ISpTTSEngine)
    {
        *ppv = static_cast<ISpTTSEngine*>(this);
    }
    else if (riid == IID_ISpObjectWithToken)
    {
        *ppv = static_cast<ISpObjectWithToken*>(this);
    }
    else
    {
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) CNvdaSapiEngine::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) CNvdaSapiEngine::Release()
{
    ULONG count = InterlockedDecrement(&m_refCount);
    if (count == 0)
    {
        delete this;
    }
    return count;
}

// ISpTTSEngine implementation
STDMETHODIMP CNvdaSapiEngine::Speak(
    DWORD dwSpeakFlags,
    REFGUID rguidFormatId,
    const WAVEFORMATEX* pWaveFormatEx,
    const SPVTEXTFRAG* pTextFragList,
    ISpTTSEngineSite* pOutputSite)
{
    if (!pTextFragList)
    {
        return E_INVALIDARG;
    }

    // Extract text from fragments
    std::wstring text = ExtractTextFromFragments(pTextFragList);
    
    if (text.empty())
    {
        return S_OK;
    }

    // Send to NVDA
    HRESULT hr = SpeakToNvda(text);
    
    // Notify SAPI that we're done
    if (pOutputSite)
    {
        pOutputSite->CompleteSkip(0);
    }
    
    return hr;
}

STDMETHODIMP CNvdaSapiEngine::GetOutputFormat(
    const GUID* pTargetFormatId,
    const WAVEFORMATEX* pTargetWaveFormatEx,
    GUID* pDesiredFormatId,
    WAVEFORMATEX** ppCoMemDesiredWaveFormatEx)
{
    // We don't produce audio, so we return a minimal format
    if (!pDesiredFormatId || !ppCoMemDesiredWaveFormatEx)
    {
        return E_POINTER;
    }

    *pDesiredFormatId = SPDFID_WaveFormatEx;
    
    // Allocate a minimal WAVEFORMATEX
    WAVEFORMATEX* pFormat = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
    if (!pFormat)
    {
        return E_OUTOFMEMORY;
    }

    // Set minimal 16-bit PCM format (we won't actually use this)
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

// ISpObjectWithToken implementation
STDMETHODIMP CNvdaSapiEngine::SetObjectToken(ISpObjectToken* pToken)
{
    if (m_pToken)
    {
        m_pToken->Release();
    }
    
    m_pToken = pToken;
    
    if (m_pToken)
    {
        m_pToken->AddRef();
    }
    
    return S_OK;
}

STDMETHODIMP CNvdaSapiEngine::GetObjectToken(ISpObjectToken** ppToken)
{
    if (!ppToken)
    {
        return E_POINTER;
    }

    *ppToken = m_pToken;
    
    if (m_pToken)
    {
        m_pToken->AddRef();
    }
    
    return S_OK;
}

// Helper methods
HRESULT CNvdaSapiEngine::SpeakToNvda(const std::wstring& text)
{
    // Test if NVDA is running
    if (nvdaController_testIfRunning() != 0)
    {
        // NVDA is not running
        return E_FAIL;
    }

    // Send text to NVDA
    long result = nvdaController_speakText(text.c_str());
    
    return (result == 0) ? S_OK : E_FAIL;
}

std::wstring CNvdaSapiEngine::ExtractTextFromFragments(const SPVTEXTFRAG* pTextFragList)
{
    std::wstringstream ss;
    
    for (const SPVTEXTFRAG* pFrag = pTextFragList; pFrag; pFrag = pFrag->pNext)
    {
        if (pFrag->pTextStart && pFrag->ulTextLen > 0)
        {
            ss << std::wstring(pFrag->pTextStart, pFrag->ulTextLen);
        }
    }
    
    return ss.str();
}

// Class Factory Implementation
CNvdaSapiEngineFactory::CNvdaSapiEngineFactory() : m_refCount(1)
{
}

CNvdaSapiEngineFactory::~CNvdaSapiEngineFactory()
{
}

STDMETHODIMP CNvdaSapiEngineFactory::QueryInterface(REFIID riid, void** ppv)
{
    if (riid == IID_IUnknown || riid == IID_IClassFactory)
    {
        *ppv = static_cast<IClassFactory*>(this);
    }
    else
    {
        *ppv = nullptr;
        return E_NOINTERFACE;
    }
    
    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) CNvdaSapiEngineFactory::AddRef()
{
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) CNvdaSapiEngineFactory::Release()
{
    ULONG count = InterlockedDecrement(&m_refCount);
    if (count == 0)
    {
        delete this;
    }
    return count;
}

STDMETHODIMP CNvdaSapiEngineFactory::CreateInstance(
    IUnknown* pUnkOuter,
    REFIID riid,
    void** ppv)
{
    if (pUnkOuter != nullptr)
    {
        return CLASS_E_NOAGGREGATION;
    }

    CNvdaSapiEngine* pEngine = new CNvdaSapiEngine();
    if (!pEngine)
    {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pEngine->QueryInterface(riid, ppv);
    pEngine->Release();
    
    return hr;
}

STDMETHODIMP CNvdaSapiEngineFactory::LockServer(BOOL fLock)
{
    // Not implemented for this simple case
    return S_OK;
}
