#!/usr/bin/env python3
"""Generate the placement-only HAT mechanical PCB in KiCad.

This board intentionally has no routed nets. It coordinates the Waveshare
mounting envelope and external JST/SMA/button locations with the shell.
"""
from pathlib import Path
import pcbnew

ROOT = Path(__file__).resolve().parent
OUT = ROOT / "templates" / "fpv-hat-placement.kicad_pcb"
PCB_OUT = ROOT / "fpv-hat" / "fpv-hat.kicad_pcb"
KICAD = Path("/usr/share/kicad/footprints")
CX, CY = 100.0, 100.0
BOARD_X, BOARD_Y = 118.5, 64.5

def mm(value): return pcbnew.FromMM(value)
def point(x, y): return pcbnew.VECTOR2I(mm(x), mm(y))

def footprint(board, library, name, ref, x, y, rotation=0):
    fp = pcbnew.FootprintLoad(str(KICAD / f"{library}.pretty"), name)
    if fp is None:
        raise RuntimeError(f"missing KiCad footprint {library}:{name}")
    fp.SetReference(ref)
    fp.SetPosition(point(x, y))
    fp.SetOrientationDegrees(rotation)
    board.Add(fp)
    return fp

def line(board, x1, y1, x2, y2, layer=pcbnew.Edge_Cuts, width=0.2):
    shape = pcbnew.PCB_SHAPE(board)
    shape.SetShape(pcbnew.SHAPE_T_SEGMENT)
    shape.SetStart(point(x1, y1)); shape.SetEnd(point(x2, y2))
    shape.SetLayer(layer); shape.SetWidth(mm(width)); board.Add(shape)

def label(board, text, x, y, size=1.2, layer=pcbnew.F_SilkS):
    item = pcbnew.PCB_TEXT(board)
    item.SetText(text); item.SetPosition(point(x, y))
    item.SetTextSize(point(size, size)); item.SetTextThickness(mm(0.2))
    item.SetLayer(layer); board.Add(item)

def generate():
    board = pcbnew.BOARD()
    board.GetDesignSettings().SetBoardThickness(mm(1.6))
    x0, x1 = CX - BOARD_X / 2, CX + BOARD_X / 2
    y0, y1 = CY - BOARD_Y / 2, CY + BOARD_Y / 2
    line(board, x0, y0, x1, y0); line(board, x1, y0, x1, y1)
    line(board, x1, y1, x0, y1); line(board, x0, y1, x0, y0)

    for index, (x, y) in enumerate(((-56.0015, -28.5), (56.0015, -28.5),
                                    (-56.0015, 28.5), (56.0015, 28.5)), 1):
        footprint(board, "MountingHole", "MountingHole_2.7mm_M2.5",
                  f"H{index}", CX+x, CY+y)
    footprint(board, "Connector_Coaxial", "SMA_Amphenol_132289_EdgeMount",
              "J2", CX-47.5, y0, 90)
    footprint(board, "Connector_Coaxial", "SMA_Amphenol_132289_EdgeMount",
              "J3", CX+30.5, y0, 90)
    footprint(board, "Connector_JST",
              "JST_XH_S7B-XH-A-1_1x07_P2.50mm_Horizontal",
              "J4", CX+22.0, y1-0.3, 270)
    # Staging location only. Exact header offset remains a release gate.
    footprint(board, "Connector_PinHeader_2.54mm",
              "PinHeader_2x20_P2.54mm_Vertical", "J1", CX-4, CY+21.0, 90)
    footprint(board, "Package_QFP", "LQFP-32_7x7mm_P0.8mm", "U3", CX-8, CY-6)
    for ref, x in (("U5", CX+11), ("U6", CX+20), ("U7", CX+29)):
        footprint(board, "Package_SO", "VSSOP-10_3x3mm_P0.5mm", ref, x, CY-4)
    for ref, y in (("SW1", CY-18), ("SW2", CY), ("SW3", CY+18)):
        footprint(board, "Button_Switch_SMD", "SW_SPST_TL3305A",
                  ref, x1-6.5, y, 90)

    label(board, "FPVDECK HAT REV A - MECHANICAL PLACEMENT ONLY", CX, y0+5, 1.3)
    label(board, "NOT ROUTED / NOT FOR FABRICATION", CX, y0+9, 1.1)
    label(board, "J1 OFFSET VERIFY AGAINST OFFICIAL HOST STEP", CX-5, CY+26, 0.9)
    label(board, "JST-XH BALANCE ONLY", CX+22, y1-5.5, 0.9)
    label(board, "SMA A", CX-47.5, y0+5, 0.9)
    label(board, "SMA B DNP", CX+30.5, y0+5, 0.9)

    OUT.parent.mkdir(parents=True, exist_ok=True)
    pcbnew.SaveBoard(str(OUT), board); pcbnew.SaveBoard(str(PCB_OUT), board)
    for suffix in (".kicad_prl", ".kicad_pro"):
        companion = OUT.with_suffix(suffix)
        if companion.exists(): companion.unlink()
    print(f"wrote {OUT.relative_to(ROOT.parent)} and {PCB_OUT.relative_to(ROOT.parent)}")

if __name__ == "__main__": generate()
