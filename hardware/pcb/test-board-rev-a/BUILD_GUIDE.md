# Test PCB Rev A build guide

> **DO NOT BUILD THIS REVISION YET.** This guide defines the future staged build
> order and the evidence needed at each stop. `READINESS.md` is authoritative.

## Before ordering

1. Complete the native KiCad schematic and pass ERC.
2. Independently audit every symbol pin and exact footprint against its current
   manufacturer drawing; print the board 1:1 and place physical samples.
3. Close the pack-negative/reference fault analysis. F2 remains DNP.
4. Route planes/tracks, establish the stack-up, run DRC and peer-review the design.
5. Generate fabrication outputs only from a signed release tag.

## Staged assembly

1. Inspect the bare board and measure every rail to ground before population.
2. Fit only bench-input protection, 3.3 V regulation, decoupling, rail LEDs, and
   test points. Power from a current-limited isolated bench supply.
3. Fit the STM32, SWD and UART. Flash bring-up firmware and exercise LEDs,
   buttons, fan output, temperature input, heartbeat and shutdown signals.
4. Fit the ADC and SPI passives. Verify reference and supply pins before commands.
5. Fit one divider channel at a time and inject cumulative voltages using an
   isolated, current-limited simulator. Compare raw code and calibrated voltage.
6. Populate all six channels only after the single-channel error and leakage tests
   pass. Exercise missing/partial taps without a battery connected.
7. Connect J4/J5 only with a continuity-checked shielded adapter. Independently
   power the VRX and decoder according to their own manuals. Never energize an NC
   pin or infer a 3.5 mm plug contact assignment.

## Prohibited during initial bring-up

- no real LiPo on J1;
- no T8L internal power wiring;
- no F2 population;
- no earthed oscilloscope ground on an unreviewed pack node;
- no VRX or decoder power through J4/J5;
- no improvised wire bundle for MIPI CSI-2 or DSI.

Follow the complete [bring-up procedure](../../../docs/TEST_PCB_BRINGUP.md) after
release. Stop immediately on unexpected current, heating, rail voltage, or a
continuity result that disagrees with the schematic/netlist.
