#include "sapi_voice.h"
#include "nvda_client.h"
#include <sstream>

SAPIVoice::SAPIVoice()
    : m_refCount(1)
    , m_nvdaClient(std::make_unique<NVDAClient>()) {
    m_nvdaClient->Initialize();
}

SAPIVoice::~SAPIVoice() {
}

// IUnknown implementation
STDMETHODIMP SAPIVoice::QueryInterface(REFIID riid, void** ppvObject) {
    if (!ppvObject) {
        return E_POINTER;
    }

    *ppvObject = nullptr;

    if (riid == IID_IUnknown || riid == IID_ISpTTSEngine) {
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
    if (!pTextFragList) {
        return E_INVALIDARG;
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
        if (!m_nvdaClient->Speak(text)) {
            // If NVDA is not available, still return success
            // to avoid breaking applications
            return S_OK;
        }
    }

    // Notify SAPI that we're done
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

    // We don't produce audio, so we return a minimal format
    *pDesiredFormatId = SPDFID_Text;
    *ppCoMemDesiredWaveFormatEx = nullptr;

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
