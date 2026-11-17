[CmdletBinding()]
param(
    [switch]$DeveloperUI,
    [string]$ImGuiDir = ''
)

$ErrorActionPreference = 'Stop'
Set-StrictMode -Version Latest
$root = Split-Path $PSScriptRoot -Parent
. (Join-Path $PSScriptRoot 'Build-Environment.ps1')
$tools = Initialize-EarthworksBuildEnvironment
$sdkDir = Join-Path $root 'external\TesmioLoader'
if (-not (Test-Path -LiteralPath (Join-Path $sdkDir 'src\tesmio_api.h'))) {
    throw "Pinned Tesmio SDK checkout is missing: $sdkDir. Run scripts\Fetch-Dependencies.ps1 first."
}
$sdkDir = (Resolve-Path -LiteralPath $sdkDir).Path
if ([string]::IsNullOrWhiteSpace($ImGuiDir)) {
    $ImGuiDir = Join-Path $root 'external\DearImGui'
}
if (-not (Test-Path -LiteralPath (Join-Path $ImGuiDir 'imgui.cpp'))) {
    throw "Pinned Dear ImGui checkout is missing: $ImGuiDir. Run scripts\Fetch-Dependencies.ps1 first."
}
$ImGuiDir = (Resolve-Path -LiteralPath $ImGuiDir).Path
Push-Location $root
try {
    $preset = if ($DeveloperUI) { 'windows-x64-developer-ui' } else { 'windows-x64' }
    & $tools.CMake --preset $preset "-DTESMIO_SDK_DIR=$sdkDir" "-DEARTHWORKS_IMGUI_DIR=$ImGuiDir"
    if ($LASTEXITCODE -ne 0) { throw 'CMake configuration failed.' }
    & $tools.CMake --build --preset $preset
    if ($LASTEXITCODE -ne 0) { throw 'Plugin build failed.' }
    $dll = Join-Path $root ("build\{0}\bin\EarthworksPlanning.dll" -f $preset)
    $details = (& dumpbin.exe /nologo /headers /exports $dll | Out-String)
    if ($LASTEXITCODE -ne 0) { throw 'DLL inspection failed.' }
    if ($details -notmatch '(?im)^\s*8664 machine') { throw 'Plugin is not an x64 DLL.' }
    foreach ($entry in @('TsmPluginApiVersion', 'TsmPluginInit', 'TsmPluginStart')) {
        if ($details -notmatch ('\b' + $entry + '\b')) { throw "Plugin export missing: $entry" }
    }
    Write-Host "Verified x64 DLL and all three Tesmio entry points: $dll"
} finally {
    Pop-Location
}
