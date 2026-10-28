#!/usr/bin/env python3
"""Generate the net-aware FpvDeck HAT layout candidate in KiCad.

The board is generated from the same part/net model as the schematic. It is a
final-form placement candidate, but fabrication remains blocked until the donor
RX5808 land pattern and Waveshare mating geometry are verified physically and
the remaining nets are interactively routed/reviewed.
"""

from pathlib import Path
import math
import sys

import pcbnew

ROOT = Path(__file__).resolve().parent
sys.path.insert(0, str(ROOT))
from gen_hat import PARTS, validate  # noqa: E402

OUT = ROOT / "templates" / "fpv-hat-placement.kicad_pcb"
PCB_OUT = ROOT / "fpv-hat" / "fpv-hat.kicad_pcb"
KICAD = Path("/usr/share/kicad/footprints")
CUSTOM = ROOT / "fpv-hat.pretty"
CX, CY = 100.0, 100.0
BOARD_X, BOARD_Y = 118.5, 64.5


def mm(value):
    return pcbnew.FromMM(value)


def point(x, y):
    return pcbnew.VECTOR2I(mm(x), mm(y))


def line(board, x1, y1, x2, y2, layer=pcbnew.Edge_Cuts, width=0.2):
    shape = pcbnew.PCB_SHAPE(board)
    shape.SetShape(pcbnew.SHAPE_T_SEGMENT)
    shape.SetStart(point(x1, y1))
    shape.SetEnd(point(x2, y2))
    shape.SetLayer(layer)
    shape.SetWidth(mm(width))
    board.Add(shape)


def box(board, x1, y1, x2, y2, layer=pcbnew.Dwgs_User, width=0.2):
    line(board, x1, y1, x2, y1, layer, width)
    line(board, x2, y1, x2, y2, layer, width)
    line(board, x2, y2, x1, y2, layer, width)
    line(board, x1, y2, x1, y1, layer, width)


def label(board, text, x, y, size=1.0, layer=pcbnew.F_SilkS, rotation=0):
    item = pcbnew.PCB_TEXT(board)
    item.SetText(text)
    item.SetPosition(point(x, y))
    item.SetTextSize(point(size, size))
    item.SetTextThickness(mm(max(0.15, size * 0.14)))
    item.SetTextAngle(pcbnew.EDA_ANGLE(rotation, pcbnew.DEGREES_T))
    item.SetLayer(layer)
    board.Add(item)


def load_footprint(footprint_name):
    library, name = footprint_name.split(":", 1)
    directory = CUSTOM if library == "FPVDeck" else KICAD / f"{library}.pretty"
    footprint = pcbnew.FootprintLoad(str(directory), name)
    if footprint is None:
        raise RuntimeError(f"missing KiCad footprint {footprint_name}")
    return footprint


def add_footprint(board, part, placement, net_map):
    x, y, rotation = placement
    fp = load_footprint(part.footprint)
    fp.SetReference(part.ref)
    fp.SetValue(part.value)
    fp.SetPosition(point(x, y))
    fp.SetOrientationDegrees(rotation)
    for pad in fp.Pads():
        number = pad.GetNumber()
        if number in part.pins:
            pad.SetNet(net_map[part.pins[number]])
    if hasattr(fp, "Reference"):
        major = part.ref.startswith(("RX", "J", "U", "SW", "BZ"))
        ref_size = 0.8 if major else 0.55
        fp.Reference().SetTextSize(point(ref_size, ref_size))
        fp.Reference().SetTextThickness(mm(0.13 if major else 0.10))
    board.Add(fp)
    return fp


def pad_position(footprints, ref, number):
    candidates = [pad for pad in footprints[ref].Pads()
                  if pad.GetNumber() == str(number)]
    if not candidates:
        raise RuntimeError(f"{ref} has no pad {number}")
    return candidates[0].GetPosition()


def track(board, net_map, net_name, start, end, width=0.30,
          layer=pcbnew.F_Cu):
    segment = pcbnew.PCB_TRACK(board)
    segment.SetNet(net_map[net_name])
    segment.SetStart(start)
    segment.SetEnd(end)
    segment.SetWidth(mm(width))
    segment.SetLayer(layer)
    board.Add(segment)


