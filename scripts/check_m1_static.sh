#!/usr/bin/env bash
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_DIR"

mkdir -p build/host
echo "=== M1 主机侧静态编译与单元测试 ==="
gcc -Wall -Wextra -Werror -std=c11 \
  -Inuttx/arch/arm/src/gd32h73x_75x \
  nuttx/arch/arm/src/gd32h73x_75x/gd32h757_cache_mpu.c \
  tests/host/test_cache_mpu.c \
  -o build/host/test_cache_mpu

./build/host/test_cache_mpu

echo "=== 头文件语法检查 (board.h) ==="
gcc -Wall -Wextra -Werror -std=c11 -x c -fsyntax-only \
  nuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/include/board.h
echo "board.h syntax check PASS"
