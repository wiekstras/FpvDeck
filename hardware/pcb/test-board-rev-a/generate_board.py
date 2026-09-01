#!/usr/bin/env python3
"""Generate the conservative Test PCB Rev A KiCad board.

Run with KiCad 7's system Python. The generated board is intentionally marked
NOT RELEASED: it captures verified footprints, placement, net intent and probe
access while the balance-reference protection gate remains open.
"""

from pathlib import Path

import pcbnew

HERE = Path(__file__).resolve().parent
FP_ROOT = Path("/usr/share/kicad/footprints")
OUT = HERE / "test-board-rev-a.kicad_pcb"

# Keep the source-controlled generated board byte-for-byte reproducible. KiCad's
# default KIID generator is random, which otherwise rewrites every footprint and
# pad UUID even when electrical intent is unchanged.
pcbnew.KIID.SeedGenerator(0x46505644)  # ASCII-ish "FPVD"


def mm(value: float) -> int:
    return pcbnew.FromMM(value)


def point(x: float, y: float) -> pcbnew.VECTOR2I:
    return pcbnew.VECTOR2I(mm(x), mm(y))


board = pcbnew.BOARD()
board.SetCopperLayerCount(4)
board.GetDesignSettings().m_TrackMinWidth = mm(0.20)
board.GetDesignSettings().m_MinClearance = mm(0.20)

nets: dict[str, pcbnew.NETINFO_ITEM] = {}


def net(name: str) -> pcbnew.NETINFO_ITEM:
    if name not in nets:
        item = pcbnew.NETINFO_ITEM(board, name)
        board.Add(item)
        nets[name] = item
    return nets[name]


for name in (
    "GND", "+5V_RAW", "+5V_FUSED", "+3V3", "BNEG_RAW", "BNEG_SENSE",
    "SPI_SCLK", "SPI_MOSI", "SPI_MISO", "ADC_CS", "ADC_RESET", "ADC_ALARM",
    "UART_TX", "UART_RX", "USB_DP", "USB_DM", "I2C_SCL", "I2C_SDA",
    "SHUTDOWN_REQ", "SBC_POWER_OK", "FAN_PWM", "VRX_VIDEO",
    "SD_DETECT", "SWDIO", "SWCLK", "NRST",
    "ADC_REFIO", "ADC_REFCAP", "DECK_MON",
):
    net(name)
for channel in range(1, 7):
    net(f"TAP{channel}_RAW")
    net(f"TAP{channel}_SER")
    net(f"TAP{channel}_ADC")
    net(f"TAP{channel}_STAGE1")
    net(f"TAP{channel}_STAGE2")

footprints: dict[str, pcbnew.FOOTPRINT] = {}


def add_fp(reference: str, value: str, library: str, name: str, x: float, y: float, rotation: float = 0) -> pcbnew.FOOTPRINT:
    footprint = pcbnew.FootprintLoad(str(FP_ROOT / f"{library}.pretty"), name)
    if footprint is None:
        raise RuntimeError(f"KiCad footprint not found: {library}:{name}")
    footprint.SetReference(reference)
    footprint.SetValue(value)
    footprint.SetPosition(point(x, y))
    footprint.SetOrientationDegrees(rotation)
    board.Add(footprint)
    footprints[reference] = footprint
    return footprint


def assign(reference: str, mapping: dict[str, str]) -> None:
    footprint = footprints[reference]
    for pad_number, net_name in mapping.items():
        pad = footprint.FindPadByNumber(pad_number)
        if pad is None:
            raise RuntimeError(f"{reference} has no pad {pad_number}")
        pad.SetNet(net(net_name))


def add_text(text: str, x: float, y: float, size: float = 1.2, layer: int = pcbnew.F_SilkS) -> None:
    item = pcbnew.PCB_TEXT(board)
    item.SetText(text)
    item.SetPosition(point(x, y))
    item.SetTextSize(point(size, size))
    item.SetTextThickness(mm(max(0.18, size * 0.14)))
    item.SetLayer(layer)
    board.Add(item)


