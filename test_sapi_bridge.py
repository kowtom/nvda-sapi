"""
Test script for NVDA SAPI Bridge

This script tests the NVDA SAPI Bridge by enumerating SAPI5 voices
and attempting to speak through the NVDA Bridge.
"""

import sys
import win32com.client

def list_sapi_voices():
    """List all available SAPI5 voices"""
    try:
        speaker = win32com.client.Dispatch("SAPI.SpVoice")
        voices = speaker.GetVoices()
        
        print("Available SAPI5 Voices:")
        print("=" * 50)
        
        for i in range(voices.Count):
            voice = voices.Item(i)
            name = voice.GetDescription()
            print(f"  {i+1}. {name}")
        
        return speaker, voices
        
    except Exception as e:
        print(f"Error listing voices: {e}")
        return None, None


def test_nvda_bridge():
    """Test the NVDA Bridge voice"""
    try:
        speaker = win32com.client.Dispatch("SAPI.SpVoice")
        voices = speaker.GetVoices()
        
        # Find NVDA Bridge
        nvda_bridge = None
        for i in range(voices.Count):
            voice = voices.Item(i)
            if "NVDA Bridge" in voice.GetDescription():
                nvda_bridge = voice
                break
        
        if nvda_bridge:
            print("\n" + "=" * 50)
            print("Testing NVDA Bridge...")
            print("=" * 50)
            
            # Set the voice
            speaker.Voice = nvda_bridge
            
            # Speak test message
            test_message = "Hello from SAPI5! This message should be spoken by NVDA."
            print(f"\nSpeaking: '{test_message}'")
            speaker.Speak(test_message)
            
            print("\nTest complete!")
            print("If you heard the message through NVDA, the bridge is working correctly.")
        else:
            print("\n" + "=" * 50)
            print("NVDA Bridge voice NOT FOUND!")
            print("=" * 50)
            print("\nPlease ensure:")
            print("1. The bridge is registered (run scripts/register.bat as Administrator)")
            print("2. NVDA is installed and running")
            return False
        
        return True
        
    except Exception as e:
        print(f"\nError during test: {e}")
        import traceback
        traceback.print_exc()
        return False


def main():
    """Main test function"""
    print("NVDA SAPI Bridge Test")
    print("=" * 50)
    print()
    
    # List all voices
    speaker, voices = list_sapi_voices()
    
    if not speaker:
        print("\nFailed to initialize SAPI5!")
        print("Please ensure SAPI5 is available on your system.")
        sys.exit(1)
    
    # Test NVDA Bridge
    test_nvda_bridge()


if __name__ == "__main__":
    main()
