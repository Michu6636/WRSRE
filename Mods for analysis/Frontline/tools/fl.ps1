# Frontline [FL] vehicles (owner 07.11): for every Warsaw Pact vehicle and aircraft of data\units.ini, a
# copy of the SR vehicle its model comes from, named "<unit name> [FL]", that the player buys like any
# other vehicle. Only these turn into the mod's units (tank / APC HQs, airfields); NATO's are not made.
# Built from the player's own subscribed workshop items into SovietRepublic\media_soviet\vehicles\
# srwar_fl_<ID> (the game's own vehicles folder: copies in workshop_wip were never offered for sale).
#   fl.ps1 [-Clean]
param([switch]$Clean, [string]$GameDir, [string]$WorkshopDir)
. (Join-Path $PSScriptRoot 'paths.ps1') -GameDir $GameDir -WorkshopDir $WorkshopDir
$media = Join-Path $game 'media_soviet'
$vehicles = Join-Path $media 'vehicles'
# Aircraft go where the game keeps its own: a plane bought from media_soviet\vehicles crashed the game
# in C3D_MESH::GetNodeHeader (07.11). Planes -> airplanes\, helicopters -> helicopters\.
$dirs = @($vehicles, (Join-Path $media 'airplanes'), (Join-Path $media 'helicopters'))
$utf8 = New-Object System.Text.UTF8Encoding($false)

if ($Clean) { foreach ($d in $dirs) { Get-ChildItem $d -Directory -Filter 'srwar_fl_*' | Remove-Item -Recurse -Force }; 'removed'; return }

# data\units.ini -> sections (first value of each key)
$types = @(); $cur = $null
foreach ($line in [IO.File]::ReadAllLines((Join-Path $PSScriptRoot '..\data\units.ini'), $utf8)) {
  $l = ($line -split ';')[0].Trim()
  if ($l -eq '') { continue }
  if ($l -match '^\[(.+)\]$') { $cur = [ordered]@{ id = $Matches[1] }; $types += $cur; continue }
  if ($cur -and $l -match '^(\w+)\s*=\s*(.*)$' -and -not $cur.Contains($Matches[1])) { $cur[$Matches[1]] = $Matches[2].Trim() }
}

. (Join-Path $PSScriptRoot 'language.ps1')

