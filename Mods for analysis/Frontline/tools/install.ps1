# SRWar installer for the player (ships in the Workshop item; tools\package.ps1 builds it).
# Run from the subscribed item: steamapps\workshop\content\784150\<id>\install.bat
#   install.ps1 [-Uninstall]
# Puts dinput8.dll (the mod) into the game folder and SRWar\ (data, sounds, music) next to it, then
# builds the [FL] vehicles and the «Оборона» buildings from the player's own Workshop subscriptions.
param([switch]$Uninstall, [switch]$CheckOnly, [string]$GameDir, [string]$WorkshopDir)
$ErrorActionPreference = 'Stop'
$log = Join-Path ([IO.Path]::GetTempPath()) ('Frontline-install-' + (Get-Date -Format 'yyyyMMdd-HHmmss') + '-' + [guid]::NewGuid().ToString('N') + '.log')
$transcript = $false
$installerMutex=$null; $installerLockHeld=$false
try {
  Start-Transcript -LiteralPath $log | Out-Null
  $transcript = $true
  Write-Host "Installation log: $log"
  $item = Split-Path $PSScriptRoot          # the Workshop item folder (this script is in its tools\)
  try { . (Join-Path $PSScriptRoot 'paths.ps1') -GameDir $GameDir -WorkshopDir $WorkshopDir }
  catch {
    if ($GameDir -or $env:SRWAR_GAME_DIR -or $CheckOnly -or $_ -notmatch 'game|Game') { throw }
    Add-Type -AssemblyName System.Windows.Forms
    $d=New-Object System.Windows.Forms.FolderBrowserDialog
    $d.Description='Workers & Resources: Soviet Republic folder (SOVIET64.exe)'
    if($d.ShowDialog() -ne 'OK'){throw 'No game folder selected'}
    . (Join-Path $PSScriptRoot 'paths.ps1') -GameDir $d.SelectedPath -WorkshopDir $WorkshopDir
  }
  if (Get-Process SOVIET64 -ErrorAction SilentlyContinue) { throw 'Close the game first / Сначала закройте игру' }

  $lockHasher=[Security.Cryptography.SHA256]::Create()
  try{$lockKey=([BitConverter]::ToString($lockHasher.ComputeHash([Text.Encoding]::UTF8.GetBytes($game.ToLowerInvariant())))).Replace('-','')}finally{$lockHasher.Dispose()}
  $installerMutex=New-Object Threading.Mutex($false,('Local\FrontlineInstaller_'+$lockKey))
  try{$installerLockHeld=$installerMutex.WaitOne(0)}catch [Threading.AbandonedMutexException]{$installerLockHeld=$true}
  if(-not $installerLockHeld){throw 'Another Frontline installation/uninstallation is running. Wait for it to finish.'}
  $dll = Join-Path $game 'dinput8.dll'
  if ($Uninstall) {
    Remove-Item $dll -Force -ErrorAction SilentlyContinue
    & (Join-Path $PSScriptRoot 'fl.ps1') -Clean -GameDir $game -WorkshopDir $workshop
    Write-Host "SRWar removed (dinput8.dll, [FL] vehicles). SRWar\ and the buildings in workshop_wip are kept."
    return
  }

  . (Join-Path $PSScriptRoot 'preflight.ps1')
  Test-InstallInputs
  . (Join-Path $PSScriptRoot 'language.ps1')
  $installedLanguage=if($english){'English'}else{'Russian'}
  Write-Host "Selected mod language: $installedLanguage (game: $gameLanguage; Steam: $steamLang)"
  . (Join-Path $PSScriptRoot 'migration.ps1')
  $migrationPlan = New-FrontlineMigrationPlan -GameDir $game -ItemDir $item
  if ($CheckOnly) { Write-Host "Preflight passed: $game; Workshop: $workshop"; return }

  Connect-FrontlineWorkshopLibraries -ItemDir $item

  # Another dinput8.dll that is not ours: keep it aside rather than overwrite it.
  if ((Test-Path $dll) -and -not (Select-String -Path $dll -Pattern 'SRWar' -SimpleMatch -Quiet)) {
    if (-not (Test-Path -LiteralPath "$dll.before_srwar")) { Copy-Item -LiteralPath $dll -Destination "$dll.before_srwar" }
    Write-Host "An existing dinput8.dll was kept as dinput8.dll.before_srwar"
  }
  Copy-Item (Join-Path $item 'dinput8.dll') $dll -Force
  $dst = Join-Path $game 'SRWar'
  New-Item -ItemType Directory -Force $dst | Out-Null
  Copy-Item (Join-Path $item 'data\*') $dst -Recurse -Force
  foreach ($sub in 'sounds', 'music') {
    $from = Join-Path $item $sub
    if (Test-Path $from) { Copy-Item $from $dst -Recurse -Force }
  }
  & (Join-Path $PSScriptRoot 'fl.ps1') -GameDir $game -WorkshopDir $workshop
  & (Join-Path $PSScriptRoot 'defence.ps1') -GameDir $game -WorkshopDir $workshop
  Write-Host ''
  Invoke-FrontlineMigration $migrationPlan
  Complete-FrontlineRetiredVehicles -GameDir $game -ItemDir $item -MigrationPlan $migrationPlan
  $languageCode=if($english){'en'}else{'ru'}
  [IO.File]::WriteAllText((Join-Path $dst 'install_language.txt'),("language=$languageCode`r`nname=$installedLanguage`r`n"),(New-Object Text.UTF8Encoding($false)))
  if($english){Write-Host 'Mod language: English / Язык мода: Английский.'}else{Write-Host 'Язык мода: Русский / Mod language: Russian.'}
  Write-Host "SRWar installed to $game. Start the game as usual. / SRWar установлен, запускайте игру как обычно."

} catch {
  Write-Host ("INSTALLATION FAILED: " + $_.Exception.Message) -ForegroundColor Red
  Write-Host "Installation log: $log"
  throw
} finally {
  if($installerLockHeld){$installerMutex.ReleaseMutex()}
  if($installerMutex){$installerMutex.Dispose()}
  if ($transcript) { Stop-Transcript | Out-Null }
}
