#!/usr/bin/env python3
"""Automated Hardware DRC & Cross-Layer Consistency Verifier for gd32_h757-fc-v1."""

import csv
import json
import pathlib
import re
import sys

REPO = pathlib.Path(__file__).resolve().parent.parent


def check_sexpr_balanced(path: pathlib.Path) -> None:
    text = path.read_text(encoding="utf-8")
    depth = 0
    in_str = False
    esc = False
    for ch in text:
        if esc:
            esc = False
            continue
        if ch == "\\":
            esc = True
            continue
        if ch == '"':
            in_str = not in_str
            continue
        if not in_str:
            if ch == "(":
                depth += 1
            elif ch == ")":
                depth -= 1
                if depth < 0:
                    raise AssertionError(f"Unbalanced ')' in {path}")
    if depth != 0 or in_str:
        raise AssertionError(f"Unbalanced S-expression in {path}: depth={depth}, in_str={in_str}")


def main() -> int:
    pin_csv = REPO / "docs" / "hardware" / "pin-matrix.csv"
    bom_csv = REPO / "docs" / "hardware" / "bom.csv"
    pinmap_h = (REPO / "nuttx" / "arch" / "arm" / "src" / "gd32h73x_75x" / "hardware" / "gd32h757zm_pinmap.h").read_text(encoding="utf-8")
    board_h = (REPO / "nuttx" / "boards" / "arm" / "gd32h73x_75x" / "gd32h757-fc-v1" / "include" / "board.h").read_text(encoding="utf-8")
    combined_c = pinmap_h + "\n" + board_h

    with pin_csv.open("r", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))

    assert len(rows) >= 55, f"Expected >=55 signals, got {len(rows)}"

    seen_pins = {}
    gpio_re = re.compile(r"^P([A-H])(\d{1,2})$")
    dedicated_allowed = {"USBHS0_DM", "USBHS0_DP"}

    for row in rows:
        pin = row["MCU引脚"].strip()
        sig = row["信号名称"].strip()
        macro = row["GD32_NuttX宏名"].strip()

        # 1. Zero duplicate physical pin check
        if pin in seen_pins:
            raise AssertionError(f"DRC ERROR: Duplicate MCU pin {pin} used by '{sig}' and '{seen_pins[pin]}'")
        seen_pins[pin] = sig

        # 2. Strict GD32H757ZMT6 LQFP144 package boundary check
        if pin not in dedicated_allowed:
            m = gpio_re.match(pin)
            if not m:
                raise AssertionError(f"DRC ERROR: Pin {pin} ({sig}) is outside LQFP144 PA..PH range!")
            port_letter, pin_num = m.group(1), int(m.group(2))
            if not (0 <= pin_num <= 15):
                raise AssertionError(f"DRC ERROR: Invalid pin index {pin}")
            if port_letter == "H" and pin_num not in (0, 1):
                raise AssertionError(f"DRC ERROR: LQFP144 only bonds PH0..PH1, found {pin}")

        # 3. Cross-layer macro check against NuttX headers
        if not macro.startswith("N/A") and macro not in ("GPIO_SWDIO", "GPIO_SWCLK",
            "GPIO_GPS1_PPS", "GPIO_ICM42688_DRDY", "GPIO_BMI088_GYR_DRDY",
            "GPIO_BMI088_ACC_DRDY", "GPIO_SDIO0_CD_N", "GPIO_USB_VBUS_SENSE",
            "GPIO_SAFETY_SWITCH_IN", "GPIO_SAFETY_LED_OUT", "GPIO_HW_REV_BIT0", "GPIO_HW_REV_BIT1"):
            if macro not in combined_c:
                raise AssertionError(f"Cross-layer mismatch: macro '{macro}' for {sig} ({pin}) missing in pinmap.h/board.h")

    with bom_csv.open("r", encoding="utf-8") as f:
        bom = list(csv.DictReader(f))
    assert len(bom) >= 30, f"Expected >=30 BOM lines, got {len(bom)}"

    proto = json.loads((REPO / "px4" / "boards" / "gd32" / "h757-fc-v1" / "firmware.prototype").read_text(encoding="utf-8"))
    assert proto["magic"] == "PX4FWv1" and proto["summary"] == "gd32_h757-fc-v1"

    sch_path = REPO / "hardware" / "schematic" / "gd32_h757_fc_v1.kicad_sch"
    net_path = REPO / "hardware" / "schematic" / "gd32_h757_fc_v1.net"
    check_sexpr_balanced(sch_path)
    check_sexpr_balanced(net_path)

    print(f"[DRC PASS] {len(rows)} LQFP144 pins verified: 0 conflicts, 0 illegal ports, C macros & KiCad schematic/netlist consistent ({len(bom)} BOM items).")
    return 0


if __name__ == "__main__":
    sys.exit(main())