$made = 0; $skipped = @()
foreach ($t in $types) {
  if ($t.side -ne 'PACT' -or $t.squad -or -not $t.model) { continue }
  $folder = Split-Path $t.model -Parent
  # A workshop item, one of the game's own vehicles (model = vehicles/<folder>/main.nmf), or a model of
  # the mod's own (srwar/<folder>/main.nmf = SovietRepublic\SRWar\models\<folder>: the WRD conversions,
  # SRWar_WRD_Port next to this repo; made on the owner's machine only).
  $src = if ($folder -like 'vehicles*') { Join-Path (Split-Path $vehicles) $folder }
         elseif ($folder -like 'srwar*') { Join-Path (Join-Path (Split-Path $media) 'SRWar\models') ($folder -replace '^srwar[\\/]', '') }
         else { Resolve-FrontlineWorkshopPath $folder }
  if (-not (Test-Path (Join-Path $src 'script.ini'))) { $skipped += "$($t.id) (no $folder)"; continue }
  # No model yet (wrd_sr.py writes everything but main.nmf, which ModelViewer makes): a vehicle without
  # its model would crash the game when bought.
  if (-not (Test-Path (Join-Path $src (Split-Path $t.model -Leaf)))) { $skipped += "$($t.id) (no $($t.model))"; continue }
  $kind = (Select-String -Path (Join-Path $src 'script.ini') -Pattern '^\s*\$TYPE\s+(\w+)' | Select-Object -First 1).Matches.Groups[1].Value
  $dstDir = switch ($kind) { 'VEHICLETYPE_AIRPLANE' { $dirs[1] } 'VEHICLETYPE_HELICOPTER' { $dirs[2] } default { $dirs[0] } }
  $name = 'srwar_fl_' + $t.id
  foreach ($d in $dirs) { if ($d -ne $dstDir -and (Test-Path (Join-Path $d $name))) { Remove-Item (Join-Path $d $name) -Recurse -Force } }
  $dst = Join-Path $dstDir $name
  $rel = (Split-Path $dstDir -Leaf) + "/" + $name   # how the game names files of this copy: "airplanes/srwar_fl_MIG21"
  New-Item -ItemType Directory -Force $dst | Out-Null
  # Model, textures, bbox, sounds: everything but the script; content hashes also detect a replacement with an older timestamp.
  foreach ($f in Get-ChildItem $src -File) {
    if ($f.Name -ieq 'script.ini' -or $f.Name -ieq 'workshopconfig.ini' -or $f.Extension -ieq '.obj' -or $f.Name -ieq 'main.mtl') { continue }   # .obj / main.mtl: wrd_sr.py's input to ModelViewer
    $to = Join-Path $dst $f.Name
    if (-not (Test-Path -LiteralPath $to) -or (Get-FileHash -LiteralPath $to -Algorithm SHA256).Hash -ne (Get-FileHash -LiteralPath $f.FullName -Algorithm SHA256).Hash) { Copy-Item -LiteralPath $f.FullName -Destination $to -Force }
  }
  foreach ($d in Get-ChildItem $src -Directory) {
    $subdir = Join-Path $dst $d.Name
    New-Item -ItemType Directory -Force $subdir | Out-Null
    Get-ChildItem -LiteralPath $d.FullName -Force | Copy-Item -Destination $subdir -Recurse -Force
  }
  # Production/purchase is separate from combat service (years). No invented end year 2000.
  $sourceScript = [IO.File]::ReadAllText((Join-Path $src 'script.ini'))
  $production = @([regex]::Matches($t.production_years, '\d{4}') | ForEach-Object { $_.Value })
  $sourceAvailable = [regex]::Match($sourceScript, '(?m)^\s*\$AVAILABLE\s+(\d{4})(?:[ \t]+(\d{4}))?')
  $service = @([regex]::Matches($t.years, '\d{4}') | ForEach-Object { $_.Value })
  if ($production.Count -eq 2) {
    $from = $production[0]; $to = $production[1]
  } elseif ($sourceAvailable.Success) {
    $from = if ($service.Count) { $service[0] } else { $sourceAvailable.Groups[1].Value }
    $to = $sourceAvailable.Groups[2].Value
    if ($to -and [int]$to -lt [int]$from) { $to = '' }
  } elseif ($service.Count) {
    $from = $service[0]; $to = if ($service.Count -gt 1) { $service[1] } else { '' }
  } else { throw "No availability dates for $($t.id)" }
  # This generated directory belongs to us. Archive files left by the former model/skins.
  # Move individual files only, with checked absolute source and destination roots.
  $sourceRoot = [IO.Path]::GetFullPath($src).TrimEnd('\') + '\'
  $destRoot = [IO.Path]::GetFullPath($dst).TrimEnd('\') + '\'
  $keep = @{}
  foreach ($file in Get-ChildItem -LiteralPath $src -File -Recurse) {
    $keep[$file.FullName.Substring($sourceRoot.Length)] = $true
  }
  $keep['script.ini'] = $true
  $stale = @(Get-ChildItem -LiteralPath $dst -File -Recurse | Where-Object { -not $keep.ContainsKey($_.FullName.Substring($destRoot.Length)) })
  if ($stale.Count) {
    $archiveRoot = [IO.Path]::GetFullPath((Join-Path $game ('SRWar\install_backups\fl\' + $t.id + '-' + [guid]::NewGuid().ToString('N')))).TrimEnd('\') + '\'
    foreach ($file in $stale) {
      if (-not $file.FullName.StartsWith($destRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'FL cleanup escaped destination' }
      $relative = $file.FullName.Substring($destRoot.Length)
      $backupFile = [IO.Path]::GetFullPath((Join-Path $archiveRoot $relative))
      if (-not $backupFile.StartsWith($archiveRoot, [StringComparison]::OrdinalIgnoreCase)) { throw 'FL backup escaped destination' }
      New-Item -ItemType Directory -Force (Split-Path $backupFile) | Out-Null
      Move-Item -LiteralPath $file.FullName -Destination $backupFile
    }
    Write-Output "$($t.id): archived $($stale.Count) obsolete resource(s) to $archiveRoot"
  }
  $out = New-Object System.Collections.Generic.List[string]
  $named = $false
  foreach ($line in [IO.File]::ReadAllLines((Join-Path $src 'script.ini'))) {
    $l = $line.Trim()
    if ($l -match '^\$(SKILL_\w+|NAME|NAME_STR|AVAILABLE|COUNTRY)\b') {
      if ($l -match '^\$NAME_STR\b' -and -not $named) { $out.Add('$NAME_STR "' + (Shown $t.name) + ' [FL]"'); $named = $true }
      continue
    }
    if ($l -eq 'end') { continue }
    # A workshop script names its own files bare ("$LANDING_GEAR_ANIM wheels.nmf"): fine inside a
    # workshop item, but from the game's folders the game looks for them under media_soviet\ and gets
    # nothing; a plane without its gear model crashed the game on purchase (C3D_MESH::GetNodeHeader,
    # 07.11). The game's own vehicles spell them out ("airplanes/L410/wheels.nmf"), so do we.
    if (-not $l.StartsWith('-')) {
      $line = [regex]::Replace($line, '(?<=^|\s)([\w\-\.]+\.(nmf|naf|mtl|dds|ini|bin|png))(?=\s|$)', {
        param($m)
        $f = $m.Groups[1].Value
        if ($l.StartsWith('$SOUND_PARAMS') -or -not (Test-Path (Join-Path $src $f))) { $f } else { $rel + '/' + $f }
      })
    }
    $out.Add($line)
  }
  if (-not $named) { $out.Insert(0, '$NAME_STR "' + (Shown $t.name) + ' [FL]"') }
  $out.Add(("`$AVAILABLE $from $to").TrimEnd())
  $out.Add('$COUNTRY 39011')
  $out.Add('end')
  [IO.File]::WriteAllLines((Join-Path $dst 'script.ini'), $out, $utf8)
  $made++
}
"[FL]: $made vehicles in $media (vehicles / airplanes / helicopters)"
if ($skipped) { "not made (model not subscribed): " + ($skipped -join ', ') }
