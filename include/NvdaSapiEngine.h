/*
 * NVDA-SAPI Bridge TTS Engine
 * Bridges SAPI5 speech requests to NVDA screen reader
 */

#ifndef NVDA_SAPI_ENGINE_H
#define NVDA_SAPI_ENGINE_H

#include <windows.h>
#include <sapi.h>
#include <sphelper.h>
#include <string>

// {E6F6B6F0-8F3A-4B3E-9F3E-3F3E3F3E3F3E}
static const GUID CLSID_NvdaSapiEngine = 
{ 0xe6f6b6f0, 0x8f3a, 0x4b3e, { 0x9f, 0x3e, 0x3f, 0x3e, 0x3f, 0x3e, 0x3f, 0x3e } };

class CNvdaSapiEngine : public ISpTTSEngine, public ISpObjectWithToken
{
public:
    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // ISpTTSEngine
    STDMETHOD(Speak)(DWORD dwSpeakFlags,
                     REFGUID rguidFormatId,
                     const WAVEFORMATEX* pWaveFormatEx,
                     const SPVTEXTFRAG* pTextFragList,
                     ISpTTSEngineSite* pOutputSite);
    
    STDMETHOD(GetOutputFormat)(const GUID* pTargetFormatId,
                               const WAVEFORMATEX* pTargetWaveFormatEx,
                               GUID* pDesiredFormatId,
                               WAVEFORMATEX** ppCoMemDesiredWaveFormatEx);

    // ISpObjectWithToken
    STDMETHOD(SetObjectToken)(ISpObjectToken* pToken);
    STDMETHOD(GetObjectToken)(ISpObjectToken** ppToken);

    // Constructor and Destructor
    CNvdaSapiEngine();
    ~CNvdaSapiEngine();

private:
    long m_refCount;
    ISpObjectToken* m_pToken;
    
    // Helper methods
    HRESULT SpeakToNvda(const std::wstring& text);
    std::wstring ExtractTextFromFragments(const SPVTEXTFRAG* pTextFragList);
};

// Class factory
class CNvdaSapiEngineFactory : public IClassFactory
{
public:
    // IUnknown
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv);
    STDMETHOD_(ULONG, AddRef)();
    STDMETHOD_(ULONG, Release)();

    // IClassFactory
    STDMETHOD(CreateInstance)(IUnknown* pUnkOuter, REFIID riid, void** ppv);
    STDMETHOD(LockServer)(BOOL fLock);

    CNvdaSapiEngineFactory();
    ~CNvdaSapiEngineFactory();

private:
    long m_refCount;
};

#endif // NVDA_SAPI_ENGINE_H
