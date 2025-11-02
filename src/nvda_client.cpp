#include "nvda_client.h"

NVDAClient::NVDAClient()
    : m_nvdaControllerClient(nullptr)
    , m_testIfRunning(nullptr)
    , m_speakText(nullptr)
    , m_cancelSpeech(nullptr) {
}

NVDAClient::~NVDAClient() {
    if (m_nvdaControllerClient) {
        FreeLibrary(m_nvdaControllerClient);
        m_nvdaControllerClient = nullptr;
    }
}

bool NVDAClient::Initialize() {
    return LoadNVDAController();
}

bool NVDAClient::LoadNVDAController() {
    // Try to load nvdaControllerClient DLL from system paths
    const wchar_t* dllNames[] = {
        L"nvdaControllerClient64.dll",
        L"nvdaControllerClient32.dll"
    };

    for (const auto* dllName : dllNames) {
        m_nvdaControllerClient = LoadLibraryW(dllName);
        if (m_nvdaControllerClient) {
            break;
        }
    }

    if (!m_nvdaControllerClient) {
        return false;
    }

    // Load function pointers
    m_testIfRunning = reinterpret_cast<nvdaController_testIfRunning_type>(
        GetProcAddress(m_nvdaControllerClient, "nvdaController_testIfRunning"));
    
    m_speakText = reinterpret_cast<nvdaController_speakText_type>(
        GetProcAddress(m_nvdaControllerClient, "nvdaController_speakText"));
    
    m_cancelSpeech = reinterpret_cast<nvdaController_cancelSpeech_type>(
        GetProcAddress(m_nvdaControllerClient, "nvdaController_cancelSpeech"));

    if (!m_testIfRunning || !m_speakText || !m_cancelSpeech) {
        FreeLibrary(m_nvdaControllerClient);
        m_nvdaControllerClient = nullptr;
        return false;
    }

    return true;
}

bool NVDAClient::IsNVDARunning() {
    if (!m_testIfRunning) {
        return false;
    }
    return m_testIfRunning() == 0;
}

bool NVDAClient::SpeakText(const wchar_t* text) {
    // Validate all parameters and state before proceeding
    if (!text) {
        return false;
    }
    
    if (text[0] == L'\0') {
        return false;
    }
    
    if (!m_speakText) {
        return false;
    }
    
    if (!m_testIfRunning) {
        return false;
    }
    
    // Check if NVDA is running - wrap in check to prevent crashes
    long testResult = 0;
    if (m_testIfRunning) {
        testResult = m_testIfRunning();
    }
    
    if (testResult != 0) {
        return false;  // NVDA not running
    }
    
    // Send text to NVDA - wrap in check to prevent crashes
    long speakResult = -1;
    if (m_speakText && text) {
        speakResult = m_speakText(text);
    }
    
    return speakResult == 0;
}

bool NVDAClient::CancelSpeech() {
    if (!m_cancelSpeech) {
        return false;
    }

    if (!IsNVDARunning()) {
        return false;
    }

    return m_cancelSpeech() == 0;
}
