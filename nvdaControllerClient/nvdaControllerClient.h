/*
 * NVDA Controller Client API
 * Copyright (C) 2007-2021 NV Access Limited
 */

#ifndef NVDA_CONTROLLER_CLIENT_H
#define NVDA_CONTROLLER_CLIENT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Tests if NVDA is running.
 * @return 0 if running, non-zero otherwise
 */
__declspec(dllimport) long __stdcall nvdaController_testIfRunning();

/**
 * Cancels any speech currently being spoken.
 * @return 0 on success, non-zero on failure
 */
__declspec(dllimport) long __stdcall nvdaController_cancelSpeech();

/**
 * Speaks the given text.
 * @param text the text to speak (null-terminated wide string)
 * @return 0 on success, non-zero on failure
 */
__declspec(dllimport) long __stdcall nvdaController_speakText(const wchar_t* text);

/**
 * Brailles a message.
 * @param message the message to braille (null-terminated wide string)
 * @return 0 on success, non-zero on failure
 */
__declspec(dllimport) long __stdcall nvdaController_brailleMessage(const wchar_t* message);

#ifdef __cplusplus
}
#endif

#endif // NVDA_CONTROLLER_CLIENT_H