def add_outline(x1: float, y1: float, x2: float, y2: float) -> None:
    points = [(x1, y1, x2, y1), (x2, y1, x2, y2), (x2, y2, x1, y2), (x1, y2, x1, y1)]
    for ax, ay, bx, by in points:
        shape = pcbnew.PCB_SHAPE(board)
        shape.SetShape(pcbnew.SHAPE_T_SEGMENT)
        shape.SetStart(point(ax, ay))
        shape.SetEnd(point(bx, by))
        shape.SetLayer(pcbnew.Edge_Cuts)
        shape.SetWidth(mm(0.15))
        board.Add(shape)


# 120 x 90 mm, deliberately spacious for probes and hand assembly.
add_outline(20, 20, 140, 110)
for ref, x, y in (("H1", 24, 24), ("H2", 136, 24), ("H3", 24, 106), ("H4", 136, 106)):
    add_fp(ref, "M3", "MountingHole", "MountingHole_3.2mm_M3", x, y)

# External balance connector and exact matched-divider footprints.
add_fp("J1", "B7B-XH-A(LF)(SN)", "Connector_JST", "JST_XH_B7B-XH-A_1x07_P2.50mm_Vertical", 28, 54, 90)
assign("J1", {"1": "BNEG_RAW", **{str(i + 1): f"TAP{i}_RAW" for i in range(1, 7)}})

# Provisional pack-negative reference link. F2 is deliberately DNP: its
# footprint gives Rev A a measurable, replaceable experiment without approving
# this topology for a real LiPo. Ground-fault, USB-ground and PPTC-drop tests in
# BATTERY_MEASUREMENT.md remain release gates.
add_fp("F2", "1206L010/60WR DNP", "Fuse", "Fuse_1206_3216Metric_Pad1.42x1.75mm_HandSolder", 32, 102)
assign("F2", {"1": "BNEG_RAW", "2": "GND"})
for ref, value, net_name, x in (
    ("TP33", "BNEG_RAW", "BNEG_RAW", 39),
    ("TP34", "LOCAL_GND", "GND", 45),
):
    add_fp(ref, value, "TestPoint", "TestPoint_Loop_D2.50mm_Drill1.0mm", x, 102)
    assign(ref, {"1": net_name})

# Pack-negative sense is intentionally a distinct ADC net so R7 is visible in
# both the netlist and probing path. Its ADC input-bias error requires calibration.
add_fp("R7", "1K00 0.1%", "Resistor_SMD", "R_0603_1608Metric_Pad0.98x0.95mm_HandSolder", 68, 40, 90)
assign("R7", {"1": "BNEG_RAW", "2": "BNEG_SENSE"})

for channel in range(1, 7):
    y = 34 + (channel - 1) * 11.5
    add_fp(f"R{channel}", "1K00 0.1%", "Resistor_SMD", "R_0603_1608Metric_Pad0.98x0.95mm_HandSolder", 42, y, 90)
    assign(f"R{channel}", {"1": f"TAP{channel}_RAW", "2": f"TAP{channel}_SER"})
    add_fp(f"RN{channel}", "ACASA1002U1002P1AT", "Resistor_SMD", "R_Array_Convex_4x0603", 50, y)
    # Vishay ACAS 0612 is four isolated elements. KiCad pad pairs are
    # 1-8, 2-7, 3-6, 4-5. Copper series links are intentionally not
    # generated until this mapping is independently checked at fab review.
    assign(f"RN{channel}", {
        "1": f"TAP{channel}_SER", "8": f"TAP{channel}_STAGE1",
        "2": f"TAP{channel}_STAGE1", "7": f"TAP{channel}_STAGE2",
        "3": f"TAP{channel}_STAGE2", "6": f"TAP{channel}_ADC",
        "4": f"TAP{channel}_ADC", "5": "GND",
    })
    add_fp(f"C{channel}", "10n C0G 50V", "Capacitor_SMD", "C_0603_1608Metric_Pad1.08x0.95mm_HandSolder", 59, y, 90)
    assign(f"C{channel}", {"1": f"TAP{channel}_ADC", "2": "GND"})
    add_fp(f"TP{channel}", f"TAP{channel}_RAW", "TestPoint", "TestPoint_Loop_D2.50mm_Drill1.0mm", 35, y)
    assign(f"TP{channel}", {"1": f"TAP{channel}_RAW"})
    add_fp(f"TP{channel + 10}", f"TAP{channel}_ADC", "TestPoint", "TestPoint_Loop_D2.50mm_Drill1.0mm", 64, y)
    assign(f"TP{channel + 10}", {"1": f"TAP{channel}_ADC"})

