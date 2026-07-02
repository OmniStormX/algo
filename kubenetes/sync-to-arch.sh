#!/bin/bash
# ============================================================
# sync-to-arch.sh — 将 kubenetes 文件夹同步到 WSL Arch Linux
# ============================================================
# 用法:
#   ./sync-to-arch.sh                  # 同步到默认路径 /root/kubenetes
#   ./sync-to-arch.sh /root/myproject  # 同步到指定路径
# ============================================================

set -euo pipefail

# ---- 配置 ----
WSL_DISTRO="archlinux"
DEFAULT_TARGET="/root/kubenetes"

# ---- 参数 ----
TARGET_DIR="${1:-$DEFAULT_TARGET}"

# ---- 获取脚本所在的 kubenetes 目录 ----
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
# Git Bash 路径 /d/algo/kubenetes → 转为 WSL 可识别的 /mnt/d/algo/kubenetes
WSL_SOURCE="$(echo "$SCRIPT_DIR" | sed -E 's|^/([a-zA-Z])/|/mnt/\1/|')"

# ---- 检查 WSL 是否在运行 ----
echo "==> 检查 WSL distro: $WSL_DISTRO ..."
wsl -d "$WSL_DISTRO" -e echo "    Arch Linux 已就绪"

# ---- 同步 ----
echo "==> 同步中..."
echo "    源:   $SCRIPT_DIR"
echo "    目标: $TARGET_DIR"

wsl -d "$WSL_DISTRO" -e bash -c "
    rm -rf '$TARGET_DIR'
    mkdir -p '$TARGET_DIR'
    cp -r '$WSL_SOURCE'/* '$TARGET_DIR'/
"

# ---- 验证 ----
echo "==> 完成! 目标目录内容:"
wsl -d "$WSL_DISTRO" -e bash -c "ls -laR '$TARGET_DIR/'"
echo ""
echo "==> 同步成功: $SCRIPT_DIR  →  $TARGET_DIR (archlinux)"
