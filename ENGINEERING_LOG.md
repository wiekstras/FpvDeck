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
  between pack B− and local ground. Physical ADC channel 0 can observe the
  `BNEG_SENSE` offset only when configured for a suitable bipolar range.
- **OPEN / SAFETY GATE:** F2 is not approval for real-LiPo use. Fault-tree review
  and current-limited isolated-simulator tests must cover all externally grounded
  states, partial insertion, powered-off behavior, and alternate return paths.
- Extended `tools/balance_frontend_error.py` with the nominal ADC-loaded return
  calculation. The deterministic result is 2.152796 mA for six 4.2 V cumulative
  taps, 3.229194 mV at 1.5 Ω and 21.527959 mV at 10 Ω. Tests lock these values.
- Added `docs/BALANCE_GROUND_FAULT_ANALYSIS.md` with explicit current-path,
  energy-limit, abort-condition and evidence requirements for unpowered, USB,
  earthed-instrument, partial-insertion, open/high-R F2 and tap-fault states.
  Production B− correction is a requirement, not an implemented claim.

## 2026-09-01 — Reproducible desktop first-frame benchmark

- Added `--benchmark-json` to the real Qt application. It records a versioned
  first-frame record containing QML-load time, first swapped frame, RSS, screen
  geometry, demo state and timeout status, then exits. CI exercises this path.
- Added `scripts/benchmark-ui`, including an excluded cache warm-up, repeated
  fresh processes and median/p95 aggregation. The raw JSON remains generated
  output rather than a source-controlled performance claim.
- **MEASURED ON THIS DEVELOPMENT CONTAINER ONLY:** three-sample Release/Xvfb
  first-frame medians ranged from 655 ms (Media playback) to 892 ms (FPV controls);
  p95 ranged from 704 to 1,034 ms and median RSS was roughly 282–285 MiB. These
  numbers are a regression baseline, not CM4 boot, touch latency, frame pacing,
  video latency, or physical-display evidence.
- The automated suite now has 19 tests, including the benchmark first-frame path
  and schema validation. All passed locally after this change.

## 2026-09-01 — ADC logical-channel correction

- Cross-checking firmware against the audited ADS8688A board map found a
  pre-hardware defect: `TAP_DUMP` iterated ADC0–ADC5 even though ADC0 is B− sense,
  ADC1 is deck monitor, and cumulative taps occupy ADC2–ADC7. The host tool also
  mislabeled its six individual ADC requests as balance channels.
- Added the explicit `fpvdeck_adc_channel_t` map. Firmware now reads ADC2–ADC7
  for `TAP_DUMP`; the Linux tool separately reports signed B− offset and deck
  monitoring, then consumes the ordered six-tap dump. Host and C dispatcher tests
  lock the map. No released hardware was affected.
- Added `tools/check_test_pcb_bom.py`: it expands CSV ranges, rejects duplicate or
  inconsistent quantities, and compares the BOM with every generated KiCad
  footprint without needing KiCad in CI. The then-current BOM covered 59 board
  footprints (with `PCB1` explicitly treated as the one non-footprint line).
- A subsequent net audit found `BNEG_RAW` and the ADC's `BNEG_SENSE` net had no
  component joining them. Added R7, an explicit 1.00 kΩ sense-path resistor,
  regenerated the deterministic board, and expanded the BOM to all 60 footprints.
  The input-bias/calibration term and fault behavior remain release-gated.

## 2026-09-01 — Corrected donor/video boundary and EU procurement gate

- Completed a repository-wide audit of T8L, ELRS, receiver, VRX, 5.8 GHz, CVBS,
  and decoder references. The canonical boundary is now explicit everywhere:
  **T8L = 2.4 GHz ELRS control donor; dedicated FpvDeck VRX = 5.8 GHz RF to
  CVBS; separate decoder = CVBS to digital; SBC = video/apps/overlay; HD panel =
  display/touch; external balance port = measurement only; donor 2×18650 =
  provisional internal source; removable SD = goggle/DVR media.**
- Found and removed an unsafe hardware assumption. Rev A J4 had implied 5 V,
  RSSI and UART for an undocumented commercial VRX, while J5 implied power/I²C/
  reset for a decoder. Both headers now assign only project pins 2=GND and
  3=CVBS; pins 1/4/5/6 are NC. Receiver and decoder keep their own documented
  power/control connections. Automated source tests prohibit the removed nets.
