#pragma once

#include <windows.h>
#include <unknwn.h>
#include <mmsystem.h>

// Minimal SAPI5 TTS Engine interface definitions
// Based on Microsoft SAPI SDK but simplified for our needs

// Forward declarations
struct SPVTEXTFRAG;
struct ISpTTSEngineSite;

// SAPI GUIDs - externally defined in sapi_guids.cpp
extern const GUID IID_ISpTTSEngine;
extern const GUID SPDFID_Text;

// SAPI Voice State
typedef enum SPVSTATE {
    SPVST_SENTENCE = 0,
    SPVST_NOUN_PHRASE = 1,
    SPVST_DECLARATIVE = 2,
    SPVST_IMPERATIVE = 3,
    SPVST_VERB = 4
} SPVSTATE;

// SAPI Text Fragment Actions
typedef enum SPVACTIONS {
    SPVA_Speak = 0,
    SPVA_Silence = (SPVA_Speak + 1),
    SPVA_Pronounce = (SPVA_Silence + 1),
    SPVA_Bookmark = (SPVA_Pronounce + 1),
    SPVA_SpellOut = (SPVA_Bookmark + 1),
    SPVA_Section = (SPVA_SpellOut + 1),
    SPVA_ParseUnknownTag = (SPVA_Section + 1)
} SPVACTIONS;

// SAPI Text Fragment structure
typedef struct SPVTEXTFRAG {
    struct SPVTEXTFRAG* pNext;
    SPVSTATE State;
    const WCHAR* pTextStart;
    ULONG ulTextLen;
    ULONG ulTextSrcOffset;
} SPVTEXTFRAG;

// ISpTTSEngineSite interface (minimal)
#undef INTERFACE
#define INTERFACE ISpTTSEngineSite

DECLARE_INTERFACE_(ISpTTSEngineSite, IUnknown)
{
    // IUnknown methods
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ISpTTSEngineSite methods - minimal set for our needs
    STDMETHOD(AddEvents)(THIS_ const void* pEventArray, ULONG ulCount) PURE;
    STDMETHOD(GetEventInterest)(THIS_ ULONGLONG* pullEventInterest) PURE;
    STDMETHOD(GetActions)(THIS) PURE;
    STDMETHOD(Write)(THIS_ const void* pBuff, ULONG cb, ULONG* pcbWritten) PURE;
    STDMETHOD(GetRate)(THIS_ long* pRateAdjust) PURE;
    STDMETHOD(GetVolume)(THIS_ USHORT* pusVolume) PURE;
    STDMETHOD(GetSkipInfo)(THIS_ ULONG* pulType, long* plNumItems) PURE;
    STDMETHOD(CompleteSkip)(THIS_ long ulNumSkipped) PURE;
};

#undef INTERFACE

// ISpTTSEngine interface (minimal)
#undef INTERFACE
#define INTERFACE ISpTTSEngine

DECLARE_INTERFACE_(ISpTTSEngine, IUnknown)
{
    // IUnknown methods
    STDMETHOD(QueryInterface)(THIS_ REFIID riid, void** ppvObject) PURE;
    STDMETHOD_(ULONG, AddRef)(THIS) PURE;
    STDMETHOD_(ULONG, Release)(THIS) PURE;

    // ISpTTSEngine methods
    STDMETHOD(Speak)(THIS_ DWORD dwSpeakFlags, REFGUID rguidFormatId,
                     const WAVEFORMATEX* pWaveFormatEx, const SPVTEXTFRAG* pTextFragList,
                     ISpTTSEngineSite* pOutputSite) PURE;
    
    STDMETHOD(GetOutputFormat)(THIS_ const GUID* pTargetFormatId,
                               const WAVEFORMATEX* pTargetWaveFormatEx,
                               GUID* pDesiredFormatId,
                               WAVEFORMATEX** ppCoMemDesiredWaveFormatEx) PURE;
};

#undef INTERFACE
