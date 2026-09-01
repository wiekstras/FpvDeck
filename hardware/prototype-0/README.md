# Prototype 0 — Bench FpvDeck

Prototype 0 is the fastest real hardware vertical slice. It proves the radio,
analog video, Linux capture, RGB compositor, touch UI and recording workflow on
a desk. It is explicitly a **functionality test**, not a latency benchmark.

```text
                  RADIO CONTROL

RadioMaster T8L
      │
      └── 2.4 GHz ELRS ───────────────────────→ Air65


                       VIDEO

Air65 analog camera + Betaflight OSD
      │
      └── onboard 5.8 GHz analog VTX
                       │
                       ▼
                  RC832 Mini VRX
                       │  CVBS (yellow RCA)
                       ▼
                 Gembird UVG-002
                       │  USB / V4L2
                       ▼
                 Raspberry Pi 5
                       │
                FpvDeck software
                   ↙          ↘
              FPV video      RGB UI
                   ↘          ↙
              5-inch Touch Display 2
```

## Reference hardware

- Raspberry Pi 5 2 GB + official 27 W supply + Active Cooler
- Raspberry Pi Touch Display 2, 5 inch, SC1975
- RC832 Mini 56-channel receiver kit from Rotorama
- Gembird UVG-002, UTV007/UTVF007 chipset family
- separate protected 12 V receiver supply

See [Buy Tonight](../PROTOTYPE_0_BUY_TONIGHT.md), [Wiring](WIRING.md), and the
[complete setup guide](../../docs/PROTOTYPE_0_SETUP.md).

## Exit criteria

- T8L remains bound and controls the Air65; props remain removed on the bench.
- The receiver shows the Air65 feed and embedded Betaflight OSD.
- Linux exposes UVG-002 through V4L2 (`usbtv` for UTV007 or `uvcvideo` for
  UTVF007) and reports a usable PAL or NTSC-derived stream.
- FpvDeck renders full-colour overlays over live video without app failure.
- Touch, menus, signal loss/reconnect and a short DVR recording work.
- Capture latency is recorded as a baseline, not accepted as a final target.

## Later stages

| Stage | Purpose |
|---|---|
| Prototype 0 | Functionality: real RF video, touch, apps and recording |
| Prototype 1 | Latency: direct decoder and display-path measurements |
| Prototype 2 | Modular custom controller/balance/power PCB |
| Prototype 3 | Integrated, mechanically validated FpvDeck |
