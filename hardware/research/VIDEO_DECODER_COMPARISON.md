# CVBS decoder comparison

| Candidate | Output / processing | PAL/NTSC and latency implications | Linux support | Lifecycle/power/cost | Decision/risk |
| --- | --- | --- | --- | --- | --- |
| ADI ADV7282A-M / `EVAL-ADV7282AMEBZ` | 1-lane CSI-2 YUV422; line-based I2P; six inputs | worldwide standards; no full-frame buffer in I2P; artifacts possible | Pi documents ADV728x(A)-M support; A part is pin/software-compatible with ADV7282-M | **NRND** silicon; EVM has current ADI page/Buy Now but stock must be verified | **Prototype evaluation selection**, not production; EVM CSI lanes are SMA, not Pi FFC |
| ADI ADV7282-M / `EVAL-ADV7282MEBZ` | 1-lane CSI-2; line-based I2P | same low-delay class | exact EVM used for Pi driver test | **NRND**, older EVM availability uncertain | exact software baseline if obtainable; still needs SMA-to-CM4 fixture |
| ADI ADV7280A-M | 1-lane CSI-2 YUV422; line-based I2P | same low-delay class | Raspberry Pi `adv7180` driver/overlay explicitly documented | **NRND**, EVM expensive | alternative only; no advantage over the better-documented ADV7282 paths |
| Renesas TW9992 | CSI-2 YUV422/RGB565; 4H comb | PAL/NTSC, weak/nonstandard sync features; exact deinterlace/output field behavior **UNKNOWN** | no maintained mainline/Pi driver confirmed | **Active**, 100 mW typ, automotive | **production lead**, high software risk |
| TI TVP5150AM1 | parallel BT.656, robust sync | worldwide standards; interlaced parallel path needs SoC support/deinterlace | mainline decoder driver exists | Active, old but obtainable | reject for CM4; fallback on parallel-capable SoM |
| USB EasyCAP-class | USB UVC/proprietary, often encoded/buffered | device-dependent buffers/deinterlace | inconsistent chip IDs/drivers | cheap/available | rejected: nondeterministic latency |
| CVBS→HDMI→capture | multiple frame-oriented bridges | commonly ≥frames of buffering | easy generic interfaces | bulky/power-heavy | rejected |

The `-M` suffix is mandatory for CSI-2. The A and non-A ADV7282 parts are documented
as pin/software-compatible, but the exact Raspberry Pi driver test used the older
`EVAL-ADV7282-M`. The currently documented `EVAL-ADV7282AMEBZ` exposes D0± and
CLK± on SMA connectors for a MIPI analyzer. It does not accept a Raspberry Pi
camera cable. A controlled-impedance SMA-to-CM4IO CSI fixture and a reviewed I²C/
power connection are separate bench deliverables. No schematic may use this table
as a pin reference.

Sources: [ADV7280A product/datasheet](https://www.analog.com/en/products/ADV7280A.html),
[ADV728x line I2P explanation](https://ez.analog.com/video/f/q-a/108553/adv7280-video-shaking-when-using-i2p),
[Pi bridge support](https://github.com/raspberrypi/documentation/blob/master/documentation/asciidoc/computers/camera/csi-2-usage.adoc),
[ADV7282A-M evaluation board](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-adv7282a-m.html),
[TW9992](https://www.renesas.com/en/products/tw9992),
[TVP5150AM1](https://www.ti.com/product/TVP5150AM1).
