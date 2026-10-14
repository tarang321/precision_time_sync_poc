$ErrorActionPreference = "Continue"
$log = Join-Path $PSScriptRoot "_build_flash.log"
$cmake = "C:\Users\tasrivas\.silabs\slt\installs\conan\p\cmakefa35ab0687064\p\bin\cmake.exe"
$buildDir = Join-Path $PSScriptRoot "cmake_gcc\build"
$commander = "C:\Users\tasrivas\.silabs\slt\installs\archive\Simplicity Commander\commander.exe"
$rps = Join-Path $buildDir "base\pts_master_si917_p6.rps"

"=== BUILD ===" | Out-File -FilePath $log -Encoding utf8
& $cmake --build $buildDir --parallel 8 *>&1 | Tee-Object -FilePath $log -Append
$buildExit = $LASTEXITCODE
"EXIT_CODE=$buildExit" | Tee-Object -FilePath $log -Append
if ($buildExit -ne 0) { exit $buildExit }

"=== FLASH ===" | Tee-Object -FilePath $log -Append
& $commander rps load $rps -s 440267921 --device SiWG917 *>&1 | Tee-Object -FilePath $log -Append
$flashExit = $LASTEXITCODE
"EXIT_CODE=$flashExit" | Tee-Object -FilePath $log -Append
exit $flashExit
