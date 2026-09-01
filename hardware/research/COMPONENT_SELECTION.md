# Component selection summary

Research snapshot: 2026-09-01. Prices are rough single-unit planning figures and
must be refreshed at purchase time. Availability means the manufacturer lifecycle
and visible supply posture, not a procurement guarantee.

| Function | Prototype decision | Production lead | Rationale / open gate |
| --- | --- | --- | --- |
| SoM | Raspberry Pi CM4 4 GB/32 GB eMMC | i.MX 8M Plus industrial SoM | CM4 has documented ADV728x support and H.264 encode; i.MX offers stronger industrial multimedia/lifecycle options |
| Decoder | `EVAL-ADV7282AMEBZ` candidate, older `EVAL-ADV7282MEBZ` software baseline | Renesas TW9992 | **WAIT:** direct distributor availability is restricted and the EVM's MIPI SMA-to-CM4IO bridge is not designed; NRND prototype only |
| Display | **Waveshare 6.25inch DSI LCD (B), SKU 35000** | 5.5–6.25-inch high-brightness DSI, TBD | 720×1560, 500-nit, optical-bonded 5-point touch with documented CM4 support; landscape scan, latency, power, outdoor and pinch-grip fit remain measurement gates |
| VRX | dedicated Skyzone SteadyView X 5.8G ground-station kit | modular dual receiver; integrated RF later | explicit RF-to-CVBS output and EU stock; **WAIT** until decoder/CM4 chain gate; latency A/B mandatory |
| MCU | STM32G0B1 Nucleo | STM32G0B1CB/CE class | USB, timers, watchdog, internal flash, CAN-FD/UCPD headroom |
| ADC | ADS8688A for Test PCB Rev A | ADS8688A unless measurement rejects it | eight protected 16-bit inputs cover B−, deck and six taps; physical accuracy/safety validation still required |
| Charger | BQ25792 EVM | BQ25792-class | integrated 1–4S buck-boost/NVDC; thermal/PD/protection design open |
| 5 V converter | TPS55288 EVM | TPS55288-class | sufficient topology/current headroom; stability/EMI/thermal proof required |
| Storage | CM4 eMMC + Transcend TS-RDF5K USB removable SD/microSD reader | soldered industrial eMMC plus separate removable-media interface | avoid SD as primary writable root; removable media is for goggle/DVR footage and test cards |
| Audio | MCU buzzer only | optional I²S codec later | avoid scope/power until a real use case exists |

## Primary sources

- Raspberry Pi CM4 brief: <https://datasheets.raspberrypi.com/cm4/cm4-product-brief.pdf>
- Raspberry Pi CSI-2 bridge documentation: <https://github.com/raspberrypi/documentation/blob/master/documentation/asciidoc/computers/camera/csi-2-usage.adoc>
- NXP i.MX 8M Plus: <https://www.nxp.com/products/i.MX8MPLUS>
- Renesas TW9992: <https://www.renesas.com/en/products/tw9992>
- Analog Devices ADV7280A: <https://www.analog.com/en/products/ADV7280A.html>
- Analog Devices ADV7282A-M EVM: <https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-adv7282a-m.html>
- ST STM32G0B1: <https://www.st.com/en/microcontrollers-microprocessors/stm32g0b1cb.html>
- TI BQ25792: <https://www.ti.com/product/BQ25792>
- TI TPS55288: <https://www.ti.com/product/TPS55288>
- Waveshare 6.25-inch DSI LCD (B): <https://www.waveshare.com/6.25inch-dsi-lcd-b.htm>
- Focus LCDs E55RB-I-MW400-C: <https://focuslcds.com/product/e55rb-i-mw400-c/>