# Precision ADC. Pin mapping follows TI SBAS680, DBT top view.
add_fp("U1", "ADS8688AIDBT", "Package_SO", "TSSOP-38_4.4x9.7mm_P0.5mm", 76, 61)
adc_mapping = {
    "1": "SPI_MOSI", "2": "ADC_RESET", "3": "GND", "4": "GND",
    "5": "ADC_REFIO", "6": "GND", "7": "ADC_REFCAP", "8": "GND", "9": "+5V_FUSED",
    "11": "GND", "12": "BNEG_SENSE", "13": "GND",
    "14": "DECK_MON", "15": "GND", "16": "TAP1_ADC", "17": "GND",
    "18": "TAP2_ADC", "19": "GND", "20": "GND", "21": "TAP3_ADC",
    "22": "GND", "23": "TAP4_ADC", "24": "GND", "25": "TAP5_ADC",
    "26": "GND", "27": "TAP6_ADC", "28": "GND", "29": "GND",
    "30": "+5V_FUSED",
    "31": "GND", "32": "GND", "33": "GND", "34": "+3V3", "35": "ADC_ALARM",
    "36": "SPI_MISO", "37": "SPI_SCLK", "38": "ADC_CS",
}
assign("U1", adc_mapping)
for ref, value, x, rail in (("C18", "22u 10V X7R", 70, "ADC_REFIO"), ("C19", "22u 10V X7R", 82, "ADC_REFCAP")):
    add_fp(ref, value, "Capacitor_SMD", "C_1206_3216Metric_Pad1.33x1.80mm_HandSolder", x, 72)
    assign(ref, {"1": rail, "2": "GND"})
for ref, value, x, y, rail in (
    ("C22", "1u 10V X7R", 70, 50, "+5V_FUSED"),
    ("C23", "1u 10V X7R", 82, 50, "+5V_FUSED"),
    ("C24", "1u 10V X7R", 82, 57, "+3V3"),
):
    add_fp(ref, value, "Capacitor_SMD", "C_0805_2012Metric_Pad1.18x1.45mm_HandSolder", x, y)
    assign(ref, {"1": rail, "2": "GND"})

# Real-time MCU. LQFP48 pad numbers follow STM32G0B1CxT GP version, DS13560.
add_fp("U2", "STM32G0B1CBT6", "Package_QFP", "LQFP-48_7x7mm_P0.5mm", 105, 61, 45)
mcu_mapping = {
    "4": "+3V3", "5": "+3V3", "6": "+3V3", "7": "GND", "10": "NRST",
    "13": "UART_TX", "14": "UART_RX", "16": "SPI_SCLK", "17": "SPI_MISO",
    "18": "SPI_MOSI", "19": "ADC_CS", "20": "ADC_RESET", "21": "ADC_ALARM",
    "33": "USB_DM", "34": "USB_DP", "35": "SWDIO", "36": "SWCLK",
    "45": "FAN_PWM", "47": "I2C_SCL", "48": "I2C_SDA",
}
assign("U2", mcu_mapping)
for ref, value, x, y in (("C25", "100n 16V X7R", 98, 52), ("C26", "4.7u 10V X7R", 113, 52)):
    add_fp(ref, value, "Capacitor_SMD", "C_0805_2012Metric_Pad1.18x1.45mm_HandSolder", x, y)
    assign(ref, {"1": "+3V3", "2": "GND"})

