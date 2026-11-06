# rythme for a web browser (WebAssembly): src\client\CMakeLists.txt with Emscripten, against
# kazilib's SFML web build (D:\code\tools\kazilib-web, shared by every project; made
# by src\dep\kazilib\web\build_deps.ps1 when missing).
# Output src\build\web\rythme-build\ index.html / .js / .wasm: open it with
#   node src\dep\kazilib\web\tools\browser.js --serve src\build\web\rythme-build
#   powershell -ExecutionPolicy Bypass -File build_web.ps1 [-Deps] [-Debug] [-Jobs <n>]
# -Deps   rebuild SFML for the web first (after a kazilib web patch change)
# -Debug  function names in stack traces and Emscripten's memory checks
param([switch]$Deps, [switch]$Debug, [int]$Jobs = 0)
$ErrorActionPreference = 'Stop'
$root = $PSScriptRoot
$web = Join-Path $root 'src\build\web'
$emsdk = 'D:\code\tools\emsdk'
$depsOut = 'D:\code\tools\kazilib-web'
$sfmlDir = Join-Path $depsOut 'sfml-web\lib\cmake\SFML'

if ($Deps -or -not (Test-Path (Join-Path $sfmlDir 'SFMLConfig.cmake'))) {
	& (Join-Path $root 'src\dep\kazilib\web\build_deps.ps1') -Out $depsOut -Emsdk $emsdk
	if ($LASTEXITCODE) { throw 'web dependencies failed' }
}

$env:EMSDK_QUIET = 1
. (Join-Path $emsdk 'emsdk_env.ps1') | Out-Null
$ninja = (Get-Command ninja -ErrorAction SilentlyContinue)
$ninja = if ($ninja) { $ninja.Source } else {
	Get-ChildItem 'C:\Program Files\Microsoft Visual Studio' -Recurse -Filter ninja.exe -ErrorAction SilentlyContinue | Select-Object -First 1 -ExpandProperty FullName
}
if (-not $ninja) { throw 'ninja.exe not found (PATH or Visual Studio)' }

$build = Join-Path $web 'rythme-build'
$linkFlags = if ($Debug) { '--profiling-funcs -sASSERTIONS=2 -sSAFE_HEAP=1' } else { '' }
emcmake cmake -S (Join-Path $root 'src\client') -B $build -G Ninja "-DCMAKE_MAKE_PROGRAM=$ninja" -DCMAKE_BUILD_TYPE=Release `
	-DSFML_STATIC_LIBRARIES=ON "-DSFML_DIR=$($sfmlDir -replace '\\', '/')" "-DCMAKE_EXE_LINKER_FLAGS=$linkFlags" | Select-String 'Error'
if ($Jobs -le 0) { $Jobs = [Math]::Max(1, [Environment]::ProcessorCount - 1) }
cmake --build $build -j $Jobs
if ($LASTEXITCODE) { throw 'web build failed' }
Write-Host "Web build: $build"
