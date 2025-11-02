#include "sapi_voice.h"
#include "nvda_client.h"
#include <sstream>

SAPIVoice::SAPIVoice()
    : m_refCount(1)
    , m_nvdaClient(std::make_unique<NVDAClient>())
    , m_pToken(nullptr) {
    m_nvdaClient->Initialize();
}

SAPIVoice::~SAPIVoice() {
    // Don't release m_pToken - SAPI manages its lifetime
    m_pToken = nullptr;
}

// IUnknown implementation
STDMETHODIMP SAPIVoice::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown) {
        *ppvObject = static_cast<ISpTTSEngine*>(this);
        AddRef();
        return S_OK;
    }
    else if (riid == IID_ISpTTSEngine) {
        *ppvObject = static_cast<ISpTTSEngine*>(this);
        AddRef();
        return S_OK;
    }
    else if (riid == IID_ISpObjectWithToken) {
        // Return same pointer as ISpTTSEngine to avoid vtable issues
        // We manually implement ISpObjectWithToken methods
        *ppvObject = static_cast<ISpTTSEngine*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) SAPIVoice::AddRef() {
    return InterlockedIncrement(&m_refCount);
}

STDMETHODIMP_(ULONG) SAPIVoice::Release() {
    LONG refCount = InterlockedDecrement(&m_refCount);
    if (refCount == 0) {
        delete this;
    }
    return refCount;
}

// ISpTTSEngine implementation
STDMETHODIMP SAPIVoice::Speak(DWORD dwSpeakFlags, REFGUID rguidFormatId,
                               const WAVEFORMATEX* pWaveFormatEx,
                               const SPVTEXTFRAG* pTextFragList,
                               ISpTTSEngineSite* pOutputSite) {
    // pOutputSite can be null during initialization/testing
    // pTextFragList can be null for silence
    
    if (!pTextFragList) {
        // No text to speak - just return success
        return S_OK;
    }

    // Build complete text from fragments
    std::wstringstream textStream;
    const SPVTEXTFRAG* pCurrentFrag = pTextFragList;

    while (pCurrentFrag) {
        if (pCurrentFrag->pTextStart && pCurrentFrag->ulTextLen > 0) {
            textStream.write(pCurrentFrag->pTextStart, pCurrentFrag->ulTextLen);
        }
        pCurrentFrag = pCurrentFrag->pNext;
    }

    std::wstring text = textStream.str();
    
    if (!text.empty()) {
        // Send text to NVDA
        // If NVDA is not available, still return success to avoid breaking applications
        m_nvdaClient->Speak(text);
    }

    // Notify SAPI that we're done (only if pOutputSite is provided)
    // CompleteSkip(0) indicates we completed without skipping anything
    if (pOutputSite) {
        pOutputSite->CompleteSkip(0);
    }

    return S_OK;
}

STDMETHODIMP SAPIVoice::GetOutputFormat(const GUID* pTargetFormatId,
                                         const WAVEFORMATEX* pTargetWaveFormatEx,
                                         GUID* pDesiredFormatId,
                                         WAVEFORMATEX** ppCoMemDesiredWaveFormatEx) {
    if (!pDesiredFormatId || !ppCoMemDesiredWaveFormatEx) {
        return E_POINTER;
    }

    // Return a standard PCM wave format
    // Even though we don't produce audio, SAPI expects a valid format
    *pDesiredFormatId = SPDFID_WaveFormatEx;
    
    // Allocate WAVEFORMATEX structure using CoTaskMemAlloc (SAPI will free it)
    WAVEFORMATEX* pFormat = (WAVEFORMATEX*)CoTaskMemAlloc(sizeof(WAVEFORMATEX));
    if (!pFormat) {
        return E_OUTOFMEMORY;
    }

    // Fill in standard 16-bit PCM format (22kHz, mono)
    pFormat->wFormatTag = WAVE_FORMAT_PCM;
    pFormat->nChannels = 1;              // Mono
    pFormat->nSamplesPerSec = 22050;     // 22 kHz
    pFormat->nAvgBytesPerSec = 44100;    // 22050 * 2 bytes per sample
    pFormat->nBlockAlign = 2;            // 2 bytes per sample (16-bit)
    pFormat->wBitsPerSample = 16;        // 16-bit
    pFormat->cbSize = 0;                 // No extra format information

    *ppCoMemDesiredWaveFormatEx = pFormat;

    return S_OK;
}

// ISpObjectWithToken implementation
STDMETHODIMP SAPIVoice::SetObjectToken(IUnknown* pToken) {
    // Store the token pointer (SAPI manages its lifetime)
    m_pToken = pToken;
    return S_OK;
}

STDMETHODIMP SAPIVoice::GetObjectToken(IUnknown** ppToken) {
    if (!ppToken) {
        return E_POINTER;
    }

    *ppToken = m_pToken;
    if (m_pToken) {
        m_pToken->AddRef();
    }
    
    return S_OK;
}

// Factory function
extern "C" HRESULT CreateSAPIVoice(IUnknown** ppUnknown) {
    if (!ppUnknown) {
        return E_POINTER;
    }

    SAPIVoice* pVoice = new (std::nothrow) SAPIVoice();
    if (!pVoice) {
        return E_OUTOFMEMORY;
    }

    *ppUnknown = static_cast<ISpTTSEngine*>(pVoice);
    return S_OK;
}
