# FPV HAT Rev A

Generated artifacts:

- `fpv-hat.kicad_sch` — KiCad-readable annotated engineering schematic sheet;
- `fpv-hat.kicad_pcb` — board-outline/placement container, not routed;
- `bom.csv` — per-reference part and assembly status;
- `netlist.txt` — authoritative checked connectivity audit;
- `fpv-hat-component-map.svg` — review view.

Regenerate with `python3 hardware/gen_hat.py`. Do not edit generated files.
This revision is intentionally not suitable for fabrication; see
[`../READINESS.md`](../READINESS.md).
