# Analog 5.8 GHz VRX comparison

Research refreshed 2026-09-01. This document evaluates **RF receivers**, whose
job is 5.8 GHz RF → CVBS. A VRX is not the T8L's 2.4 GHz ELRS transmitter and is
not the CVBS decoder. Published sensitivity values are not directly comparable
without bandwidth, test criterion, calibration, and video-quality threshold.

## Prototype candidates

| Candidate | RF/video evidence | Control/RSSI evidence | Power/mechanical | Availability snapshot | Integration risk | Decision |
| --- | --- | --- | --- | --- | --- | --- |
| **RC832 Mini, Rotorama product 27309-1** | **CONFIRMED:** 4.9–5.9 GHz, A/B/E/F/R/L/X, 56 ch; PAL/NTSC 1.0 Vp-p/75 Ω; RP-SMA; supplied linear antenna and proprietary AV lead | front-panel FR/CH buttons and retained selection; no host control or RSSI output claimed | **CONFIRMED:** 12 V/130 mA; 50×50×15 mm, 43 g; supplied power cable | Rotorama CZ €55.49, in stock; ships all EU states, €6.49 GLS to Holland | exact OEM and 3.5 mm conductor map unknown, mitigated by using supplied leads; no diversity | **SELECTED for Prototype 0 functionality** |
| Skyzone SteadyView X 5.8G | **CONFIRMED:** 5.3–6 GHz/48 ch; −98 dBm ±1 dB; narrow-band filter; mix 1/2/3, diversity and single; 2× SMA-K 50 Ω; 1.0 Vp-p typ/75 Ω CVBS | wheel and display confirmed; ELRS backpack channel sync confirmed; SBC API and calibrated external RSSI **UNKNOWN** | **CONFIRMED:** 6.5–26 V; 12 V×240 mA normal/180 mA single; 65×32×32 mm receiver, 49 g; 5.5×2.1 mm DC and 3.5 mm video | France €219 before tax and Lithuania €250.23 tax included, both in-stock snapshots | cable conductor map, fusion latency and complete decoder bridge remain open | **Prototype RF-to-CVBS lead; WAIT for complete chain gate** |
| TBS Fusion | **CONFIRMED by manufacturer:** −96 dBm, active video fusion | UART protocol/Crossfire ecosystem documented; exact standalone CVBS breakout still needs adapter validation | goggle-bay module, 5 V×340 mA published | manufacturer previously reported filter-related stock interruption; refresh before any benchmark buy | adapter power/video extraction and lifecycle | A/B benchmark if borrowed/verified, not baseline |
| ImmersionRC rapidFIRE | dual-receiver image reconstruction and strong field reputation | goggle controls; safe external control/RSSI API **UNKNOWN** | goggle-bay module needs power/video adapter | EU listings exist but price/stock and primary technical docs are fragmented | more reverse engineering than standalone kit | benchmark only after exact revision/manual inspection |
| SpeedyBee 5.8 GHz Goggles Receiver Module | current low-cost bay module; CVBS behavior expected for goggles but safe standalone pinout needs primary proof | mobile/app features vary by product revision; external control/RSSI **UNKNOWN** | module-bay power/connector | current retail presence, revision ambiguity | insufficient verified integration documentation | rejected for Rev A baseline |
| RTC6715-class commodity modules | common SPI-tuned analog receiver IC/module class | SPI tuning possible; module RSSI/video pins vary | small/low cost; module quality and filters vary widely | broad but inconsistent supply | custom filtering, shielding, diversity, CVBS switching and undocumented clones | future controlled RF test only |
| Custom dual RF frontend | architecture can expose full control/RSSI | project-defined | optimized size/power possible | requires selected production chipset | RF matching/filter/LNA/shielding/EMC/qualification and multipath algorithms | explicitly deferred beyond first prototypes |

## Selected prototype stage

Prototype 0 uses RC832 Mini because it creates a complete, inexpensive CVBS
handoff immediately. It is not selected for final RF performance and has no
software channel-control claim. Prototype 1 retains SteadyView X as the measured
quality/diversity lead after its downstream decoder bridge is available.

Skyzone SteadyView X is the best-supported standalone RF-to-CVBS lead found. The
[manufacturer page](https://www.skyzonefpv.com/en-de/products/skyzone-stvx-steadyview-x-receiver)
publishes the electrical/video specifications, and current EU listings include
the ground-station power lead, 3.5 mm video cable, RHCP patch, and omni antenna:

- [La Caméra Embarquée, France](https://www.lacameraembarquee.fr/en/fpv-receivers/19734-skyzone-steadyview-x-58ghz-receiver-with-ips-screen.html)
- [Baltic Drones, Lithuania](https://balticdrones.eu/products/vrx-skyzone-steadyview-x-4-9-5-8g-ips)

It is not `BUY NOW` yet. The receiver itself passes gates 1–5—standard analog
5.8 GHz reception, CVBS, known power, supplied output cable, and a compatible
75 Ω decoder input class. The complete chain fails gate 6 because the selected
decoder EVM exposes CSI-2 on SMA and the CM4IO bridge is not designed. The cable
contact map also needs continuity verification, so J4 never assumes its plug
pinout.

## What the ELRS backpack is—and is not

SteadyView X includes an ELRS backpack feature for channel synchronization. It
does not carry the aircraft-control link and does not replace or duplicate the
T8L's built-in 2.4 GHz ELRS transmitter. V1 may leave it unused and set channels
with the receiver's wheel until a published integration protocol is available.

## Bench acceptance matrix

Every candidate uses the same calibrated RF source/VTX, splitter/cabling,
termination, channel, antennas or conducted fixture, and recorded video criterion.

| Test | Evidence required |
| --- | --- |
| CVBS electrical | amplitude, sync level, DC offset and 75 Ω termination for PAL/NTSC |
| Sensitivity | picture-quality/loss threshold across bands with generator level log |
| Selectivity | adjacent-channel rejection at controlled desired/interferer levels |
| Weak signal | noise, tearing, color loss and OSD readability—not just lock/no-lock |
| Multipath | repeatable attenuated/delayed-path fixture; compare mix/diversity modes |
| Latency | ≥1,000 LED/photodiode transitions by receiver mode; p50/p95/p99 |
| Recovery | loss/relock and channel-switch distribution for PAL and NTSC |
| RSSI | repeatability and monotonicity, only if an externally documented value exists |
| EMC | susceptibility to CM4/display/MIPI/DC-DC and emissions into T8L 2.4 GHz control |
| Thermal/power | current and case temperature at min/nominal/max documented supply |

## Remaining unknowns

- SteadyView X 3.5 mm cable conductor map: **NEEDS MEASUREMENT**.
- fusion/image-reconstruction buffering: **NEEDS MEASUREMENT**.
- host-accessible channel control and RSSI: **UNKNOWN; do not guess pins**.
- production integrated receiver chipset/module: **OPEN RESEARCH**, after a proven
  modular prototype and RF requirements derived from measurements.

Additional primary source: [TBS Fusion](https://www.team-blacksheep.com/products/prod%3Atbs_fusion).
