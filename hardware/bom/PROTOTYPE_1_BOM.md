# Prototype 1 planning BOM

This is a purchase-planning list, not an approved assembly BOM. Prices are rough
2026 single-unit allowances excluding VAT/shipping. Refresh stock before ordering.

| Qty | Manufacturer | MPN / exact item | Description | Allowance | Datasheet/source | Lifecycle / alternative |
| ---: | --- | --- | --- | ---: | --- | --- |
| 1 | Raspberry Pi | CM4104032 or chosen 4 GB/32 GB eMMC CM4 variant | compute module | €85 | [CM4 brief](https://datasheets.raspberrypi.com/cm4/cm4-product-brief.pdf) | through ≥Jan 2034; exact variant verify |
| 1 | Raspberry Pi | CM4 IO Board | documented bench carrier | €45 | [CM docs](https://www.raspberrypi.com/documentation/computers/compute-module.html) | other proven carriers acceptable |
| 1 | Raspberry Pi | Touch Display 2, 5-inch | 720×1280 DSI/touch prototype display | €45 | [display docs](https://www.raspberrypi.com/documentation/accessories/touch-display-2.html) | Waveshare DSI D alternative |
| 1 | Analog Devices | EVAL-ADV7282-MEBZ or verified ADV7280A-M EVM | CVBS-to-CSI bridge evaluation | €250 allowance | [ADV7280A](https://www.analog.com/en/products/ADV7280A.html) | exact stock/connector kit verify |
| 1 | Skyzone | SteadyView X 5.8G ground-station kit | dual analog receiver/CVBS out | €150 | [product](https://www.skyzonefpv.com/en-de/products/skyzone-stvx-steadyview-x-receiver) | TBS Fusion/rapidFIRE benchmarks |
| 1 | ST | NUCLEO-G0B1RE | MCU development board | €25 | [board family](https://www.st.com/en/evaluation-tools/nucleo-g0b1re.html) | exact current stock verify |
| 1 | Texas Instruments | ADS7066EVM-PDK | ADC candidate A evaluation | €180 allowance | [EVM](https://www.ti.com/product/ADS7066) | ADS8688A candidate B |
| 1 | Texas Instruments | ADS8688EVM-PDK or current ADS8688A EVM | protected-AFE ADC evaluation | €250 allowance | [ADS8688A](https://www.ti.com/product/ADS8688A) | exact A-version EVM compatibility verify |
| 1 | Texas Instruments | BQ25792EVM | charger/power-path evaluation | €220 allowance | [EVM](https://www.ti.com/tool/BQ25792EVM) | do not attach cells before procedure review |
| 1 | Texas Instruments | TPS55288EVM-053 | 5 V rail evaluation | €150 allowance | [EVM](https://www.ti.com/tool/TPS55288EVM-053) | output configuration/thermal design required |
| 2 | TBD | 50 Ω lab antennas / conducted fixture | VRX comparison | €60 | fixture design pending | connectors/adapters included |
| 1 | TBD | protected 2S2P pack test fixture | energy source, fused/monitored | €100 | pack design pending | bench supply first |
| lot | known vendors | documented cables, FFC adapters, 75 Ω coax/termination | interconnect/test | €150 | freeze in wiring doc | no improvised CSI wiring |

Expected module/evaluation spend is roughly €1,700 before instruments. EVM cost is
intentional: it buys known-good references and reduces the risk of debugging an
unproven decoder, precision AFE and power board simultaneously.

