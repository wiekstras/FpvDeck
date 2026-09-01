# Buy now — EU bench kit

> **Prototype 1 archive — do not use this list for tonight's functional bench.**
> The current short, complete Prototype 0 purchase decision is
> [PROTOTYPE_0_BUY_TONIGHT.md](PROTOTYPE_0_BUY_TONIGHT.md). The CM4/direct-decoder
> items below remain later latency-bench research.

Checked 2026-09-01. Prices and stock are snapshots, not quotations; open every
linked product page and verify exact MPN, VAT destination, stock, and connector
contents at checkout. This list intentionally excludes the analog-video chain
until its decoder-to-CM4 bridge is credible.

## Already provided by the donor — do not buy again

- RadioMaster T8L and its 2.4 GHz ExpressLRS transmitter/RF path;
- two Hall gimbals and usable T8L switches/buttons;
- T8L control electronics;
- dual-18650 compartment/arrangement, pending electrical verification.

The T8L provides **no 5.8 GHz analog VRX, CVBS decoder, HD display, removable
media reader, or external-battery measurement frontend**.

## Purchase-cleared independent bench items

| Qty | Exact item | EU source snapshot | Purpose |
| ---: | --- | --- | --- |
| 1 | Raspberry Pi `CM4104032`, CM4 4 GB/32 GB eMMC/WLAN | [BerryBase, €127.10 incl. VAT](https://www.berrybase.de/en/raspberry-pi-compute-module-4-4gb-ram-32gb-flash-wlan-bt) ([Farnell alternative, €138.23 incl. VAT](https://nl.farnell.com/en-NL/raspberry-pi/cm4104032/rpi-module-4-4gb-ram-32gb-emmc/dp/3563490?gross_price=true)) | Linux/video/UI evaluation |
| 1 | Raspberry Pi Compute Module 4 IO Board | [BerryBase, €35.90 incl. VAT](https://www.berrybase.de/en/raspberry-pi-compute-module-4-io-board) ([Rasppishop alternative, €35.90 incl. VAT](https://www.rasppishop.de/CM4-IO-Board)) | documented CSI/DSI/USB carrier |
| 1 | Waveshare `6.25inch DSI LCD (B)`, SKU 35000 | [OpenELAB Germany, €74.95 incl. VAT](https://openelab.de/products/waveshare-6-25inch-dsi-lcd-b-capacitive-touch-display) ([manufacturer direct, about €52 before import/VAT](https://www.waveshare.com/product/luckfox/6.25inch-dsi-lcd-b.htm)) | selected 720×1560 five-touch bench display |
| 1 | Waveshare `19134` CM4 DSI adapter | [BerryBase listing on CM4IO page, €2.50 incl. VAT](https://www.berrybase.de/en/raspberry-pi-compute-module-4-io-board) | 22-pin CM4IO to display FFC adaptation; verify included cables first |
| 1 | Mean Well `GST36E12-P1J`, 12 V/3 A | [Reichelt, €14.95 incl. VAT](https://www.reichelt.de/de/de/shop/produkt/steckernetzteil_36_w_12_v_3_a-171106) ([Farnell alternative, €18.23 ex VAT](https://de.farnell.com/en-DE/mean-well/gst36e12-p1j/adaptor-ac-dc-12v-3a/dp/2815899)) | isolated CM4IO bench supply; lab supply remains preferred for fault work |
| 1 | ST `NUCLEO-G0B1RE` | [Mouser, €16.80 snapshot](https://www.mouser.es/es/ProductDetail/STMicroelectronics/NUCLEO-G0B1RE?qs=pUKx8fyJudAJjwoMOxyZhg%3D%3D) ([ST product page](https://www.st.com/en/evaluation-tools/nucleo-g0b1re.html)) | firmware/USB/SPI bring-up without Test PCB |
| 1 | ST `STLINK-V3SET` | [Mouser Germany, €39.10 snapshot](https://www.mouser.de/de/ProductDetail/STMicroelectronics/STLINK-V3SET?qs=qSfuJ%252Bfl%2Fd5d4WYsOW6M6w%3D%3D) ([V3MINIE smaller alternative](https://www.mouser.de/de/ProductDetail/STMicroelectronics/STLINK-V3MINIE?qs=MyNHzdoqoQKcLQe5Jawcgw%3D%3D)) | external SWD/debugger; Nucleo's onboard probe covers the first firmware step |
| 1 | Transcend `TS-RDF5K` USB-A SD/microSD reader | [Reichelt, €7.95 incl. VAT](https://www.reichelt.de/de/de/shop/produkt/card_reader_extern_usb_3_2_gen_1_schwarz_rdf5-147618) ([white TS-RDF5W alternative](https://www.reichelt.de/de/de/shop/produkt/card_reader_extern_usb_3_0_weiss_rdf5-147619)) | removable goggle/DVR media on CM4IO USB; not internal eMMC storage |
| 1 | SanDisk `SDSQQNR-128G-GN6IA` High Endurance 128 GB | [MediaMarkt, €33.99 incl. VAT](https://www.mediamarkt.de/de/product/_sandisk-high-endurance-2551316.html) ([current EU offer index](https://geizhals.de/sandisk-high-endurance-r100-w60-microsdxc-128gb-kit-sdsqqnr-128g-gn6ia-a2041130.html)) | repeatable removable-media/DVR endurance testing |

Nominal purchase-cleared subtotal: **€353.24 incl. VAT where shown**, excluding
shipping and assuming the lower-priced debugger/card/display sources above. Add a
10% cable/adapter contingency: **€388.56**. This is not the whole Prototype 1.

## Buy small samples, not a board build

- 2× JST `B7B-XH-A(LF)(SN)` from [Mouser Germany](https://www.mouser.de/ProductDetail/JST-Commercial/B7B-XH-ALFSN?qs=cdbOS8ANM9DLeRSemn13Pg%3D%3D) for exact footprint/fit inspection.
- 1–2× ADS8688AIDBT only if useful for stocking consolidation; the package cannot
  be meaningfully evaluated on a solderless breadboard. [Mouser Europe showed
  €25.08 and stock](https://eu.mouser.com/en/ProductDetail/Texas-Instruments/ADS8688AIDBT?qs=xufs61hA5MZv%2F46DP%252BYoHw%3D%3D).
- mating XH housings/contacts only for a keyed simulator harness; never construct
  a lead capable of backfeeding a real pack.

## Explicitly not purchase-cleared

- SteadyView X 5.8 GHz VRX: product stage is credible, but the complete
  VRX→decoder→CM4 chain has not passed its gate;
- `EVAL-ADV7282AMEBZ`: restricted distributor availability and no reviewed
  SMA-to-CM4IO CSI bridge;
- Test PCB bare boards, stencil, or full component quantities;
- another ELRS transmitter module, extra 18650s, charger/power board, or T8L
  internal harnesses.
