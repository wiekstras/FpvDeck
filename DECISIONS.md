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
- **CONFIRMED:** the pinned Waveshare BSP configures the 720×1280 panel with its
  30 Hz timing macro and 58 MHz DPI clock. Latency budgeting therefore uses a
  0–33.3 ms panel-phase term; 60 Hz is not assumed without vendor timing data.
- **Decision:** Use TVP5150 register `0x03 = 0x0D`, `0x0D = 0x00`, `0x0F = 0x00`,
  and `0x28 = 0x00`, derived from TI SLES209E. Driver code verifies device ID
  before configuration and allows the documented clock-stretch/write settle time.
- **Decision:** Use TPF131-TR (`LCSC C155439`) for buffered AV output. It is a
  currently stocked 3.0–5.5 V, 2 V/V video line driver capable of 75-ohm loads.
  A 1:2 high-impedance input divider and 75-ohm back termination restore nominal
  amplitude without loading the selected CVBS node. This exact circuit remains
  simulation/bench-gated before fabrication.

## RF and diversity

- **Decision:** RX A and RX B are fitted for the final diversity assembly. RX B
  may be DNP only for staged bring-up; both paths remain symmetric and separately
  filtered from the 5 V header rail.
- **UNKNOWN / MUST VERIFY:** RX5808 modules sold under the same name vary. The
  specified 11-pin mapping is captured in the schematic, but the custom footprint
  is deliberately marked `VERIFY_RX5808_V1`. Measure the actual modules before
  ordering a PCB. No assumed pad geometry is fabrication-approved.
- **Decision:** Software changes both receivers together on the shared DATA/CLK
  bus with separate latch-enable lines. Hardware diversity remains independent
  of firmware after tuning.
- **Decision:** JP1 defaults the RSSI comparator into the video-switch select.
  Its alternate position routes ESP32-P4 GPIO2 to the switch so a later
  calibrated RSSI/decoder-lock policy can be tested without respinning the HAT.
  This remains classic diversity: rapidFIRE's documented image fusion is
  proprietary and cannot be claimed with one analog selector and one decoder.

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
- **Decision:** The spectrum scanner visits all 48 configured A/B/E/F/R/L table
  entries rather than inventing intermediate channel labels. It takes raw dual
  RSSI readings through a serialized ADS1115 path, reports volts, and uses an
  initial 80 ms post-tune settle time. That delay and the volts-to-signal curve
  remain HIL calibration items.
- **Decision:** Lost-quad beeps are non-blocking 50 ms tones with a provisional
  interval mapped from 900 ms at 0 V to 150 ms at the ADS1115 2.048 V full scale.
  The UI continues to show volts, not a misleading percentage. Receiver HIL
  data must define the final useful cadence curve.
- **Decision:** Spectator streaming is isolated behind `app_stream`. The initial
  build provides UI/state boundaries but does not claim a validated 15 fps stream
  until ESP-Hosted, JPEG throughput and simultaneous display capture are measured.
- **Decision:** The generated schematic and PCB are design artifacts, not an
  order authorization. `hardware/READINESS.md` owns the release gates.

## 2026-10-27 — build and design review

- **CONFIRMED:** a clean ESP-IDF v6.1 target build produces `fpvdeck.bin`
  (under 0.9 MiB) with no warnings from FpvDeck-owned code. Notes emitted
  by upstream ESP-IDF Kconfig files are outside this repository.
- **Correction:** diversity polarity is now consistent end-to-end: LMV331
  open/high means RSSI A is stronger, the 3157 high-select path is video A, and
  `ACTIVE_RX` high means RX A.
- **Correction:** the first HAT generator mapped the ADS1115 as though analog
  channels occupied pins 1–4. TI SBAS444E shows DGS pins 1=ADDR, 2=ALERT,
  3=GND, 4–7=AIN0–AIN3, 8=VDD, 9=SDA and 10=SCL. All three instances are now
  corrected and each has the required 100 nF supply bypass. No earlier generated
  board artifact is safe to fabricate.
- **Decision:** TI Figure 6-1 is authoritative for decoder bypassing: 100 nF is
  placed locally on PLL_AVDD, CH_AVDD, DVDD and IO_DVDD. The ME6211 receives
  explicit 1 µF input/output capacitors. Crystal load pads stay DNP/TUNE because
  TI's value depends on board stray capacitance; the current crystal selection
  remains REVIEW until its full tolerance budget satisfies ±50 ppm.
- **Decision:** do not claim live video, live battery readings, streaming, or a
  fabrication-ready HAT until hardware-in-loop tests close their explicit gates.

## 2026-10-27 — simulator and prototype mechanics

- **Decision:** the desktop simulator is a native SDL2/LVGL build of production
  app sources, not a separate web mock-up. Hardware calls are linked to desktop
  backends and the FreeRTOS scanner worker is replaced only in the simulator.
- **Decision:** keep the visual system flat and information-dense: solid fills,
  thin borders, no gradients or software shadows, opaque video overlays and
  100 ms transitions. This reads as a modern field instrument while avoiding
  expensive full-screen blending on ESP32-P4.
- **Decision:** enable LVGL's native ESP32-P4 PPA draw unit with 128-byte bursts.
  It accelerates only eligible draw tasks; rounded cards and text retain the
  software path. LVGL draw-buffer alignment is 128 bytes to match the configured
  P4 L2 cache line, as enforced by the driver. No performance gain is claimed
  until measured on the board.
- **CONFIRMED:** Waveshare's 2026-04-08 mechanical drawing specifies a 126.9 ×
  70.7 mm front, 110.32 × 62.28 mm active area, 118.509 × 64.509 mm rear-board
  envelope, 11.6 mm stack, and four M2.5 holes on 112.003 × 57 mm centers.
- **Decision:** add a 24.1 mm right control rail around the host module so the
  three physical buttons can be used without consuming active screen area.
  This makes the adjustable first shell 151 × 78.5 mm; it is a prototype UX
  choice, not a frozen industrial design.
- **Decision:** use the exact active Amphenol `132289-12` end-launch SMA variant
  in BOM/CAD rather than the ambiguous family number `132289`.
- **Decision:** change the balance connector from top-entry B7B-XH-A to
  side-entry `S7B-XH-A-1`. It preserves the 7-pin XH electrical interface while
  allowing a bottom-edge balance lead without a plug protruding through the rear.
- **UNKNOWN / MUST VERIFY:** the current header location, button footprint used
  for the mechanical proxy, SMA/JST positions, header stack, battery cavity and
  port clearances require the official host STEP plus received-part measurements.
  The KiCad board is intentionally unrouted and its silkscreen states this.

## 2026-10-28 — antenna orientation

- **Decision:** RX A exits through the left PCB/enclosure edge and RX B exits at
  the top-right. The orthogonal arrangement matches the requested handheld
  layout and improves physical separation between unlike antenna orientations.
- **Decision:** RX B stays inboard of the confirmed upper-right M2.5 host mount.
  This makes its provisional grounded-coplanar launch about 9.4 mm rather than
  placing a receiver body over a mounting hole; the project limit remains 10 mm.
