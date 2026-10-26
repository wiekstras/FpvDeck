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
| `battery` | calibrated taps, cell subtraction, limits and storage state |
| `signal_logic` | RSSI smoothing/peak and scanner strongest-three |
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
