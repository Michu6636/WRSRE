# Shared development/installer path resolution; independent game, item and Workshop libraries.
param([string]$GameDir,[string]$WorkshopDir)
$ErrorActionPreference='Stop'
function Get-FrontlineSteamLibraries {
 $roots=New-Object Collections.Generic.List[string]
 $steamSources=if($env:SRWAR_STEAM_DIR){@($env:SRWAR_STEAM_DIR)}else{@((Get-ItemProperty 'HKCU:\Software\Valve\Steam' -ErrorAction SilentlyContinue).SteamPath,(Get-ItemProperty 'HKLM:\SOFTWARE\WOW6432Node\Valve\Steam' -ErrorAction SilentlyContinue).InstallPath)}
 foreach($path in $steamSources){
  if(-not $path){continue};$path=[IO.Path]::GetFullPath($path);if($roots -notcontains $path){$roots.Add($path)}
  $vdf=Join-Path $path 'steamapps\libraryfolders.vdf'
  if(Test-Path -LiteralPath $vdf){
   foreach($match in [regex]::Matches([IO.File]::ReadAllText($vdf),'"(?:path|\d+)"\s+"([^"]+)"')){
    $library=$match.Groups[1].Value.Replace('\\','\');if($library -match '^[A-Za-z]:[\\/]'){$library=[IO.Path]::GetFullPath($library);if($roots -notcontains $library){$roots.Add($library)}}
   }
  }
 }
 return $roots.ToArray()
}
function Resolve-FrontlineWorkshopPath([string]$Relative) {
 foreach($root in $script:frontlineWorkshopRoots){$candidate=Join-Path $root $Relative;if(Test-Path -LiteralPath $candidate){return $candidate}}
 return Join-Path $workshop $Relative
}
function Connect-FrontlineWorkshopLibraries([string]$ItemDir) {
 # Engine paths resolve from the game's Steam library. Link external item folders locally;
 # no Workshop assets are duplicated, changed, distributed or deleted.
 $expected=Join-Path $gameSteamapps 'workshop\content\784150';$ids=New-Object 'Collections.Generic.HashSet[string]'
 foreach($catalog in 'units.ini','defence.ini'){
  foreach($line in [IO.File]::ReadAllLines((Join-Path $ItemDir "data\$catalog"))){if($line -match '^\s*(model|material|member|source|park)\s*='){foreach($m in [regex]::Matches(($line -split ';',2)[0],'(?<!\d)(\d{9,10})/')){[void]$ids.Add($m.Groups[1].Value)}}}
 }
 $created=New-Object Collections.Generic.List[string]
 try{
  foreach($id in $ids){
   $source=[IO.Path]::GetFullPath((Resolve-FrontlineWorkshopPath $id));$target=[IO.Path]::GetFullPath((Join-Path $expected $id))
   if($source.Equals($target,[StringComparison]::OrdinalIgnoreCase)){continue}
   if(Test-Path -LiteralPath $target){
    $existing=Get-Item -LiteralPath $target -Force
    if(-not($existing.Attributes -band [IO.FileAttributes]::ReparsePoint)){throw "Different Workshop copy already exists in the game library: $target"}
    $link=[string]@($existing.Target)[0];$link=$link -replace '^\\\?\?\\',''
    if(-not [IO.Path]::GetFullPath($link).Equals($source,[StringComparison]::OrdinalIgnoreCase)){throw "Workshop link has a different target: $target"};continue
   }
   if(-not(Test-Path -LiteralPath $source -PathType Container)){throw "Workshop source missing: $source"}
   New-Item -ItemType Directory -Force -Path $expected | Out-Null
   New-Item -ItemType Junction -Path $target -Target $source | Out-Null;$created.Add($target)
   Write-Host "Workshop library linked: $id"
  }
 }catch{
  foreach($target in $created){if((Get-Item -LiteralPath $target -Force).Attributes -band [IO.FileAttributes]::ReparsePoint){[IO.Directory]::Delete($target)}}
  throw "Cannot connect Workshop library before installation: $_"
 }
}
$workspaceFile=Join-Path (Split-Path (Split-Path $PSScriptRoot)) 'workspace.json'
$cfg=if(Test-Path -LiteralPath $workspaceFile){Get-Content -LiteralPath $workspaceFile -Raw | ConvertFrom-Json}else{$null}
$steamapps=$PSScriptRoot;while($steamapps -and (Split-Path $steamapps -Leaf) -ne 'steamapps'){$steamapps=Split-Path $steamapps}
$libraries=@(Get-FrontlineSteamLibraries)
$explicitGame=if($GameDir){$GameDir}elseif($env:SRWAR_GAME_DIR){$env:SRWAR_GAME_DIR}else{''}
if($explicitGame){$GameDir=$explicitGame}else{
 $candidates=New-Object Collections.Generic.List[string]
 foreach($candidate in @($(if($cfg){$cfg.game}),$(if($steamapps){Join-Path $steamapps 'common\SovietRepublic'}))+@($libraries | ForEach-Object {Join-Path $_ 'steamapps\common\SovietRepublic'})){
  if($candidate -and (Test-Path -LiteralPath (Join-Path $candidate 'SOVIET64.exe'))){$absolute=[IO.Path]::GetFullPath($candidate);if($candidates -notcontains $absolute){$candidates.Add($absolute)}}
 }
 if($candidates.Count -gt 1){throw 'Multiple game installations found; supply -GameDir or choose the game folder'}
 $GameDir=if($candidates.Count){$candidates[0]}else{''}
}
if(-not $GameDir -or -not(Test-Path -LiteralPath (Join-Path $GameDir 'SOVIET64.exe'))){throw "Game executable not found: $GameDir\SOVIET64.exe. Supply -GameDir or SRWAR_GAME_DIR."}
$game=[IO.Path]::GetFullPath($GameDir);$gameSteamapps=Split-Path (Split-Path $game)
$explicitWorkshop=if($WorkshopDir){$WorkshopDir}elseif($env:SRWAR_WORKSHOP_DIR){$env:SRWAR_WORKSHOP_DIR}else{''}
if($explicitWorkshop -and -not(Test-Path -LiteralPath $explicitWorkshop -PathType Container)){throw "Workshop folder not found: $explicitWorkshop"}
$script:frontlineWorkshopRoots=@()
foreach($candidate in @($explicitWorkshop,$(if($cfg){$cfg.workshop}),(Join-Path $gameSteamapps 'workshop\content\784150'),$(if($steamapps){Join-Path $steamapps 'workshop\content\784150'}))+@($libraries | ForEach-Object {Join-Path $_ 'steamapps\workshop\content\784150'})){
 if($candidate -and (Test-Path -LiteralPath $candidate -PathType Container)){$absolute=[IO.Path]::GetFullPath($candidate);if($script:frontlineWorkshopRoots -notcontains $absolute){$script:frontlineWorkshopRoots+=,$absolute}}
}
if(-not $script:frontlineWorkshopRoots.Count){throw 'Workshop folder not found. Supply -WorkshopDir or SRWAR_WORKSHOP_DIR.'}
$workshop=$script:frontlineWorkshopRoots[0]
