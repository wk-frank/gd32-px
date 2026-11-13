#!/usr/bin/env bash
set -uo pipefail

echo "=== GD32H757 PX4/NuttX 环境核查 $(date -u +%Y-%m-%dT%H:%M:%SZ) ==="
echo "Host: $(uname -a)"
echo "PWD: $(pwd)"

missing=0
for tool in git python3 gcc g++ make cmake ninja arm-none-eabi-gcc kconfig-mconf genromfs; do
  if command -v "$tool" >/dev/null 2>&1; then
    ver=$("$tool" --version 2>&1 | head -n 1 || true)
    echo "[OK] $tool -> $(command -v "$tool") ($ver)"
  else
    echo "[MISSING] $tool"
    missing=$((missing + 1))
  fi
done

echo "--- 磁盘空间 ---"
df -h .

echo "--- 汇总 ---"
if [ "$missing" -gt 0 ]; then
  echo "RESULT: BLOCKED_MISSING_TOOLS (missing=$missing)"
  exit 2
fi
echo "RESULT: PASS"
exit 0
