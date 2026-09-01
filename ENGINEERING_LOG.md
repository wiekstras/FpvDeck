# Engineering log

This is the durable chronological record for decisions, evidence, experiments,
and unresolved facts. Status labels are used deliberately:

- **CONFIRMED** — supported by a primary source or a repeatable test.
- **LIKELY** — supported by indirect evidence; verify before depending on it.
- **UNKNOWN** — no defensible evidence yet.
- **NEEDS MEASUREMENT** — must be established on the physical unit or bench.

## 2026-08-31 — Repository bootstrap

- The supplied repository directory was empty and was not a Git repository.
- Temporary product codename: **OpenPocket**. The application platform is
  **FPVDeck**. Other candidates retained for later consideration: FieldView,
  VectorDeck, and PilotGlass.
- Selected Qt 6 + QML for the first desktop application. Qt gives a direct
  embedded-Linux path, a scene graph suited to composited overlays, hardware
  input support, SQLite integration, and a C++ escape hatch for zero-copy video.
- The desktop milestone uses C++ services behind QObject interfaces and QML for
  presentation. Simulation is a backend selection, not a separate application.
- **CONFIRMED:** Raspberry Pi documents ADV728x-M CSI-2 bridge support through
  the `adv7180` V4L2 driver, with no interlaced capture support. An ADV7280/7282
  variant with the low-delay I2P block and the `-M` CSI output is required.
- **CONFIRMED:** Analog Devices states the ADV728x I2P is line based rather than
  frame buffered. This is lower latency but trades away motion-adaptive quality.
- **CONFIRMED:** ADV7280A is marked not recommended for new designs. It is a
  prototype-enabling choice, not yet the production decoder.
- **CONFIRMED:** Renesas TW9992 is active, supports NTSC/PAL, CSI-2 YUV422 or
  RGB565 output, and is specified at 100 mW typical. Mainline/Raspberry Pi driver
  support has not been confirmed and is the primary integration risk.
- **CONFIRMED:** CM4 exposes CSI and DSI, has H.264 1080p30 encode, and has a
  stated production lifetime through at least January 2034. This makes it the
  Prototype 1 compute choice despite newer CM5 CPU performance.
- **CONFIRMED:** NXP i.MX 8M Plus has dual CSI, DSI, a 2D/3D GPU, and H.264/H.265
  encode to 1080p60. It remains the leading production-oriented SoM family,
  subject to decoder compatibility and measured end-to-end latency.
- **CONFIRMED:** RadioMaster's current T8L documentation specifies two 18650
  cells, 6.0–8.4 V input, 100 mW maximum 2.4 GHz ELRS, USB-C charging/update/
  simulator support, and Hall gimbals. Schematics, board dimensions, internal
  connector pinout, CRSF access, and cell topology are **UNKNOWN**.
- **LIKELY:** keeping the donor mainboard intact is the least risky V1 RF plan.
  No T8L signal will be tapped until the reverse-engineering procedure has been
  completed.
- **CONFIRMED:** Skyzone SteadyView X publishes -98 dBm ±1 dB sensitivity,
  dual-antenna mix/diversity modes, 1 Vpp/75 ohm CVBS output, and a ground-station
  kit. It is the Prototype 1 VRX lead because its standalone output is explicit.
  Its fusion latency and weak-signal behavior are **NEEDS MEASUREMENT**.
- **CONFIRMED:** TBS Fusion publishes -96 dBm, 5 V/340 mA, active video fusion,
  and a UART protocol, but the manufacturer currently lists it out of stock due
  to a filter-component shortage. Retain as an A/B benchmark, not the baseline.
- **CONFIRMED:** Winstar WF50DSYA3MNN0 is a 5-inch, 720×1280, 4-lane MIPI DSI,
  IPS panel with 1100 cd/m² typical luminance. It leads the production display
  shortlist, but needs a verified Linux panel initialization sequence and a
  physical latency/sample-availability check.
- Prototype display lead: Raspberry Pi Touch Display 2 (5 inch) or Waveshare
  5-inch DSI (D), both 720×1280 and already supported on Raspberry Pi. Brightness
  and display scanout latency remain **NEEDS MEASUREMENT**.

## Decision gates

1. No manufacturing release may use an unverified footprint, pinout, register
   script, or electrical rating.
2. Decoder, VRX fusion mode, and display choices remain provisional until the
   high-speed-camera/photodiode latency fixture produces distributions, not just
   a single best-case observation.
3. The flight-control path is independent of Linux. Linux failure must not stop
   the T8L/ELRS control link.

## 2026-08-31 — Desktop vertical slice and staged hardware baseline

- Installed the Linux Qt 6/CMake/Ninja/FFmpeg/GStreamer build prerequisites and
  recorded the reproducible package list in `docs/BUILD.md` and CI.
- Implemented a Qt Quick shell with FPV, Battery, Flights, DVR, Aircraft, Settings,
  and development Simulator screens. `scripts/generate-test-video` creates a
  copyright-free 720×576/50 synthetic stream containing pseudo quad-OSD pixels;
  colored FPVDeck widgets are a separate QML scene layer.
