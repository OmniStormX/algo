@echo off
chcp 65001 >nul

set WSL_DISTRO=archlinux
set DEFAULT_TARGET=/root/kubenetes

set TARGET_DIR=%DEFAULT_TARGET%
if not "%~1"=="" set TARGET_DIR=%~1

powershell -NoProfile -Command ^
  "$src = '%~dp0'.TrimEnd('\');" ^
  "$wslSrc = '/mnt/' + $src[0].ToString().ToLower() + $src.Substring(2).Replace('\','/');" ^
  "Write-Host '============================================';" ^
  "Write-Host '  同步 kubenetes 到 WSL Arch Linux';" ^
  "Write-Host '============================================';" ^
  "Write-Host ('  源:   ' + $src);" ^
  "Write-Host ('  目标: ' + '%TARGET_DIR%');" ^
  "Write-Host '';" ^
  "Write-Host '[1/3] 检查 WSL ...';" ^
  "wsl -d %WSL_DISTRO% -e echo '    Arch Linux 已就绪';" ^
  "if ($LASTEXITCODE -ne 0) { Write-Host '[错误] WSL 未运行'; pause; exit 1 };" ^
  "Write-Host '[2/3] 同步中 ...';" ^
  "$cmd = 'rm -rf ''%TARGET_DIR%'' && mkdir -p ''%TARGET_DIR%'' && cp -r ''' + $wslSrc + '/*'' ''%TARGET_DIR%/''';" ^
  "wsl -d %WSL_DISTRO% -e bash -c $cmd;" ^
  "Write-Host '[3/3] 验证 ...';" ^
  "Write-Host '';" ^
  "wsl -d %WSL_DISTRO% -e bash -c ('ls -laR ''%TARGET_DIR%/''');" ^
  "Write-Host '';" ^
  "Write-Host '============================================';" ^
  "Write-Host ('  完成! 已同步到 %TARGET_DIR%');" ^
  "Write-Host '============================================'"

pause
