# Power architecture

![FpvDeck power architecture](assets/diagrams/power-architecture.svg)

## Provisional topology

```text
USB-C receptacle + ESD/CC/PD
       -> charger/power path selected after donor cell topology is measured
       -> protected, serviceable two-cell 18650 pack
              ├─ fused/filtered T8L input (only after original topology verified)
              ├─ TPS55288-class 5.1 V high-current buck-boost -> CM4/display/USB
              ├─ quiet filtered VRX/video rails
              └─ always-on low-power MCU rail -> power button/load switches
```

The provisional diagram does not mean these rails exist in the T8L. The donor's
two-18650 topology and safe added-load budget remain unknown. On the modular bench,
CM4IO, the dedicated 5.8 GHz VRX, decoder EVM, and Test PCB use their separately
documented/current-limited supplies. External balance taps are measurement inputs
only and never power any FpvDeck rail.

BQ25792 is an active 1–4 cell, 5 A buck-boost charger with NVDC power path,
integrated FETs/ADC, I²C, 3.6–24 V input, and USB-PD-range OTG. It does not itself
remove the need for correct USB-C policy/protection, pack protection, thermistors,
fusing, cell matching, regulatory work, and safe mechanical cell retention. It
is only a valid lead after confirming whether the T8L's two cells are series or
parallel and whether its existing charging circuit stays in use.

TPS55288 is an active four-switch 2.7–36 V buck-boost with programmable current
limit and spread spectrum. Its 16 A figure is switch/inductor current, not a claim
that any board can deliver 16 A at 5 V. Inductor, MOSFETs, copper, thermal design,
stability, peak battery current, and EMI must be calculated and measured.

## First-pass power budget

| Load | Typical estimate | Peak estimate | Confidence |
| --- | ---: | ---: | --- |
| CM4 + eMMC | 3.5 W | 7 W | platform-dependent estimate |
| 5.5–6.25-inch display/backlight | 3.5 W | 7 W | envelope estimate; **NEEDS MEASUREMENT** |
| SteadyView X | 2.9 W | 3.5 W | 12 V × 240 mA published typical |
| decoder + carrier | 0.4 W | 0.8 W | estimate |
| T8L/ELRS | 0.7 W | 1.5 W | **NEEDS MEASUREMENT** |
| MCU/ADC/storage/misc. | 0.8 W | 2 W | estimate |
| **system before conversion loss** | **10.8 W** | **19.8 W** | not measured |

The required two-cell pack using 3 Ah, 3.6 V cells stores about 21.6 Wh nominal,
regardless of whether the cells are 2S1P or 1S2P. At 11–16 W input, before reserve,
age, cold-weather, and conversion losses, this suggests roughly 1.0–1.7 hours.
The initial runtime gate is therefore ≥1.25 h at typical FPV+DVR load, with a
1.5 h stretch goal. Lower screen brightness, a more efficient VRX rail, and
measured compute tuning matter. Adding cells is not assumed because the product
requirement specifies the T8L's two internal 18650 cells.

The exact pack topology, protection, charger, and power-path IC are **UNKNOWN**
until the donor is measured non-destructively. Bench Prototype 1 uses isolated,
current-limited supplies; it does not connect to or charge the T8L cells.

## Shutdown state machine

Short power press wakes the MCU and enables compute. A normal off request makes
the MCU send `ShutdownRequest`; Linux stops recording, commits SQLite, syncs and
unmounts writable data, then returns `ShutdownReady`. The MCU waits for rail-good/
heartbeat loss and removes the compute load while keeping charger/wake logic alive.

A ≥8 s hold is emergency forced-off. The MCU records the cause and removes power
even without Linux acknowledgement. The next boot runs filesystem/database checks
and presents a recovery notice. Brownout prediction requests an expedited shutdown;
DVR segmentation limits the unrecoverable interval.
