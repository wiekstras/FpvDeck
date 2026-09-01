# Roadmap

## M0 — desktop foundation (usable simulator)

Qt/QML touch shell, synthetic file video, color overlay, FPV controls/flight lock,
battery/media/storage/receiver/diagnostic faults, SQLite flight history, keyboard
inputs, binary protocol framing, and automated build/runtime tests are implemented.
Remaining: richer aircraft/battery CRUD, real file recording, pointer-driven QML
tests, macOS verification, and source reconnect/resolution tests.

## M0.5 — Prototype 0 real-video bench

The Raspberry Pi 5, official 5-inch capacitive display, RC832 Mini and
UVG-002 reference chain is selected and order-documented. V4L2 enumeration,
raw preview, hardware-mode launch, missing-device fallback, diagnostics and a
pre-overlay Qt recorder branch are implemented. Next: receive parts, continuity-
check the receiver power lead, show real Air65 video/OSD, capture frame/drop and
recovery evidence, and measure the deliberately unoptimized latency baseline.

## M1 — measured low-latency bench video

Acquire the selected decoder/VRX/display/CM4 hardware; build the latency fixture;
bring up V4L2; implement bounded DMABUF display; measure all modes; implement
hardware encode/segmented DVR; and freeze the Prototype 1 wiring/BOM.

## M2 — Test PCB / Controller-I/O Rev A

The placed/unrouted Test PCB project, exact-MPN BOM, ADC error model, bring-up
command core, host self-test, and staged procedure exist. Next: resolve protected
pack reference, complete native schematic/ERC and pin/footprint audit, route/DRC,
then release and perform HIL bring-up. The later controller board adds complete
controls, thermals, watchdog, and power sequencing after these circuits pass.

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
