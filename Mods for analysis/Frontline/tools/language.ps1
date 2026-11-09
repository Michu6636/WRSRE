# Shared selection, matching src/lang_choice.h: Russian game -> Russian; all others -> English.
# paths.ps1 must be loaded first.
$names = New-Object System.Collections.Hashtable([StringComparer]::Ordinal)
$utf8 = New-Object System.Text.UTF8Encoding($false)
$manifest = Join-Path $gameSteamapps 'appmanifest_784150.acf'
$steamLang = if (Test-Path -LiteralPath $manifest) { [regex]::Match([IO.File]::ReadAllText($manifest), '"language"\s+"(\w+)"').Groups[1].Value } else { '' }
$gameConfig = Join-Path $game 'media_soviet\config.ini'
$gameLanguage = if (Test-Path -LiteralPath $gameConfig) { [regex]::Match([IO.File]::ReadAllText($gameConfig), 'LANGUAGE2\s+([^\s]+)').Groups[1].Value } else { '' }
$gameLanguage = [IO.Path]::GetFileName($gameLanguage.Trim('"').Replace('/', '\')).ToLowerInvariant()
$english = if ($gameLanguage -and $gameLanguage -ne 'auto') { $gameLanguage -ne 'sovietrussian.btf' }
           else { $steamLang.ToLowerInvariant() -ne 'russian' }
function Read-LanguageText([string]$text) {
  [regex]::Replace($text, '\\(.)', {
    param($m)
    switch -CaseSensitive ($m.Groups[1].Value) { 'n' { "`n" } 't' { "`t" } default { $m.Groups[1].Value } }
  })
}
$langFile = Join-Path $PSScriptRoot '..\data\lang_en.txt'
if ($english) {
  if (-not (Test-Path -LiteralPath $langFile)) { throw "English translation file missing: $langFile" }
  foreach ($line in [IO.File]::ReadAllLines($langFile, $utf8)) {
    if ($line -match '^"((?:[^"\\]|\\.)*)"\s*=\s*"((?:[^"\\]|\\.)*)"\s*$') {
      $ru = Read-LanguageText $Matches[1]
      $en = Read-LanguageText $Matches[2]
      if ($en) { $names[$ru] = $en }
    }
  }
}
function Shown([string]$name) { if ($names.ContainsKey($name)) { $names[$name] } else { $name } }
