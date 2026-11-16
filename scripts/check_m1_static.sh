#!/usr/bin/env bash
set -euo pipefail

REPO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_DIR"

echo "=== [1/4] 生成 KiCad 原理图 (.kicad_sch) 与网表 (.net) ==="
python3 scripts/generate_kicad_schematic.py

echo "=== [2/4] 运行硬件 DRC 零冲突与跨层一致性检查 ==="
python3 scripts/verify_design.py

echo "=== [3/4] 编译并运行 M1/M3 主机侧 C11 单元测试 ==="
mkdir -p build/host
gcc -Wall -Wextra -Werror -std=c11 \
  -Inuttx/arch/arm/include/gd32h73x_75x \
  -Inuttx/arch/arm/src/gd32h73x_75x \
  -Inuttx/arch/arm/src/gd32h73x_75x/hardware \
  -Inuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/include \
  -Ipx4/boards/gd32/h757-fc-v1/src \
  nuttx/arch/arm/src/gd32h73x_75x/gd32h757_cache_mpu.c \
  nuttx/arch/arm/src/gd32h73x_75x/gd32h757_pmu_safety.c \
  nuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/src/gd32_boot.c \
  px4/boards/gd32/h757-fc-v1/src/timer_config.c \
  tests/host/test_cache_mpu.c \
  -o build/host/test_cache_mpu

./build/host/test_cache_mpu

echo "=== [4/4] 独立头文件严格语法检查 ==="
gcc -Wall -Wextra -Werror -std=c11 \
  -Inuttx/arch/arm/src/gd32h73x_75x/hardware \
  -Inuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/include \
  -x c -fsyntax-only - <<< '#include "board.h"'

gcc -Wall -Wextra -Werror -std=c11 \
  -Inuttx/arch/arm/include/gd32h73x_75x \
  -Inuttx/arch/arm/src/gd32h73x_75x \
  -Inuttx/arch/arm/src/gd32h73x_75x/hardware \
  -Inuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/include \
  -Ipx4/boards/gd32/h757-fc-v1/src \
  -x c -fsyntax-only - <<< '#include "board_config.h"'

echo "ALL CHECKS PASSED."
