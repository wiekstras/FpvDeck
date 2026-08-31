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
