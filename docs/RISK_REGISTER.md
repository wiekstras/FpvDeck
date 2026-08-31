# Risk register

| ID | Risk | Severity | Likelihood | Mitigation / exit evidence |
| --- | --- | --- | --- | --- |
| V1 | Digital path exceeds usable FPV latency | Critical | High | photodiode distributions; depth-one DMABUF/KMS path; reject parts/modes over budget |
| V2 | Decoder line-doubling artifacts obscure detail | High | Medium | PAL/NTSC flight-scene A/B; tune filters; compare active alternatives without frame buffering |
| V3 | ADV7280A lifecycle ends prototype path | High | High | isolate decoder board; develop TW9992 driver/carrier; lifetime buy is not default strategy |
| R1 | Digital noise degrades 5.8 GHz VRX | Critical | Medium | module baseline, physical zones, filtered rails, shield option, spectrum/RSSI A/B |
| R2 | Display/Wi-Fi noise degrades 2.4 GHz ELRS | Critical | Medium | Wi-Fi off, preserve antenna geometry, conducted/radiated tests at each subsystem state |
| P1 | T8L power topology/load margin misunderstood | Critical | Medium | non-invasive measurement; independent supplies until verified; no unknown pin drive |
| P2 | Linux power loss corrupts storage | High | Medium | MCU shutdown state machine, segmented DVR, WAL/FULL sync, 1,000-cycle fault test |
| B1 | Balance connector hot-plug/reverse causes damage | Critical | Medium | protected EVM, arbitrary-order test matrix, fault calculations, keyed enclosure |
| S1 | Recorder/app back-pressures live video | Critical | Medium | process/queue isolation, newest-frame policy, forced I/O/CPU fault tests |
| D1 | High-brightness panel lacks supported init/stock | High | Medium | prototype supported panel; obtain Winstar samples/init support before carrier freeze |
| M1 | Added mass/heat makes T8L ergonomics poor | High | Medium | measured CAD envelope, center-of-gravity mockup, 2S/2S2P trade study, thermal dummy |
| C1 | Current VRX modules become unavailable | High | Medium | standardized modular CVBS/power/control bay; qualify two receivers |

