# Battery ADC comparison

| Candidate | Specs relevant here | Advantages | Disadvantages | Decision |
| --- | --- | --- | --- | --- |
| TI ADS7066 | 8 ch, 16 bit, 250 kSPS, 0–Vref/2Vref, 2.5 V internal ref, averaging, SPI, 4 mW | compact/low power, CRC-capable protocol, enough channels for 8S | external divider/protection/buffer; mux settling and hot-plug proof | **EVM candidate A** |
| TI ADS8688A | 8 ch, 16 bit, 500 kSPS, ranges including 0–10.24 V, 1 MΩ AFE, internal 4.096 V reference, ±20 V OVP, SPI, 65 mW | robust integrated AFE, modelable 0.85–1.15 MΩ input, industrial range, channel alarms | power/size/cost; bias and input-impedance drift must be included in divider calibration | **SELECTED for Test PCB Rev A** |
| TI ADS1115 ×2 | 4 ch each, 16-bit delta-sigma, I²C | cheap dev boards, slow signals acceptable | two references/gain paths, weak hot-plug protection, mux/I²C robustness | Prototype 1 quick experiment only |
| ADI LTC6811 | 12 direct cells, 16-bit, <1.2 mV max total error | excellent cell-stack measurement/open-wire features | stack-powered/high-voltage BMS architecture; arbitrary 1S checker plug-in mismatch | rejected for V1 checker |
| MCU ADC | typically 12-bit, shared reference | no extra IC | insufficient protected precision/repeatability and coupling | rejected |

The Rev A ADS8688A design uses a matched 3:1 divider plus 1 kΩ series resistance,
the 0–10.24 V input range, and per-channel two-point calibration. See
[battery measurement](../../docs/BATTERY_MEASUREMENT.md) and the executable error
budget. Eight-cell support remains outside Rev A.

Sources: [ADS7066](https://www.ti.com/product/ADS7066),
[ADS8688A](https://www.ti.com/product/ADS8688A),
[ADS1115](https://www.ti.com/product/ADS1115),
[LTC6811](https://www.analog.com/en/products/ltc6811-1.html).
