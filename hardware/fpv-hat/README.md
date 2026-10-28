# FPV HAT Rev A

Generated artifacts:

- `fpv-hat.kicad_sch` — KiCad-readable annotated engineering schematic sheet;
- `fpv-hat.kicad_pcb` — four-layer, net-aware final-form placement candidate
  with all modeled parts and provisional short RF launches;
- `bom.csv` — per-reference part and assembly status;
- `netlist.txt` — authoritative checked connectivity audit;
- `fpv-hat-component-map.svg` — review view.
- `export/` — top/bottom assembly views and a placement-only STEP model.

Regenerate with `python3 hardware/gen_hat.py`. Do not edit generated files.
KiCad maintainers can regenerate the layout template with
`python3 hardware/gen_pcb_mechanical.py`, then export review views with
`./hardware/export_hat.sh`; these require KiCad's Python module and CLI.
The 28 × 23 mm RX5808 V1 footprint is provisional until checked against the
salvaged modules. This revision is intentionally not suitable for fabrication;
see [`../READINESS.md`](../READINESS.md) and the
[RX5808/diversity research](../research/RX5808_FINAL_HAT.md).
