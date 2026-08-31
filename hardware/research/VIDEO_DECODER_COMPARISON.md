# CVBS decoder comparison

| Candidate | Output / processing | PAL/NTSC and latency implications | Linux support | Lifecycle/power/cost | Decision/risk |
| --- | --- | --- | --- | --- | --- |
| ADI ADV7280A-M | 1-lane CSI-2 YUV422; line-based I2P | worldwide standards; no field/frame buffer in I2P; artifacts possible | Raspberry Pi `adv7180` driver/overlay explicitly documented | **NRND**, ~€7–15 IC; EVM expensive | **Prototype selected**, not production |
| ADI ADV7282-M | 1-lane CSI-2; line-based I2P; fewer inputs/package variant | same low-delay class | Pi docs say tested with EVAL-ADV7282-M | older family/lifecycle must be checked | acceptable prototype alternative |
| Renesas TW9992 | CSI-2 YUV422/RGB565; 4H comb | PAL/NTSC, weak/nonstandard sync features; exact deinterlace/output field behavior **UNKNOWN** | no maintained mainline/Pi driver confirmed | **Active**, 100 mW typ, automotive | **production lead**, high software risk |
| TI TVP5150AM1 | parallel BT.656, robust sync | worldwide standards; interlaced parallel path needs SoC support/deinterlace | mainline decoder driver exists | Active, old but obtainable | reject for CM4; fallback on parallel-capable SoM |
| USB EasyCAP-class | USB UVC/proprietary, often encoded/buffered | device-dependent buffers/deinterlace | inconsistent chip IDs/drivers | cheap/available | rejected: nondeterministic latency |
| CVBS→HDMI→capture | multiple frame-oriented bridges | commonly ≥frames of buffering | easy generic interfaces | bulky/power-heavy | rejected |

`ADV7280A-M` and `ADV7280A` are not interchangeable interfaces: the `-M` part is
required for CSI-2. No schematic may use the table as a pin reference.

Sources: [ADV7280A product/datasheet](https://www.analog.com/en/products/ADV7280A.html),
[ADV728x line I2P explanation](https://ez.analog.com/video/f/q-a/108553/adv7280-video-shaking-when-using-i2p),
[Pi bridge support](https://github.com/raspberrypi/documentation/blob/master/documentation/asciidoc/computers/camera/csi-2-usage.adoc),
[TW9992](https://www.renesas.com/en/products/tw9992),
[TVP5150AM1](https://www.ti.com/product/TVP5150AM1).