- **VERIFIED:** the application builds and plays the generated video under an X11
  virtual display. A captured frame shows both in-video OSD and independent
  translucent receiver/telemetry/system/battery overlays.
- Added simulated Battery, Video/VRX, Telemetry, Input, and DVR services. Faults
  include signal loss/weak/roll/black, PAL/NTSC, telemetry loss/corruption/version,
  1S–6S pack/imbalance/under/over/disconnect/bad taps, and storage-full.
- Added SQLite schema migrations for aircraft, batteries, flights, telemetry,
  recordings and battery observations. WAL, foreign keys and FULL synchronous
  mode are enabled. Migration/reopen persistence is tested.
- Implemented matching allocation-free C firmware and C++ Linux codecs for the
  v1 COBS + CRC-16/CCITT frame. Payload layouts intentionally remain unfrozen.
- Local CI-equivalent result: 6/6 tests pass (battery math, C++ protocol, database,
  services, QML smoke, MCU C protocol).
- Created two empty KiCad project containers. No schematic was fabricated from
  uncertain interfaces; capture is gated on EVM and T8L measurements.
- Prototype 1 component lead set: CM4, Pi-supported DSI panel, ADV728x-M EVM,
  SteadyView X, STM32G0B1 devboard, ADS7066/ADS8688A EVM comparison, BQ25792 and
  TPS55288 evaluation boards. These remain measurement-gated decisions.

## 2026-09-01 — Canonical identity, showcase, and display ergonomics

- **CONFIRMED:** the canonical project/repository name is **FpvDeck** and the Git
  remote is `git@github.com:wiekstras/FpvDeck.git`. The existing local `main`
  history was preserved. `origin` had no branch heads when inspected; no history
  rewrite or force push was performed.
- Replaced the temporary OpenPocket-facing identity with FpvDeck in build metadata,
  application metadata, architecture prose, license attribution, and repository
  presentation. The existing MIT license text was preserved.
- Added deterministic Home, FPV, Balance, Media, Flights, and Diagnostics showcase
  states. `scripts/screenshot-demo` launches the actual Qt/QML application under
  Xvfb with synthetic video and in-memory SQLite, producing the committed 1280×720
  captures. Static Media/Batteries records remain explicitly labeled simulated;
  they are not claimed as completed persistence or storage backends.
- Added editable SVG branding, Mermaid architecture sources, a rendered system
  diagram, repository issue/PR templates, contribution/security guidance, and a
  status-accurate README.
- Product direction changed after ergonomics review: the screen should be larger
  and high-mounted, with both gimbal finger envelopes clear for pinch grip. The
  target range is now 5.5–6.25 inches. The Waveshare 6.25-inch 720×1560 DSI module
  is a larger bench candidate; its brightness, landscape scan direction, power,
  fit, and latency are **UNKNOWN / NEEDS MEASUREMENT**. The supported 5-inch Pi
  panel remains a bring-up fallback, not the desired final size.
- Generated two industrial-design images with the built-in image-generation tool.
  Version 2 enlarges and raises the screen, lowers/separates the gimbals, and adds
  palm support and pinch clearance. Both are labeled **CONCEPT RENDER** and are
  explicitly prohibited as dimensional/PCB inputs in `docs/assets/renders/README.md`.
- **VERIFIED:** the first canonical GitHub Actions run passed, including all 12
  tests and required-document checks. Its Node 20 deprecation annotation prompted
  an upgrade to official `actions/checkout@v6` and `actions/cache@v5`, both using
  the Node 24 action runtime on current GitHub-hosted runners.

## 2026-09-01 — Touchscreen vertical slice and Test PCB Rev A pre-release

- Touch is now a hard product requirement. The simulator was converted to a
  touchscreen-first interaction model with reusable QML components, minimum
  field-friendly targets, FPV tap-to-reveal controls, timed auto-hide, deliberate
  flight lock, a detailed external-battery card, touch media playback, receiver
  scan/favorites, and a development touch indicator. Physical/keyboard paths
  remain available for critical functions.
- Added separate `InteractionService`, `StorageService`, `MediaService`, and
  `SystemService` interfaces. Simulation covers SD removal/corruption/full,
  MCU/ADC/VRX loss, over-temperature, deck drain/charging, touch, media playback,
  and video/receiver faults. Diagnostics exposes those states without coupling
  the UI directly to hardware code.
- **VERIFIED:** the expanded local suite has 18 passing tests, including all
  deterministic application states, service fault/state behavior, the MCU frame
  codec, board-command dispatch, and host hardware-test tool.
- **CONFIRMED:** Waveshare currently documents `6.25inch DSI LCD (B)`, SKU 35000,
  as a 720×1560 IPS module with 500 cd/m² luminance, 178-degree viewing, up to
  60 Hz, five-point capacitive touch, I2C touch, optical bonding, and a
  74.70×159.18 mm cover. It is selected for Prototype 1 because the user requires
  a larger screen and pinch-grip geometry. Module latency, landscape scan,
  sunlight performance, thermal behavior, glove behavior, and actual fit are
  **NEEDS MEASUREMENT**; it is not a production-panel freeze.
