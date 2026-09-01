# Test PCB Rev A release readiness

**State: NOT RELEASED / DO NOT FABRICATE / DO NOT CONNECT A LIPO**

The repository currently contains a deterministic 120 × 90 mm, four-layer,
60-footprint placement with named nets and inspection exports. It contains zero
routed tracks. This is meaningful design progress, not a manufacturing package.

| Gate | State | Required evidence |
| --- | --- | --- |
| Requirements and modular partition | PASS | documented blocks, test access, RF/MIPI kept off-board |
| Exact BOM ↔ footprint coverage | PASS (automated) | `tools/check_test_pcb_bom.py` covers all 60 footprints |
| J4/J5 interface safety | PASS for net intent | only pin 2 GND and pin 3 CVBS; all assumed power/control removed |
| Native schematic and ERC | OPEN | reviewed `.kicad_sch`, zero unexplained ERC findings |
| ADC/MCU pad-map second-person audit | OPEN | signed datasheet/package checklist |
| ACAS array element/pad validation | OPEN | drawing review plus sample continuity evidence |
| Pack-negative fault safety | OPEN / SAFETY | close fault tree and isolated current-limited tests; F2 stays DNP |
| Power/ground/analog routing | OPEN | routed board, planes, impedance/current calculations |
| DRC and netlist comparison | OPEN | clean reports stored with release artifact |
| Exact connector/footprint fit | OPEN | 1:1 print and purchased-component fit log |
| Bring-up firmware on target MCU | OPEN | Nucleo first, then board SWD/CLI/self-test evidence |
| Independent release review | OPEN | signed checklist and tagged fabrication release |

## Video compatibility gate

The standalone SteadyView X specifications establish a credible RF-to-CVBS
stage, but the complete chain is not purchase-cleared. `EVAL-ADV7282AMEBZ` exposes
MIPI CSI-2 on SMA connectors, current distributor availability is restricted, and
the controlled-impedance SMA-to-CM4IO bridge is not designed. Therefore:

- dedicated 5.8 GHz VRX: `WAIT — CHAIN GATE`;
- decoder EVM: `WAIT — STOCK + BRIDGE GATE`;
- bare Test PCB and assembly: `WAIT — PCB RELEASE GATES`.

Compute, CM4IO, touchscreen, MCU development board, and general debugging tools
are independently useful and may be bought under `hardware/BUY_NOW.md`.
