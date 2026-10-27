# Engineering log

## 2026-10-26 — standalone field-tool reset

- Retired the T8L radio-computer direction. The active project is now a dedicated
  FPV screen and 2S–6S battery checker on Waveshare's ESP32-P4 5-inch platform.
- Verified the official 40-pin pinout and found 22 ordinary GPIOs plus I²C. The
  requested HAT needs 21 GPIOs, so the mapping fits without consuming either USB
  interface. GPIO2/header pin 8 remains spare.
- Verified from TI SLES209E that TVP5150 address select low gives 7-bit `0x5C`,
  device ID is `0x51/0x50`, discrete-sync YCbCr format is register `0x0D[2:0]=0`,
  and output-enable bits yield register `0x03=0x0D` for this design.
- Selected TPF131-TR as the external video line driver after finding an active
  LCSC listing and 75-ohm-load capability. Its gain requires a deliberate 1:2
  input attenuator; this branch remains bench-gated.
- ESP-IDF v6.1 is the pinned toolchain. Waveshare's official LCD5 BSP v1.0.4 and
  HX8394 component v2.1.0 provide the display/touch base.
- Created host-testable pure-C logic, direct ESP-IDF drivers, LVGL app modules,
  deterministic hardware generation and new product documentation.

## 2026-10-27 — first complete build and review

- Installed the project-local ESP-IDF v6.1 toolchain and completed a real
  `esp32p4` build. `fpvdeck.bin` remains under 0.9 MiB; FpvDeck-owned sources compile
  with `-Wall -Wextra -Werror`.
- Host tests pass and the HAT generator reports 78 parts / 90 checked nets.
- Corrected the diversity comparator/switch polarity so `ACTIVE_RX=1` selects
  RX A, matching the product requirement.
- Inspected the official host schematic's battery block: raw `BAT` is not on
  the 40-pin header; the host already has a 200 kΩ/100 kΩ divider into GPIO20.
  The HAT's reserved internal-cell divider is now DNP pending a justified source.
- KiCad CLI opens/exports the generated schematic. The PCB is intentionally a
  placement-only container and remains blocked from fabrication.
- Added calibrated sampling of the Waveshare board's existing GPIO20 BAT_ADC,
  including host-tested no-battery/low/critical thresholds and a critical-power
  screen. Actual voltage accuracy and power-cut behavior remain HIL gates.
- Registered Up/OK/Down as an LVGL keypad on the default focus group. Removed
  hard-coded pack and RSSI values from production screens; apps now show explicit
  empty/waiting states until real acquisition supplies measurements.
- Added non-blocking sampling tasks for cumulative balance taps (500 ms) and
  dual RSSI/ACTIVE_RX (100 ms). Battery, monitor and finder screens consume
  snapshots on LVGL timers, show calibrated voltages, and retain a clean empty
  state when the HAT is absent. RSSI remains in volts until a receiver-specific
  calibration curve is measured; the UI does not invent a percentage.
- The FPV monitor now tunes both RX5808 footprints from touch arrows or the
  Up/Down hardware buttons. A deliberate OK long press opens the A/B/E/F/R/L
  band picker; selections persist through NVS. The complete ESP32-P4 firmware
  still builds successfully after this input path was added.
- Replaced the spectrum placeholder with a background 48-channel table sweep.
  ADS1115 access is mutex-serialized against the continuous signal sampler; the
  LVGL task only renders snapshots. The scanner restores the previous frequency,
  exposes the strongest three results as monitor shortcuts, and reports hardware
  absence honestly. Host checks and the complete ESP32-P4 build still pass.
- Completed the finder interaction: it has an app-session peak, large channel
  controls behind an explicit lock, saved lock selection and a non-blocking
  50 ms tone state machine whose cadence rises with raw RSSI voltage. Boundary
  behavior is host-tested; physical RSSI/cadence calibration remains required.
- Replaced the static Settings mock-up with live NVS-backed brightness, buzzer,
  video-standard and masked WPA2 password controls. Saved brightness is applied
  at boot and stored settings are range-sanitized. Calibration stays explicitly
  unavailable until the protected analog path passes HIL.
- Corrected the latency budget after inspecting the pinned Waveshare component:
  it uses the HX8394 720×1280 **30 Hz** timing macro, not 60 Hz. The provisional
  FpvDeck-added budget now includes up to 33.3 ms of panel phase.
- A primary-source HAT audit found the generated ADS1115 VSSOP pin order was
  incorrect. Corrected U5/U6/U7 to TI SBAS444E's DGS pinout and added a 100 nF
  VDD bypass at each device. Regenerated BOM/netlist/schematic artifacts replace
  the invalid earlier outputs; the board remains explicitly non-fabricable.
- Continued the source audit with TI TVP5150 Figure 6-1: added four local decoder
  bypass capacitors, ME6211 input/output capacitors and TI's 100 kΩ parallel
  crystal resistor. Crystal load footprints are deliberately DNP/TUNE because
  their value depends on the chosen crystal and measured stray capacitance.
  CVBS anti-aliasing and system-level ESD remain explicit release gates.
- Closed the settings/decoder gap: persisted Auto, NTSC M/J and PAL B/G/H/I/N
  now program TVP5150 register 0x28 using TI-defined values 0x00/0x02/0x04 at
  boot and when changed. Capture-buffer standard changes still require HIL.

## 2026-10-27 — acceptance checkpoint

- `./scripts/check` passes: host unit tests are green and the HAT generator
  validates 90 parts / 90 nets with no generated-artifact drift.
- `./scripts/build-firmware` passes against pinned ESP-IDF 6.1 with
  `-Wall -Wextra -Werror` on project sources. `fpvdeck.bin` is `0xde2a0` bytes;
  the remaining build notes originate in upstream ESP-IDF Kconfig files.
