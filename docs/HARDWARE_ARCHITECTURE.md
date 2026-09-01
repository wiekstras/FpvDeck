# Hardware architecture

## Current selections

| Function | Prototype 1 | Production-oriented lead | Status |
| --- | --- | --- | --- |
| Compute | Raspberry Pi CM4, 4 GB/32 GB eMMC, no wireless preferred | i.MX 8M Plus industrial SoM | Prototype selected; production shortlist |
| CVBS decode | EVAL-ADV7282AMEBZ candidate; older EVAL-ADV7282MEBZ is Pi-tested baseline | Renesas TW9992 | NRND prototype path; exact EVM stock and SMA-to-CSI fixture open |
| VRX | Skyzone SteadyView X standalone ground-station kit | modular dual-VRX bay; integrated RF later | RF-to-CVBS lead; purchase waits for complete chain gate |
| Display | Waveshare 6.25inch DSI LCD (B), SKU 35000 | 5.5–6.25-inch high-brightness DSI panel | Prototype selected; five-touch required; latency/outdoor/pinch-grip gates open |
| Controller MCU | NUCLEO-G0B1RE then STM32G0B1CBT6 | STM32G0B1CB/CE class | Test-board package selected from current ST pinout |
| Battery ADC | ADS8688A EVM/direct Test PCB | ADS8688A unless measurements reject it | Test-board selected with calibrated matched divider |
| Main battery | bench supplies, then protected two-cell 18650 fixture | serviceable two-cell pack matching verified donor topology | Series/parallel topology **UNKNOWN**; charger not frozen |
| Charger | BQ25792 evaluation | BQ25792-class 1–4S buck-boost/NVDC | Lead; not schematic-approved |
| 5 V rail | TPS55288 evaluation | TPS55288-class buck-boost | Lead; load/EMI validation open |

CM4 is preferred over CM5 for Prototype 1 because CM4's documented H.264 encoder
and established ADV728x capture path reduce two critical risks. CM5 remains a CPU
performance benchmark, not the baseline DVR platform.

The 6.25-inch screen is mounted landscape above the gimbal finger envelopes. Its
159.18 mm cover-lens width is confirmed; T8L placement and pinch-grip clearance are
not. A non-powered ergonomic buck is an enclosure gate.

## Physical partitioning

- ELRS antenna and T8L RF circuitry stay in the original low-noise region.
- 5.8 GHz receive antennas belong above/away from the display, CM4, USB, switching
  nodes, and the 2.4 GHz antenna. One directional and one omnidirectional antenna
  is a useful field configuration; connectors must be keyed/labeled for 50 ohm RF.
- The VRX and CVBS input use a shielded/ground-referenced zone with a dedicated,
  filtered rail. CVBS should not cross switch nodes or MIPI pairs.
- CSI/DSI and USB stay over continuous ground and use manufacturer impedance/
  length guidance. Do not route a split in the return plane below them.
- Battery tap protection and ADC live at the balance connector, away from the RF
  and switcher hot loops. Digital isolation is not assumed necessary at 8S, but
  fault current limiting and ground strategy are mandatory.
- Antennas, heatsinks, shield cans, FFC bends, and connector extraction volumes
  are first-class mechanical keepouts.

## Interfaces

The MCU-to-Linux baseline is USB CDC for Prototype 1 because it provides framing
separation, standard host support, and power-domain visibility. A debug UART is
retained. SPI is reserved as a future performance option, not needed for human-
rate controls/sensors. The VRX interface is initially 75-ohm CVBS plus its native
control mechanism; no undocumented module-bay pins are assumed.

Test PCB Rev A keeps DSI, CSI and RF off-board. Display connects directly to the
CM4IO DSI port, ADV728x evaluation hardware to CSI through a reviewed interposer,
and VRX through 75-ohm coax. The decoder EVM's MIPI SMA ports require a dedicated
controlled-impedance bridge fixture; they are not connected through Test PCB Rev A.
J4/J5 express only the intended CVBS handoff: pin 2 ground and pin 3 video; all
other pins are NC. They provide no module power, RSSI, enable, or undocumented
control signals. The board is still unrouted, so this is net intent only.
