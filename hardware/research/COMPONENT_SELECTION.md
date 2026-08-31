# Component selection summary

Research snapshot: 2026-08-31. Prices are rough single-unit planning figures and
must be refreshed at purchase time. Availability means the manufacturer lifecycle
and visible supply posture, not a procurement guarantee.

| Function | Prototype decision | Production lead | Rationale / open gate |
| --- | --- | --- | --- |
| SoM | Raspberry Pi CM4 4 GB/32 GB eMMC | i.MX 8M Plus industrial SoM | CM4 has documented ADV728x support and H.264 encode; i.MX offers stronger industrial multimedia/lifecycle options |
| Decoder | ADV7280A-M/ADV7282-M EVM | Renesas TW9992 | proven Pi driver versus active lifecycle; exact latency and TW9992 driver are open |
| Display | 5-inch Pi DSI fallback + Waveshare 6.25-inch evaluation | 5.5–6.25-inch high-brightness DSI, TBD | larger/high-mounted product direction; brightness, landscape scan, latency, power and fit remain gates |
| VRX | Skyzone SteadyView X | modular dual receiver | explicit standalone CVBS, strong published sensitivity; latency A/B mandatory |
| MCU | STM32G0B1 Nucleo | STM32G0B1CB/CE class | USB, timers, watchdog, internal flash, CAN-FD/UCPD headroom |
| ADC | ADS7066 and ADS8688A EVMs | not frozen | low-power compact versus protected integrated AFE |
| Charger | BQ25792 EVM | BQ25792-class | integrated 1–4S buck-boost/NVDC; thermal/PD/protection design open |
| 5 V converter | TPS55288 EVM | TPS55288-class | sufficient topology/current headroom; stability/EMI/thermal proof required |
| Storage | CM4 eMMC + removable USB/SD export during prototype | soldered industrial eMMC plus recovery partition | avoid SD as primary writable root; exact production MPN deferred to SoM |
| Audio | MCU buzzer only | optional I²S codec later | avoid scope/power until a real use case exists |

## Primary sources

- Raspberry Pi CM4 brief: <https://datasheets.raspberrypi.com/cm4/cm4-product-brief.pdf>
- Raspberry Pi CSI-2 bridge documentation: <https://github.com/raspberrypi/documentation/blob/master/documentation/asciidoc/computers/camera/csi-2-usage.adoc>
- NXP i.MX 8M Plus: <https://www.nxp.com/products/i.MX8MPLUS>
- Renesas TW9992: <https://www.renesas.com/en/products/tw9992>
- Analog Devices ADV7280A: <https://www.analog.com/en/products/ADV7280A.html>
- ST STM32G0B1: <https://www.st.com/en/microcontrollers-microprocessors/stm32g0b1cb.html>
- TI BQ25792: <https://www.ti.com/product/BQ25792>
- TI TPS55288: <https://www.ti.com/product/TPS55288>
- Waveshare 6.25-inch DSI LCD: <https://www.waveshare.com/6.25inch-dsi-lcd.htm>
- Focus LCDs E55RB-I-MW400-C: <https://focuslcds.com/product/e55rb-i-mw400-c/>
