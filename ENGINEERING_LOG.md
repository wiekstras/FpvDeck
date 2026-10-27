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
