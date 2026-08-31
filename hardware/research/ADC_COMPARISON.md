# Battery ADC comparison

| Candidate | Specs relevant here | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- | --- |
| TI ADS7066 | 8 ch, 16 bit, 250 kSPS, 0–Vref/2Vref, 2.5 V internal ref, averaging, SPI, 4 mW | compact/low power, CRC-capable protocol, enough channels for 8S | external divider/protection/buffer; mux settling and hot-plug proof | **EVM candidate A** |
| TI ADS8688A | 8 ch, 16 bit, 500 kSPS, independent ranges to ±10.24 V, 1 MΩ AFE, internal ref, ±20 V OVP, SPI, 65 mW | robust integrated input AFE, easy settling, industrial range | power/size/cost; 8S still exceeds range/protection without attenuation | **EVM candidate B** |
| TI ADS1115 ×2 | 4 ch each, 16-bit delta-sigma, I²C | cheap dev boards, slow signals acceptable | two references/gain paths, weak hot-plug protection, mux/I²C robustness | Prototype 1 quick experiment only |
| ADI LTC6811 | 12 direct cells, 16-bit, <1.2 mV max total error | excellent cell-stack measurement/open-wire features | stack-powered/high-voltage BMS architecture; arbitrary 1S checker plug-in mismatch | rejected for V1 checker |
| MCU ADC | typically 12-bit, shared reference | no extra IC | insufficient protected precision/repeatability and coupling | rejected |

Approximate single-unit IC cost is not frozen; planning range is roughly €8–25
depending on ADC and source. Integration risk dominates this selection.

Sources: [ADS7066](https://www.ti.com/product/ADS7066),
[ADS8688A](https://www.ti.com/product/ADS8688A),
[ADS1115](https://www.ti.com/product/ADS1115),
[LTC6811](https://www.analog.com/en/products/ltc6811-1.html).