- **CONFIRMED:** Skyzone's current 5.8G SteadyView X documentation specifies
  5.3–6 GHz/48 channels, −98 dBm ±1 dB, two standard SMA-K 50 Ω inputs,
  mix/diversity/single modes, 1.0 Vp-p typical into 75 Ω, 6.5–26 V input, and
  12 V×240 mA normal/180 mA single operation. Its current ground-station kits
  include the XT60/barrel power lead, 3.5 mm video cable, RHCP patch and omni.
  The 3.5 mm conductor map, host control/RSSI API and fusion latency remain
  **UNKNOWN / NEEDS MEASUREMENT**.
- Located two actual EU receiver offers at the research snapshot: La Caméra
  Embarquée (France, €219 shown before tax, in stock) and Baltic Drones
  (Lithuania, €250.23 tax included, in stock). This validates EU availability,
  not end-to-end compatibility.
- The receiver remains `WAIT`, not `BUY NOW`. Although RF→CVBS is credible,
  `EVAL-ADV7282AMEBZ` exposes MIPI CSI-2 on SMA and direct Mouser inspection
  reported restricted availability. No reviewed controlled-impedance
  SMA-to-CM4IO bridge exists. An earlier cached Mouser result conflicted with the
  direct page, so only checkout-time direct stock is accepted.
- Purchase-cleared independent items are CM4104032, CM4IO, Waveshare 6.25-inch
  DSI LCD B SKU 35000, adapter/supply, NUCLEO-G0B1RE, ST-LINK, Transcend RDF5
  removable-media reader, and a 128 GB High Endurance test card. The snapshot
  subtotal is €353.24 before shipping; a 10% small-adapter contingency makes
  €388.56. Full gated bench architecture including video, a released Test PCB
  and fixtures is budgeted at €1,280–1,920, excluding lab instruments.
- Generated and committed top/bottom/component views for the 120×90 mm four-layer
  board, plus layout, component-map, staged-build, and readiness documents. The
  board still has **60 footprints and zero routed tracks**. Exports are inspection
  views, not fabrication outputs; PCB fabrication and real-LiPo connection remain
  prohibited.
- Corrected self-test semantics: Rev A cannot automatically test a VRX link
  because it exposes only a passive CVBS handoff. Protocol bit 5 is now named
  `CVBS_BREAKOUT`; simulated/real tooling reports it `INTERACTIVE` or `NOT TESTED`
  until a continuity/terminated-video fixture supplies evidence.
- Simulator Diagnostics now displays separate T8L control, ELRS TX 2.4 GHz,
  5.8 GHz video VRX, CVBS decoder and digital-video states. Independent T8L,
  ELRS, VRX and decoder fault injection is service-tested. A follow-up software
  boundary audit moved channel, RSSI, favorites and scanning out of `VideoService`
  into a dedicated `RadioService`; video standard/capture state remains separate.
- **VERIFIED:** documentation gates, 60-footprint BOM check, four Test PCB source
  safety tests, firmware/tool self-test behavior, complete C/C++/QML build, and
  all 19 CTest entries passed locally after the correction. The deterministic
  diagnostics showcase was regenerated from the actual application.

## 2026-09-01 — Prototype 0 reference hardware frozen

- Project priority changed from final hardware optimization to the shortest safe
  path to real Air65 video. The desktop simulator is now described as pre-hardware;
  **Prototype 0 is a functionality bench**, Prototype 1 is the direct-decoder
  latency bench, Prototype 2 is modular custom hardware and Prototype 3 is the
  integrated product.
- Selected **Raspberry Pi 5 2 GB** for the bench. Current simulator RSS is roughly
  285 MiB, so 2 GB is sufficient; Pi 5 adds useful decode/compositor margin for a
  small premium over Pi 4 2 GB. This does not change the production SoM decision.
- Selected **Raspberry Pi Touch Display 2 5-inch, MPN SC1975**. Kiwi's Dutch page
  showed €36.49 before VAT and immediate stock. Raspberry Pi documentation
  confirms 720×1280, five-point capacitive DSI touch, supplied Pi 5 22-to-15-way
  FFC, GPIO power lead, Pi GPIO pins 2/6, and Pi 5 support. It is intentionally
  cheap Prototype 0 hardware; the larger pinch-grip display direction remains.
- Selected the Rotorama **RC832 Mini** kit for Prototype 0 only. Vendor/manual
  evidence confirms 56 A/B/E/F/R/L/X channels, RP-SMA, 12 V/130 mA, 3.5 mm AV,
  PAL/NTSC 1.0 Vp-p into 75 Ω, and included receiver/linear antenna/power/video
  leads. Rotorama showed €55.49 in stock and documents EU delivery, including
  €6.49 GLS to Holland. OEM identity, barrel dimension/polarity and AV conductor
  map remain unknown; the build uses supplied leads and requires continuity checks.
