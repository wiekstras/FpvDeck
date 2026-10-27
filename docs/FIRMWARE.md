# Firmware

ESP-IDF v6.1, C, LVGL 9.5, and Waveshare BSP 1.0.4 are pinned. The firmware is
deliberately direct: device modules expose small functions and applications call
them without a service/plugin framework.

## Modules

| Module | Responsibility |
| --- | --- |
| `rx5808` | table lookup, register math, 25-bit LSB-first tuning |
| `tvp5150` | reset, ID verification, format/output setup, lock status |
| `video` | ESP32-P4 ISP-DVP one-field RGB565 capture |
| `ads1115` | 16-bit single-shot reads with selectable PGA |
| `battery` | 500 ms ADS sampling, calibrated taps, cell subtraction and limits |
| `signal_logic` | serialized dual-RSSI sampling, smoothing/peak, scanner ranking and finder cadence |
| `settings` | versioned settings blob in NVS |
| `buttons` / `buzzer` | local controls and PWM tones |
| `deck_power` | calibrated onboard GPIO20 BAT_ADC and warning thresholds |

## Build

```sh
./scripts/setup-idf
./scripts/build-firmware
```

Flash only after confirming the board revision and serial device:

```sh
source .deps/esp-idf/export.sh
idf.py -p /dev/ttyACM0 flash monitor
```

## Host tests

```sh
./scripts/test-host
```

Hardware-free tests compile the exact production math sources under GCC. Video
DMA, display handoff, I²C electrical behavior, Wi-Fi/JPEG coexistence and power
shutdown require hardware-in-loop tests.

Acquisition runs outside the LVGL task. UI timers only copy the latest snapshot,
so slow ADS conversions cannot stall touch or rendering. Until RX5808 RSSI is
bench-calibrated, it is displayed as volts rather than a fabricated percentage.
The scanner owns serialized raw reads while sweeping all 48 A/B/E/F/R/L table
entries, settles 80 ms after each retune, and restores the saved monitor channel
when a sweep completes. Both the settle time and voltage-to-signal mapping require
measurement with the selected physical RX5808 modules.

The finder uses a 150–900 ms beep interval mapped over the ADS1115's 0–2.048 V
RSSI input span. This mapping is deterministic and host-tested, but provisional:
physical RX5808 characterization must replace the full-scale mapping with useful
near/far thresholds before it can be considered a calibrated field instrument.