# Bench power: protected 5 V input and 3.3 V LDO. Test PCB does not include
# the final 2S charger/buck-boost architecture.
add_fp("J2", "5V BENCH IN", "TerminalBlock_Phoenix", "TerminalBlock_Phoenix_MKDS-1,5-2-5.08_1x02_P5.08mm_Horizontal", 51, 104, 180)
assign("J2", {"1": "+5V_RAW", "2": "GND"})
add_fp("F1", "MF-MSMF050-2", "Fuse", "Fuse_1206_3216Metric_Pad1.42x1.75mm_HandSolder", 64, 103)
assign("F1", {"1": "+5V_RAW", "2": "+5V_FUSED"})
add_fp("U3", "TLV75533PDBVR", "Package_TO_SOT_SMD", "SOT-23-5", 78, 103)
assign("U3", {"1": "+5V_FUSED", "2": "GND", "3": "+5V_FUSED", "5": "+3V3"})
for ref, value, x, rail in (("C20", "1u 10V X7R", 72, "+5V_FUSED"), ("C21", "1u 10V X7R", 84, "+3V3")):
    add_fp(ref, value, "Capacitor_SMD", "C_0805_2012Metric_Pad1.18x1.45mm_HandSolder", x, 103)
    assign(ref, {"1": rail, "2": "GND"})

# Module-safe low-speed interfaces. These are project pinouts, not claims about
# any undocumented T8L or VRX internal connector.
add_fp("J3", "SBC LINK", "Connector_PinHeader_2.54mm", "PinHeader_2x10_P2.54mm_Vertical", 128, 48, 90)
assign("J3", {"1": "+5V_FUSED", "2": "GND", "3": "+3V3", "4": "GND", "5": "UART_TX", "6": "UART_RX", "7": "SHUTDOWN_REQ", "8": "SBC_POWER_OK", "9": "I2C_SCL", "10": "I2C_SDA", "11": "SD_DETECT"})
# The selected receiver is a separately powered ground-station module. Only its
# documented 75-ohm CVBS output and ground cross Rev A. Pins 1/4/5/6 are NC and
# must not be repurposed from an undocumented receiver/module-bay pinout.
add_fp("J4", "VRX CVBS ONLY", "Connector_PinHeader_2.54mm", "PinHeader_1x06_P2.54mm_Vertical", 132, 76, 90)
assign("J4", {"2": "GND", "3": "VRX_VIDEO"})
add_fp("J5", "DECODER CVBS ONLY", "Connector_PinHeader_2.54mm", "PinHeader_1x06_P2.54mm_Vertical", 132, 88, 90)
assign("J5", {"2": "GND", "3": "VRX_VIDEO"})
add_fp("J6", "SWD", "Connector_PinHeader_1.27mm", "PinHeader_2x05_P1.27mm_Vertical", 106, 103)
assign("J6", {"1": "+3V3", "2": "SWDIO", "3": "GND", "4": "SWCLK", "5": "GND", "10": "NRST"})

# Rail and communication test points.
for index, (name, net_name, x) in enumerate((("5V", "+5V_FUSED", 92), ("3V3", "+3V3", 98), ("GND", "GND", 104)), start=30):
    add_fp(f"TP{index}", name, "TestPoint", "TestPoint_Loop_D2.50mm_Drill1.0mm", x, 92)
    assign(f"TP{index}", {"1": net_name})

add_text("FPVDECK TEST PCB REV A", 66, 24, 1.6)
add_text("NOT RELEASED - REVIEW GATES IN README", 58, 28, 1.0)
add_text("BALANCE INPUT: MEASUREMENT ONLY", 24, 92, 0.9)
add_text("F2 B- LINK: DNP / REVIEW", 22, 106, 0.75)
add_text("ANALOG / PROBE AREA", 40, 24, 0.9)
add_text("MCU / ADC", 82, 35, 0.9)
add_text("MODULE INTERFACES", 116, 24, 0.9)
add_text("J4/J5: 2 GND  3 CVBS  OTHERS NC", 109, 96, 0.65)

board.SetFileName(str(OUT))
pcbnew.SaveBoard(str(OUT), board)
print(f"Generated {OUT}")
