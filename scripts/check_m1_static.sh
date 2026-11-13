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

echo "=== 头文件语法检查 (board.h / board_config.h) ==="
gcc -Wall -Wextra -Werror -std=c11 -Inuttx/boards/arm/gd32h73x_75x/gd32h757-fc-v1/include \
  -x c -fsyntax-only - <<< '#include "board.h"'
gcc -Wall -Wextra -Werror -std=c11 -Ipx4/boards/gd32/h757-fc-v1/src \
  -x c -fsyntax-only - <<< '#include "board_config.h"'
echo "board.h & board_config.h syntax check PASS"

echo "=== JSON 元数据与引脚表检查 ==="
python3 - <<'PY'
import csv, json

with open("px4/boards/gd32/h757-fc-v1/firmware.prototype", "r", encoding="utf-8") as f:
    meta = json.load(f)
assert meta["magic"] == "PX4FWv1"
assert isinstance(meta["board_id"], int)
print(f"firmware.prototype JSON PASS (board_id={meta['board_id']}, summary={meta['summary']})")

with open("docs/hardware/pin-matrix.csv", "r", encoding="utf-8") as f:
    rows = list(csv.DictReader(f))
print(f"pin-matrix.csv loaded: {len(rows)} functional domains")
PY
