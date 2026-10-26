# Video path and latency

```text
5.8 GHz RF -> RX5808 -> CVBS -> 74LVC1G3157 -> TVP5150AM1
-> 8-bit YCbCr 4:2:2 field -> ESP32-P4 ISP -> RGB565
-> PPA scale/90-degree rotation -> LVGL overlay -> MIPI-DSI panel
```

## Strategy

The system captures a single 288-line PAL or 240-line NTSC field. It does not
wait for or synthesize a deinterlaced frame. This accepts interlace artifacts in
motion to avoid a full frame of latency. PPA scaling/rotation must write into a
display buffer while LVGL draws only sparse overlay areas.

## Initial latency budget

| Stage | Engineering target | Confidence |
| --- | ---: | --- |
| RX demodulator + CVBS path | < 1 ms incremental | needs measurement |
| decoder lock/pipeline | < 1 field | data-sheet architecture; measure |
| field acquisition | 16.7 ms NTSC / 20 ms PAL maximum | structural |
| ISP/PPA handoff | < 3 ms | benchmark required |
| display scan alignment | 0–16.7 ms at 60 Hz | panel phase dependent |
| FpvDeck added | target median < 25 ms, p95 < 40 ms | unvalidated |

The primary acceptance test is an LED in the camera view plus a display-facing
photodiode sampled by an oscilloscope. Record at least 1,000 transitions for PAL
and NTSC and publish median/p95/p99. A high-speed camera is a secondary check.

## Open integration gate

ESP-IDF v6.1 provides ISP DVP input with HSYNC/VSYNC and YUV422→RGB565. The
current driver configures and captures fields. Zero-copy PPA-to-panel reuse and
the correct sampling edge/polarity must be validated on real TVP5150 hardware.
