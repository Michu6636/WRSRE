# Raw PE Data Read — RVA 0x90B07C and RVA 0x8F3E98

**Target:** SOVIET64.exe v1.1.1.9, PE 0x6A3EB6AD, base 0x140000000 (file), ASLR ON
**SHA-256:** 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
**Generated:** 2026-10-30 via PowerShell direct file read

---

## 1. Command Source


```powershell
$p = "C:\Workers Resources Soviet Republic\SOVIET64.exe"
$data = [System.IO.File]::ReadAllBytes($p)
$hash = Get-FileHash -Path $p -Algorithm SHA256
# .rdata section: VA=0x86C000, RO=0x86B000
$rva1 = 0x90B07C
$fo1 = 0x86B000 + ($rva1 - 0x86C000)
$b1 = $data[$fo1..($fo1+3)]
[BitConverter]::ToSingle($b1, 0)
$rva2 = 0x8F3E98
$fo2 = 0x86B000 + ($rva2 - 0x86C000)
# read UTF-16LE through terminator
```

## 2. Executable Identity

| Field | Value |
|-------|-------|
| Path | C:\Workers Resources Soviet Republic\SOVIET64.exe |
| SHA-256 | 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8 |
| Image base | 0x140000000 |
| PE timestamp | 0x6A3EB6AD |
| Machine | 0x8664 (AMD64) |

## 3. Section Mapping

| Property | Value |
|----------|-------|
| RVA | 0x90B07C |
| Section | .rdata |
| Section VA range | 0x86C000 – 0x991C62 |
| Section raw offset | 0x86B000 |
| File offset | 0x86B000 + (0x90B07C − 0x86C000) = 0x90A07C |

## 4. Four Bytes at RVA 0x90B07C

| Format | Value |
|--------|-------|
| Raw bytes (LE) | 00-00-7A-44 |
| As uint32 LE | 0x447A0000 |
| As IEEE-754 float32 LE | 1000.0 |

## 5. UTF-16LE String at RVA 0x8F3E98

| Property | Value |
|----------|-------|
| RVA | 0x8F3E98 |
| File offset | 0x86B000 + (0x8F3E98 − 0x86C000) = 0x8F2E98 |
| Decoded string | "%.2fl" |
| Terminator | 00-00 |

## 6. Actual Command Output

### 6.1 SHA-256

```text
SHA256: 296644A9F207D609031FC2AE73FED2DCB34619A1D55A35D1C7B51965CE6841B8
```

### 6.2 Float32 at RVA 0x90B07C

```text
FO1: 0x90A07C
BYTES: 00-00-7A-44
FLOAT: 1000
```

### 6.3 UTF-16LE at RVA 0x8F3E98

```text
FO2: 0x8F2E98
STR: %.2fl
TERM: 00-00
```

