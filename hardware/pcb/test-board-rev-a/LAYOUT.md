# Test PCB Rev A layout

> **PRE-RELEASE VIEW — NOT FOR FABRICATION.** The board is placed and net-assigned
> but has no routed tracks or planes. The views below communicate architecture;
> they do not prove connectivity, signal integrity, clearance, or manufacturability.

## Inspection views

- [Top copper/silkscreen view](exports/test-board-rev-a-top.svg)
- [Bottom copper/silkscreen view](exports/test-board-rev-a-bottom.svg)
- [Component-map view](exports/test-board-rev-a-component-map.svg)

## Functional zones

| Area | Contents | Purpose |
| --- | --- | --- |
| Left | J1, RN1–RN6, R1–R6, C1–C6, TP1–TP25 | protected external-pack input, cumulative-tap dividers, raw and ADC probing |
| Centre | U1 ADS8688A, U2 STM32G0B1, local decoupling | precision conversion and real-time control |
| Lower | J2 bench input, F1/D1/U3, rail LEDs/test points | current-limited bench power and 3.3 V generation |
| Right | J3 SBC, J4 VRX CVBS, J5 decoder CVBS, J6 expansion/debug | modular low-speed and analog handoff interfaces |

The 5.8 GHz RF receiver, decoder EVM, CM4 IO board, and touchscreen are external
modules. RF, MIPI CSI-2, and MIPI DSI are deliberately absent from Rev A.

## Video handoff intent

```text
5.8 GHz antennas
       │ 50 Ω RF
       ▼
Skyzone SteadyView X (separately powered, documented 6.5–26 V input)
       │ documented 3.5 mm A/V output; cable contact map must be verified
       │ 75 Ω CVBS
       ▼
J4 VRX CVBS ONLY: pin 2 GND, pin 3 VRX_VIDEO, all other pins NC
       │ board net intent only — no routed copper in this revision artifact
       ▼
J5 DECODER CVBS ONLY: pin 2 GND, pin 3 VRX_VIDEO, all other pins NC
       │ short shielded 75 Ω connection; decoder provides its specified termination
       ▼
ADV7282A-M evaluation hardware (separately powered)
       │ MIPI CSI-2 over SMA — reviewed controlled-impedance bridge still required
       ▼
CM4 IO Board → DSI → HD capacitive touchscreen
```

J4/J5 provide neither module power nor channel control. No SteadyView X RSSI,
serial, enable, or internal pinout is assumed. The Test PCB also does not add a
75 Ω terminator; the actual decoder input and cabling must implement the decoder
manufacturer's reference circuit. Until routing, J4 and J5 are placement/net
intent—not a functioning CVBS path.

## Placement rules for routing

- Route `VRX_VIDEO` as a short, ground-referenced 75 Ω path only after the chosen
  stack-up and decoder termination are known. Prefer coax for the modular bench.
- Keep the balance-input section away from switching nodes, USB, MIPI, and RF.
- Give every cumulative tap and ADC input a probeable point without long stubs.
- Keep the ADS8688A analog return continuous and separate from switcher hot loops.
- Preserve access to SWD, UART, rail, pack-return, fan, and shutdown test points.