- CM4 remains the Prototype 1 compute choice because the selected module has a
  documented CM4 DSI connection, ADV728x-M has a documented Raspberry Pi capture
  path, and CM4 provides an H.264 encoder. This is an evaluation convenience
  decision, not proof that CM4 is the production SoM.
- Selected ADS8688A for Test PCB Rev A: eight protected 16-bit channels and a
  0–10.24 V input range allow all six cumulative balance taps, pack-negative
  sense, and deck monitoring without per-channel op amps. The exact resistor
  network is Vishay `ACASA1002U1002P1AT`; three matched 10 kΩ elements form the
  top leg and one forms the bottom leg, followed by a separate 1 kΩ input
  resistor. All values remain subject to physical calibration and review.
- **VERIFIED BY MODEL, NOT HARDWARE:** a seeded 20,000-sample Monte Carlo across
  -20 to 70 °C after two-point calibration produced 2.76 mV tap p95 and 3.84 mV
  derived-cell p95; p99 was 4.26/6.19 mV. This model includes documented ADC
  gain/offset/input-impedance drift, ratio tracking, quantization, and calibration
  uncertainty. It is not a guaranteed accuracy specification.
- Created Test PCB Rev A as a real KiCad project and generated a 120×90 mm placed,
  net-assigned, **unrouted** board containing exact lead packages, six divider
  arrays, ADS8688A, STM32G0B1CBT6, power protection/regulation, test points, and
  modular SBC/VRX/decoder/debug headers. PDF, Gerber, and drill export succeed.
  The design is explicitly **NOT RELEASED FOR FABRICATION** until a native
  schematic/ERC, ground-reference protection, independent pad-map audit, routing,
  DRC, 1:1 footprint check, and design review are complete.
- A primary-datasheet audit before release found the initial generator had
  misassigned ADS8688A channel 2/4/5 and AVDD pads. Because the board was unrouted
  and explicitly gated, no released hardware was affected. The DBT mapping was
  corrected to channel-positive pads 16/18/21/23/25/27, AVDD pads 9/30, and the
  paired ground pins. CI now parses and verifies the critical ADC and MCU mapping.
- High-speed MIPI/DSI/CSI and RF circuitry stay off Test PCB Rev A. Decoder EVM,
  display, CM4 IO board, and VRX remain replaceable modules, reducing Rev A risk.
- Added a host-tested MCU board-command subset and `tools/fpvdeck_hw_test.py`.
  It can run deterministically with `--simulate` now and will use a POSIX serial
  port when HAL firmware exists. It reports identity, MCU/ADC state, six balance
  channels, storage detect, temperature, and VRX communication.
- Created exact-MPN Test PCB and system BOMs plus procurement staging. The bare
  PCB and unresolved EVM/adaptor connections are in **WAIT**, preventing an
  apparently complete shopping list from becoming an unsafe fabrication order.
- Post-procurement audit corrected the decoder evaluation-board assumption. ADI's
  `EVAL-ADV7282AMEBZ` exposes the CSI clock/data pairs on SMA connectors for a
  MIPI analyzer, not a Pi FFC. Raspberry Pi tested the older, software-compatible
  `EVAL-ADV7282MEBZ`; ADI marks the silicon family NRND. The current A-M kit is now
  a **VERIFY STOCK AND BRIDGE** item, with a separate controlled-impedance
  SMA-to-CM4IO fixture required before purchase/use. Production remains TW9992
  research rather than an ADV728x lifecycle commitment.

## 2026-09-01 — Provisional pack-reference test path

- The placed Rev A design correctly kept `BNEG_RAW` separate from local ground,
  but that also left the six cumulative-tap dividers without a completed battery
  return. A direct copper bond was rejected because USB, grounded test equipment,
  the SBC, and other modules can form alternate fault-current paths.
- Added F2 as a **DNP-only** Littelfuse `1206L010/60WR` candidate between
  `BNEG_RAW` and local ground, plus TP33/TP34 on both sides. The exact part is
  manufacturer-rated 0.10 A hold, 0.25 A trip and 60 V maximum; the manufacturer
  lists 0.5 Ω minimum initial, 1.5 Ω typical initial, and 10 Ω maximum
  resistance after trip/reflow. Its generic KiCad land pattern is still subject
  to exact drawing review.
- Six ideal 4.2 V/cell divider channels draw 2.151 mA total. That produces about
  3.23 mV typical and as much as 21.5 mV at the published post-trip/reflow maximum
  between pack B− and local ground. ADC channel 6 can observe the negative
  `BNEG_SENSE` offset only when configured for a suitable bipolar range.
- **OPEN / SAFETY GATE:** F2 is not approval for real-LiPo use. Fault-tree review
  and current-limited isolated-simulator tests must cover all externally grounded
  states, partial insertion, powered-off behavior, and alternate return paths.
