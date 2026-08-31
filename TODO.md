# Engineering TODO

Items remain here until completed and evidenced by a test, document, or artifact.

## Milestone 1 — desktop vertical slice

- [x] Build and launch FPVDeck on Linux with `./scripts/dev`.
- [x] Implement shell navigation: FPV, Battery, Flights, DVR, Aircraft, Settings.
- [x] Render synthetic moving video with a color overlay that remains interactive.
- [x] Simulate PAL/NTSC, weak signal, loss, rolling, black frames, and reconnect.
- [x] Simulate 1S–6S packs, imbalance, unplug, under/overvoltage, noise, and bad taps.
- [x] Implement SQLite schema, versioned migrations, and initial flight persistence.
- [x] Add keyboard controls and development-only simulator panel.
- [x] Add unit, integration, protocol, database, service, and QML smoke tests.
- [x] Run CI-equivalent build/test locally and fix all failures.
- [ ] Add full battery/aircraft CRUD repositories and editing UI.
- [ ] Implement actual test-file DVR segments and interruption recovery tests.
- [ ] Add QML interaction/screenshot tests, not only graph startup.
- [ ] Add simulated source disconnect/reconnect and resolution-change assertions.
- [ ] Verify and package the macOS development path.

## Milestone 2 — bench architecture

- [x] Establish initial compute/decoder/display/VRX shortlist from primary sources.
- [ ] Obtain/borrow ADV7280A-M or ADV7282-M evaluation hardware.
- [ ] Confirm exact CSI cable/adapter wiring from board documentation; do not infer.
- [ ] Buy or borrow SteadyView X and at least one benchmark receiver.
- [ ] Measure VRX mode latency, decoder latency, display latency, lock/recovery time.
- [ ] Test PAL and NTSC with weak/nonstandard VTX signals.
- [ ] Verify CM4 hardware encoder can record without adding display-path buffers.
- [ ] Prototype pre-overlay and composited recording concurrently.
- [ ] Freeze Prototype 1 BOM only after source/stock checks at purchase time.
- [ ] Produce a verified wiring document with connector part numbers.

## T8L reverse engineering

- [ ] Photograph both PCB sides at high resolution with scale reference.
- [ ] Measure PCB, gimbal, mount, switch, antenna, connector, and battery geometry.
- [ ] Confirm whether the two 18650 cells are series or parallel by measurement.
- [ ] Determine USB behavior: RF active/inactive in HID/VCP/web configuration modes.
- [ ] Identify connector grounds and rails power-off before any powered probing.
- [ ] Determine whether CRSF telemetry is accessible without disturbing RF timing.
- [ ] Characterize T8L current at each RF power and operating mode.

## Electrical and PCB

- [ ] Bench-evaluate ADS7066 and ADS8688A frontends before selecting ADC topology.
- [ ] Specify hot-plug/reverse/ESD protection with worst-case fault calculations.
- [ ] Build 1S–8S precision source test jig and calibration procedure.
- [x] Select STM32G0B1 family lead and add host-tested firmware protocol core.
- [ ] Freeze exact STM32G0B1 package/part after I/O and flash budget.
- [ ] Finalize 2S/2S2P power budget from measured module consumption.
- [ ] Select charger, USB-C PD, 5 V converter, quiet RF/analog rails, and load switches.
- [ ] Create Controller-I/O Rev A schematics only from verified datasheets.
- [ ] Run ERC, peer checklist, footprint audit, and bring-up review before fabrication.

## Reliability and release

- [ ] Implement watchdog, bounded service restarts, safe shutdown, and brownout tests.
- [ ] Define performance benchmark tooling and acceptance thresholds.
- [ ] Add HIL protocol and manufacturing diagnostics.
- [ ] Create reproducible embedded image and A/B update/recovery design.
- [ ] Add KiCad CI checks and firmware cross-build once toolchains are pinned.
