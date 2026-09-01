# Prototype cost model

Snapshot: 2026-09-01, EUR. Prices are unit planning values; VAT treatment is noted
where known and shipping is excluded. The model separates money that unlocks
useful work now from the unresolved analog-video chain.

| Group | Subtotal | Confidence / assumption |
| --- | ---: | --- |
| CM4 + CM4IO | €163.00 | German retail, VAT included |
| 6.25-inch touch + DSI adapter | €77.45 | German retail, VAT included; adapter may already be unnecessary |
| CM4IO 12 V supply | €14.95 | German retail, VAT included |
| Nucleo + external ST-LINK | €55.90 | Nucleo price locale/VAT verify; V3SET snapshot |
| USB removable-media reader + 128 GB endurance card | €41.94 | German retail, VAT included |
| **BUY NOW subtotal** | **€353.24** | before shipping; minor rounding/source drift from live pages expected |
| Cable, headers and bench-fixture contingency | €50 | allowance |
| **Immediate working budget** | **€403.24** | does not include lab instruments |

## Gated additions

| Group | Allowance | Gate |
| --- | ---: | --- |
| SteadyView X ground-station kit, antennas/cables included | €219–250 | complete decoder/CM4 chain credibility; cable continuity test |
| ADV7282A-M evaluation board | €203–300 | real stock, board revision and NRND acceptance |
| controlled-impedance MIPI SMA→CM4IO bridge | €100–250 | schematic, connector mapping, layout and fab quotes |
| 75 Ω coax/adapters/termination | €35–70 | exact receiver cable and EVM connector inventory |
| Test PCB Rev A, five boards + components + stencil | €220–400 | schematic/ERC, safety, routing/DRC, footprint and release gates |
| isolated precision tap simulator/fixture | €100–250 | fixture design and existing lab-equipment inventory |

Full bench architecture allowance is therefore approximately **€1,280–1,920**
including the immediate kit, gated video chain, a released Test PCB build, and
fixtures—but excluding oscilloscope, lab supply, multimeter, shipping and import
charges. This replaces the earlier misleading single total that treated all EVMs
as simultaneously orderable.

The T8L is already owned. The model contains **no extra ELRS transmitter** and no
separate initial VRX antennas because the selected SteadyView X EU kits include a
patch and an omni antenna.
