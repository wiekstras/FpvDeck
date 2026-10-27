# FPV HAT Rev A

Generated artifacts:

- `fpv-hat.kicad_sch` — KiCad-readable annotated engineering schematic sheet;
- `fpv-hat.kicad_pcb` — mechanical placement board with the confirmed host
  mounting pattern and provisional SMA/JST/button locations; not routed;
- `bom.csv` — per-reference part and assembly status;
- `netlist.txt` — authoritative checked connectivity audit;
- `fpv-hat-component-map.svg` — review view.
- `export/` — top/bottom assembly views and a placement-only STEP model.

Regenerate with `python3 hardware/gen_hat.py`. Do not edit generated files.
KiCad maintainers can regenerate the mechanical template with
`python3 hardware/gen_pcb_mechanical.py`; this requires KiCad's Python module.
This revision is intentionally not suitable for fabrication; see
[`../READINESS.md`](../READINESS.md).
