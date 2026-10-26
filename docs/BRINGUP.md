# HAT bring-up

Do not connect an aircraft battery during first bring-up.

1. Compare assembled PCB to schematic/BOM and inspect orientation under magnification.
2. With power removed, measure resistance from 5 V, 3.3 V, 1.8 V analog and 1.8 V digital to ground.
3. Power the Waveshare board alone from current-limited USB and record header rails.
4. Fit a HAT with RX modules and external battery disconnected. Set a conservative current limit.
5. Confirm 5 V/3.3 V, then 1.8 V before touching firmware.
6. Scan shared I²C. Expect board devices plus 0x48, 0x49, 0x4A and 0x5C; investigate every difference.
7. Read TVP5150 IDs 0x51/0x50; inspect RESETB and 14.31818 MHz clock.
8. Fit RX A with an antenna/dummy load, tune a known legal channel, inspect CVBS on a 75 Ω instrument.
9. Verify RSSI voltage never exceeds the ADS1115 or ESP32 rail under all RF conditions.
10. Capture a static PAL/NTSC source; confirm data order, PCLK edge, sync polarity and field height.
11. Enable the panel handoff, then overlay, and measure dropped fields/latency.
12. Fit RX B; sweep a controlled RF source and verify `ACTIVE_RX` polarity and switch hysteresis.
13. Test AV out into 75 Ω only after its review gate closes.
14. Inject cumulative tap voltages with six isolated/current-limited channels. Verify raw ADC, calibration, 2S–6S math, missing tap and partial insertion.
15. Only after fault testing, connect a fused real pack while monitoring current and temperature.

Pass/fail evidence belongs in `ENGINEERING_LOG.md`; failures create persistent
items in `TODO.md`.
