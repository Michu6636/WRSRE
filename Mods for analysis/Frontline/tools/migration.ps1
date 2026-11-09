# Frontline 0.3 save migration. Loaded by install.ps1; no Python/runtime dependencies.
# Binary layout evidence and supported build: MIGRATION_0.3.md.
if (-not ('Frontline03.VehicleFile' -as [type])) {
Add-Type -TypeDefinition @"
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
namespace Frontline03 {
 public sealed class ModelField { public int Offset, Width; public string Value; }
 public static class VehicleFile {
  static int I(byte[] b,int p) { if(p<0 || (long)p+4>b.Length) throw new InvalidDataException("Truncated vehicle integer"); return BitConverter.ToInt32(b,p); }
  static int N(byte[] b,int p) { int n=I(b,p); if(n<0 || n>1000000) throw new InvalidDataException("Invalid vehicle block count"); return n; }
  static void Skip(byte[] b,ref int p,long n) { if(n<0 || n>b.Length-(long)p) throw new InvalidDataException("Truncated vehicle block"); p+=(int)n; }
  static void Field(byte[] b,int p,int width,List<ModelField> fields) {
   if(p<0 || (long)p+width>b.Length) throw new InvalidDataException("Truncated model field");
   int end=Array.IndexOf(b,(byte)0,p,width); if(end<0) throw new InvalidDataException("Unterminated model field");
   fields.Add(new ModelField {Offset=p,Width=width,Value=Encoding.ASCII.GetString(b,p,end-p)});
  }
  public static List<ModelField> Parse(byte[] b) {
   int total=N(b,0),p=4; var fields=new List<ModelField>();
   for(int i=0;i<total;i++) {
    int s=p; Skip(b,ref p,2024); if(I(b,s)!=i) throw new InvalidDataException("Vehicle IDs/format not supported");
    int version=N(b,s+0x7e4); if(version>1) throw new InvalidDataException("Unsupported vehicle extension");
    int ext=p; if(version==1) Skip(b,ref p,64);
    Field(b,s+0x728,128,fields);
    Skip(b,ref p,(long)N(b,s+8)*4+(long)N(b,s+12)*72+(long)N(b,s+0x7c0)*4+(long)N(b,s+24)*4);
    int routes=N(b,s+0xb0); for(int k=0;k<routes*2;k++) {int r=p;Skip(b,ref p,24);Skip(b,ref p,(long)N(b,r+8)*72);}
    int[] offsets={0xb4,0xc0,0xc4,0xd0,0xd4,0xa8,0x100,0x720,0x724};
    int[] sizes={4,8,8,12,12,12,8,24,24};
    for(int k=0;k<offsets.Length;k++) Skip(b,ref p,(long)N(b,s+offsets[k])*sizes[k]);
    int named=N(b,s+0x7dc); if(named>1) throw new InvalidDataException("Invalid vehicle name flag"); if(named==1) Skip(b,ref p,512);
    if(I(b,s+0x7d4)==-1) Skip(b,ref p,12);
    if(version==1) {
     Skip(b,ref p,(long)N(b,ext)*12+(long)N(b,ext+0x10)*16);
     if(b[ext+0x14]>1) throw new InvalidDataException("Invalid vehicle extension flag"); if(b[ext+0x14]==1) Skip(b,ref p,128);
     Skip(b,ref p,N(b,ext+0x24));
     int extra=N(b,ext+0x34);for(int k=0;k<extra;k++){Field(b,p,128,fields);Skip(b,ref p,128);}
    }
   }
   if(p!=b.Length) throw new InvalidDataException("Unsupported trailing vehicle data"); return fields;
  }
  public static byte[] Rewrite(byte[] input,Dictionary<string,string> map,out int changes) {
   var fields=Parse(input);byte[] output=(byte[])input.Clone();changes=0;
   foreach(var f in fields) {
    string next;
    if(!map.TryGetValue(f.Value.Replace('\\','/'),out next) || next==f.Value) continue;
    byte[] name=Encoding.ASCII.GetBytes(next);if(name.Length>=f.Width)throw new InvalidDataException("Replacement model path too long");
    Array.Clear(output,f.Offset,f.Width);Buffer.BlockCopy(name,0,output,f.Offset,name.Length);changes++;
   }
   var after=Parse(output); if(fields.Count!=after.Count || input.Length!=output.Length)throw new InvalidDataException("Vehicle structure changed");
   int cursor=0;
   for(int k=0;k<fields.Count;k++) {
    var f=fields[k];for(;cursor<f.Offset;cursor++) if(input[cursor]!=output[cursor])throw new InvalidDataException("Unexpected binary change");
    cursor=f.Offset+f.Width;string next;if(map.TryGetValue(f.Value.Replace('\\','/'),out next) && after[k].Value!=next)throw new InvalidDataException("Model validation failed");
   }
   for(;cursor<input.Length;cursor++)if(input[cursor]!=output[cursor])throw new InvalidDataException("Unexpected binary tail change");return output;
  }
 }
}
"@
}
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
function Get-FrontlineHash([byte[]]$Bytes) {
 $sha=[Security.Cryptography.SHA256]::Create()
 try { ([BitConverter]::ToString($sha.ComputeHash($Bytes))).Replace('-','').ToLowerInvariant() } finally { $sha.Dispose() }
}
function Get-FrontlineCatalog([string]$ItemDir) {
 $types=@{}; $id=''
 foreach($line in [IO.File]::ReadAllLines((Join-Path $ItemDir 'data\units.ini'))) {
  $line=($line -split ';',2)[0].Trim()
  if($line -match '^\[(.+)\]$') { $id=$Matches[1];$types[$id]=@{hp=10.0;ammo=(New-Object Collections.Generic.List[int])};continue }
  if(-not $id){continue}
  if($line -match '^hp\s*=\s*(\S+)'){$types[$id].hp=[double]::Parse($Matches[1],[Globalization.CultureInfo]::InvariantCulture)}
  if($line -match '^weapon\s*='){
   $ammo=0;if($line -match '\bammo=(\d+)'){$ammo=[int]$Matches[1]};$types[$id].ammo.Add($ammo)
  }
 }
 return $types
}
function Convert-FrontlineState([byte[]]$Bytes,$Config,$Types) {
 $utf8=New-Object Text.UTF8Encoding($false,$true)
 $text=$utf8.GetString($Bytes); $newline=if($text.Contains("`r`n")){"`r`n"}else{"`n"}
 $out=New-Object Collections.Generic.List[string];$changed=0
 $schema=[regex]::Match($text.TrimStart([char]0xfeff),'(?m)^srwar\s+(\d+)\s*$')
 if($schema.Success -and [int]$schema.Groups[1].Value -notin 1,2){throw 'Unsupported srwar.ini schema'}
 foreach($line in ($text -split '\r?\n')) {
  $v=$line.Trim().TrimStart([char]0xfeff) -split '\s+';$position=-1
  switch($v[0]) {
   'unit' {if($v.Count -lt 15 -or $v.Count -gt 37){throw 'Invalid unit record'};$position=1}
   'hq_squad' {if($v.Count -ne 8){throw 'Invalid hq_squad record'};$position=3}
   'muster' {if($v.Count -lt 4 -or $v.Count -gt 7){throw 'Invalid muster record'};$position=1}
  }
  if($position -lt 0){$out.Add($line);continue}
  # Validate every numeric token without depending on the player's Windows locale.
  for($i=1;$i -lt $v.Count;$i++){
   if($i -eq $position){continue};$number=0.0
   if(-not [double]::TryParse($v[$i],[Globalization.NumberStyles]::Float,[Globalization.CultureInfo]::InvariantCulture,[ref]$number) -or [double]::IsNaN($number) -or [double]::IsInfinity($number)){throw "Invalid numeric $($v[0]) record"}
  }
  $before=$v[$position];$alias=$Config.aliases.PSObject.Properties[$before]
  if($alias){$v[$position]=[string]$alias.Value}
  $next=$v[$position]
  if(-not $Types.ContainsKey($next)) { $changed++;continue } # Plan B: unavailable unit/roster entry is not restored.
  if($alias) {
   if($v[0] -eq 'unit') {
    $hp=[double]::Parse($v[5],[Globalization.CultureInfo]::InvariantCulture)
    $v[5]=([Math]::Min($hp,$Types[$next].hp)).ToString('0.00',[Globalization.CultureInfo]::InvariantCulture)
    for($k=0;$k -lt 4 -and 20+$k -lt $v.Count;$k++) {
     $cap=if($k -lt $Types[$next].ammo.Count){$Types[$next].ammo[$k]}else{-1}
     $old=[int]$v[20+$k]
     $v[20+$k]=if($cap -lt 0){'-1'}elseif($cap -eq 0){[string]$old}elseif($old -lt 0){[string]$cap}else{[string][Math]::Min($old,$cap)}
    }
   } elseif($v[0] -eq 'hq_squad') {
    $v[7]=([Math]::Min([double]::Parse($v[7],[Globalization.CultureInfo]::InvariantCulture),$Types[$next].hp)).ToString('0.00',[Globalization.CultureInfo]::InvariantCulture)
   }
   $updated=($v -join ' ');if($line.StartsWith([string][char]0xfeff)){$updated=([string][char]0xfeff)+$updated};$out.Add($updated);$changed++
  } else {$out.Add($line)}
 }
 $result=if($changed){$utf8.GetBytes(($out -join $newline))}else{$Bytes}
 return [pscustomobject]@{Bytes=[byte[]]$result;Changes=$changed}
}
function Convert-FrontlineSave($Files,$Config,$Types,$ModelMap) {
 $result=@{}; foreach($key in $Files.Keys){$result[$key]=$Files[$key]}
 $state=Convert-FrontlineState $Files['srwar.ini'] $Config $Types;$result['srwar.ini']=$state.Bytes
 $models=0
 if($Files.ContainsKey('vehicles.bin')) {
  foreach($field in [Frontline03.VehicleFile]::Parse($Files['vehicles.bin'])) {
   $name=$field.Value.Replace('\','/');$item=($name -split '/',2)[0]
   if($Config.retired_items -contains $item -and -not $ModelMap.ContainsKey($name)){throw "No replacement mapping for retired Workshop vehicle: $name. Saves were not changed."}
  }
  $result['vehicles.bin']=[Frontline03.VehicleFile]::Rewrite($Files['vehicles.bin'],$ModelMap,[ref]$models)}
 $changed=$state.Changes+$models
 if($changed -or -not $Files.ContainsKey('frontline_migration_0_3.json')) {
  $stamp=@{version='0.3';state_sha256=(Get-FrontlineHash $result['srwar.ini']);changes=$changed}
  if($result.ContainsKey('vehicles.bin')){$stamp['vehicles_sha256']=Get-FrontlineHash $result['vehicles.bin']}
  $result['frontline_migration_0_3.json']=[Text.Encoding]::UTF8.GetBytes(($stamp | ConvertTo-Json))
 }
 return [pscustomobject]@{Files=$result;Changes=$changed;Models=$models}
}
function Read-FrontlineZipEntry($Entry) {
 $stream=$Entry.Open();$memory=New-Object IO.MemoryStream
 try {$stream.CopyTo($memory);return ,$memory.ToArray()}finally{$memory.Dispose();$stream.Dispose()}
}
function New-FrontlineMigrationPlan([string]$GameDir,[string]$ItemDir) {
 $config=[IO.File]::ReadAllText((Join-Path $ItemDir 'data\migration_0_3.json')) | ConvertFrom-Json
 if($config.version -ne '0.3'){throw 'Invalid migration table version'}
 $types=Get-FrontlineCatalog $ItemDir
 foreach($property in $config.aliases.PSObject.Properties){if(-not $types.ContainsKey([string]$property.Value)){throw "Migration target missing: $($property.Value)"}}
 $modelMap=New-Object 'Collections.Generic.Dictionary[string,string]' ([StringComparer]::OrdinalIgnoreCase)
 foreach($property in $config.models.PSObject.Properties){$modelMap.Add($property.Name,[string]$property.Value)}
 $plan=New-Object Collections.Generic.List[object];$count=0;$binary=0
 foreach($root in 'save','save_cloud') {
  $base=Join-Path $GameDir "media_soviet\$root";if(-not(Test-Path -LiteralPath $base)){continue}
  foreach($dir in Get-ChildItem -LiteralPath $base -Directory) {
   $state=Join-Path $dir.FullName 'srwar.ini';if(-not(Test-Path -LiteralPath $state)){continue}
   if($dir.Attributes -band [IO.FileAttributes]::ReparsePoint){throw "Save junction is not supported: $dir"}
   $files=@{}
   foreach($name in 'srwar.ini','vehicles.bin','frontline_migration_0_3.json'){
    $path=Join-Path $dir.FullName $name;if(Test-Path -LiteralPath $path){$files[$name]=[IO.File]::ReadAllBytes($path)}
   }
   try{$converted=Convert-FrontlineSave $files $config $types $modelMap}catch{throw "Cannot migrate save $($dir.FullName): $_"};$binary+=$converted.Models
   foreach($name in $converted.Files.Keys){
    if($files.ContainsKey($name) -and (Get-FrontlineHash $files[$name]) -eq (Get-FrontlineHash $converted.Files[$name])){continue}
    $plan.Add([pscustomobject]@{Path=(Join-Path $dir.FullName $name);Relative="$root\$($dir.Name)\$name";Before=$files[$name];After=$converted.Files[$name];Committed=$false})
   };$count++
  }
  foreach($zip in Get-ChildItem -LiteralPath $base -File -Filter '*.zip') {
   $original=[IO.File]::ReadAllBytes($zip.FullName);$memory=New-Object IO.MemoryStream;$memory.Write($original,0,$original.Length);$memory.Position=0
   $archive=New-Object IO.Compression.ZipArchive($memory,[IO.Compression.ZipArchiveMode]::Update,$true)
   $zipChanged=$false
   try {
    $names=@{};foreach($e in $archive.Entries){if($names.ContainsKey($e.FullName)){throw "Duplicate ZIP entry in $zip"};$names[$e.FullName]=$e}
    foreach($stateName in @($names.Keys | Where-Object {$_ -match '(^|/)srwar\.ini$'})) {
     $prefix=$stateName.Substring(0,$stateName.Length-9);$files=@{}
     foreach($name in 'srwar.ini','vehicles.bin','frontline_migration_0_3.json'){if($names.ContainsKey($prefix+$name)){$files[$name]=Read-FrontlineZipEntry $names[$prefix+$name]}}
     try{$converted=Convert-FrontlineSave $files $config $types $modelMap}catch{throw "Cannot migrate ZIP $($zip.FullName), ${stateName}: $_"};$binary+=$converted.Models
     foreach($name in $converted.Files.Keys) {
      if($files.ContainsKey($name) -and (Get-FrontlineHash $files[$name]) -eq (Get-FrontlineHash $converted.Files[$name])){continue}
      $old=$archive.GetEntry($prefix+$name);if($old){$old.Delete()}
      $entry=$archive.CreateEntry($prefix+$name);$stream=$entry.Open();try{$bytes=$converted.Files[$name];$stream.Write($bytes,0,$bytes.Length)}finally{$stream.Dispose()};$zipChanged=$true
     };$count++
    }
   } finally {$archive.Dispose()}
   if($zipChanged){$plan.Add([pscustomobject]@{Path=$zip.FullName;Relative="$root\$($zip.Name)";Before=$original;After=$memory.ToArray();Committed=$false})};$memory.Dispose()
  }
 }
 if($binary -gt 0 -and (Get-FileHash -LiteralPath (Join-Path $GameDir 'SOVIET64.exe') -Algorithm SHA256).Hash.ToLowerInvariant() -ne $config.supported_exe_sha256){throw 'Save vehicle migration requires the supported game build (1.1.1.9). No saves were changed.'}
 Write-Host "Save migration preflight: $count Frontline saves, $($plan.Count) files to update, $binary vehicle model replacements."
 return [pscustomobject]@{Files=$plan;GameDir=$GameDir;Backup=$null;SaveCount=$count}
}
function Write-FrontlineMigrationFile($row) {
 $temp=$row.Path+'.frontline-'+[guid]::NewGuid().ToString('N')+'.tmp'
 try {
  [IO.File]::WriteAllBytes($temp,$row.After)
  if((Get-FrontlineHash ([IO.File]::ReadAllBytes($temp))) -ne (Get-FrontlineHash $row.After)){throw 'Migration temporary file validation failed'}
  if($null -ne $row.Before){[IO.File]::Replace($temp,$row.Path,[NullString]::Value)}else{[IO.File]::Move($temp,$row.Path)};$row.Committed=$true
 } finally {if(Test-Path -LiteralPath $temp){Remove-Item -LiteralPath $temp -Force}}
}
function Invoke-FrontlineMigration($Plan) {
 if(-not $Plan.Files.Count){Write-Host 'Save migration: already current.';return}
 foreach($row in $Plan.Files){
  if($null -ne $row.Before){if(-not(Test-Path -LiteralPath $row.Path) -or (Get-FrontlineHash ([IO.File]::ReadAllBytes($row.Path))) -ne (Get-FrontlineHash $row.Before)){throw "Save changed since preflight: $($row.Path)"}}
  elseif(Test-Path -LiteralPath $row.Path){throw "Save marker appeared since preflight: $($row.Path)"}
 }
 $backup=Join-Path $Plan.GameDir ('SRWar\migration_backups\0.3_'+(Get-Date -Format 'yyyyMMdd_HHmmss')+'_'+[guid]::NewGuid().ToString('N'));$Plan.Backup=$backup
 New-Item -ItemType Directory -Force -Path $backup | Out-Null
 # Back up every affected original before writing the first save. Journal covers newly created markers too.
 $journal=@()
 foreach($row in $Plan.Files){
  if($null -ne $row.Before){$to=Join-Path $backup $row.Relative;New-Item -ItemType Directory -Force (Split-Path $to) | Out-Null;[IO.File]::WriteAllBytes($to,$row.Before)}
  $journal+=@{path=$row.Path;relative=$row.Relative;existed=($null -ne $row.Before);before_sha256=$(if($null -ne $row.Before){Get-FrontlineHash $row.Before}else{$null});after_sha256=(Get-FrontlineHash $row.After)}
 }
 [IO.File]::WriteAllText((Join-Path $backup 'journal.json'),($journal | ConvertTo-Json -Depth 5),(New-Object Text.UTF8Encoding($false)))
 try {
  foreach($row in $Plan.Files){Write-FrontlineMigrationFile $row}
 } catch {
  $failure=$_
  foreach($row in $Plan.Files){if(-not $row.Committed){continue};if($null -ne $row.Before){[IO.File]::WriteAllBytes($row.Path,$row.Before)}else{Remove-Item -LiteralPath $row.Path -Force}}
  throw "Save migration rolled back: $failure. Backup: $backup"
 }
 Write-Host "Save migration complete. Backup: $backup"
}

function Restore-FrontlineMigration([string]$GameDir,[string]$BackupDir) {
 $gameFull=[IO.Path]::GetFullPath($GameDir).TrimEnd('\')+'\'
 $backupFull=[IO.Path]::GetFullPath($BackupDir).TrimEnd('\')+'\'
 if(-not $backupFull.StartsWith(($gameFull+'SRWar\migration_backups\'),[StringComparison]::OrdinalIgnoreCase)){throw 'Backup must be inside this game migration_backups folder'}
 $journal=[IO.File]::ReadAllText((Join-Path $BackupDir 'journal.json')) | ConvertFrom-Json
 foreach($entry in $journal){
  $target=[IO.Path]::GetFullPath($entry.path)
  if(-not($target.StartsWith(($gameFull+'media_soviet\save\'),[StringComparison]::OrdinalIgnoreCase) -or $target.StartsWith(($gameFull+'media_soviet\save_cloud\'),[StringComparison]::OrdinalIgnoreCase))){throw 'Invalid restore target'}
  $saved=[IO.Path]::GetFullPath((Join-Path $BackupDir $entry.relative))
  if(-not $saved.StartsWith($backupFull,[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid backup relative path'}
  if(-not(Test-Path -LiteralPath $target) -or (Get-FrontlineHash ([IO.File]::ReadAllBytes($target))) -ne $entry.after_sha256){throw "Save changed since migration; refusing to overwrite $target"}
  if($entry.existed -and (Get-FrontlineHash ([IO.File]::ReadAllBytes($saved))) -ne $entry.before_sha256){throw 'Backup checksum mismatch'}
 }
 $retirement=@()
 $retirementFile=Join-Path $BackupDir 'retired_vehicles.json'
 if(Test-Path -LiteralPath $retirementFile){$retirement=[IO.File]::ReadAllText($retirementFile) | ConvertFrom-Json}
 foreach($resource in $retirement){
  $target=[IO.Path]::GetFullPath($resource.path);$source=[IO.Path]::GetFullPath($resource.backup)
  if(-not $target.StartsWith(($gameFull+'media_soviet\vehicles\srwar_fl_'),[StringComparison]::OrdinalIgnoreCase) -or -not $source.StartsWith(($gameFull+'SRWar\install_backups\retired_fl_'),[StringComparison]::OrdinalIgnoreCase)){throw 'Invalid retired vehicle restore path'}
  if(Test-Path -LiteralPath $target){throw 'Retired vehicle destination already exists; refusing overwrite'}
  foreach($hash in $resource.hashes.PSObject.Properties){$asset=[IO.Path]::GetFullPath((Join-Path $source $hash.Name));if(-not $asset.StartsWith($source.TrimEnd('\')+'\',[StringComparison]::OrdinalIgnoreCase) -or (Get-FileHash -LiteralPath $asset -Algorithm SHA256).Hash -ne $hash.Value){throw 'Retired vehicle backup checksum mismatch'}}
 }
 foreach($entry in $journal){if($entry.existed){[IO.File]::WriteAllBytes($entry.path,[IO.File]::ReadAllBytes((Join-Path $BackupDir $entry.relative)))}else{Remove-Item -LiteralPath $entry.path -Force}}
 foreach($resource in $retirement){Copy-Item -LiteralPath $resource.backup -Destination $resource.path -Recurse}
 Write-Host 'Save migration backup restored.'
}
function Complete-FrontlineRetiredVehicles([string]$GameDir,[string]$ItemDir,$MigrationPlan=$null) {
 $retirement=New-Object Collections.Generic.List[object]
 $config=[IO.File]::ReadAllText((Join-Path $ItemDir 'data\migration_0_3.json')) | ConvertFrom-Json
 $remaining=New-Object 'Collections.Generic.HashSet[string]' ([StringComparer]::OrdinalIgnoreCase)
 $scanComplete=$true
 foreach($root in 'save','save_cloud') {
  $base=Join-Path $GameDir "media_soviet\$root";if(-not(Test-Path -LiteralPath $base)){continue}
  foreach($file in Get-ChildItem -LiteralPath $base -Recurse -File -Filter 'vehicles.bin') {
   try{foreach($field in [Frontline03.VehicleFile]::Parse([IO.File]::ReadAllBytes($file.FullName))){[void]$remaining.Add($field.Value)}}catch{$scanComplete=$false}
  }
  foreach($file in Get-ChildItem -LiteralPath $base -File -Filter '*.zip') {
   $archive=$null
   try{
    $archive=[IO.Compression.ZipFile]::OpenRead($file.FullName)
    foreach($entry in $archive.Entries){if($entry.FullName -match '(^|/)vehicles\.bin$'){foreach($field in [Frontline03.VehicleFile]::Parse((Read-FrontlineZipEntry $entry))){[void]$remaining.Add($field.Value)}}}
   }catch{$scanComplete=$false}finally{if($archive){$archive.Dispose()}}
  }
 }
 $vehicleRoot=[IO.Path]::GetFullPath((Join-Path $GameDir 'media_soviet\vehicles')).TrimEnd('\\')+'\'
 $backupRoot=[IO.Path]::GetFullPath((Join-Path $GameDir ('SRWar\install_backups\retired_fl_'+[guid]::NewGuid().ToString('N')))).TrimEnd('\\')+'\'
 foreach($id in $config.retired_fl) {
  if($id -notmatch '^[A-Za-z0-9_]+$'){throw 'Invalid retired FL ID'}
  $folder=[IO.Path]::GetFullPath((Join-Path $vehicleRoot ('srwar_fl_'+$id)))
  if(-not $folder.StartsWith($vehicleRoot,[StringComparison]::OrdinalIgnoreCase)){throw 'Unsafe retired vehicle path'}
  if(-not(Test-Path -LiteralPath $folder -PathType Container)){continue}
  if((Get-Item -LiteralPath $folder).Attributes -band [IO.FileAttributes]::ReparsePoint){throw 'Retired vehicle folder is a junction; not moved'}
  if($scanComplete -and -not $remaining.Contains('srwar_fl_'+$id) -and -not $remaining.Contains('vehicles/srwar_fl_'+$id)) {
   $to=[IO.Path]::GetFullPath((Join-Path $backupRoot ('srwar_fl_'+$id)))
   if(-not $to.StartsWith($backupRoot,[StringComparison]::OrdinalIgnoreCase)){throw 'Unsafe retired vehicle backup path'}
   New-Item -ItemType Directory -Force -Path $backupRoot | Out-Null
   $hashes=@{};foreach($asset in Get-ChildItem -LiteralPath $folder -Recurse -File){$hashes[$asset.FullName.Substring($folder.Length+1)]=(Get-FileHash -LiteralPath $asset.FullName -Algorithm SHA256).Hash}
   Move-Item -LiteralPath $folder -Destination $to
   $retirement.Add(@{kind="folder";path=$folder;backup=$to;hashes=$hashes})
   Write-Host "Retired [FL] vehicle archived: $id"
  } else {
   # Other/non-Frontline saves may still reference this type: retain its resources, hide new purchases.
   $script=Join-Path $folder 'script.ini'
   if(Test-Path -LiteralPath $script){
    $text=[IO.File]::ReadAllText($script);$next=[regex]::Replace($text,'(?m)^\$AVAILABLE[^\r\n]*','$AVAILABLE 0 0')
    if($next -ne $text){New-Item -ItemType Directory -Force -Path $backupRoot | Out-Null;Copy-Item -LiteralPath $script -Destination (Join-Path $backupRoot ($id+'_script.ini'));[IO.File]::WriteAllText($script,$next,(New-Object Text.UTF8Encoding($false)))}
   }
   Write-Host "Retired [FL] resources retained for other/unverified saves: $id"
  }
 }

 if($MigrationPlan -and $MigrationPlan.Backup -and $retirement.Count){[IO.File]::WriteAllText((Join-Path $MigrationPlan.Backup 'retired_vehicles.json'),($retirement.ToArray() | ConvertTo-Json -Depth 8),(New-Object Text.UTF8Encoding($false)))}
}
