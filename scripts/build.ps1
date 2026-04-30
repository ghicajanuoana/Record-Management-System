param(
    [string]$ZigVersion = "0.13.0"
)

$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$toolsDir = Join-Path $projectRoot "tools"
$zigFolder = Join-Path $toolsDir "zig-windows-x86_64-$ZigVersion"
$zigExe = Join-Path $zigFolder "zig.exe"
$zipPath = Join-Path $toolsDir "zig.zip"
$buildDir = Join-Path $projectRoot "build"

if (-not (Test-Path $zigExe)) {
    New-Item -ItemType Directory -Force $toolsDir | Out-Null
    $downloadUrl = "https://ziglang.org/download/$ZigVersion/zig-windows-x86_64-$ZigVersion.zip"
    Invoke-WebRequest -Uri $downloadUrl -OutFile $zipPath
    Expand-Archive -Path $zipPath -DestinationPath $toolsDir -Force
}

New-Item -ItemType Directory -Force $buildDir | Out-Null

& $zigExe c++ `
    -std=c++17 `
    -Wall `
    -Wextra `
    -Wpedantic `
    -I (Join-Path $projectRoot "include") `
    (Join-Path $projectRoot "src\\main.cpp") `
    (Join-Path $projectRoot "src\\models\\Book.cpp") `
    (Join-Path $projectRoot "src\\repositories\\BookRepository.cpp") `
    (Join-Path $projectRoot "src\\services\\BookService.cpp") `
    (Join-Path $projectRoot "src\\ui\\Menu.cpp") `
    -o (Join-Path $buildDir "record_manager.exe")
