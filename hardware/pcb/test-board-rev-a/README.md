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
in `WAIT` until these gates close. Compute, display, MCU development hardware and
measurement equipment can be ordered independently. The VRX and decoder remain
behind the documented end-to-end compatibility gate.

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
- right: SBC header plus separate VRX-CVBS and decoder-CVBS breakouts;
- direct CM4-to-display DSI and decoder-to-CM4 CSI stay off-board.

F2 (`1206L010/60WR`) and TP33/TP34 provide a replaceable, probeable experiment
between `BNEG_RAW` and local ground; R7 explicitly connects `BNEG_RAW` to the
separate ADC `BNEG_SENSE` net. F2 is **not an approved protection design**.
The PPTC's normal resistance produces measurable offset, its trip behavior is not
a substitute for a complete ground-fault design, and USB/bench/SBC grounds can
create alternate current paths. Keep it DNP and use only a current-limited,
non-battery simulator until the release gate in `docs/BATTERY_MEASUREMENT.md`
closes.

The controlled experiment matrix is
[`docs/BALANCE_GROUND_FAULT_ANALYSIS.md`](../../../docs/BALANCE_GROUND_FAULT_ANALYSIS.md).

J4 and J5 deliberately connect only pin 2 ground and pin 3 CVBS. All other pins
are NC. The SteadyView X receiver is powered through its documented 6.5–26 V
barrel/XT60 ground-station harness, not Test PCB 5 V. The decoder EVM also keeps
its own documented power/control/MIPI connections. Rev A therefore proves the
analog handoff without inventing either module's internal pinout.

## Board documents and views

- [Layout](LAYOUT.md)
- [Component map](COMPONENT_MAP.md)
- [Build guide](BUILD_GUIDE.md)
- [Release readiness](READINESS.md)
- [Fabrication notes](FABRICATION_NOTES.md)

The committed [top](exports/test-board-rev-a-top.svg),
[bottom](exports/test-board-rev-a-bottom.svg), and
[component-map](exports/test-board-rev-a-component-map.svg) SVGs are inspection
views of the placed, unrouted board. They are not fabrication outputs.
