# FpvDeck documentation

FpvDeck has two independent RF paths: the donor T8L supplies 2.4 GHz ELRS radio
control, while FpvDeck adds its own 5.8 GHz analog video receiver. The external
balance port measures an aircraft battery only; it never powers the deck.

## Start here

- [System architecture](ARCHITECTURE.md)
- [Hardware architecture](HARDWARE_ARCHITECTURE.md)
- [Software architecture](SOFTWARE_ARCHITECTURE.md)
- [Roadmap](ROADMAP.md), [risk register](RISK_REGISTER.md), and
  [repository TODO](../TODO.md)

## Video, display, and latency

- [Video pipeline](VIDEO_PIPELINE.md)
- [Latency budget and measurement](LATENCY.md)
- [Performance targets](PERFORMANCE.md)
- [Touch interaction design](TOUCH_UX.md)

## Donor, power, and measurement

- [T8L integration boundary](T8L_INTEGRATION.md)
- [Safe T8L reverse engineering](REVERSE_ENGINEERING.md)
- [Power architecture](POWER.md)
- [External battery measurement](BATTERY_MEASUREMENT.md)
- [Balance ground-fault analysis](BALANCE_GROUND_FAULT_ANALYSIS.md)

## Software and testing

- [Application/service API](APP_API.md)
- [Data model](DATA_MODEL.md)
- [MCU protocol](MCU_PROTOCOL.md)
- [Simulator](SIMULATOR.md)
- [Build](BUILD.md), [development](DEVELOPMENT.md), and [testing](TESTING.md)

## Hardware prototypes

- [Prototype stages](PROTOTYPES.md)
- [Test PCB bring-up](TEST_PCB_BRINGUP.md)
- [Test PCB Rev A project](../hardware/pcb/test-board-rev-a/README.md)
- [EU order list](../hardware/ORDER_LIST_EU.md) and
  [procurement gates](../hardware/PROCUREMENT_PLAN.md)
- [Component research](../hardware/research/COMPONENT_SELECTION.md)
