# Bench and bring-up tools

This list distinguishes required capabilities from optional premium brands. Use
existing suitable instruments rather than buying duplicates.

## Required before Test PCB power-up

- current-limited 0–15 V laboratory supply with readable voltage/current;
- CAT-rated DMM with millivolt resolution, resistance and diode modes;
- ST-LINK: `STLINK-V3SET` is the robust purchase lead; the Nucleo onboard ST-LINK
  is sufficient for early firmware work;
- 3.3 V USB-UART adapter with selectable I/O voltage;
- ESD mat/wrist strap, magnification, fine soldering/rework tools;
- 1:1 printer/caliper for footprint fit verification;
- inline fuse leads, shrouded 4 mm leads, nonconductive base and standoffs.

## Required before analog-video integration

- 100 MHz or faster oscilloscope with a true 75 Ω termination/feed-through;
- short, known 75 Ω coax and connector adapters;
- continuity tester for the supplied SteadyView 3.5 mm video cable;
- PAL/NTSC VTX or calibrated composite generator;
- high-speed camera initially, then the documented LED/photodiode latency fixture.

## Strongly recommended

- eight-channel 24 MS/s or faster logic analyzer for SPI/UART/I²C timing;
- isolated/differential measurement capability for switching-power and pack work;
- thermal camera or contact thermocouples;
- programmable or multi-channel isolated source for cumulative-tap injection.

An earthed oscilloscope ground must never be clipped to an unreviewed LiPo node.
Initial balance tests use an isolated current-limited simulator—not a real pack.
See [ground-fault analysis](../docs/BALANCE_GROUND_FAULT_ANALYSIS.md).
