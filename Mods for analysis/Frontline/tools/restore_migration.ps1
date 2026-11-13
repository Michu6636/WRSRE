param([Parameter(Mandatory=$true)][string]$GameDir,[Parameter(Mandatory=$true)][string]$BackupDir)
$ErrorActionPreference='Stop'
if(Get-Process SOVIET64 -ErrorAction SilentlyContinue){throw 'Close the game before restoring saves'}
. (Join-Path $PSScriptRoot 'migration.ps1')
Restore-FrontlineMigration -GameDir $GameDir -BackupDir $BackupDir