- Selected **Gembird UVG-002** after the final Netherlands delivery audit rejected
  USB-Live2 as tonight's primary: its identified offers were either long-lead or
  not demonstrably deliverable to the Netherlands. Informatique showed UVG-002 at
  €12.95 including VAT/in stock; bol showed €19.70 delivered/in stock. Gembird
  specifies composite/S-Video, PAL 720×576/25, NTSC 720×480/30 and UTV007 or
  UTVF007 silicon. A deeper gate found two Linux paths: UTV007 uses `usbtv`;
  UTVF007 commonly enumerates as UVC `534d:0021` and uses `uvcvideo`. The latter
  is reported to squelch weak/noisy input, so it can prove functionality but not
  final FPV weak-signal quality. The exact received ID/formats remain a receipt
  gate; Hauppauge remains the known-identity fallback if neither path appears.
- Added a separate protected 12 V/1 A PCEnergy supply and centre-positive female
  barrel-to-terminal adapter. High-resolution vendor imagery showed that the
  supplied receiver power cable is barrel-to-red-two-pole, not bare wire. A final
  procurement audit rejected relying on RCY/BEC identification: the connector
  family is unpublished and direct Dutch pigtail listings were insufficiently
  verifiable. Prototype 0 instead removes that connector from the **replaceable
  supplied accessory lead**, continuity-maps its barrel, and lands the stripped
  conductors in the marked terminal adapter. Pi and VRX use separate wall
  supplies; receiver and T8L remain untouched.
- `hardware/PROTOTYPE_0_BUY_TONIGHT.md` totals the reference chain at roughly
  €246.47 including VAT plus €23–€35 multi-shop shipping, including a meter and
  cutter/stripper for a complete safe bring-up path. It excludes CM4,
  premium screen, ADV728x EVM, Test PCB, battery frontend, enclosure and duplicate
  ELRS hardware.

## 2026-09-01 — Real capture software path

- `VideoService` now distinguishes `simulated`, `file` and `v4l2` backends and
  publishes device identity/availability without coupling receiver channel state
  back into video. `--video-device /dev/videoN` selects an exact Qt `QCamera`;
  an unavailable device produces a visible lost state instead of application exit.
- The FPV page selects the QCamera capture session or existing MediaPlayer behind
  the same VideoOutput/overlay graph. A QMediaRecorder tee records the live camera
  before QML overlays to timestamped MP4 files. Recorder errors stop/report DVR
  without modifying VideoService; real codec/container behavior awaits Pi hardware.
- Added `fpvdeck-video-list`, `test-video`, `prototype0` and `setup-pi`. Raw video
  validation is deliberately independent of the full application. `prototype0`
  falls back to generated video when no capture device exists, retaining software
  usability before parts arrive.
- Diagnostics now reports the capture backend/device separately, and developer
  touch-trace mode adds video FPS, process CPU/RSS and temperature. Dropped-frame
  and electrical capture-latency values remain `—` until real driver/fixture data
  exists; the UI does not fabricate them.
- Added backend switching/reconnect, missing `/dev/video999`, recorder error and
  numeric V4L2 node/format discovery tests. The CI-equivalent suite increased
  from 19 to 20 CTest entries and passed locally after implementation.
- Device auto-selection now ignores metadata-only `/dev/video*` nodes and prefers
  a formatted node named `usbtv`. Raw bring-up and the launcher accept an
  explicit driver-reported composite input index; no input number is guessed.
- The live Qt camera watches `videoInputsChanged`: removal marks capture lost and
  reconnection at the requested path reconfigures/restarts the same camera object.
- Closed the owned-aircraft procurement ambiguity: if no serviceable BT2.0 1S
  LiHV pack/charger is already present, the buy-tonight document names a verified
  in-stock LAVA 260 mAh five-pack and VIFLY WhoopStor V3 as conditional additions.
  They are not duplicate purchases for an already complete Air65 kit.
- Final electrical-safety audit found that Rotorama's one-page Mini manual omits
  DC-jack polarity even though vendor imagery shows a factory red/black barrel
  lead. Wiring now permits first power only when factory red maps to centre and
  black maps to sleeve by continuity; an unmarked or contradictory received lead
  requires written vendor confirmation. No trial polarity is allowed.
- New DVR sessions now reset elapsed time. A first full test run exposed an
  intermittent missing-device multimedia teardown timeout; detaching QCamera,
  VideoOutput and MediaRecorder from CaptureSession while capture is unavailable
  removed that unnecessary backend path. The complete suite then passed cleanly.
- Re-ran shell syntax, installer dry-run, four Python V4L2 selection tests,
  whitespace and documentation checks, complete C/C++/QML build, all 20 CTest
  entries, and the 60-footprint Test PCB/BOM consistency check successfully.
