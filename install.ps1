# PowerShell script to install and register NVDA SAPI Bridge
# Must be run as Administrator

param(
    [switch]$Uninstall
)

# Check for admin privileges
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
if (-not $isAdmin) {
    Write-Error "This script must be run as Administrator"
    exit 1
}

# CLSID for the NVDA SAPI Bridge
$CLSID = "{2F9DE777-6B8D-4ECE-AA3A-1D7F5B2A7E2E}"
$TokenName = "NVDA"

# Get script directory
$ScriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path

# Determine system architecture and DLL paths
$Is64Bit = [Environment]::Is64BitOperatingSystem
$Dll32Path = Join-Path $ScriptDir "build\x86\nvda_sapi_bridge.dll"
$Dll64Path = Join-Path $ScriptDir "build\x64\nvda_sapi_bridge.dll"

# Installation directory
$InstallDir = "$env:ProgramFiles\NVDA SAPI Bridge"
$InstallDir32 = "${env:ProgramFiles(x86)}\NVDA SAPI Bridge"

function Register-Voice {
    param($DllPath, $Is64BitDll)
    
    if (-not (Test-Path $DllPath)) {
        Write-Error "DLL not found: $DllPath"
        return $false
    }
    
    # Determine registry path
    if ($Is64BitDll) {
        $RegPath = "HKLM:\SOFTWARE\Classes\CLSID\$CLSID"
        $TargetDir = $InstallDir
    } else {
        if ($Is64Bit) {
            $RegPath = "HKLM:\SOFTWARE\WOW6432Node\Classes\CLSID\$CLSID"
            $TargetDir = $InstallDir32
        } else {
            $RegPath = "HKLM:\SOFTWARE\Classes\CLSID\$CLSID"
            $TargetDir = $InstallDir
        }
    }
    
    # Create installation directory and copy DLL
    New-Item -ItemType Directory -Force -Path $TargetDir | Out-Null
    $TargetDll = Join-Path $TargetDir "nvda_sapi_bridge.dll"
    Copy-Item $DllPath $TargetDll -Force
    
    # Register COM object
    New-Item -Path $RegPath -Force | Out-Null
    New-ItemProperty -Path $RegPath -Name "(Default)" -Value "NVDA SAPI Bridge" -Force | Out-Null
    New-Item -Path "$RegPath\InprocServer32" -Force | Out-Null
    New-ItemProperty -Path "$RegPath\InprocServer32" -Name "(Default)" -Value $TargetDll -Force | Out-Null
    New-ItemProperty -Path "$RegPath\InprocServer32" -Name "ThreadingModel" -Value "Both" -Force | Out-Null
    
    # Register SAPI voice token
    $VoicesPath = "HKLM:\SOFTWARE\Microsoft\Speech\Voices\Tokens\$TokenName"
    if ($Is64Bit -and -not $Is64BitDll) {
        $VoicesPath = "HKLM:\SOFTWARE\WOW6432Node\Microsoft\Speech\Voices\Tokens\$TokenName"
    }
    
    New-Item -Path $VoicesPath -Force | Out-Null
    New-ItemProperty -Path $VoicesPath -Name "(Default)" -Value "NVDA Screen Reader" -Force | Out-Null
    New-ItemProperty -Path $VoicesPath -Name "CLSID" -Value $CLSID -Force | Out-Null
    New-ItemProperty -Path $VoicesPath -Name "409" -Value "NVDA Screen Reader" -Force | Out-Null
    
    # Add attributes
    New-Item -Path "$VoicesPath\Attributes" -Force | Out-Null
    New-ItemProperty -Path "$VoicesPath\Attributes" -Name "Language" -Value "409" -Force | Out-Null
    New-ItemProperty -Path "$VoicesPath\Attributes" -Name "Gender" -Value "Neutral" -Force | Out-Null
    New-ItemProperty -Path "$VoicesPath\Attributes" -Name "Name" -Value "NVDA" -Force | Out-Null
    New-ItemProperty -Path "$VoicesPath\Attributes" -Name "Vendor" -Value "NVDA SAPI Bridge" -Force | Out-Null
    
    return $true
}

function Unregister-Voice {
    param($Is64BitDll)
    
    # Determine registry path
    if ($Is64BitDll) {
        $RegPath = "HKLM:\SOFTWARE\Classes\CLSID\$CLSID"
        $VoicesPath = "HKLM:\SOFTWARE\Microsoft\Speech\Voices\Tokens\$TokenName"
        $TargetDir = $InstallDir
    } else {
        if ($Is64Bit) {
            $RegPath = "HKLM:\SOFTWARE\WOW6432Node\Classes\CLSID\$CLSID"
            $VoicesPath = "HKLM:\SOFTWARE\WOW6432Node\Microsoft\Speech\Voices\Tokens\$TokenName"
            $TargetDir = $InstallDir32
        } else {
            $RegPath = "HKLM:\SOFTWARE\Classes\CLSID\$CLSID"
            $VoicesPath = "HKLM:\SOFTWARE\Microsoft\Speech\Voices\Tokens\$TokenName"
            $TargetDir = $InstallDir
        }
    }
    
    # Remove registry keys
    if (Test-Path $RegPath) {
        Remove-Item -Path $RegPath -Recurse -Force
    }
    if (Test-Path $VoicesPath) {
        Remove-Item -Path $VoicesPath -Recurse -Force
    }
    
    # Remove installation directory
    if (Test-Path $TargetDir) {
        Remove-Item -Path $TargetDir -Recurse -Force
    }
}

# Main execution
if ($Uninstall) {
    Write-Host "Uninstalling NVDA SAPI Bridge..."
    
    if ($Is64Bit) {
        Write-Host "Unregistering 64-bit voice..."
        Unregister-Voice -Is64BitDll $true
        Write-Host "Unregistering 32-bit voice..."
        Unregister-Voice -Is64BitDll $false
    } else {
        Write-Host "Unregistering 32-bit voice..."
        Unregister-Voice -Is64BitDll $false
    }
    
    Write-Host "Uninstallation complete!"
} else {
    Write-Host "Installing NVDA SAPI Bridge..."
    
    if ($Is64Bit) {
        Write-Host "Registering 64-bit voice..."
        if (Register-Voice -DllPath $Dll64Path -Is64BitDll $true) {
            Write-Host "64-bit voice registered successfully"
        }
        
        Write-Host "Registering 32-bit voice for WOW64..."
        if (Register-Voice -DllPath $Dll32Path -Is64BitDll $false) {
            Write-Host "32-bit voice registered successfully"
        }
    } else {
        Write-Host "Registering 32-bit voice..."
        if (Register-Voice -DllPath $Dll32Path -Is64BitDll $false) {
            Write-Host "32-bit voice registered successfully"
        }
    }
    
    Write-Host ""
    Write-Host "Installation complete!"
    Write-Host "The NVDA voice should now be available in SAPI5 applications."
    Write-Host "Make sure NVDA is running when using legacy applications."
}
