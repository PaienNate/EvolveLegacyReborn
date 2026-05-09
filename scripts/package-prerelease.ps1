param(
    [Parameter(Mandatory = $true)]
    [string]$BintrustPath,

    [Parameter(Mandatory = $true)]
    [string]$OutputZip
)

$ErrorActionPreference = "Stop"

$repoRoot = Resolve-Path (Join-Path $PSScriptRoot "..")
$stagingRoot = Join-Path $repoRoot "artifacts\prerelease-staging"

if (Test-Path $stagingRoot) {
    Remove-Item $stagingRoot -Recurse -Force
}

New-Item -ItemType Directory -Path $stagingRoot | Out-Null
New-Item -ItemType Directory -Path (Join-Path $stagingRoot "EvolveCrack") | Out-Null
New-Item -ItemType Directory -Path (Join-Path $stagingRoot "certs") | Out-Null

Copy-Item $BintrustPath (Join-Path $stagingRoot "BINTRUST.dll")
Copy-Item (Join-Path $repoRoot "EvolveHooker\Dll1\EvolveLogging.ini") (Join-Path $stagingRoot "EvolveLogging.ini")
Copy-Item (Join-Path $repoRoot "packaging\EvolveLogging.internal.example.ini") (Join-Path $stagingRoot "EvolveLogging.internal.example.ini")
Copy-Item (Join-Path $repoRoot "packaging\README-prerelease.md") (Join-Path $stagingRoot "README-prerelease.md")
Copy-Item (Join-Path $repoRoot "THIRD_PARTY_NOTICES.txt") (Join-Path $stagingRoot "THIRD_PARTY_NOTICES.txt")
Copy-Item (Join-Path $repoRoot "LICENSE") (Join-Path $stagingRoot "LICENSE")
Copy-Item (Join-Path $repoRoot "EvolveServerEmulator\EvolveCrack\*") (Join-Path $stagingRoot "EvolveCrack") -Recurse
Copy-Item (Join-Path $repoRoot "EvolveServerEmulator\certs\mitmproxy-ca-cert.pem") (Join-Path $stagingRoot "certs\mitmproxy-ca-cert.pem")
Copy-Item (Join-Path $repoRoot "EvolveServerEmulator\certs\mitmproxy-ca.pem") (Join-Path $stagingRoot "certs\mitmproxy-ca.pem")

if (Test-Path $OutputZip) {
    Remove-Item $OutputZip -Force
}

Compress-Archive -Path (Join-Path $stagingRoot "*") -DestinationPath $OutputZip
