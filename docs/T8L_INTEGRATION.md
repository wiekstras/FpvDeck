# RadioMaster T8L integration

## Confirmed public facts

RadioMaster's current product page and 2026 manual specify a 165×120×70 mm,
204 g transmitter with X5 Hall gimbals, two 18650 cells, a 6.0–8.4 V operating
range, USB-C charge/update/simulator functions, SX1281-based 2.4 GHz ELRS, and up
to 100 mW RF power. The firmware is a special T8L edition configured by a USB
web UI rather than EdgeTX.

A published teardown identifies an ESP8285 and SX1281 and shows a single main
PCB, plugged gimbal/trim assemblies, some soldered switches, ceramic 2.4 GHz ELRS
antenna, separate PCB Wi-Fi antenna, and an unlabeled connector. This teardown is
useful orientation but not authority for a pinout.

Sources:

- <https://radiomasterrc.com/products/t8l-radio-controller>
- <https://radiomasterrc.com/pages/user-manuals>
- <https://oscarliang.com/radiomaster-t8l/>

## V1 reuse decision

Reuse the complete T8L mainboard, two Hall gimbals, useful controls, associated
2.4 GHz ELRS antenna/RF path, and its two-cell compartment. Existing charge/USB
behavior is reused only where measurement confirms it remains safe with the added
load. Mount the FPV computer as an electrically separate subsystem. Shared power
is allowed only after load, ground, charging, brownout, and conducted-noise tests.

The T8L contains **no FpvDeck analog-video subsystem**. It does not provide the
5.8 GHz VRX, CVBS decoder, compute, touchscreen, removable media, or external
balance checker. ELRS control and 5.8 GHz analog video are independent RF paths.

The Linux/MCU side does not sit in the stick-to-ELRS path. Read-only T8L status may
come first from USB HID/VCP if tests prove RF remains fully active and timing is
unaffected. Telemetry/CRSF tapping is deferred and optional.

## Unknowns

- PCB/gimbal/mount/connector dimensions: **NEEDS MEASUREMENT**.
- Two-cell electrical topology and protection: **NEEDS MEASUREMENT**.
- Unlabeled connector pinout/function: **UNKNOWN; DO NOT CONNECT**.
- Internal rail voltages and current margin: **NEEDS MEASUREMENT**.
- Whether USB simulator/VCP mode leaves RF active: **NEEDS MEASUREMENT**.
- Accessible CRSF UART/telemetry and voltage levels: **UNKNOWN**.
- T8L schematic/board files and RF layout stackup: **UNKNOWN**.

## Antenna/EMC rules

Keep the 2.4 GHz antenna in its original geometry and free-space boundary. Do not
place a display FPC, metal heatsink, battery, shield, or 5.8 GHz antenna inside
its near field without chamber/field comparison. Disable CM4 Wi-Fi by default;
if later enabled, use an external antenna isolated from ELRS and prove link margin.

Keep switcher hot loops, CSI/DSI, USB, and high-edge-rate clocks away from both
2.4 GHz ELRS transmit/control and 5.8 GHz analog receive paths. Filter VRX/decoder
rails, use shield footprints
where measurements justify them, and compare receiver noise floor with each
digital subsystem enabled/disabled.
