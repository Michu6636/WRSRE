# Validate the package and all catalog sources before modifying the game.
function Test-InstallInputs {
  $errors = New-Object System.Collections.Generic.List[string]
  foreach ($rel in 'dinput8.dll', 'data\units.ini', 'data\defence.ini', 'data\lang_en.txt', 'data\migration_0_3.json', 'tools\migration.ps1', 'tools\paths.ps1', 'tools\language.ps1', 'tools\fl.ps1', 'tools\defence.ps1') {
    if (-not (Test-Path -LiteralPath (Join-Path $item $rel) -PathType Leaf)) { $errors.Add("Package file missing: $rel") }
  }
  if ($errors.Count) { throw ($errors -join "`n") }
  $migrationConfig=[IO.File]::ReadAllText((Join-Path $item 'data\migration_0_3.json')) | ConvertFrom-Json
  if($migrationConfig.supported_exe_sha256 -and (Get-FileHash -LiteralPath (Join-Path $game 'SOVIET64.exe') -Algorithm SHA256).Hash.ToLowerInvariant() -ne $migrationConfig.supported_exe_sha256){throw 'Unsupported game executable. Frontline 0.3 requires the supported 1.1.1.9 DX11 x64 build; no files were installed.'}
  $utf8 = New-Object Text.UTF8Encoding($false)
  foreach ($catalog in 'units.ini', 'defence.ini') {
    $squads = @{}
    $section = ''
    foreach ($line in [IO.File]::ReadAllLines((Join-Path $item "data\$catalog"), $utf8)) {
      $l = ($line -split ';', 2)[0].Trim()
      if ($l -match '^\[(.+)\]$') { $section = $Matches[1] }
      if ($l -match '^squad\s*=\s*[^0\s]') { $squads[$section] = $true }
    }
    $section = ''; $side = ''
    foreach ($line in [IO.File]::ReadAllLines((Join-Path $item "data\$catalog"), $utf8)) {
      $l = ($line -split ';', 2)[0].Trim()
      if ($l -match '^\[(.+)\]$') { $section = $Matches[1]; $side = ''; continue }
      if ($l -match '^side\s*=\s*(\S+)') { $side = $Matches[1]; continue }
      if ($l -notmatch '^(model|material|member|source)\s*=\s*(.+)$') { continue }
      $key = $Matches[1]; $value = $Matches[2]
      foreach ($rel in ($value -split '\|')) {
        $rel = $rel.Trim()
        if ($rel -match '^srwar/') { continue } # optional private conversion, absent in public builds
        if ($rel -match '^\d{9,10}/') { $path = Resolve-FrontlineWorkshopPath $rel }
        elseif ($rel -match '^game/(.+)$') {
          $folder = $Matches[1]
          foreach ($file in "buildings\$folder.nmf", "buildings\$folder.mtl", "buildings\${folder}_e.mtl", "buildings_types\$folder.ini", "buildings_types\$folder.bbox", "buildings_types\$folder.fire") {
            if (-not (Test-Path -LiteralPath (Join-Path $game "media_soviet\$file") -PathType Leaf)) { $errors.Add("${section}: game building file missing: $file") }
          }
          continue
        } else { $path = Join-Path $game "media_soviet\$rel" }
        if (-not (Test-Path -LiteralPath $path)) { $errors.Add("${section}: missing $key $rel"); continue }
        if ($key -eq 'source') {
          foreach ($file in 'building.ini', 'renderconfig.ini') {
            if (-not (Test-Path -LiteralPath (Join-Path $path $file) -PathType Leaf)) { $errors.Add("${section}: missing $rel/$file") }
          }
        }
        if ($key -eq 'model' -and $side -eq 'PACT' -and -not $squads.ContainsKey($section)) {
          if (-not (Test-Path -LiteralPath (Join-Path (Split-Path $path) 'script.ini') -PathType Leaf)) { $errors.Add("${section}: source vehicle script.ini missing") }
        }
      }
    }
  }
  if ($errors.Count) { throw ("Installation preflight failed. Let Steam download the required items and retry.`n" + ($errors -join "`n")) }
  # Probe existing destination directories before copying; remove only the exact file created here.
  $targets = @($game, (Join-Path $game 'media_soviet'), (Join-Path $game 'SRWar'))
  $targets += 'vehicles', 'airplanes', 'helicopters', 'workshop_wip' | ForEach-Object { Join-Path $game "media_soviet\$_" }
  foreach ($target in $targets) {
    $probeDir = $target
    while (-not (Test-Path -LiteralPath $probeDir -PathType Container)) { $probeDir = Split-Path $probeDir }
    $probe = Join-Path $probeDir ('.frontline-write-' + [guid]::NewGuid().ToString('N'))
    try {
      $stream = [IO.File]::Open($probe, [IO.FileMode]::CreateNew, [IO.FileAccess]::Write, [IO.FileShare]::None)
      $stream.Dispose()
    } catch { throw "Cannot write to $target : $($_.Exception.Message)" }
    finally { if (Test-Path -LiteralPath $probe) { Remove-Item -LiteralPath $probe -Force } }
  }
}
