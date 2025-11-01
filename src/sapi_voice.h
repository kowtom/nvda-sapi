#pragma once

#include <windows.h>
#include "sapi_minimal.h"
#include <string>
#include <memory>

// Forward declarations
class NVDAClient;

/**
 * SAPI5 Voice implementation that forwards speech to NVDA
 * Implements ISpTTSEngine interface for SAPI5 compatibility
 */
class SAPIVoice : public ISpTTSEngine {
public:
    SAPIVoice();
    virtual ~SAPIVoice();

    // IUnknown methods
    STDMETHOD(QueryInterface)(REFIID riid, void** ppvObject) override;
    STDMETHOD_(ULONG, AddRef)() override;
    STDMETHOD_(ULONG, Release)() override;

    // ISpTTSEngine methods
    STDMETHOD(Speak)(DWORD dwSpeakFlags, REFGUID rguidFormatId, 
                     const WAVEFORMATEX* pWaveFormatEx, const SPVTEXTFRAG* pTextFragList,
                     ISpTTSEngineSite* pOutputSite) override;
    
    STDMETHOD(GetOutputFormat)(const GUID* pTargetFormatId, const WAVEFORMATEX* pTargetWaveFormatEx,
                               GUID* pDesiredFormatId, WAVEFORMATEX** ppCoMemDesiredWaveFormatEx) override;

private:
    LONG m_refCount;
    std::unique_ptr<NVDAClient> m_nvdaClient;
};

// Factory function
extern "C" HRESULT CreateSAPIVoice(IUnknown** ppUnknown);
