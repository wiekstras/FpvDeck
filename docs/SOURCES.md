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
- [RX5808 Product Specification V1.0](https://www.foxtechfpv.com/product/5.8G%20modules/rx5808/RX5808-Spec-V1.pdf) — published 28 × 23 × 3 mm envelope, 12-pad electrical order, supply and CVBS characteristics; donor fit still needs measurement.
- [ImmersionRC rapidFIRE operator manual](https://m.media-amazon.com/images/I/B1448syfrzL.pdf) — distinction between proprietary image-fusion mode and classic diversity.
- [TI SN74LVC1G3157](https://www.ti.com/product/SN74LVC1G3157) — analog-switch bandwidth, break-before-make operation and on resistance.
- [TI LMV331](https://www.ti.com/lit/ds/symlink/lmv331.pdf) — DBV pin order, input limits and open-collector comparator behavior.
- [Espressif ESP-IDF v6.1](https://github.com/espressif/esp-idf/tree/v6.1) — ISP-DVP camera APIs and PPA/display stack.

The RX5808 land pattern is provisional because the V1 publication is not a
modern recommended-footprint drawing and the salvaged donor underside has not
yet been measured. It is not promoted to a fabrication-verified footprint.
