# Test PCB Rev A component map

![Placed top-side component map](exports/test-board-rev-a-component-map.svg)

This map is generated from the committed KiCad board. It shows where subsystems
are intended to live; the board remains unrouted and unreleased.

| Block | References | Notes |
| --- | --- | --- |
| Balance connector | J1 | exact JST `B7B-XH-A(LF)(SN)`; external pack is measurement-only |
| Tap protection/dividers | RN1–RN6, R1–R7, C1–C6 | exact matched arrays; B− sense is separate from the DNP return experiment |
| Precision ADC | U1 | ADS8688AIDBT, ADC0 B−, ADC1 deck monitor, ADC2–ADC7 tap 1–6 |
| Hardware controller | U2 | STM32G0B1CBT6 plus SWD/UART/test access |
| Bench power | J2, F1, D1, U3 | board-development input; not a frozen T8L power design |
| Experimental return | F2, TP33, TP34 | DNP; only for current-limited fault experiments |
| Linux interface | J3 | project-defined low-speed SBC communication/control |
| 5.8 GHz VRX handoff | J4 | CVBS-only: 2=GND, 3=CVBS, others NC; receiver powered externally |
| Decoder handoff | J5 | CVBS-only: 2=GND, 3=CVBS, others NC; decoder powered externally |
| Expansion/debug | J6 and test points | bring-up access; verify every mating cable before use |

## Off-board modules

The following are not components supplied by the T8L and are not integrated on
Rev A: the dedicated 5.8 GHz analog VRX, CVBS decoder EVM, CM4/CM4IO, HD
touchscreen, removable-media reader, and their power supplies. The T8L contributes
the independent 2.4 GHz ELRS control radio, its RF path, gimbals/controls, and
potentially its two-cell battery source after measurement.
