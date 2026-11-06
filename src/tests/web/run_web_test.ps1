# rythme end to end in a headless browser (kazilib web\tools\browser.js: Edge, WebGL2 on
# SwiftShader): one fret, three held together and released one by one, all ten at once.
# Screenshots in testerino\web\<local | live>\ (read them: the steps check what the
# game logged, not what it drew). Exit code = failed steps.
#   powershell -ExecutionPolicy Bypass -File src\tests\web\run_web_test.ps1 [-Url https://kazin.dev/projects/rythme/]
# -Url  a deployed page instead of the local build (src\build\web\rythme-build, build_web.ps1)
param([string]$Url = '')
$ErrorActionPreference = 'Stop'
$root = Resolve-Path (Join-Path $PSScriptRoot '..\..\..')
$shots = Join-Path $root ('testerino\web\' + $(if ($Url) { 'live' } else { 'local' }))
New-Item -ItemType Directory -Force $shots | Out-Null
$browser = Join-Path $root 'src\dep\kazilib\web\tools\browser.js'
$build = Join-Path $root 'src\build\web\rythme-build'
if (-not $Url -and -not (Test-Path (Join-Path $build 'index.wasm'))) { throw 'no src\build\web\rythme-build\index.wasm: run build_web.ps1' }

# Screenshot names in the steps land in the shots folder
$steps = Join-Path $shots 'play.json'
(Get-Content (Join-Path $PSScriptRoot 'play.json') -Raw) -replace '"shot": "', ('"shot": "' + ($shots -replace '\\', '/') + '/') |
	Set-Content -Encoding utf8 $steps

if ($Url) {
	node $browser --url $Url --steps $steps --timeout 60
} else {
	node $browser --serve $build --port 8095 --steps $steps --timeout 60
}
exit $LASTEXITCODE
