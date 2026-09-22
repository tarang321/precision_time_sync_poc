# Regenerate + rebuild all PTS cmake_gcc projects against SiSDK 2026.6.1 / WiSeConnect 4.1.1
$ErrorActionPreference = "Continue"
$root = "C:\Users\tasrivas\SimplicityStudio\v6_workspace\precision_time_sync_poc"
$sdk  = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\simpleca33d691c539\p"
$wc   = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\wiseca1ca655dfba28\p"
$slc  = "C:\Users\tasrivas\.silabs\slt\installs\archive\slc-cli-v6.0.23\slc_cli\slc.bat"
$cmake = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\cmakefa35ab0687064\p\bin\cmake.exe"
$ninjaDir = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\ninja1a38fc85adcf7\p"
$gccBin = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\gcc-a999d2e027337f\p\bin"
$env:PATH = "$gccBin;$ninjaDir;$(Split-Path $cmake);$env:PATH"
$env:ARM_GCC_DIR = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\gcc-a999d2e027337f\p"
$env:NINJA_EXE_PATH = Join-Path $ninjaDir "ninja.exe"

$log = Join-Path $root "_rebuild_all_sdk411.log"
"" | Set-Content $log

function Log($msg) {
  $line = "[{0}] {1}" -f (Get-Date -Format "HH:mm:ss"), $msg
  Write-Host $line
  Add-Content $log $line
}

$projects = Get-ChildItem $root -Recurse -Filter "*.slcp" | Where-Object {
  $_.FullName -match "phase_0[1-6]"
} | Sort-Object FullName

Log "Found $($projects.Count) .slcp projects"

$genFail = @()
$buildFail = @()
$buildOk = @()

foreach ($p in $projects) {
  $isBg24 = $p.DirectoryName -match "bg24"
  $name = $p.Name
  Log "GENERATE $name ($($p.DirectoryName))"

  if ($isBg24) {
    $args = @("generate", "-p", $p.FullName, "-o", "cmake", "-tlcn", "gcc",
              "--sdk-package-path=$sdk", "--tt", "--force", "--generator-timeout=180")
  } else {
    $args = @("generate", "-p", $p.FullName, "-o", "cmake", "-tlcn", "gcc",
              "--sdk-package-path=$sdk,$wc", "--tt", "--force", "--generator-timeout=180")
  }

  & $slc @args 2>&1 | Tee-Object -FilePath $log -Append | Out-Null
  if ($LASTEXITCODE -ne 0) {
    Log "GENERATE FAIL $name exit=$LASTEXITCODE"
    $genFail += $name
    continue
  }
  Log "GENERATE OK $name"

  $cmakeDir = Join-Path $p.DirectoryName "cmake_gcc"
  if (-not (Test-Path (Join-Path $cmakeDir "CMakePresets.json"))) {
    Log "BUILD SKIP $name (no cmake_gcc/CMakePresets.json)"
    continue
  }

  # Clean prior config so new SDK paths are picked up
  $buildDir = Join-Path $cmakeDir "build"
  if (Test-Path $buildDir) {
    Remove-Item -Recurse -Force $buildDir -ErrorAction SilentlyContinue
  }

  Log "BUILD $name"
  Push-Location $cmakeDir
  try {
    & $cmake --workflow --preset project 2>&1 | Tee-Object -FilePath $log -Append | Out-Null
    if ($LASTEXITCODE -ne 0) {
      Log "BUILD FAIL $name exit=$LASTEXITCODE"
      $buildFail += $name
    } else {
      Log "BUILD OK $name"
      $buildOk += $name
    }
  } finally {
    Pop-Location
  }
}

Log "======== SUMMARY ========"
Log "GENERATE FAIL: $($genFail.Count) $($genFail -join ', ')"
Log "BUILD OK: $($buildOk.Count)"
$buildOk | ForEach-Object { Log "  OK $_" }
Log "BUILD FAIL: $($buildFail.Count)"
$buildFail | ForEach-Object { Log "  FAIL $_" }

if ($genFail.Count -gt 0 -or $buildFail.Count -gt 0) { exit 1 } else { exit 0 }