- Local `main` has been pushed incrementally to canonical `origin`. No hardware
  was flashed, no parts were ordered, and the HAT is still blocked from
  fabrication by `hardware/READINESS.md` (RX5808 mechanics, balance protection,
  analog input/AV validation, crystal margin, routed layout and independent review).

## 2026-10-27 — native desktop simulator

- Added a macOS/Linux SDL2 build that compiles the production LVGL launcher,
  Battery, Monitor, Finder, Stream and Settings screens. The Scanner retains the
  same app contract and shared receiver/signal logic, with a desktop timer in
  place of its FreeRTOS worker.
- Introduced a narrow portable platform seam for display brightness and result
  types. The ESP32-P4 build remains successful and hardware drivers remain the
  firmware implementations; desktop backends are linked only by the simulator.
- The simulator models a 2S–6S external pack, imbalance/disconnect, Deck cell,
  dual RSSI, channel-dependent RF peaks, diversity selection, video noise and
  signal loss. A persistent top-layer control drawer changes these states by
  touch without external hardware.
- Added synthetic moving analog-video imagery with a Betaflight-style source OSD
  below FpvDeck's RGB receiver overlay. This is a UI/pipeline visualization, not
  a latency or decoder simulation.
- Verified `./scripts/test-simulator`, `./scripts/test-host`, the HAT generator,
  and a complete ESP-IDF v6.1 `esp32p4` firmware build.

## 2026-10-27 — prototype CAD and connector coordination

- Read Waveshare's official 2026-04-08 PDF/DXF and captured confirmed front,
  active-area, rear-board, stack and M2.5 mounting-pattern dimensions in one
  OpenSCAD parameter file. The official 2026-08-12 STEP remains the next source
  for port/header offsets and detailed keep-outs.
- Added a 151 × 78.5 mm adjustable clamshell concept with a landscape display
  opening, dedicated three-button rail, two top SMA passages, bottom balance
  port, locating lip and host mounting bosses. Generated manifold front/rear
  STL files and review PNGs are committed.
- Selected JST `S7B-XH-A-1` side entry so a balance lead exits the bottom edge,
  and made the SMA part exact as active Amphenol `132289-12`. Both choices are
  traced to manufacturer drawings; final clearance still needs 1:1 test prints.
- Replaced the empty KiCad board rectangle with a 118.5 × 64.5 mechanical
  placement board containing the official mounting pattern, external connector
  footprints, button rail and major video/ADC packages. It has zero routed tracks
  and carries explicit NOT FOR FABRICATION markings; STEP/SVG/PNG exports exist
  solely for enclosure coordination.

## 2026-10-27 — simulator application coverage

- Added a `--screen` selector for every production LVGL application so layout
  and state review on macOS does not require navigating from the launcher.
- Replaced the launcher-only smoke test with seven isolated headless tests for
  launcher, battery, monitor, scanner, finder, stream and settings. Process
  isolation keeps one application's timers from masking another startup error.
- Added SDL renderer capture and a macOS-compatible screenshot script. The first
  captures found disabled LVGL float formatting and low-contrast default object
  styles; enabled float formatting in simulator/firmware configuration and fixed
  the affected production screens before committing the generated previews.

## 2026-10-27 — modern UI and ESP32-P4 render profile

- Replaced the uniform default-button launcher with an asymmetric, priority-led
  dashboard and introduced a shared `ui_theme` used by production firmware and
  the desktop simulator.
- Rebuilt Battery around a dominant pack summary, metric cards and persistent
  per-cell voltage bars; rebuilt Monitor around an opaque four-metric receiver
  rail; rebuilt Finder around proximity, peak and receiver metrics. Scanner,
  Settings, Stream, dialogs, buttons and simulator controls now share the same
  palette, focus and pressed states.
- Kept rendering deliberately cheap: no gradients, blur, shadows or perpetual
  animation; 100 ms transitions; static object trees; Finder UI reduced from
  20 Hz to 10 Hz. Screen/card scrollbars are explicitly suppressed.
- Enabled LVGL's ESP32-P4 PPA draw unit for supported opaque fills alongside the
  existing two software draw units and style cache. The first build correctly
  rejected LVGL's old 4-byte draw-buffer alignment; PPA requires it to equal the
  P4 L2 cache line, so both are now 128 bytes. Physical frame-time and bandwidth
  measurement remain required before calling the profile optimized.
- Regenerated actual SDL renderer captures and retained isolated startup tests
  for all seven screens.
- Final validation passed: seven simulator smoke tests, host logic tests, the
  90-part/90-net HAT generator check, and the ESP-IDF 6.1 P4 build. The resulting
  firmware image is `0xde240` bytes with 89% of the app partition free.

## 2026-10-27 — README concept imagery

- Added a clean screen-first product hero and a field-workflow concept image to
  the README. Both depict the current no-transmitter architecture and are marked
  as non-dimensional concept visualization.
- The field image reinforces that an external flight pack connects only through
  its balance lead for measurement; it does not imply that the pack powers the
  Deck. KiCad and measured CAD remain authoritative over generated imagery.

## 2026-10-27 — clean-checkout simulator CI repair

- GitHub Actions failed because the desktop simulator referenced ESP-IDF's
  generated `managed_components/lvgl__lvgl` directory, which does not exist in a
  clean checkout before an IDF component-manager run.
- The simulator now uses that local LVGL tree when available and otherwise
  fetches the same LVGL 9.5.0 release from its official repository with a pinned
  SHA-256 archive digest. A forced clean-fetch build and all seven smoke tests
  pass locally.
