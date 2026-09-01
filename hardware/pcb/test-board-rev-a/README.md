# FpvDeck Test PCB Rev A

This KiCad project is the modular bring-up board for the external balance checker,
real-time MCU, precision ADC, controls, low-speed module interfaces, and shutdown
coordination. It deliberately does **not** route MIPI DSI/CSI or integrate RF.

## Release state

**NOT RELEASED FOR FABRICATION.** The generated `.kicad_pcb` is a real KiCad board
with verified package classes, net intent, probe access, and mechanical placement.
It is not being represented as manufacturing-ready while these gates remain:

1. independently verify `ACASA1002U1002P1AT` pad-to-element pairing against the
   Vishay drawing and continuity-test a sample;
2. freeze and fault-test the protected B− reference path; provisional F2 must
   remain DNP until that review explicitly approves population;
3. complete the native schematic/ERC review;
4. route power, ground, analog and USB; add planes; run DRC;
5. peer-check the ADS8688A and STM32 pad maps against current datasheets;
6. validate every footprint against the exact ordered MPN, then print 1:1.

The component BOM and system order list intentionally place bare-PCB fabrication
in `WAIT` until these gates close. Development boards, display, decoder EVM and
measurement equipment can be ordered immediately.

## Regeneration

KiCad 7 with its Python module is currently used:

```bash
python3 hardware/pcb/test-board-rev-a/generate_board.py
kicad-cli pcb export pdf -o /tmp/test-board.pdf \
  hardware/pcb/test-board-rev-a/test-board-rev-a.kicad_pcb
```

Generated output is source controlled so contributors can inspect the board
without running the generator. Do not hand-edit the generated PCB without also
updating or retiring the generator.

## Partitioning

- left: JST-XH input, precision divider arrays, raw/ADC test points;
- center: ADS8688A and STM32G0B1;
- bottom: current-limited bench 5 V and local 3.3 V;
- right: project-defined SBC, VRX and decoder-EVM headers;
- direct CM4-to-display DSI and decoder-to-CM4 CSI stay off-board.

F2 (`1206L010/60WR`) and TP33/TP34 provide a replaceable, probeable experiment
between `BNEG_RAW` and local ground. F2 is **not an approved protection design**.
The PPTC's normal resistance produces measurable offset, its trip behavior is not
a substitute for a complete ground-fault design, and USB/bench/SBC grounds can
create alternate current paths. Keep it DNP and use only a current-limited,
non-battery simulator until the release gate in `docs/BATTERY_MEASUREMENT.md`
closes.

The VRX and decoder headers are FpvDeck generic interfaces. They do not claim the
pinout of any undocumented commercial module.
