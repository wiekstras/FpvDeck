# Latency budget and validation

Latency is a release gate for FpvDeck. Values in this document are either
targets or estimates until a physical chain has been measured. They are not
product claims.

## Selected Prototype 1 path

```text
camera -> FC analog OSD -> VTX/RF -> SteadyView X CVBS
       -> ADV7282A-M evaluation hardware (I2P enabled)
       -> one-lane CSI-2 -> V4L2/DMABUF -> Qt scene graph compositor
       -> DRM/KMS -> Waveshare 6.25inch DSI LCD (B)
```

The decoder and display are evaluation choices, not production freezes.
Raspberry Pi documents ADV728x-M support through the Linux `adv7180` driver and
states that the CSI receiver does not support interlaced input. The `-M` CSI
variant with the line-based I2P block is therefore mandatory for this path.
Analog Devices describes I2P as line based; it avoids a full-frame
motion-adaptive deinterlacer but still needs physical latency verification.

The evaluation board is not a cable-ready Pi accessory. ADI routes its CSI clock
and data pairs to SMA connectors for a MIPI analyzer. The CM4 experiment therefore
needs a reviewed controlled-impedance SMA-to-CSI fixture plus I²C/power wiring;
Test PCB Rev A's generic decoder header is not that high-speed fixture.

The live branch must use V4L2 buffers exported as DMABUFs and imported by a
custom Qt Quick scene-graph video item. CPU copies, a `QVideoSink` image-copy
loop, HDMI capture, USB EasyCAP devices, and GStreamer elements with implicit
queues are excluded from the release path unless measurements overturn this
decision. The recorder receives a tee from capture and may drop recording
frames; it must never back-pressure the display branch.

## Working budget

The table deliberately gives ranges because analog cameras, VRX fusion,
decoder lock, scanout phase, and panel electronics vary. `FpvDeck added` starts
at VRX CVBS output and ends at emitted display light.

| Stage | Working estimate | Status / validation need |
| --- | ---: | --- |
| FPV camera exposure/readout | 4–20 ms | aircraft dependent; measure fixture source |
| FC analog OSD insertion | <1 ms target | generally line-domain; verify reference FC |
| VTX + RF propagation | <1 ms target | propagation is negligible; VTX processing varies |
| Diversity/fusion VRX | 0–5 ms target | **NEEDS MEASUREMENT** in diversity and mix modes |
| ADV728x I2P + CSI launch | below one field target | line-based, exact value **NEEDS MEASUREMENT** |
| CSI/V4L2 buffer availability | 16.7–40 ms phase dependent | NTSC/PAL field/frame boundary behavior must be traced |
| DMABUF import / handoff | <2 ms p95 | benchmark on target kernel and CM4 |
| compositor wait + scanout phase | 0–16.7 ms at 60 Hz | cap queued frames and present newest complete field |
| display electronics + pixel response | 8–25 ms provisional | module processing and landscape scan **UNKNOWN** |

Initial engineering targets:

- VRX-CVBS-to-display-photon: **45 ms p95 target**, **33 ms stretch goal**.
- Full camera-glass-to-display-glass: expected roughly **45–110 ms** depending
  on the aircraft camera and asynchronous phase; establish a measured
  distribution before advertising a number.
- Signal recovery to useful image: **<250 ms p95 target** after a stable CVBS
  signal returns.
- UI touch-to-visible-feedback: **<100 ms p95**, without increasing video queue
  depth or frame misses.

PAL fields arrive every 20 ms; NTSC fields arrive approximately every 16.67 ms.
Both standards must be tested. A result obtained with a static test pattern or
one scanout phase is insufficient.

## Queue and scheduling rules

1. Capture uses the smallest stable V4L2 buffer set; start with three buffers
   and test two. A newly completed field replaces stale unpresented content.
2. No unbounded queue is permitted between decoder and display.
3. The Qt render thread performs only texture import and overlay composition.
   Database, thumbnails, network, and file I/O stay off it.
4. Overlay animation is disabled or simplified if it changes video frame-time
   p99 by more than 1 ms or causes missed presentations.
5. DVR encoding has bounded input queues. On overload, recording reports an
   error/drop while live display continues.
6. Linux services may restart independently. T8L/ELRS control never depends on
   Linux, capture, touch, or display.

## Measurement fixture

Use an electronically timed fixture, with a high-speed camera as a useful
cross-check rather than the only clock.

### Preferred method

1. Drive a fast LED in view of the FPV camera from a microcontroller GPIO.
2. Split or buffer the same GPIO edge into logic-analyzer channel A.
3. Affix a reverse-biased photodiode over a high-contrast patch on the FpvDeck
   display and feed its conditioned output to channel B.
4. Alternate black/white source states using a pseudo-random interval so display
   scanning cannot phase-lock to the stimulus.
5. Capture at least 1,000 transitions for PAL and NTSC, with DVR off/on and UI
   animations off/on.
6. Report minimum, median, p95, p99, maximum, standard deviation, and lost or
   duplicated-field count. Retain raw traces and software/kernel versions.

The source LED transition to display photodiode transition measures complete
glass-to-glass behavior. To isolate FpvDeck-added delay, also inject a generated
CVBS pattern whose electrical transition is recorded on channel A. A dual-head
photodiode placed over source and display can be used when electrical access is
impractical.

### High-speed-camera cross-check

Record the source LED and display in the same frame at at least 1,000 fps. Count
frames across many transitions and account for rolling shutter. This method is
good at revealing scan direction, tearing, deinterlacing artifacts, and spatial
latency variation, but its quantization and shutter timing are weaker than the
electronic fixture.

## Test matrix

| Variable | Required cases |
| --- | --- |
| Standard | PAL, NTSC |
| Signal | strong, weak/noisy, loss and recovery, rolling/nonstandard sync |
| VRX | diversity, mix/fusion, each antenna input independently |
| Recording | off, pre-overlay, composited, both requested |
| UI | clean FPV, controls visible, notification/animation load |
| Thermal | cold start, steady-state warm, thermal-warning boundary |
| Display | portrait-native panel mounted landscape; both image rotations |
| Storage | normal, slow card, nearly full, removed during recording |

Landscape scan direction is important: the selected panel is portrait-native.
Measure the top, center, and bottom of the landscape image because the perceived
delay and tearing location can vary by a significant fraction of a 60 Hz frame.

## Instrumentation to implement

- timestamp decoder/CSI buffer completion with `CLOCK_MONOTONIC_RAW`;
- timestamp DMABUF import, scene-graph render start/end, and page-flip events;
- expose queue depth, dropped/stale fields, frame-time p50/p95/p99, and decoder
  lock transitions in Diagnostics;
- store a benchmark manifest containing hardware revision, kernel, device-tree
  overlay, application commit, display, decoder, VRX mode, and video standard;
- add `tools/video-latency-report` once real trace format is established.

## Decision gates

- Do not select a USB or HDMI capture path because it is convenient; accept one
  only if its p95 measured latency and recovery meet the same gate.
- Do not freeze the Waveshare module for enclosure tooling until scan behavior,
  brightness, touch, thermal load, and p95 latency are measured.
- Do not claim dual DVR streams are practical until encoder load and display
  frame-time remain inside limits during power-loss and slow-storage tests.

Primary-source links and exact module candidates are maintained in
[VIDEO_DECODER_COMPARISON.md](../hardware/research/VIDEO_DECODER_COMPARISON.md),
[DISPLAY_COMPARISON.md](../hardware/research/DISPLAY_COMPARISON.md), and
[SBC_COMPARISON.md](../hardware/research/SBC_COMPARISON.md).
