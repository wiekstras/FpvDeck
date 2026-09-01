# Engineering TODO

Items remain here until completed and evidenced by a test, document, or artifact.

## Milestone 1 — desktop vertical slice

- [x] Build and launch FPVDeck on Linux with `./scripts/dev`.
- [x] Implement shell navigation: Home, FPV, Balance, Media, Flights, Batteries,
  Aircraft, DVR, Diagnostics, and Settings.
- [x] Render synthetic moving video with a color overlay that remains interactive.
- [x] Simulate PAL/NTSC, weak signal, loss, rolling, black frames, and reconnect.
- [x] Simulate 1S–6S packs, imbalance, unplug, under/overvoltage, noise, and bad taps.
- [x] Implement SQLite schema, versioned migrations, and initial flight persistence.
- [x] Add keyboard controls and development-only simulator panel.
- [x] Add unit, integration, protocol, database, service, and QML smoke tests.
- [x] Run CI-equivalent build/test locally and fix all failures.
- [x] Add deterministic demo states and reproducible 1280×720 showcase captures.
- [x] Add touchscreen-first navigation, FPV auto-hide controls, flight lock,
  removable-SD/media simulation, receiver scan/favorites, and fault injection.
- [x] Add service tests for touch timing/lockout, storage/media faults, deck state,
  receiver selection, and battery extrema.
- [ ] Add full battery/aircraft CRUD repositories and editing UI.
- [ ] Implement actual test-file DVR segments and interruption recovery tests.
- [ ] Add pointer-driven QML interaction/golden tests; service behavior and all
  deterministic page states are covered, but current QML tests still launch
  complete graphs rather than synthesizing taps/swipes.
- [ ] Add simulated source disconnect/reconnect and resolution-change assertions.
- [ ] Verify and package the macOS development path.

## Milestone 2 — bench architecture

- [x] Establish initial compute/decoder/display/VRX shortlist from primary sources.
- [x] Select the currently shipping Waveshare 6.25inch DSI LCD (B), SKU 35000,
  as the Prototype 1 touchscreen module from documented display/touch support.
- [ ] Measure/fit the 6.25-inch module and compare it against the 5-inch bright
  fallback; the production outdoor panel remains measurement-gated.
- [ ] Price the SMA-to-CM4 CSI fixture, then obtain/borrow `EVAL-ADV7282AMEBZ`
  or the exact older Pi-tested `EVAL-ADV7282MEBZ` only after stock confirmation.
- [ ] Confirm exact CSI cable/adapter wiring from board documentation; do not infer.
- [ ] Buy or borrow SteadyView X and at least one benchmark receiver.
- [ ] Measure VRX mode latency, decoder latency, display latency, lock/recovery time.
- [ ] Test PAL and NTSC with weak/nonstandard VTX signals.
- [ ] Verify CM4 hardware encoder can record without adding display-path buffers.
- [ ] Prototype pre-overlay and composited recording concurrently.
- [x] Create a part-numbered bench system BOM, Test PCB BOM, human order list,
  and procurement staging plan.
- [ ] Refresh distributor stock/pricing and ADV728x EVM/interface availability at
  purchase time; do not treat documented approximate prices as live quotations.
- [ ] Produce a verified wiring document with connector part numbers.

## T8L reverse engineering

- [ ] Photograph both PCB sides at high resolution with scale reference.
- [ ] Measure PCB, gimbal, mount, switch, antenna, connector, and battery geometry.
- [ ] Confirm whether the two 18650 cells are series or parallel by measurement.
- [ ] Determine USB behavior: RF active/inactive in HID/VCP/web configuration modes.
- [ ] Identify connector grounds and rails power-off before any powered probing.
- [ ] Determine whether CRSF telemetry is accessible without disturbing RF timing.
- [ ] Characterize T8L current at each RF power and operating mode.
- [ ] Run thumb/pinch user trials and measure gimbal finger envelopes, palm support,
  grip angle, center of mass, neck-strap balance, and screen sight line.

