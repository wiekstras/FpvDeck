# Performance and benchmark plan

| Workload | Metric | Initial gate | Instrumentation |
| --- | --- | ---: | --- |
| boot | power-good to first valid live frame | <8 s | MCU timestamp + photodiode |
| video | CVBS-in to photons | p95 ≤45 ms | dual photodiode/scope |
| video | capture/display continuity | source-rate, no app-caused stalls | frame sequence + DRM flips |
| overlay | input event to changed pixel | p95 <50 ms | MCU event + photodiode |
| UI | scene render rate | 60 Hz nominal | Qt frame timing |
| DVR | press to active status | <500 ms | monotonic trace |
| DVR | live latency delta while recording | <2 ms p95 delta | paired latency trials |
| VRX | channel request to stable lock | <500 ms target | RF/video fixture |
| services | idle resident memory | <350 MB CM4 target | cgroup/`smaps` |
| thermals | 60 min sun-equivalent steady state | no throttling at design ambient | thermocouples + SoC telemetry |
| power | typical FPV+DVR input | ≤16 W target | calibrated power analyzer |
| runtime | typical brightness, healthy two-cell 18650 pack | ≥1.25 h; 1.5 h stretch | coulomb/energy log |

Benchmarks report versions, build mode, backend, fixture, environment and full
distributions. Desktop simulator numbers are tracked for regressions but never
substituted for target hardware evidence.

## Desktop first-frame benchmark

Run:

```sh
./scripts/benchmark-ui
```

The Release build performs one excluded warm-up and three fresh-process runs of
Home, FPV, FPV controls, Battery, Media, playback, and Diagnostics under a
1280×720 Xvfb display. Each app writes schema-versioned JSON with QML-load time,
first swapped frame, resident memory, dimensions, state, and timeout status to
`generated/benchmarks/ui/`. The summary reports median and p95. Override sample
count with `FPVDECK_BENCHMARK_RUNS`; keep it at least three for comparisons.

This measures process start to first rendered frame on the development computer.
It does not measure touch-to-pixel response, steady-state frame pacing, decoded
video latency, CM4 startup, DRM/KMS, or physical display scanout. Those require
the trace/photodiode methods above. Do not compare results from different hosts,
Qt versions, render backends, or build modes without recording those differences.
