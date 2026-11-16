#!/usr/bin/env python3
"""Generate KiCad 7/8 S-expression schematic (.kicad_sch) and netlist (.net)
from the verified pin-matrix.csv and bom.csv for gd32_h757-fc-v1."""

import csv
import pathlib
import uuid

REPO = pathlib.Path(__file__).resolve().parent.parent
PIN_CSV = REPO / "docs" / "hardware" / "pin-matrix.csv"
BOM_CSV = REPO / "docs" / "hardware" / "bom.csv"
OUT_DIR = REPO / "hardware" / "schematic"


def det_uuid(seed: str) -> str:
    return str(uuid.uuid5(uuid.NAMESPACE_DNS, f"gd32-h757-fc-v1:{seed}"))


def build_netlist(pins, bom):
    lines = [
        '(export (version "E")',
        '  (design',
        '    (source "gd32_h757_fc_v1.kicad_sch")',
        '    (date "2026-11-16")',
        '    (tool "GD32-PX Schematic Generator 1.0")',
        '    (sheet (number "1") (name "/") (tstamps "/")',
        '      (title_block',
        '        (title "GD32H757ZMT6 Commercial PX4 v1.17.0 Flight Controller")',
        '        (company "GD32-PX Autonomous Hardware Architecture")',
        '        (rev "1.0")',
        '        (date "2026-11-16")',
        '      )',
        '    )',
        '  )',
        '  (components',
    ]
    for item in bom:
        ref = item["位号(Designator)"].split("..")[0]
        val = item["型号(MPN)"]
        fp = item["封装"]
        desc = item["关键参数与选型说明"].replace('"', "'")
        lines.extend([
            f'    (comp (ref "{ref}")',
            f'      (value "{val}")',
            f'      (footprint "{fp}")',
            f'      (description "{desc}")',
            '    )',
        ])
    lines.append('  )')
    lines.append('  (nets')

    power_nets = [
        ("1", "GND"),
        ("2", "VDD_5V_MAIN"),
        ("3", "VDD_5V_PERIPH"),
        ("4", "VDD_3V3_MCU"),
        ("5", "VDD_3V3_SENSORS"),
        ("6", "VCORE_0V9_LDO"),
    ]
    for code, pname in power_nets:
        lines.append(f'    (net (code "{code}") (name "{pname}")')
        lines.append(f'      (node (ref "U1") (pin "PWR_{pname}"))')
        lines.append('    )')

    for idx, row in enumerate(pins, start=10):
        sig = row["信号名称"]
        mcu_pin = row["MCU引脚"]
        domain = row["功能域"]
        lines.append(f'    (net (code "{idx}") (name "/{domain}/{sig}")')
        lines.append(f'      (node (ref "U1") (pin "{mcu_pin}"))')
        lines.append('    )')

    lines.append('  )')
    lines.append(')')
    return "\n".join(lines) + "\n"


def build_kicad_sch(pins):
    root_uuid = det_uuid("root")
    lines = [
        '(kicad_sch (version 20230121) (generator "gd32_px_architect")',
        f'  (uuid "{root_uuid}")',
        '  (paper "A3")',
        '  (title_block',
        '    (title "GD32H757ZMT6 PX4 v1.17.0 Flight Controller (gd32_h757-fc-v1)")',
        '    (date "2026-11-16")',
        '    (rev "1.0")',
        '    (company "GD32-PX Project")',
        '    (comment 1 "MCU: GD32H757ZMT6 LQFP144 @ 600MHz, Internal LDO PMU Mode (AN111 Safe)")',
        '    (comment 2 "Sensors: ICM-42688-P (SPI0) + BMI088 (SPI1) + BMP390/BMM150 (I2C0) + FM25V02A (SPI3)")',
        '    (comment 3 "Actuators/Bus: 8ch Gate-Protected PWM/DShot (TIMER0+TIMER3), Dual CAN-FD (CAN0+CAN2), SDIO0, USBHS0")',
        '  )',
    ]

    sheets = [
        ("Sheet1_Power_PMU", "sheet1_power_pmu.kicad_sch", 25.4, 30.0),
        ("Sheet2_MCU_Clock_USBHS", "sheet2_mcu_clock_usb.kicad_sch", 120.0, 30.0),
        ("Sheet3_Dual_IMU_Baro_Mag", "sheet3_sensors.kicad_sch", 215.0, 30.0),
        ("Sheet4_FRAM_MicroSD_SDIO0", "sheet4_storage.kicad_sch", 25.4, 115.0),
        ("Sheet5_8Ch_DShot_Dual_CANFD", "sheet5_actuators_can.kicad_sch", 120.0, 115.0),
        ("Sheet6_6xUART_RC_XOR_Safety", "sheet6_uarts_rc_ui.kicad_sch", 215.0, 115.0),
    ]
    for sname, sfile, sx, sy in sheets:
        suuid = det_uuid(sname)
        lines.extend([
            f'  (sheet (at {sx:.2f} {sy:.2f}) (size 78.00 62.00)',
            '    (stroke (width 0.25) (type solid))',
            '    (fill (color 245 248 252 1.0000))',
            f'    (uuid "{suuid}")',
            f'    (property "Sheetname" "{sname}" (at {sx:.2f} {sy - 1.5:.2f} 0)',
            '      (effects (font (size 1.8 1.8) bold) (justify left bottom))',
            '    )',
            f'    (property "Sheetfile" "{sfile}" (at {sx:.2f} {sy + 64.5:.2f} 0)',
            '      (effects (font (size 1.3 1.3)) (justify left top))',
            '    )',
            '  )',
        ])

    for i, row in enumerate(pins):
        sig = f"{row['信号名称']}_{row['MCU引脚']}"
        col = i // 30
        r = i % 30
        gx = 315.0 + col * 52.0
        gy = 28.0 + r * 8.0
        guuid = det_uuid(sig)
        lines.extend([
            f'  (global_label "{sig}" (shape bidirectional) (at {gx:.2f} {gy:.2f} 0)',
            '    (effects (font (size 1.27 1.27)) (justify left))',
            f'    (uuid "{guuid}")',
            '    (property "Intersheetrefs" "${INTERSHEET_REFS}" (at 0 0 0)',
            '      (effects (font (size 1.27 1.27)) hide)',
            '    )',
            '  )',
        ])

    lines.append(')')
    return "\n".join(lines) + "\n"


def main():
    OUT_DIR.mkdir(parents=True, exist_ok=True)
    with PIN_CSV.open("r", encoding="utf-8") as f:
        pins = list(csv.DictReader(f))
    with BOM_CSV.open("r", encoding="utf-8") as f:
        bom = list(csv.DictReader(f))

    net_path = OUT_DIR / "gd32_h757_fc_v1.net"
    sch_path = OUT_DIR / "gd32_h757_fc_v1.kicad_sch"
    net_path.write_text(build_netlist(pins, bom), encoding="utf-8")
    sch_path.write_text(build_kicad_sch(pins), encoding="utf-8")
    print(f"Generated {sch_path.relative_to(REPO)} and {net_path.relative_to(REPO)} ({len(pins)} signal nets, {len(bom)} BOM items).")


if __name__ == "__main__":
    main()
