# Engineering decisions

This file records choices made without interrupting the unattended build. Open
questions are safety gates, not invitations to guess.

## 2026-10-26 — product reset

- **Decision:** FpvDeck is now a standalone FPV screen and battery field tool.
  All radio-control, T8L, ELRS, CRSF, Linux SBC, Qt and custom-transmitter work
  was removed from the active tree. Git history preserves it at `fd47ebd`.
- **Decision:** Use the fixed Waveshare ESP32-P4-WIFI6-Touch-LCD-5 reference
  platform and a rear 40-pin HAT. This makes the screen the product center and
  keeps the RF/video/battery board replaceable.
- **Decision:** Pin ESP-IDF to v6.1, the current stable minor release at this
  session date. The Waveshare BSP is pinned separately so upstream changes do
  not silently alter display initialization.

## Header GPIO mapping

- **CONFIRMED:** Waveshare's official schematic and pinout expose 22 ordinary
  GPIOs plus shared I²C on the 40-pin header. The HAT needs 21 GPIOs. Dedicated
  USB-HS pins 25/27 and optional USB-FS GPIO24/25 pins 21/23 remain unused.
- **Decision:** The DVP bus uses the contiguous early header GPIO group; control
  signals use the later group. GPIO2/header pin 8 is held as the only spare.
- **Decision:** Reuse board I²C on GPIO7/GPIO8. Known onboard addresses do not
  collide with ADS1115 `0x48/0x49/0x4A` or TVP5150 `0x5C`. Bring-up must still
  scan the actual board before enabling HAT drivers.

## Video

- **Decision:** Capture one field at a time: PAL 720×288 or NTSC 720×240. Avoid
  full-frame deinterlacing. The ISP converts BT.601 YUV422 to RGB565; PPA then
  scales/rotates for the portrait-native panel. This is the shortest credible
  latency path.
- **Decision:** Use TVP5150 register `0x03 = 0x0D`, `0x0D = 0x00`, `0x0F = 0x00`,
  and `0x28 = 0x00`, derived from TI SLES209E. Driver code verifies device ID
  before configuration and allows the documented clock-stretch/write settle time.
- **Decision:** Use TPF131-TR (`LCSC C155439`) for buffered AV output. It is a
  currently stocked 3.0–5.5 V, 2 V/V video line driver capable of 75-ohm loads.
  A 1:2 high-impedance input divider and 75-ohm back termination restore nominal
  amplitude without loading the selected CVBS node. This exact circuit remains
  simulation/bench-gated before fabrication.

## RF and diversity

- **Decision:** RX A is fitted; RX B, its comparator channel and second antenna
  may be DNP for first bring-up. The PCB keeps both paths symmetric.
- **UNKNOWN / MUST VERIFY:** RX5808 modules sold under the same name vary. The
  specified 11-pin mapping is captured in the schematic, but the custom footprint
  is deliberately marked `VERIFY_RX5808_V1`. Measure the actual modules before
  ordering a PCB. No assumed pad geometry is fabrication-approved.
- **Decision:** Software changes both receivers together on the shared DATA/CLK
  bus with separate latch-enable lines. Hardware diversity remains independent
  of firmware after tuning.

## Battery measurement and power

- **Decision:** The external JST-XH balance connector is measurement-only and
  never powers FpvDeck. A per-tap 100 kΩ/12 kΩ divider and 100 nF filter feed the
  ADS1115s. Physical protection and connector-keying review remain release gates.
- **Conflict found:** the official Waveshare schematic does not expose raw
  `BAT` on the 40-pin header. It already divides `BAT` through 200 kΩ/100 kΩ to
  `BAT_ADC` on ESP32-P4 GPIO20. Therefore the requested ADS1115 #2 internal-cell
  input cannot be connected through the HAT without an undocumented flying wire.
  Rev A reserves that ADS channel but marks its 100 kΩ/100 kΩ HAT divider DNP;
  firmware will use the onboard GPIO20 divider after ADC calibration/HIL. This
  avoids an unsafe board modification and does not guess a nonexistent header pin.
- **Decision:** Calibration is per cumulative tap, not per derived cell. This is
  necessary because cell values are differences between independently scaled
  cumulative measurements.
- **UNKNOWN / MUST VERIFY:** The Waveshare battery connector polarity, permitted
  cell protection arrangement and charge-current suitability for a chosen
  3000–4000 mAh pack must be checked against the received board and battery.

## Firmware scope

- **Decision:** Hardware-free logic is ordinary C compiled by host GCC. Hardware
  drivers use ESP-IDF directly; there is no plugin/service abstraction layer.
- **Decision:** Spectator streaming is isolated behind `app_stream`. The initial
  build provides UI/state boundaries but does not claim a validated 15 fps stream
  until ESP-Hosted, JPEG throughput and simultaneous display capture are measured.
- **Decision:** The generated schematic and PCB are design artifacts, not an
  order authorization. `hardware/READINESS.md` owns the release gates.

## 2026-10-27 — build and design review

- **CONFIRMED:** a clean ESP-IDF v6.1 target build produces `fpvdeck.bin`
  (approximately 876 KiB) with no warnings from FpvDeck-owned code. Notes emitted
  by upstream ESP-IDF Kconfig files are outside this repository.
- **Correction:** diversity polarity is now consistent end-to-end: LMV331
  open/high means RSSI A is stronger, the 3157 high-select path is video A, and
  `ACTIVE_RX` high means RX A.
- **Decision:** do not claim live video, live battery readings, streaming, or a
  fabrication-ready HAT until hardware-in-loop tests close their explicit gates.
