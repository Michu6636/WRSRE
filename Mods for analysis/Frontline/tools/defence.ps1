# Builds the «Оборона» buildings (data\defence.ini) as a local SR mod in
# SovietRepublic\media_soviet\workshop_wip\<id>\ from the player's own subscribed workshop models.
#   defence.ps1 [-Id 9100000001]
param([string]$Id = '9100000001', [string]$GameDir, [string]$WorkshopDir)
. (Join-Path $PSScriptRoot 'paths.ps1') -GameDir $GameDir -WorkshopDir $WorkshopDir
. (Join-Path $PSScriptRoot 'language.ps1')
$dst = Join-Path $game "media_soviet\workshop_wip\$Id"
$utf8 = New-Object System.Text.UTF8Encoding($false)

# data\defence.ini -> ordered list of sections
$defs = @(); $cur = $null
foreach ($line in [IO.File]::ReadAllLines((Join-Path $PSScriptRoot '..\data\defence.ini'), $utf8)) {
  $l = $line.Trim()
  if ($l -eq '' -or $l.StartsWith(';')) { continue }
  if ($l -match '^\[(.+)\]$') { $cur = [ordered]@{ id = $Matches[1] }; $defs += $cur; continue }
  if ($l -match '^(\w+)\s*=\s*(.*)$') { $cur[$Matches[1]] = $Matches[2].Trim() }
}

New-Item -ItemType Directory -Force $dst | Out-Null
$config = @(('$ITEM_ID ' + $Id), '', '$ITEM_TYPE WORKSHOP_ITEMTYPE_BUILDING', '', '$VISIBILITY 2')
# Lines of the source building.ini that describe what the building is; ours replace them.
$drop = '^\s*\$(NAME|NAME_STR|TYPE_\w+|CIVIL_BUILDING|QUALITY_OF_LIVING|WORKERS_NEEDED|PROFESORS_NEEDED|CITIZEN_ABLE_SERVE|STORAGE\s+RESOURCE_TRANSPORT_PASSANGER|WORKING_VEHICLES_NEEDED|HELIPORT_AREA)\b'
foreach ($d in $defs) {
  $item, $folder = $d.source -split '/', 2
  $out = Join-Path $dst $d.id
  Remove-Item $out -Recurse -Force -ErrorAction SilentlyContinue
  if ($item -eq 'game') {
    # One of the game's own buildings (media_soviet\buildings + buildings_types), laid out as a
    # workshop building folder.
    $media = Join-Path $game 'media_soviet'
    New-Item -ItemType Directory -Force $out | Out-Null
    Copy-Item (Join-Path $media "buildings\$folder.nmf") (Join-Path $out 'model.nmf')
    foreach ($ext in 'bbox', 'fire', 'ini') { Copy-Item (Join-Path $media "buildings_types\$folder.$ext") (Join-Path $out "building.$ext") }
    # $TEXTURE paths in the game's materials are relative to media_soviet, so they work as they are.
    Copy-Item (Join-Path $media "editor\tool_$folder.png") (Join-Path $out 'imagegui.png') -ErrorAction SilentlyContinue
    Copy-Item (Join-Path $media "buildings\$folder.mtl") (Join-Path $out 'material.mtl')
    Copy-Item (Join-Path $media "buildings\${folder}_e.mtl") (Join-Path $out 'material_e.mtl')
    $rc = @('$TYPE_WORKSHOP', 'MODEL model.nmf', 'MATERIAL material.mtl', 'MATERIALEMISSIVE material_e.mtl', 'LIFE 4000.0', 'EXPLOSION_GROUP 2',
            'DERBIS_FALLING_FX buildingfall1 1.4', 'DERBIS_FALLED_FX buildingfall2 1.4', 'DERBIS_FALLED_SFX collapse', 'DERBIS_NUM 35',
            'DERBIS_FALLING_FX_MAXTIME 1.0', 'DERBIS_SCALE 1.4', 'DERBIS_MESH buildings/buildingwreck1.nmf buildings/buildingwreck.mtl',
            'DERBIS_MESH buildings/buildingwreck2.nmf buildings/buildingwreck.mtl', 'DERBIS_MESH buildings/buildingwreck3.nmf buildings/buildingwreck.mtl', 'END')
    [IO.File]::WriteAllText((Join-Path $out 'renderconfig.ini'), ($rc -join "`r`n") + "`r`n")
  } else {
    $src = Resolve-FrontlineWorkshopPath "$item\$folder"
    if (-not (Test-Path $src)) { Write-Warning "$($d.id): subscribe to workshop item $item (missing $src)"; continue }
    Copy-Item $src $out -Recurse -Force
    # Materials of that item point to a shared ..\textures folder.
    $tex = Resolve-FrontlineWorkshopPath "$item\textures"
    if ((Test-Path $tex) -and -not (Test-Path (Join-Path $dst 'textures'))) { Copy-Item $tex (Join-Path $dst 'textures') -Recurse -Force }
  }
  $ini = Join-Path $out 'building.ini'
  $body = [IO.File]::ReadAllLines($ini, $utf8) | Where-Object { $_ -notmatch $drop }
  $head = @(('$NAME_STR "' + (Shown $d.name) + '"')) + ($d.type -split '\|' | ForEach-Object { $_.Trim() })
  if ($d.workers) { $head += '$WORKERS_NEEDED ' + $d.workers }   # fortifications: no staff
  $head += ''
  [IO.File]::WriteAllText($ini, (($head + $body) -join "`r`n") + "`r`n", $utf8)
  $config += '$OBJECT_BUILDING ' + $d.id
  Write-Output "$($d.id): $($d.name) from $($d.source)"
}
$category = Shown 'SRWar: Оборона республики'
$description = Shown 'Здания для обороны республики: штабы, казармы, склады и укрепления.'
$config += @('', ('$ITEM_NAME "' + $category + '"'), '', ('$ITEM_DESC "' + $description + '"'))
[IO.File]::WriteAllText((Join-Path $dst 'workshopconfig.ini'), ($config -join "`r`n") + "`r`n", $utf8)
Write-Output "installed to $dst"
