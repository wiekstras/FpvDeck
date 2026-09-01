# Prototype stages and exit criteria

## Prototype 0 — desktop

**BOM:** normal Linux/macOS computer, Qt 6, FFmpeg. **Connections:** none.
**Tests:** C++/C units, SQLite migration, QML runtime, simulated fault scenarios.
**Risk:** desktop multimedia behavior does not prove target zero-copy behavior.
**Exit:** all core apps navigate without hardware, overlays remain independent of
video, persistent flight records work, simulator faults are repeatable, CI green.

## Prototype 1 — modular bench

**BOM:** CM4/IO carrier, selected 6.25-inch DSI touch display, ADV7282A-M evaluation
board, SteadyView X ground station, STM32G0 devboard, two ADC EVMs, isolated bench
supplies, RF dummy/attenuation/test equipment, logic analyzer and scope.

**Connections:** documented module cables only. Separate current-limited supplies
until grounds/load behavior are validated. No unknown T8L connection.

**Tests:** PAL/NTSC lock/recovery; photodiode latency; DMABUF queue; DVR stress;
VRX sensitivity/mode latency; DSI scan; conducted noise; ADC precision/faults;
MCU protocol/reconnect; T8L coexistence.

**Exit:** p95 latency path meets gate or a documented redesign is chosen; 8-hour
video/DVR soak; 1,000 signal reconnects; power budget measured; Prototype 1 wiring
and purchase BOM frozen.

## Prototype 2 — Controller-I/O Rev A

**BOM:** custom PCB with STM32G0, selected ADC/protected frontend, controls,
temperature/fan, power state logic, USB CDC/debug, modular VRX/T8L/compute headers.

**Tests:** ERC/footprint audit before order; boundary scan where possible; current-
limited bring-up; precision source calibration; arbitrary balance hot-plug faults;
HIL buttons/watchdog/shutdown; ESD pre-check; self-test coverage.

**Exit:** no unsafe fault current, calibration target met over temperature, 1,000
shutdown/brownout cycles, recovery/update method proven, all test points accessible.

## Prototype 3 — compute carrier

**BOM:** CM4 carrier, CSI/DSI, power, eMMC module, USB-C/data, decoder or decoder
module, debug/test connectors and shielding options. VRX remains modular.

**Tests:** impedance/layout review; rail sequencing; high-speed eye/compliance where
applicable; full latency/thermal/EMC repeat; RF degradation A/B; storage endurance.

**Exit:** 100-hour soak and thermal margin, no material RF loss, reproducible image,
all carrier interfaces pass manufacturing diagnostics.

## Prototype 4 — integrated candidate

Integrate only proven circuits. Complete DFM/DFT, enclosure/drop/ingress/ergonomic
tests, compliance pre-scan, service procedure, supply-chain audit and pilot build.
