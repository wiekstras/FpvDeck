# Latency budget and measurement

## Budget

No decoder or display latency has been measured yet. The values below are design
allocations, not part specifications.

| Stage, CVBS input onward | Allocation | Confidence / main risk |
| --- | ---: | --- |
| VRX fusion before CVBS output | 0–5 ms desired | **UNKNOWN**, test mix/diversity/single modes |
| CVBS decoder + line I2P | <1 field, desired <2 ms | Line-based, but exact delay **UNKNOWN** |
| CSI receive until usable completed buffer | 8–20 ms | Likely dominant; full-frame DMA behavior |
| buffer handoff + texture import | <2 ms | Requires DMABUF zero-copy proof |
| compositor scheduling | 0–16.7 ms | phase and queue depth |
| DSI scanout + panel response | 8–25 ms | panel scan direction/controller **UNKNOWN** |
| **CVBS-in to photon target** | **p95 ≤45 ms** | stretch goal ≤33 ms |

Full glass-to-glass includes camera sensor/readout, FC OSD insertion, VTX/VRX RF,
and phase between source and display. Report both full path and CVBS-in-to-display
so handheld changes are distinguishable from aircraft changes.

## Measurement fixture

1. Drive an addressable high-speed LED and a video stimulus transition from the
   same microcontroller edge. For full-path tests, point the FPV camera at the LED.
   For decoder-only tests, generate a valid CVBS white/black transition locked to
   a separate trigger output.
2. Place matched photodiodes on the source LED and a small high-contrast LCD patch.
   Capture both analog signals plus trigger on an oscilloscope. A ≥1,000 fps camera
   is a secondary cross-check, not the primary sub-millisecond instrument.
3. Log at least 1,000 randomized transitions. Compute median, p95, p99, minimum,
   maximum, and histogram; do not quote only the fastest event.
4. Repeat PAL/NTSC, each VRX mode, each display brightness, DVR off/on, composited
   recording if supported, thermal steady-state, weak RF, and CPU stress.
5. Instrument V4L2 dequeue, compositor submit, DRM page-flip, and frame sequence.
   Correlate software timestamps to the physical distribution.
6. Store raw traces, fixture firmware, scope settings, versions, and wiring photos.

The fixture itself needs a loopback calibration measuring LED/photodiode channel
skew. Screen photodiode placement must account for landscape rotation and panel
scan direction; test top, center, and bottom.

