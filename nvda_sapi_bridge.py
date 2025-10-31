"""
NVDA SAPI Bridge - A SAPI5 to NVDA bridge using Python

This module implements a SAPI5 TTS engine that forwards speech to NVDA.
"""

import sys
import os
import ctypes
from ctypes import wintypes
import pythoncom
import win32com.server.register
from win32com.server.exception import COMException
import win32com.client

# NVDA Controller Client DLL functions
nvdaControllerClient = None
nvda_speak = None
nvda_cancel = None
nvda_test = None


def load_nvda_controller():
    """Load NVDA controller client DLL"""
    global nvdaControllerClient, nvda_speak, nvda_cancel, nvda_test
    
    # Try to load the DLL
    dll_names = ["nvdaControllerClient64.dll", "nvdaControllerClient32.dll"]
    
    for dll_name in dll_names:
        try:
            nvdaControllerClient = ctypes.windll.LoadLibrary(dll_name)
            
            # Get function pointers
            nvda_speak = nvdaControllerClient.nvdaController_speakText
            nvda_speak.argtypes = [wintypes.LPCWSTR]
            nvda_speak.restype = ctypes.c_long
            
            nvda_cancel = nvdaControllerClient.nvdaController_cancelSpeech
            nvda_cancel.restype = ctypes.c_long
            
            nvda_test = nvdaControllerClient.nvdaController_testIfRunning
            nvda_test.restype = ctypes.c_long
            
            # Test if NVDA is running
            if nvda_test() == 0:
                return True
                
        except (OSError, AttributeError):
            continue
    
    return False


class NVDASAPIBridge:
    """SAPI5 TTS Engine that forwards to NVDA
    
    Note: This is a simplified SAPI5 voice implementation. Full SAPI5 TTS engine
    interface (ISpTTSEngine) is complex and not fully supported by pywin32.
    This implementation works as a basic voice for many SAPI5 applications,
    but may not be compatible with all applications that require complete
    TTS engine interfaces.
    """
    
    _reg_clsid_ = "{A1F4C0E0-8B6E-4B5F-9F4A-1E2D3C4B5A6F}"
    _reg_desc_ = "NVDA SAPI Bridge TTS Engine"
    _reg_progid_ = "NVDA.SAPIBridge.1"
    _public_methods_ = ['Speak', 'GetOutputFormat', 'SetObjectToken']
    # Note: _com_interfaces_ is empty because ISpTTSEngine is not directly
    # available in pywin32. This works as a basic voice but may have
    # compatibility limitations with some SAPI5 applications.
    _com_interfaces_ = []
    
    def __init__(self):
        """Initialize the bridge"""
        self.nvda_available = load_nvda_controller()
    
    def Speak(self, text):
        """Speak text through NVDA"""
        if self.nvda_available:
            try:
                # Check if NVDA is still running
                if nvda_test() == 0:
                    nvda_speak(str(text))
                else:
                    # NVDA stopped, try to reload
                    self.nvda_available = load_nvda_controller()
            except (OSError, ctypes.WinError) as e:
                # Log error for debugging but don't crash
                # In production, consider using proper logging
                print(f"Warning: NVDA speech failed: {e}", file=sys.stderr)
        return 0  # S_OK
    
    def GetOutputFormat(self):
        """Return audio format (not used since we don't produce audio)"""
        return None
    
    def SetObjectToken(self, token):
        """Set the voice token"""
        return 0  # S_OK


def register_nvda_bridge():
    """Register the NVDA SAPI Bridge"""
    from win32com.server.register import UseCommandLine
    
    print("Registering NVDA SAPI Bridge...")
    
    try:
        # Register COM server
        UseCommandLine(NVDASAPIBridge)
        
        # Add to SAPI voices
        import winreg
        
        voice_key = r"SOFTWARE\Microsoft\Speech\Voices\Tokens\NVDABridge"
        
        with winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, voice_key) as key:
            winreg.SetValueEx(key, None, 0, winreg.REG_SZ, "NVDA Bridge")
            winreg.SetValueEx(key, "CLSID", 0, winreg.REG_SZ, 
                             NVDASAPIBridge._reg_clsid_)
            winreg.SetValueEx(key, "409", 0, winreg.REG_SZ, "NVDA Bridge")
        
        # Set attributes
        attr_key = voice_key + r"\Attributes"
        with winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, attr_key) as key:
            winreg.SetValueEx(key, "Language", 0, winreg.REG_SZ, "409")
            winreg.SetValueEx(key, "Gender", 0, winreg.REG_SZ, "Neutral")
            winreg.SetValueEx(key, "Age", 0, winreg.REG_SZ, "Adult")
            winreg.SetValueEx(key, "Vendor", 0, winreg.REG_SZ, "NVDA Community")
            winreg.SetValueEx(key, "Name", 0, winreg.REG_SZ, "NVDA Bridge")
        
        print("Registration successful!")
        print("NVDA Bridge is now available as a SAPI5 voice.")
        
    except Exception as e:
        print(f"Registration failed: {e}")
        print("Make sure you run this script as Administrator.")
        sys.exit(1)


def unregister_nvda_bridge():
    """Unregister the NVDA SAPI Bridge"""
    from win32com.server.register import UnregisterServer
    
    print("Unregistering NVDA SAPI Bridge...")
    
    try:
        # Unregister COM server
        UnregisterServer(NVDASAPIBridge._reg_clsid_)
        
        # Remove from SAPI voices
        import winreg
        
        voice_key = r"SOFTWARE\Microsoft\Speech\Voices\Tokens\NVDABridge"
        try:
            winreg.DeleteKey(winreg.HKEY_LOCAL_MACHINE, voice_key + r"\Attributes")
            winreg.DeleteKey(winreg.HKEY_LOCAL_MACHINE, voice_key)
        except OSError:
            # Key may not exist if never registered or already removed
            pass
        
        print("Unregistration successful!")
        
    except Exception as e:
        print(f"Unregistration failed: {e}")
        sys.exit(1)


if __name__ == "__main__":
    if len(sys.argv) > 1:
        if sys.argv[1] == "--register":
            register_nvda_bridge()
        elif sys.argv[1] == "--unregister":
            unregister_nvda_bridge()
        else:
            print("Usage:")
            print("  python nvda_sapi_bridge.py --register     Register the bridge")
            print("  python nvda_sapi_bridge.py --unregister   Unregister the bridge")
    else:
        print("NVDA SAPI Bridge")
        print("================")
        print()
        print("Usage:")
        print("  python nvda_sapi_bridge.py --register     Register the bridge")
        print("  python nvda_sapi_bridge.py --unregister   Unregister the bridge")
        print()
        print("Note: Run as Administrator for registration/unregistration")