## Repository and project presentation

- [x] Configure canonical `origin` for `git@github.com:wiekstras/FpvDeck.git`
  after inspecting local remotes and branches.
- [x] Add a status-accurate product README, concept disclaimer, editable branding,
  source-controlled diagrams, UI captures, contributor guide, security policy,
  issue forms, and pull-request checklist.
- [x] Preserve the existing MIT license while renaming the contributor identity to
  FpvDeck.
- [ ] Export/upload the source-controlled social preview in GitHub repository settings.
- [ ] Replace concept renders with measured CAD renders after T8L and module geometry
  is captured; retain concept history and labeling.

## Electrical and PCB

- [x] Select ADS8688A for Test PCB Rev A and model the calibrated divider/ADC
  error budget; retain ADS7066 as a lower-power future integration candidate.
- [x] Create a real Test PCB Rev A KiCad PCB/project with verified lead packages,
  explicit nets, probe points, modular headers, and manufacturing gates.
- [x] Add CI verification that the Test PCB BOM quantities/reference ranges match
  every footprint in the committed generated board.
- [ ] Create the native KiCad schematic from the independently checked pin/net
  map; run ERC before routing. The placed/unrouted board is **NOT RELEASED**.
- [ ] Independently verify ACAS 0612 resistor-array pad pairing and every ADC/MCU
  pad mapping against manufacturer package drawings.
- [ ] Resolve protected external-pack negative/reference connection and prove no
  unsafe backfeed path before any LiPo connection. Rev A now has DNP
  `1206L010/60WR` plus TP33/TP34 for a current-limited experiment; this does not
  close the release gate.
- [ ] Route planes/tracks, run DRC, print 1:1 footprints, and complete independent
  design review before generating an orderable fabrication release.
- [ ] Bench-evaluate the ADS8688A frontend and compare measured calibration drift
  with `tools/balance_frontend_error.py`.
- [x] Add normal-state pack-return/PPTC-drop calculations and a controlled
  ground-fault experiment matrix; hardware validation and gate closure remain open.
- [ ] Specify hot-plug/reverse/ESD protection with worst-case fault calculations.
- [ ] Build 1S–8S precision source test jig and calibration procedure.
- [x] Select STM32G0B1 family lead and add host-tested firmware protocol core.
- [ ] Freeze exact STM32G0B1 package/part after I/O and flash budget.
- [ ] Finalize the required two-cell 18650 power budget from measured module
  consumption and confirmed donor series/parallel topology.
- [ ] Select charger, USB-C PD, 5 V converter, quiet RF/analog rails, and load switches.
- [ ] Create Controller-I/O Rev A schematics only from verified datasheets.
- [ ] Run ERC, peer checklist, footprint audit, and bring-up review before fabrication.

## Reliability and release

- [ ] Implement watchdog, bounded service restarts, safe shutdown, and brownout tests.
- [x] Add repeated desktop first-frame/RSS benchmark tooling and initial system
  acceptance thresholds. Touch-to-pixel, frame pacing and target-hardware probes
  remain to implement.
- [x] Add a versioned bring-up command subset, host-tested board dispatch layer,
  simulated `fpvdeck-hw-test`, and a staged hardware bring-up procedure.
- [x] Align firmware/host ADC identities with the audited board map: B− on ADC0,
  deck monitor on ADC1, and balance taps on ADC2–ADC7.
- [ ] Bind firmware board HAL callbacks to STM32G0 peripherals and validate the
  commands on NUCLEO-G0B1RE before porting to the Rev A MCU footprint.
- [ ] Run `fpvdeck-hw-test` against physical hardware and add fixture-assisted
  per-channel pass/fail limits.
- [ ] Create reproducible embedded image and A/B update/recovery design.
- [ ] Add KiCad CI checks and firmware cross-build once toolchains are pinned.
