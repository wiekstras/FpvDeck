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
