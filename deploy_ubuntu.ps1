# Builds the web version (build_web.ps1), sends it to the Ubuntu box with the Caddy
# snippet and installer (deploy\ubuntu\), and installs it there:
# https://kazin.dev/projects/rythme/ (static files; no service).
#   powershell -ExecutionPolicy Bypass -File deploy_ubuntu.ps1 [-Box kazin-music] [-NoBuild]
param([string]$Box = 'kazin-music', [switch]$NoBuild)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot

if (-not $NoBuild) {
	& (Join-Path $root 'build_web.ps1')
}

$stage = Join-Path $root 'src\build\upload'
Remove-Item -Recurse -Force $stage -ErrorAction SilentlyContinue
New-Item -ItemType Directory -Force (Join-Path $stage 'web') | Out-Null
Copy-Item (Join-Path $root 'src\build\web\rythme-build\index.*') (Join-Path $stage 'web')
Copy-Item (Join-Path $root 'deploy\ubuntu\*') $stage

ssh $Box 'rm -rf /tmp/rythme-upload'
scp -q -r $stage "${Box}:/tmp/rythme-upload"
if ($LASTEXITCODE) { throw 'upload failed' }
ssh $Box 'sudo bash /tmp/rythme-upload/install.sh && rm -rf /tmp/rythme-upload'
if ($LASTEXITCODE) { throw 'install failed' }
Write-Host 'Deployed: https://kazin.dev/projects/rythme/'
