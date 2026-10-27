# Verified source register

Primary sources used for code and design decisions:

- [Waveshare product documentation](https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-5) — display, touch, power features and header pinout.
- [Waveshare schematic PDF](https://files.waveshare.com/wiki/ESP32-P4-WIFI6-Touch-LCD-5/ESP32-P4-WIFI6-Touch-LCD-5-Schematic.pdf) — header mapping and onboard buses.
- [Waveshare mechanical sources](https://github.com/waveshareteam/ESP32-P4-WIFI6-Touch-LCD-5/tree/main/hardware/mechanical) — official 2026-04-08 DXF/PDF dimensions and 2026-08-12 STEP model.
- [Waveshare ESP-IDF examples](https://github.com/waveshareteam/ESP32-P4-WIFI6-Touch-LCD-XC) — BSP display/touch initialization.
- [JST XH drawing](https://order.jst-mfg.com/InternetShop/app/pdf_show.php?kbn=1&key=eXH.pdf) — side-entry S7B-XH-A-1 envelope.
- [Amphenol 132289-12 product/drawing](https://www.amphenolrf.com/de-de/part/132289-12/3189/) — active end-launch SMA dimensions.
- [TI TVP5150AM1 SLES209E](https://www.ti.com/lit/gpn/tvp5150am1) — pinout, reset, I²C, output format and status registers.
- [TI ADS1115 SBAS444E](https://www.ti.com/lit/ds/symlink/ads1115.pdf) — PGA, addresses and conversion format.
- [Espressif ESP-IDF v6.1](https://github.com/espressif/esp-idf/tree/v6.1) — ISP-DVP camera APIs and PPA/display stack.

The RX5808 V1 electrical order comes from the project requirement and remains
`NEEDS MEASUREMENT`; it is not promoted to a verified physical footprint.
