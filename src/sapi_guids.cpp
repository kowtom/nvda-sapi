#include "sapi_minimal.h"

// Define the GUIDs
// Note: DEFINE_GUID declares them; we need to instantiate them

// IID_ISpTTSEngine
// {5B559F40-E952-11D2-BB91-00C04F8EE6C0}
const GUID IID_ISpTTSEngine = 
    {0x5B559F40, 0xE952, 0x11D2, {0xBB, 0x91, 0x00, 0xC0, 0x4F, 0x8E, 0xE6, 0xC0}};

// SPDFID_Text
// {7CEEF9F9-3D13-11d2-9EE7-00C04F797396}
const GUID SPDFID_Text = 
    {0x7CEEF9F9, 0x3D13, 0x11d2, {0x9E, 0xE7, 0x00, 0xC0, 0x4F, 0x79, 0x73, 0x96}};
