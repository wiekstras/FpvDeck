# Prototype procurement plan

## Order now

These items unlock independent work even if the custom PCB changes:

- Raspberry Pi CM4 eMMC module and official CM4 IO Board;
- Waveshare 6.25inch DSI LCD (B), exact SKU 35000;
- Skyzone SteadyView X kit and correctly gendered antennas;
- NUCLEO-G0B1RE, STLINK-V3MINIE and 3.3 V USB-UART;
- 75 Ω coax/adapters/terminator;
- JST XH connector samples and non-conductive bench hardware;
- high-endurance removable microSD;
- exact Test PCB semiconductors/passives in small spare quantities if shipping
  consolidation makes sense. These parts remain useful for breadboard/EVM work.

Immediate exit tests: display/touch boots in landscape, CM4 runs simulator,
firmware protocol runs on Nucleo, VRX produces terminated CVBS, and all item labels
match their order records.

## Verify stock and interface before order

- Analog Devices `EVAL-ADV7282AMEBZ` or exact Pi-tested `EVAL-ADV7282MEBZ`.
  Obtain the exact user guide, board revision and seller stock confirmation.
  These boards expose MIPI lanes on SMA; price and design a controlled-impedance
  SMA-to-CM4IO CSI fixture before committing to the EVM purchase.
- ADS8688EVM-PDK. It is valuable but expensive; skip if unavailable or if Rev A
  board release is close.
- Any 5.8 GHz antenna whose connector gender/polarization is not explicit.

## Order after Test PCB passes release review

- Test PCB Rev A bare boards, quantity 5;
- board-specific stencils and assembly tooling;
- exact SWD header/cable after its 1:1 footprint check;
- full quantities from `TEST_PCB_REV_A.csv`;
- a second ADC/MCU as immediate rework stock.

## Order after Test PCB bring-up passes

- components for a revised routed controller/power board;
- candidate 2S charger and 5 V buck-boost EVMs;
- protected cell fixture parts and thermal hardware;
- enclosure prototype materials tied to measured CAD.

## Wait

- final 18650 cells/pack, charging USB-C PD solution and production power PCB;
- custom integrated VRX RF circuitry;
- CM4 production carrier high-speed connectors;
- final enclosure, machined parts and final-length flex cables;
- T8L internal wiring harnesses based on unconfirmed pinouts.

The purpose of `WAIT` is to prevent sunk cost, not to stall bench work. Display,
compute, receiver, Nucleo and measurement equipment can be tested independently.
