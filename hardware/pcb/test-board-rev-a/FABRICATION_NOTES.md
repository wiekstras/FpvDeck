# Fabrication and assembly notes — pre-release

- The project declares four copper layers. A 1.6 mm FR-4, 1 oz outer-copper board
  is the current planning stack. Final dielectric/copper stack is not frozen.
- Minimum planning rules are 0.20 mm clearance and 0.20 mm trace; analog/power
  traces will be widened during routing.
- Assemble MCU, regulator, power protection and headers first. Leave ADC/divider
  section DNP until 5 V/3.3 V and USB/UART are verified.
- J1 must be exact JST `B7B-XH-A(LF)(SN)`. Print the land pattern at 1:1 and place
  the purchased connector before release.
- RN1–RN6 are exact Vishay `ACASA1002U1002P1AT`; do not substitute a bussed array.
- F2 is an experimental `1206L010/60WR` pack-reference link and is DNP. Its
  generic KiCad land pattern must be checked against the Littelfuse drawing, and
  it must not be fitted for initial power-up or real-LiPo use.
- Clean flux around high-impedance analog nodes and inspect at magnification.
- No conformal coat until leakage and calibration stability have been measured.
- Production files require schematic/ERC, PCB DRC, netlist comparison, fab drawing,
  drill review, solder-paste review and an independent release signature.
