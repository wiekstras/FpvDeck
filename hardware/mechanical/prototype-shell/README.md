# Prototype enclosure CAD

This directory contains a source-controlled OpenSCAD clamshell for fit and UX
prototyping. It is not a final enclosure or an authorization to print around an
unmeasured battery/HAT stack.

The model deliberately includes:

- the official 126.9 × 70.7 mm Waveshare front envelope and 110.32 × 62.28 mm
  active area;
- four host mounting points on the official 112.003 × 57 mm pattern;
- a right-side rail with UP / OK / DOWN openings;
- top-left and top-right SMA passages above the two receiver modules;
- a bottom opening for a side-entry JST-XH 7-pin balance connector;
- transparent host, HAT and connector fit-check bodies in the assembly view.

Build on macOS with:

```sh
brew install openscad
./hardware/mechanical/prototype-shell/export.sh
```

Generated STL and PNG files are committed so the design can be reviewed without
OpenSCAD. Edit `dimensions.scad`, never dimensions embedded in the model.

## Required physical fit checks

1. Print the front 2 mm high and verify glass/active-area registration.
2. Print both top rear connector corners; test both SMA parts, then print the
   bottom connector edge for the XHP-7 plug.
3. Measure the received RX5808 modules, chosen 1S pack, header stack and button
   actuators before printing a complete shell.
4. Confirm USB, microSD, power-button, speaker and ventilation access against
   Waveshare's official STEP model; those openings are intentionally absent.
5. Add screw inserts and retention only after wall/material tests.

## Status of dimensions

Confirmed host values come from Waveshare's official 2026-04-08 mechanical PDF.
JST body values come from JST's XH drawing. SMA barrel diameter comes from
Amphenol drawing 132289-12. Connector positions, wall thickness, HAT plane,
button cap diameter and battery cavity are design parameters pending real-part
measurement.
