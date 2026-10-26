#!/usr/bin/env python3
"""Generate the FpvDeck HAT design artifacts from one checked net model.

The generated board is a placement/routing study, not a fabrication release.
RX5808 mechanical data and the Waveshare mating envelope remain physical gates.
"""

from __future__ import annotations

import argparse
import csv
import html
import sys
import uuid
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "fpv-hat"


@dataclass(frozen=True)
class Part:
    ref: str
    value: str
    footprint: str
    mpn: str
    status: str
    pins: dict[str, str]
    note: str = ""


def part(ref: str, value: str, footprint: str, mpn: str, status: str,
         pins: dict[str, str], note: str = "") -> Part:
    return Part(ref, value, footprint, mpn, status, pins, note)


PARTS = [
    part("J1", "WAVESHARE 40PIN", "Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical",
         "M20-9982046", "REQUIRED", {"1": "+5V", "2": "+3V3", "3": "+5V", "4": "I2C_SDA",
         "5": "GND", "6": "I2C_SCL", "7": "DVP_D0", "9": "DVP_D1", "11": "DVP_D2",
         "12": "DVP_D3", "14": "DVP_D4", "15": "DVP_D5", "16": "DVP_D6", "17": "DVP_D7",
         "18": "+3V3", "20": "DVP_PCLK", "22": "DVP_HSYNC", "24": "DVP_VSYNC",
         "26": "GND", "28": "RX_DATA", "30": "RX_CLK", "31": "RX_LE_A", "32": "RX_LE_B",
         "34": "ACTIVE_RX", "35": "TVP_RESETB", "36": "BTN_UP", "37": "BTN_OK",
         "38": "BTN_DOWN", "39": "BUZZER", "40": "GND"},
         "GPIO mapping is sourced from Waveshare schematic sheet 1/2."),
    part("RXA", "RX5808 MODULE A", "FPVDeck:RX5808_PLACEHOLDER_VERIFY", "RX5808 module", "REQUIRED",
         {"1": "GND", "2": "RF_A", "3": "GND", "4": "RX_DATA", "5": "RX_LE_A", "6": "RX_CLK",
          "7": "GND", "8": "+5V", "9": "RSSI_A", "10": "NC_AUDIO_A", "11": "VIDEO_A"},
         "SPEC V1 electrical order; real module footprint/pin order must be measured."),
    part("RXB", "RX5808 MODULE B", "FPVDeck:RX5808_PLACEHOLDER_VERIFY", "RX5808 module", "DNP",
         {"1": "GND", "2": "RF_B", "3": "GND", "4": "RX_DATA", "5": "RX_LE_B", "6": "RX_CLK",
          "7": "GND", "8": "+5V", "9": "RSSI_B", "10": "NC_AUDIO_B", "11": "VIDEO_B"},
         "Optional diversity receiver; same physical verification gate as RXA."),
    part("J2", "SMA EDGE A", "Connector_Coaxial:SMA_Amphenol_132289_EdgeMount", "132289", "REQUIRED",
         {"1": "RF_A", "2": "GND"}),
    part("J3", "SMA EDGE B", "Connector_Coaxial:SMA_Amphenol_132289_EdgeMount", "132289", "DNP",
         {"1": "RF_B", "2": "GND"}),
    # LMV331: high/open output means RSSI_A > RSSI_B. The 3157 B2 input is
    # therefore VIDEO_A so ACTIVE_RX high consistently means receiver A.
    part("U1", "RSSI COMPARATOR", "Package_TO_SOT_SMD:SOT-23-5", "LMV331IDBVR", "REQUIRED",
         {"1": "ACTIVE_RX", "2": "GND", "3": "RSSI_A_HYS", "4": "RSSI_B", "5": "+3V3"}),
    part("R1", "10k 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED",
         {"1": "ACTIVE_RX", "2": "+3V3"}, "Comparator open-drain pull-up."),
    part("R2", "1M 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-071ML", "REQUIRED",
         {"1": "ACTIVE_RX", "2": "RSSI_A_HYS"}, "Comparator hysteresis feedback."),
    part("R3", "10k 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED",
         {"1": "RSSI_A", "2": "RSSI_A_HYS"}),
    part("U2", "VIDEO SWITCH", "Package_TO_SOT_SMD:SOT-23-6", "SN74LVC1G3157DBVR", "REQUIRED",
         {"1": "VIDEO_A", "2": "GND", "3": "VIDEO_B", "4": "VIDEO_SELECTED", "5": "+3V3", "6": "ACTIVE_RX"}),
    part("U3", "CVBS DECODER", "Package_QFP:LQFP-32_7x7mm_P0.8mm", "TVP5150AM1PBSR", "REQUIRED",
         {"1": "TVP_AIP1A", "2": "TVP_AIP1B", "3": "AGND", "4": "+1V8A", "5": "XTAL1", "6": "XTAL2",
          "7": "AGND", "8": "TVP_RESETB", "9": "DVP_PCLK_RAW", "10": "+3V3", "11": "DVP_D7_RAW",
          "12": "DVP_D6_RAW", "13": "DVP_D5_RAW", "14": "DVP_D4_RAW", "15": "DVP_D3_RAW",
          "16": "DVP_D2_RAW", "17": "DVP_D1_RAW", "18": "DVP_D0_RAW", "19": "GND", "20": "+1V8D",
          "21": "I2C_SCL", "22": "I2C_SDA", "23": "FID_PD", "24": "DVP_VSYNC_RAW",
          "25": "DVP_HSYNC_RAW", "26": "AVID_PD", "27": "INTREQ_PD", "28": "+3V3", "29": "REFP",
          "30": "REFM", "31": "AGND", "32": "+1V8A"}),
    part("U4", "1.8V LDO", "Package_TO_SOT_SMD:SOT-23-5", "ME6211C18M5G", "REQUIRED",
         {"1": "+5V", "2": "GND", "3": "+5V", "5": "+1V8"}),
    part("FB1", "600R@100MHz", "Inductor_SMD:L_0603_1608Metric", "BLM18AG601SN1D", "REQUIRED",
         {"1": "+1V8", "2": "+1V8A"}),
    part("FB2", "600R@100MHz", "Inductor_SMD:L_0603_1608Metric", "BLM18AG601SN1D", "REQUIRED",
         {"1": "+1V8", "2": "+1V8D"}),
    part("Y1", "14.31818MHz", "Crystal:Crystal_SMD_3225-4Pin_3.2x2.5mm", "ECS-143-20-33Q-JES-TR", "REVIEW",
         {"1": "XTAL1", "2": "GND", "3": "XTAL2", "4": "GND"},
         "Verify oscillator start/load with chosen crystal before release."),
    part("J4", "BALANCE 2S-6S", "Connector_JST:JST_XH_B7B-XH-A_1x07_P2.50mm_Vertical",
         "B7B-XH-A(LF)(SN)", "REQUIRED", {"1": "GND", "2": "TAP1", "3": "TAP2", "4": "TAP3",
         "5": "TAP4", "6": "TAP5", "7": "TAP6"}, "Measurement only; no pack energy powers the Deck."),
]

