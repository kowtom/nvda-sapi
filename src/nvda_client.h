#pragma once

#include <windows.h>

/**
 * NVDA Client - Handles communication with NVDA screen reader
 * Uses NVDA's controller client API to send speech text
 */
class NVDAClient {
public:
    NVDAClient();
    ~NVDAClient();

    /**
     * Initialize connection to NVDA
     * @return true if successful, false otherwise
     */
    bool Initialize();

    /**
     * Send text to NVDA for speech (C-string version for COM safety)
     * @param text Null-terminated wide string to speak
     * @return true if successful, false otherwise
     */
    bool SpeakText(const wchar_t* text);

    /**
     * Cancel any ongoing speech
     * @return true if successful, false otherwise
     */
    bool CancelSpeech();

    /**
     * Check if NVDA is running
     * @return true if NVDA is running, false otherwise
     */
    bool IsNVDARunning();

private:
    HMODULE m_nvdaControllerClient;
    
    // Function pointers to NVDA controller client API
    typedef long(__stdcall *nvdaController_testIfRunning_type)();
    typedef long(__stdcall *nvdaController_speakText_type)(const wchar_t*);
    typedef long(__stdcall *nvdaController_cancelSpeech_type)();

    nvdaController_testIfRunning_type m_testIfRunning;
    nvdaController_speakText_type m_speakText;
    nvdaController_cancelSpeech_type m_cancelSpeech;

    bool LoadNVDAController();
};
