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
| D1 | Selected 6.25-inch module has unacceptable latency/outdoor/landscape behavior | High | Medium | CM4-supported bench module; photodiode, luminance, thermal and scan-direction tests; Winstar production sample later |
| D2 | Touch produces false input or poor field/glove response | High | Medium | large controls, flight lock, physical critical paths, water/RF/glove bench matrix |
| M1 | Added mass/heat makes T8L ergonomics poor | High | Medium | measured CAD envelope, center-of-gravity mockup, two-cell energy budget, thermal dummy |
| C1 | Current VRX modules become unavailable | High | Medium | modular 75 Ω CVBS handoff, separate documented power and only optional verified control; qualify two receivers |
| C2 | ADV7282A-M EVM cannot be safely/cheaply bridged to CM4IO | Critical | High | hold VRX/decoder purchase; review 100 Ω SMA-to-CSI fixture, stock and lane mapping; evaluate alternate active decoder/SoM pair |
| E1 | Pre-release PCB pad/net/footprint error reaches fabrication | Critical | Medium | NOT RELEASED marker, native schematic/ERC, source pin-map test, independent audit, 1:1 print, DRC and signed release checklist |
