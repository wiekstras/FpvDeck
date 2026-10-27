# Engineering TODO

Completed items stay checked; safety-critical unknowns stay visible.

## Architecture reset

- [x] Remove active T8L, ELRS, Linux SBC and Qt application architecture.
- [x] Establish ESP32-P4 screen + FPV HAT + 1S battery product boundary.
- [x] Record decisions and unresolved facts in `DECISIONS.md`.
- [x] Map the official Waveshare 40-pin header without using USB pins.

## Firmware

- [x] Implement RX5808 tables, register math and 25-bit frame construction.
- [x] Implement ADS1115 reads and battery tap/cell math.
- [x] Implement RSSI smoothing, peak hold and scanner peak selection.
- [x] Implement TVP5150 ID/config/status driver.
- [x] Add GPIO buttons, PWM buzzer, NVS settings and LVGL app launcher.
- [x] Register Up/OK/Down as an LVGL keypad for touch-independent navigation.
- [x] Add host tests and a one-command runner.
- [x] Build the complete ESP32-P4 image with pinned ESP-IDF 6.1 dependencies.
- [ ] Validate DVP `SCLK` polarity/edge and field framing on physical hardware.
- [ ] Complete zero-copy field capture/PPA/display handoff against ESP-IDF v6.1 HIL.
- [ ] Implement and benchmark ESP-Hosted SoftAP + hardware JPEG MJPEG stream.
- [x] Bind live ADS1115 battery/RSSI and active-diversity data into app views,
      retaining explicit empty states when hardware is absent.
- [x] Add touch and Up/Down RX5808 channel control plus an OK-long-press band
      picker to the FPV monitor.
- [x] Add a non-blocking 48-channel receiver sweep, live RSSI chart and
      strongest-three actions with serialized ADC access.
- [x] Add a session-local finder peak, explicit channel lock and non-blocking
      RSSI-driven buzzer cadence.
- [x] Replace static settings mock data with persisted brightness, buzzer,
      video-standard and masked WPA2 password controls.
- [ ] Implement the guided per-tap calibration workflow after the protected
      balance frontend passes bench bring-up.
- [ ] Measure the correct post-tune settle time and derive an RX5808-specific
      RSSI voltage calibration curve.
- [ ] Bind captured video fields into the monitor display after PPA/panel HIL.
- [ ] Validate low-battery shutdown against the Waveshare power controller.
- [x] Read the host board's existing BAT_ADC on GPIO20; do not
      connect the reserved ADS1115 deck-cell input without an approved source.
- [ ] Calibrate GPIO20 BAT_ADC against a DMM across the usable 1S range.

## Hardware

- [x] Generate a checked KiCad 8 schematic, BOM and netlist from source.
- [x] Create a placed PCB design draft and source-controlled board views.
- [ ] Measure actual RX5808 modules; freeze custom footprint and antenna launch.
- [ ] Confirm Waveshare HAT clearance, header orientation and board outline.
- [ ] Simulate/bench the CVBS input termination and TPF131 AV-output branch.
- [ ] Validate comparator hysteresis thresholds across actual RSSI ranges.
- [ ] Add/verify balance-port reverse, partial-insertion and ESD protection.
- [ ] Run independent schematic review, ERC, PCB DRC and 1:1 footprint print.
- [ ] Release fabrication files only after every `hardware/READINESS.md` gate passes.

## Product validation

- [ ] Measure capture-to-panel latency for PAL and NTSC over 1,000 transitions.
- [ ] Measure weak-signal/diversity behavior and video-switch artifacts.
- [ ] Characterize 1S runtime and thermal behavior with 3000/4000 mAh packs.
- [ ] Test battery accuracy for 2S–6S with calibrated precision sources.
- [ ] Design measured enclosure CAD after hardware stack dimensions are frozen.
