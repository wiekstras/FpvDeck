# Analog VRX comparison

| Candidate | Published performance/features | Integration | Availability/power/cost | Decision |
| --- | --- | --- | --- | --- |
| Skyzone SteadyView X 5.8G | -98 dBm ±1 dB, narrow-band filter, two SMA, mix/diversity/single, 48 channels, 1 Vpp/75 Ω CVBS | standalone ground-station kit and AV cable; ELRS backpack; control API not documented | current product, 6.5–26 V, 12 V×240 mA typical, ~€120–160 | **Prototype lead** |
| TBS Fusion | -96 dBm, active video fusion, UART protocol, Crossfire integration | goggle module plus power adapter; explicit control protocol | manufacturer out of stock due filter shortage, 5 V×340 mA, US$135 | A/B benchmark; supply risk |
| ImmersionRC rapidFIRE | strong community reputation, dual receiver/image repair | goggle module; current primary specifications/stock fragmented | regional stock only, premium | benchmark if obtainable |
| Foxeer WildFire | dual receiver/diversity/image reconstruction | goggle/ground modules existed | lifecycle/stock unclear | reject baseline |
| Discrete RTC6715 modules | SPI tuning, common 5.8 GHz implementation | custom RF filters/LNA/diversity/video switching required | cheap, highly variable modules | future research only |
| Custom integrated dual RF | maximum control and packaging | RF matching, SAW/LNA, shielding, qualification and multipath algorithms | high NRE/risk | explicitly deferred beyond V1 |

Published sensitivity values are not directly comparable without bandwidth,
method, BER/SNR criterion and unit calibration. Bench tests use the same calibrated
RF source, splitter/cabling, antennas or conducted fixtures, and video criterion.

Measure each receiver's single/diversity/fusion latency, sensitivity, adjacent-
channel rejection, recovery, multipath, RSSI repeatability, channel switch time,
noise injected into CVBS, and immunity to FpvDeck's own 2.4 GHz/digital clocks.

Sources: [SteadyView X](https://www.skyzonefpv.com/en-de/products/skyzone-stvx-steadyview-x-receiver),
[TBS Fusion](https://www.team-blacksheep.com/products/prod%3Atbs_fusion).
