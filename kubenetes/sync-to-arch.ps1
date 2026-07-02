# sync-to-arch.ps1
param(
    [string]$TargetDir = "/root/kubenetes"
)

$WslDistro = "archlinux"
$ScriptDir  = Split-Path -Parent $MyInvocation.MyCommand.Path
$Drive      = $ScriptDir[0].ToString().ToLower()
$WslSource  = "/mnt/" + $Drive + $ScriptDir.Substring(2).Replace("\", "/")

Write-Host "============================================"
Write-Host "  sync kubenetes -> WSL Arch Linux"
Write-Host "============================================"
Write-Host "  Source: $ScriptDir"
Write-Host "  Target: $TargetDir"
Write-Host ""

Write-Host "[1/3] Checking WSL ..."
wsl -d $WslDistro -e echo "    Arch Linux ready"
if ($LASTEXITCODE -ne 0) {
    Write-Host "[ERROR] WSL distro '$WslDistro' not running"
    Pause
    exit 1
}

Write-Host "[2/3] Syncing ..."
wsl -d $WslDistro -e bash -c "rm -rf ""$TargetDir"" && mkdir -p ""$TargetDir"" && cp -r ""$WslSource""/* ""$TargetDir""/"

Write-Host "[3/3] Verify ..."
Write-Host ""
wsl -d $WslDistro -e bash -c "ls -laR ""$TargetDir""/"

Write-Host ""
Write-Host "============================================"
Write-Host "  Done! Synced to $TargetDir"
Write-Host "============================================"
Pause
