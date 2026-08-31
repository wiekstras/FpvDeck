# Hardware architecture

## Current selections

| Function | Prototype 1 | Production-oriented lead | Status |
| --- | --- | --- | --- |
| Compute | Raspberry Pi CM4, 4 GB/32 GB eMMC, no wireless preferred | i.MX 8M Plus industrial SoM | Prototype selected; production shortlist |
| CVBS decode | ADV7280A-M or ADV7282-M evaluation hardware | Renesas TW9992 | Prototype selected; production driver work open |
| VRX | Skyzone SteadyView X standalone ground-station kit | modular dual-VRX bay; integrated RF later | Selected pending latency A/B |
| Display | 5-inch Raspberry Pi Touch Display 2 or Waveshare 5-inch DSI D | Winstar WF50DSYA3MNN0, 1100-nit IPS | Leads; all latency unmeasured |
| Controller MCU | STM32G0B1 development board | STM32G0B1CB/CE class | Selected family; exact package after pin budget |
| Battery ADC | ADS7066 and ADS8688A EVM A/B | not frozen | Bench decision required |
| Main battery | bench supplies, then protected 2S2P 18650 fixture | serviceable protected 2S2P | Cell and regulatory design open |
| Charger | BQ25792 evaluation | BQ25792-class 1–4S buck-boost/NVDC | Lead; not schematic-approved |
| 5 V rail | TPS55288 evaluation | TPS55288-class buck-boost | Lead; load/EMI validation open |

CM4 is preferred over CM5 for Prototype 1 because CM4's documented H.264 encoder
and established ADV728x capture path reduce two critical risks. CM5 remains a CPU
performance benchmark, not the baseline DVR platform.

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