for index in range(3):
    address = 0x48 + index
    addr_net = "GND" if index == 0 else ("+3V3" if index == 1 else "I2C_SDA")
    channels = (["TAP1_ADC", "TAP2_ADC", "TAP3_ADC", "TAP4_ADC"] if index == 0 else
                ["TAP5_ADC", "TAP6_ADC", "DECK_ADC", "SPARE_ADC"] if index == 1 else
                ["RSSI_A_ADC", "RSSI_B_ADC", "SPARE_RSSI2", "SPARE_RSSI3"])
    PARTS.append(part(f"U{5 + index}", f"ADS1115 0x{address:02X}", "Package_SO:VSSOP-10_3x3mm_P0.5mm",
                      "ADS1115IDGSR", "REQUIRED",
                      {"1": channels[0], "2": channels[1], "3": channels[2], "4": channels[3],
                       "5": "GND", "6": "I2C_SDA", "7": "I2C_SCL", "8": "+3V3", "9": addr_net,
                       "10": f"ADC{index + 1}_ALERT"}))

for index in range(1, 7):
    PARTS.extend([
        part(f"RT{index}", "100k 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-07100KL", "REQUIRED",
             {"1": f"TAP{index}", "2": f"TAP{index}_ADC"}),
        part(f"RB{index}", "12k 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0712KL", "REQUIRED",
             {"1": f"TAP{index}_ADC", "2": "GND"}),
        part(f"CT{index}", "10n 50V C0G", "Capacitor_SMD:C_0603_1608Metric", "GRM1885C1H103JA01D", "REQUIRED",
             {"1": f"TAP{index}_ADC", "2": "GND"}),
        part(f"TP{index}", f"TAP{index}", "TestPoint:TestPoint_Plated_Hole_D2.0mm", "TEST POINT", "REQUIRED",
             {"1": f"TAP{index}"}),
    ])