def placement_table():
    p = {
        # Orthogonal antenna placement: A exits the left edge; B exits the
        # top-right edge. Both receiver RF pads remain 2.25 mm from the SMA
        # center pad. RX B is shifted inboard to clear the host mounting hole;
        # its launch remains below the project's 10 mm limit.
        "J2": (40.75, 94.0, 0), "J3": (138.5, 67.75, 90),
        "RXA": (57.0, 86.38, 0), "RXB": (137.0, 84.0, 270),
        "FRA": (73.5, 84.0, 90), "CRA1": (76.0, 82.5, 90),
        "CRA2": (76.0, 85.5, 90), "FRB": (132.0, 101.0, 0),
        "CRB1": (135.0, 101.0, 0), "CRB2": (138.0, 101.0, 0),
        "U1": (82.0, 73.5, 0), "R1": (85.5, 71.5, 0),
        "R2": (85.5, 73.5, 0), "R3": (85.5, 75.5, 0),
        "C1": (79.5, 72.0, 90), "JP1": (90.0, 73.5, 0),
        "U2": (94.0, 73.5, 0), "C2": (97.0, 71.8, 90),
        "U3": (108.0, 86.0, 0), "U4": (123.0, 82.0, 0),
        "CIN1": (126.0, 80.5, 90), "COUT1": (126.0, 83.5, 90),
        "FB1": (119.5, 80.5, 0), "FB2": (119.5, 83.5, 0),
        "Y1": (97.5, 87.0, 0), "RY1": (97.5, 90.0, 0),
        "CY1": (94.5, 86.0, 90), "CY2": (94.5, 89.0, 90),
        "R20": (101.0, 78.0, 0), "R21": (104.0, 78.0, 0),
        "C20": (107.0, 78.0, 0), "C21": (109.5, 79.5, 90),
        "C22": (113.0, 91.0, 0), "C23": (110.0, 93.0, 0),
        "C24": (113.0, 93.0, 0), "R22": (116.0, 89.0, 90),
        "R23": (118.0, 89.0, 90), "R24": (120.0, 89.0, 90),
        "R25": (115.5, 83.0, 90),
        "CD1": (104.0, 91.5, 90), "CD2": (106.0, 92.5, 90),
        "CD3": (110.0, 96.0, 0), "CD4": (113.0, 96.0, 0),
        "U8": (124.0, 95.0, 0), "C25": (121.0, 95.0, 90),
        "C26": (124.0, 91.5, 0), "C27": (127.0, 91.5, 0),
        "R29": (128.0, 95.0, 0),
        "J5": (51.5, 132.25, 0),
        "U5": (104.0, 108.0, 0), "CA1": (104.0, 111.0, 0),
        "U6": (111.0, 108.0, 0), "CA2": (111.0, 111.0, 0),
        "U7": (118.0, 108.0, 0), "CA3": (118.0, 111.0, 0),
        "R26": (108.0, 114.0, 0), "R27": (112.0, 114.0, 0),
        "R30": (118.0, 104.0, 0), "R31": (121.0, 104.0, 0),
        "J4": (122.0, 132.0, 0), "J1": (66.0, 124.0, 90),
        "BZ1": (50.0, 113.0, 0), "Q1": (57.5, 113.0, 0),
        "R28": (61.0, 113.0, 0),
        "SW1": (153.0, 105.0, 90), "SW2": (153.0, 115.0, 90),
        "SW3": (153.0, 125.0, 90),
    }

    for index in range(11):
        p[f"RD{index}"] = (106.0 + (index % 6) * 2.2,
                            97.0 + (index // 6) * 2.6, 90)

    for index in range(1, 7):
        x = 121.0 + (index - 1) * 4.5
        p[f"RT{index}"] = (x, 116.0, 90)
        p[f"RB{index}"] = (x, 119.0, 90)
        p[f"CT{index}"] = (x, 122.0, 90)
        p[f"TP{index}"] = (x, 126.0, 0)
    return p


def generate():
    nets = validate()
    board = pcbnew.BOARD()
    board.SetCopperLayerCount(4)
    board.GetDesignSettings().SetBoardThickness(mm(1.6))
    x0, x1 = CX - BOARD_X / 2, CX + BOARD_X / 2
    y0, y1 = CY - BOARD_Y / 2, CY + BOARD_Y / 2
    line(board, x0, y0, x1, y0)
    line(board, x1, y0, x1, y1)
    line(board, x1, y1, x0, y1)
    line(board, x0, y1, x0, y0)

    net_map = {}
    for name in sorted(nets):
        net = pcbnew.NETINFO_ITEM(board, name)
        board.Add(net)
        net_map[name] = net

    placements = placement_table()
    missing = sorted({part.ref for part in PARTS} - set(placements))
    extra = sorted(set(placements) - {part.ref for part in PARTS})
    if missing or extra:
        raise RuntimeError(f"placement mismatch: missing={missing}, extra={extra}")

    footprints = {}
    for part in PARTS:
        footprints[part.ref] = add_footprint(
            board, part, placements[part.ref], net_map)

    for index, (x, y) in enumerate(((-56.0015, -28.5), (56.0015, -28.5),
                                    (-56.0015, 28.5), (56.0015, 28.5)), 1):
        hole_part = type("Hole", (), {
            "ref": f"H{index}", "value": "M2.5",
            "footprint": "MountingHole:MountingHole_2.7mm_M2.5",
            "pins": {}})()
        add_footprint(board, hole_part, (CX + x, CY + y, 0), net_map)

    # Only the critical RF launches are routed in this generated candidate.
    # Their 1 mm width is not a 50-ohm claim; final width/clearance comes from
    # the selected four-layer fabricator stack-up and field-solver calculation.
    rf_lengths = {}
    for sma, receiver, net in (("J2", "RXA", "RF_A"),
                               ("J3", "RXB", "RF_B")):
        start = pad_position(footprints, sma, 1)
        end = pad_position(footprints, receiver, 2)
        length = math.hypot(pcbnew.ToMM(end.x - start.x),
                            pcbnew.ToMM(end.y - start.y))
        if length > 10.0:
            raise RuntimeError(f"{net} launch is {length:.2f} mm; limit is 10 mm")
        rf_lengths[net] = length
        track(board, net_map, net, start, end, width=1.0)

    j2 = pad_position(footprints, "J2", 1)
    j3 = pad_position(footprints, "J3", 1)
    if abs(pcbnew.ToMM(j2.x) - x0) > 0.01:
        raise RuntimeError("SMA A must terminate at the left board edge")
    if abs(pcbnew.ToMM(j3.y) - y0) > 0.01:
        raise RuntimeError("SMA B must terminate at the top board edge")

    box(board, 42.5, 73.5, 77.5, 100.0)
    box(board, 78.0, 69.5, 132.0, 99.5)
    box(board, 124.5, 69.5, 149.0, 99.5)
    box(board, 98.5, 101.0, 150.0, 130.0)
    label(board, "RX A / LEFT", 44.0, 86.0, 0.8, pcbnew.Dwgs_User, 90)
    label(board, "RX B / TOP RIGHT", 148.0, 84.0, 0.7, pcbnew.Dwgs_User, 90)
    label(board, "VIDEO / DIVERSITY", 105.0, 98.0, 0.75, pcbnew.Dwgs_User)
    label(board, "BALANCE ADC", 143.5, 128.5, 0.75, pcbnew.Dwgs_User)
    label(board, "FPVDECK HAT REV A", 84.0, 130.0, 0.95)
    label(board, "VERIFY RX5808 + HOST BEFORE FAB", 83.0, 127.5, 0.65)
    label(board, "BALANCE ONLY", 136.0, y1 - 1.8, 0.65)
    label(board, "SMA A LEFT", x0 + 4.0, 94.0, 0.65, rotation=90)
    label(board, "SMA B TOP RIGHT", 138.5, y0 + 4.5, 0.65)
    label(board, "AUTO / MCU", 90.0, 70.5, 0.65)
    label(board, "UP", 148.5, 105.0, 0.7)
    label(board, "OK", 148.5, 115.0, 0.7)
    label(board, "DOWN", 147.5, 125.0, 0.7)

    OUT.parent.mkdir(parents=True, exist_ok=True)
    pcbnew.SaveBoard(str(OUT), board)
    pcbnew.SaveBoard(str(PCB_OUT), board)
    for suffix in (".kicad_prl", ".kicad_pro"):
        companion = OUT.with_suffix(suffix)
        if companion.exists():
            companion.unlink()
    print(f"wrote net-aware {len(PARTS)}-part layout to {PCB_OUT.relative_to(ROOT.parent)}")
    print("RF launches: " + ", ".join(
        f"{name}={length:.2f} mm" for name, length in rf_lengths.items()))
    print("critical RF launches routed; remaining nets intentionally await review routing")


if __name__ == "__main__":
    generate()
