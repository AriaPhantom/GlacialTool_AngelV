param(
    [Parameter(Mandatory = $true)]
    [string]$OutDir,
    [string]$InstalledBin = "C:\\vcpkg\\installed\\x64-windows\\bin"
)

$OutDir = $OutDir.Trim('"')
$OutDir = $OutDir.Replace('"', '')
$OutDir = $OutDir.TrimEnd('\')

$InstalledBin = $InstalledBin.Trim('"')
$InstalledBin = $InstalledBin.Replace('"', '')
$InstalledBin = $InstalledBin.TrimEnd('\')

if ($OutDir -match '\\Debug\\?$' -and $InstalledBin -match '\\\\bin$') {
    $debugBin = $InstalledBin -replace '\\\\bin$', '\\debug\\bin'
    if (Test-Path -LiteralPath $debugBin) {
        $InstalledBin = $debugBin
    }
}

$logPath = Join-Path $OutDir "vcpkg.applocal.log"
$installedRoot = Split-Path $InstalledBin -Parent

function Copy-FromLog([string]$logFile) {
    Get-Content -LiteralPath $logFile | ForEach-Object {
        $line = $_.Trim()
        if ([string]::IsNullOrWhiteSpace($line)) {
            return
        }

        if (-not (Test-Path -LiteralPath $line)) {
            return
        }

        $destPath = $null
        if ($line.StartsWith($InstalledBin, [System.StringComparison]::OrdinalIgnoreCase)) {
            $destPath = Join-Path $OutDir (Split-Path $line -Leaf)
        } elseif ($line.StartsWith($installedRoot, [System.StringComparison]::OrdinalIgnoreCase)) {
            $relative = $line.Substring($installedRoot.Length).TrimStart('\')
            $destPath = Join-Path $OutDir $relative
        } else {
            $destPath = Join-Path $OutDir (Split-Path $line -Leaf)
        }

        $destDir = Split-Path $destPath -Parent
        if (-not (Test-Path -LiteralPath $destDir)) {
            New-Item -ItemType Directory -Path $destDir -Force | Out-Null
        }

        Copy-Item -LiteralPath $line -Destination $destPath -Force
    }
}

if (Test-Path -LiteralPath $logPath) {
    Copy-FromLog $logPath
    exit 0
}

$vcpkgRoot = Split-Path (Split-Path $installedRoot -Parent) -Parent
$applocal = Join-Path $vcpkgRoot "scripts\\buildsystems\\msbuild\\applocal.ps1"
if (-not (Test-Path -LiteralPath $applocal)) {
    Write-Warning "vcpkg.applocal.log not found and applocal.ps1 is missing. Skipping DLL copy."
    exit 0
}

$exe = Get-ChildItem -Path $OutDir -Filter *.exe -File | Select-Object -First 1
if (-not $exe) {
    Write-Warning "No .exe found in $OutDir to resolve dependencies."
    exit 0
}

& $applocal -targetBinary $exe.FullName -installedDir $InstalledBin