for index, raw, out in [(0, "DVP_D0_RAW", "DVP_D0"), (1, "DVP_D1_RAW", "DVP_D1"),
                         (2, "DVP_D2_RAW", "DVP_D2"), (3, "DVP_D3_RAW", "DVP_D3"),
                         (4, "DVP_D4_RAW", "DVP_D4"), (5, "DVP_D5_RAW", "DVP_D5"),
                         (6, "DVP_D6_RAW", "DVP_D6"), (7, "DVP_D7_RAW", "DVP_D7"),
                         (8, "DVP_PCLK_RAW", "DVP_PCLK"), (9, "DVP_HSYNC_RAW", "DVP_HSYNC"),
                         (10, "DVP_VSYNC_RAW", "DVP_VSYNC")]:
    PARTS.append(part(f"RD{index}", "33R 1%", "Resistor_SMD:R_0402_1005Metric", "RC0402FR-0733RL", "REQUIRED",
                      {"1": raw, "2": out}))

PARTS.extend([
    part("R20", "37.4R 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0737R4L", "REQUIRED",
         {"1": "VIDEO_SELECTED", "2": "VIDEO_TERM"}),
    part("R21", "37.4R 1%", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0737R4L", "REQUIRED",
         {"1": "VIDEO_TERM", "2": "GND"}),
    part("C20", "100n", "Capacitor_SMD:C_0603_1608Metric", "GRM188R71H104KA93D", "REQUIRED",
         {"1": "VIDEO_TERM", "2": "TVP_AIP1A"}),
    part("C21", "100n", "Capacitor_SMD:C_0603_1608Metric", "GRM188R71H104KA93D", "REQUIRED",
         {"1": "TVP_AIP1B", "2": "GND"}),
    part("C22", "1u", "Capacitor_SMD:C_0603_1608Metric", "GRM188R61A105KA61D", "REQUIRED",
         {"1": "REFP", "2": "GND"}),
    part("C23", "1u", "Capacitor_SMD:C_0603_1608Metric", "GRM188R61A105KA61D", "REQUIRED",
         {"1": "REFM", "2": "GND"}),
    part("C24", "1u", "Capacitor_SMD:C_0603_1608Metric", "GRM188R61A105KA61D", "REQUIRED",
         {"1": "REFP", "2": "REFM"}),
    part("R22", "10k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED", {"1": "FID_PD", "2": "GND"}),
    part("R23", "10k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED", {"1": "AVID_PD", "2": "GND"}),
    part("R24", "10k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED", {"1": "INTREQ_PD", "2": "GND"}),
    part("R25", "10k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0710KL", "REQUIRED", {"1": "DVP_D7_RAW", "2": "GND"}, "I2CSEL low at reset."),
    part("R26", "100k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-07100KL", "DNP", {"1": "DECK_CELL", "2": "DECK_ADC"},
         "Raw host BAT is not on the 40-pin header; do not fit or wire without review."),
    part("R27", "100k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-07100KL", "DNP", {"1": "DECK_ADC", "2": "GND"},
         "Reserved ADS channel; host GPIO20 already senses BAT through 200k/100k."),
    part("Q1", "BUZZER DRIVER", "Package_TO_SOT_SMD:SOT-23", "MMBT3904LT1G", "REQUIRED",
         {"1": "BUZZER_BASE", "2": "GND", "3": "BUZZER_LOW"}),
    part("R28", "2.2k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-072K2L", "REQUIRED", {"1": "BUZZER", "2": "BUZZER_BASE"}),
    part("BZ1", "MAGNETIC BUZZER", "Buzzer_Beeper:Buzzer_12x9.5RM7.6", "TMB12A05", "REVIEW",
         {"1": "+5V", "2": "BUZZER_LOW"}, "Footprint and current require final chosen buzzer verification."),
    part("SW1", "UP", "Button_Switch_SMD:SW_SPST_TL3301AN", "TL3301AF160QG", "REQUIRED", {"1": "BTN_UP", "2": "GND"}),
    part("SW2", "OK", "Button_Switch_SMD:SW_SPST_TL3301AN", "TL3301AF160QG", "REQUIRED", {"1": "BTN_OK", "2": "GND"}),
    part("SW3", "DOWN", "Button_Switch_SMD:SW_SPST_TL3301AN", "TL3301AF160QG", "REQUIRED", {"1": "BTN_DOWN", "2": "GND"}),
    part("U8", "AV BUFFER", "Package_TO_SOT_SMD:SOT-23-6", "TPF131-TR", "REVIEW",
         {"1": "AV_BUF_IN", "2": "GND", "3": "AV_BUF_BIAS", "4": "AV_BUF_EN", "5": "+5V", "6": "AV_BUF_OUT"},
         "DNP until direct-CVBS input scaling and exact application circuit are bench-validated."),
    part("J5", "AV OUT 3.5MM", "Connector_Audio:Jack_3.5mm_CUI_SJ1-3523N_Horizontal", "SJ1-3523N", "REVIEW",
         {"1": "GND", "2": "AV_OUT", "3": "GND"}),
    part("R29", "75R", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-0775RL", "REVIEW", {"1": "AV_BUF_OUT", "2": "AV_OUT"}),
    part("R30", "1k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-071KL", "REQUIRED", {"1": "RSSI_A", "2": "RSSI_A_ADC"}),
    part("R31", "1k", "Resistor_SMD:R_0603_1608Metric", "RC0603FR-071KL", "REQUIRED", {"1": "RSSI_B", "2": "RSSI_B_ADC"}),
])

# These nets are intentionally exposed as test nodes or option boundaries.
ALLOW_SINGLE = {"NC_AUDIO_A", "NC_AUDIO_B", "ADC1_ALERT", "ADC2_ALERT", "ADC3_ALERT",
                "SPARE_ADC", "SPARE_RSSI2", "SPARE_RSSI3", "DECK_CELL", "AV_BUF_IN", "AV_BUF_BIAS",
                "AV_BUF_EN", "ACTIVE_RX"}


def validate() -> dict[str, list[str]]:
    refs = [p.ref for p in PARTS]
    if len(refs) != len(set(refs)):
        raise ValueError("duplicate reference designator")
    nets: dict[str, list[str]] = {}
    for p in PARTS:
        for pin, net in p.pins.items():
            nets.setdefault(net, []).append(f"{p.ref}.{pin}")
    lonely = {n: e for n, e in nets.items() if len(e) < 2 and n not in ALLOW_SINGLE and not n.startswith("RF_")}
    if lonely:
        raise ValueError(f"nets with fewer than two endpoints: {lonely}")
    return nets


def write_bom() -> None:
    with (OUT / "bom.csv").open("w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f, lineterminator="\n")
        writer.writerow(["Reference", "Value", "Manufacturer part number", "Footprint", "Status", "Assembly note"])
        for p in sorted(PARTS, key=lambda x: x.ref):
            writer.writerow([p.ref, p.value, p.mpn, p.footprint, p.status, p.note])


def write_netlist(nets: dict[str, list[str]]) -> None:
    lines = ["# Generated by hardware/gen_hat.py; do not edit.", ""]
    for name in sorted(nets):
        lines.append(f"{name}: " + ", ".join(nets[name]))
    (OUT / "netlist.txt").write_text("\n".join(lines) + "\n", encoding="utf-8")


def uid(seed: str) -> str:
    return str(uuid.uuid5(uuid.NAMESPACE_URL, "fpvdeck-hat/" + seed))


def write_schematic(nets: dict[str, list[str]]) -> None:
    """Write a KiCad-openable annotated design sheet.

    Component connectivity is intentionally label-based and the checked textual
    netlist is authoritative until symbol pin mapping receives a second review.
    """
    lines = [
        '(kicad_sch (version 20221206) (generator eeschema)',
        f'  (uuid {uid("sheet")})', '  (paper "A3")', '  (lib_symbols)',
        '  (text "FPVDECK 5-INCH FIELD TOOL - HAT REV A / ENGINEERING REVIEW" (at 20 15 0)',
        '    (effects (font (size 2.5 2.5)) (justify left bottom))',
        f'    (uuid {uid("title")}))',
        '  (text "NOT RELEASED FOR FABRICATION - RX5808 footprint, AV buffer and mechanics are gated" (at 20 21 0)',
        '    (effects (font (size 1.4 1.4)) (justify left bottom))',
        f'    (uuid {uid("warning")}))',
    ]
    blocks = [
        ("POWER + HOST", ["J1", "U4", "FB1", "FB2"]),
        ("5.8 GHz DIVERSITY", ["RXA", "RXB", "J2", "J3", "U1", "U2"]),
        ("CVBS TO DVP", ["U3", "Y1", "R20", "R21", "C20", "C21", "C22", "C23", "C24"]),
        ("BATTERY + RSSI ADC", ["J4", "U5", "U6", "U7"]),
        ("USER I/O", ["SW1", "SW2", "SW3", "Q1", "BZ1"]),
        ("BUFFERED AV OUT (REVIEW/DNP)", ["U8", "J5", "R29"]),
    ]
    y = 32
    by_ref = {p.ref: p for p in PARTS}
    for name, refs in blocks:
        lines.extend([
            f'  (text "{name}" (at 20 {y} 0)',
            '    (effects (font (size 1.6 1.6)) (justify left bottom))',
            f'    (uuid {uid("block-" + name)}))',
        ])
        y += 5
        for ref in refs:
            p = by_ref[ref]
            pin_summary = "  ".join(f"{pin}:{net}" for pin, net in p.pins.items())
            text = f"{p.ref}  {p.value}  [{p.status}]  {pin_summary}".replace('"', "'")
            lines.extend([
                f'  (text "{text}" (at 24 {y} 0)',
                '    (effects (font (size 0.95 0.95)) (justify left bottom))',
                f'    (uuid {uid("part-" + ref)}))',
            ])
            y += 4
        y += 3
    lines.extend(['  (sheet_instances (path "/" (page "1")))', ')'])
    text = "\n".join(lines) + "\n"
    if text.count("(") != text.count(")"):
        raise ValueError("unbalanced schematic parentheses")
    (OUT / "fpv-hat.kicad_sch").write_text(text, encoding="utf-8")


def write_project() -> None:
    (OUT / "fpv-hat.kicad_pro").write_text('{"board": {}, "boards": [], "cvpcb": {}, "erc": {}, "meta": {"filename": "fpv-hat.kicad_pro", "version": 1}, "net_settings": {}, "pcbnew": {}, "schematic": {}, "text_variables": {}}\n', encoding="utf-8")


def write_board_svg() -> None:
    major = [p for p in PARTS if p.ref in {"J1", "RXA", "RXB", "J2", "J3", "U1", "U2", "U3", "U4", "U5", "U6", "U7", "J4", "SW1", "SW2", "SW3", "BZ1", "U8", "J5"}]
    positions = [(35, 30), (135, 42), (135, 82), (215, 30), (215, 82), (260, 58), (310, 58),
                 (395, 58), (355, 118), (430, 125), (485, 125), (540, 125), (58, 145), (240, 156),
                 (280, 156), (320, 156), (365, 156), (510, 55), (555, 55)]
    boxes = []
    for p, (x, y) in zip(major, positions):
        color = "#cc8f36" if p.status in {"DNP", "REVIEW"} else "#42c9bd"
        boxes.append(f'<rect x="{x}" y="{y}" width="70" height="28" rx="4" fill="#122731" stroke="{color}"/>')
        boxes.append(f'<text x="{x+35}" y="{y+12}" text-anchor="middle" class="ref">{html.escape(p.ref)}</text>')
        boxes.append(f'<text x="{x+35}" y="{y+22}" text-anchor="middle" class="value">{html.escape(p.value[:16])}</text>')
    svg = f'''<svg xmlns="http://www.w3.org/2000/svg" width="1200" height="440" viewBox="0 0 620 220">
<style>.title{{font:700 13px sans-serif;fill:#e9fbfa}}.ref{{font:700 8px sans-serif;fill:#e9fbfa}}.value{{font:5px sans-serif;fill:#a9c6cb}}.note{{font:7px sans-serif;fill:#efb85c}}</style>
<rect width="620" height="220" fill="#071016"/><rect x="12" y="15" width="596" height="190" rx="8" fill="#0b1921" stroke="#42616b" stroke-width="1.5"/>
<text x="24" y="12" class="title">FPVDeck HAT Rev A - COMPONENT PLACEMENT CONCEPT</text>
<text x="310" y="199" text-anchor="middle" class="note">NOT FOR FABRICATION · RF / MECHANICAL / AV BUFFER REVIEW GATES OPEN</text>
{''.join(boxes)}</svg>'''
    (OUT / "fpv-hat-component-map.svg").write_text(svg, encoding="utf-8")


def write_minimal_board() -> None:
    board = '''(kicad_pcb (version 20221018) (generator fpvdeck_gen_hat)
  (general (thickness 1.6))
  (paper "A4")
  (layers (0 "F.Cu" signal) (31 "B.Cu" signal) (36 "B.SilkS" user "B.Silkscreen")
          (37 "F.SilkS" user "F.Silkscreen") (44 "Edge.Cuts" user))
  (setup (pad_to_mask_clearance 0))
  (gr_rect (start 20 20) (end 120 85) (stroke (width 0.2) (type default)) (fill none) (layer "Edge.Cuts"))
  (gr_text "FPVDECK HAT REV A - PLACEMENT ONLY / NOT FOR FAB" (at 70 24) (layer "F.SilkS")
    (effects (font (size 1.3 1.3) (thickness 0.22))))
  (gr_text "RX5808 FOOTPRINT MUST BE MEASURED" (at 70 81) (layer "F.SilkS")
    (effects (font (size 1 1) (thickness 0.18))))
)\n'''
    (OUT / "fpv-hat.kicad_pcb").write_text(board, encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--check", action="store_true", help="regenerate and validate outputs")
    parser.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    nets = validate()
    write_bom()
    write_netlist(nets)
    write_schematic(nets)
    write_project()
    write_minimal_board()
    write_board_svg()
    print(f"Generated {len(PARTS)} parts and {len(nets)} nets in {OUT}")
    print("Checks passed: unique references, net endpoint count, balanced schematic")
    return 0


if __name__ == "__main__":
    sys.exit(main())
