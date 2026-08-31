# Production-oriented BOM (pre-schematic)

No reference designators or quantities are frozen because no release schematic
exists. This table records lifecycle leads only.

| Manufacturer | Candidate MPN | Function | Planning price | Status / alternative |
| --- | --- | --- | ---: | --- |
| Renesas | TW9992AT-NA1-GET | PAL/NTSC CSI-2 decoder | €6–12 | active; driver and full datasheet access gate |
| Winstar | WF50DSYA3MNN0 | 5-inch 1100-nit DSI IPS | €35–70 | sample/init/latency gate |
| ST | STM32G0B1CBT6 class | hardware controller | €3–7 | exact package/flash after pin budget |
| TI | ADS7066RTER | 8-channel 16-bit ADC | €8–15 | frontend candidate; ADS8688A alternative |
| TI | BQ25792RQMR | 1–4S buck-boost charger/NVDC | €8–15 | USB-C policy and thermal validation |
| TI | TPS55288RPMR | main buck-boost | €5–10 | inductor/FET/thermal/EMI design gate |
| NXP/SoM vendor | i.MX 8M Plus industrial SoM TBD | production compute | €90–180 | vendor/lifecycle/BSP evaluation required |
| TDK/Murata/vendor TBD | qualified filters/inductors | power and RF isolation | TBD | select from calculations and stock |

Supplier examples and alternates will be added only when footprints, ratings and
source longevity are verified. Distributor CAD is not accepted as the sole land-
pattern authority; package drawings come from manufacturer datasheets.

