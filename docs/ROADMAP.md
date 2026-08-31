# Roadmap

## M0 — desktop foundation (in progress)

Qt/QML shell, synthetic file video, color overlay, battery/DVR/video faults,
SQLite migration and flight history, keyboard inputs, binary protocol framing,
and automated build/runtime tests are implemented. Remaining: richer aircraft/
battery CRUD, telemetry simulator, real file recording, QML interaction tests,
macOS verification, and source reconnect/resolution tests.

## M1 — measured bench video

Acquire the selected decoder/VRX/display/CM4 hardware; build the latency fixture;
bring up V4L2; implement bounded DMABUF display; measure all modes; implement
hardware encode/segmented DVR; and freeze the Prototype 1 wiring/BOM.

## M2 — Controller-I/O Rev A

Protected balance measurement, MCU, dedicated controls, thermals, watchdog, power
button/sequencing, debug headers, test points, and modular VRX/compute connectors.
Complete ERC/footprint/design review before fabrication, then HIL bring-up.

## M3 — compute carrier

CM4 carrier with proven CSI/DSI routing, storage, USB-C/data/debug, power partition,
and mechanical stack. Measure EMC impact on both RF links. Evaluate i.MX 8M Plus
production SoMs using the same decoder/display fixtures.

## M4 — integrated field prototype

Custom enclosure, serviceable pack, sunlight/thermal/rain ingress evaluation,
field flight logging, recovery image, manufacturing self-test, and multi-unit soak.

## M5 — production candidate

Supply-chain/lifecycle audit, compliance pre-scan, DFM/DFT, controlled release
files, reproducible OS/firmware builds, security/update policy, and pilot build.

