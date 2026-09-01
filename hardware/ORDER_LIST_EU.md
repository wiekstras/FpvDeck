# EU Prototype 1 order list

**Research snapshot:** 2026-09-01. **Architecture:** T8L supplies control/2.4 GHz
ELRS only; FpvDeck adds a separate 5.8 GHz analog VRX and a separate CVBS decoder.
The machine-readable companion is
[`bom/PROTOTYPE_SYSTEM_EU.csv`](bom/PROTOTYPE_SYSTEM_EU.csv).

Status meanings: `BUY NOW` is independently usable and interface-credible;
`BUY SOON` requires a named check that does not invalidate the rest of the kit;
`WAIT` has an unresolved compatibility, safety, lifecycle, or PCB release gate.

## Already owned

| Item | Source | Boundary |
| --- | --- | --- |
| RadioMaster T8L donor | user-owned | 2.4 GHz ELRS TX/RF, gimbals, useful controls, controller electronics, initial two-18650 arrangement pending measurement |

No additional ELRS transmitter is required. None of the video, display, compute,
removable-media, or balance-measurement hardware comes from the T8L.

## Compatibility chain

```text
Aircraft 5.8 GHz analog VTX
  → SteadyView X 5.8 GHz VRX
  → documented 1.0 Vp-p / 75 Ω CVBS output
  → ADV7282A-M decoder candidate
  → one-lane MIPI CSI-2 on EVM SMA connectors
  → UNDESIGNED controlled-impedance SMA-to-CM4IO bridge
  → Raspberry Pi CM4
  → DSI
  → Waveshare 6.25-inch capacitive touchscreen
```

The first two handoffs are documented. The CSI bridge is not. Therefore the VRX
and decoder are `WAIT`, not `BUY NOW`, despite current receiver stock.

## Compute, display, media and development

The purchase-cleared list, source alternatives, VAT notes and live-price snapshot
are maintained in [BUY_NOW.md](BUY_NOW.md). These items total about €353 before
shipping and contingency.

## Dedicated 5.8 GHz analog VRX — WAIT: chain gate

**Lead:** Skyzone SteadyView X 5.8G ground-station kit, quantity 1.

- **CONFIRMED:** standard 5.8 GHz analog reception; two SMA-K 50 Ω antenna ports;
  1.0 Vp-p typical/75 Ω CVBS; 6.5–26 V input; supplied XT60-to-barrel power lead,
  3.5 mm video cable, RHCP patch, and omni antenna.
- **EU primary:** [La Caméra Embarquée, France, €219 before tax shown, in stock](https://www.lacameraembarquee.fr/en/fpv-receivers/19734-skyzone-steadyview-x-58ghz-receiver-with-ips-screen.html).
- **EU alternative:** [Baltic Drones, Lithuania, €250.23 taxes included, in stock](https://balticdrones.eu/products/vrx-skyzone-steadyview-x-4-9-5-8g-ips).
- **Manufacturer reference:** [Skyzone, US$183 plus checkout shipping/tax](https://www.skyzonefpv.com/en-de/products/skyzone-stvx-steadyview-x-receiver).
- **UNKNOWN:** conductor map of the supplied 3.5 mm cable; Linux/SBC channel-control
  protocol; externally accessible calibrated RSSI; fusion latency.
- **Action:** continuity-map the supplied cable and verify terminated CVBS on an
  oscilloscope/display before any custom harness. Use its wheel for channel control
  until a published protocol exists.

The two supplied antennas are sufficient for first testing. Do not buy separate
antennas now; verify connector gender and polarization on receipt before spares.
The receiver's ELRS *backpack* is for VTX/VRX channel synchronization and is not a
replacement or duplicate of the T8L flight-control transmitter.

## CVBS decoder — WAIT: stock + bridge gate

**Candidate:** Analog Devices `EVAL-ADV7282AMEBZ`, quantity 1, allowance €250.

- [ADI evaluation-board page](https://www.analog.com/en/resources/evaluation-hardware-and-software/evaluation-boards-kits/eval-adv7282a-m.html)
  and [UG-1175](https://www.analog.com/media/en/technical-documentation/user-guides/eval-adv7282amebz-ug-1175.pdf)
  confirm the decoder role and EVM interfaces.
- [Mouser Germany](https://www.mouser.de/en/ProductDetail/Analog-Devices/EVAL-ADV7282AMEBZ?qs=1mbolxNpo8dXn4uu0WGlNw%3D%3D)
  reported restricted availability when directly checked; an earlier search cache
  showed €203.05/limited stock, so only the product page at checkout is authoritative.
- The EVM exposes CSI clock/data on SMA, not a Raspberry Pi camera FFC. A reviewed
  100 Ω differential bridge with continuous return, exact lane mapping, and CM4IO
  connector mapping is required. This is not an ordinary cable purchase.
- ADV7282A-M is NRND. It may be a latency prototype tool but is not the frozen
  production decoder. The older `EVAL-ADV7282MEBZ` is the closest Raspberry Pi
  driver-validation baseline if a genuine board can be borrowed.

## Test PCB and balance frontend — WAIT: board release

The exact orderable component set is
[`TEST_PCB_REV_A.csv`](bom/TEST_PCB_REV_A.csv). Its major parts include
STM32G0B1CBT6, ADS8688AIDBT, exact Vishay matched resistor arrays, JST
B7B-XH-A(LF)(SN), protected bench input, regulation, probes and connectors.

Do not order bare boards or the full BOM. The current board is unrouted, lacks a
native reviewed schematic/ERC result, and has an open pack-return safety gate.
`hardware/pcb/test-board-rev-a/READINESS.md` lists the release evidence.

## Power boundary

- **Bench:** Mean Well 12 V supply for CM4IO and a separate current-limited lab
  supply for Test PCB/fault work.
- **VRX:** use the receiver's documented 6.5–26 V input and supplied harness;
  Test PCB J4 does not power it.
- **Decoder:** use only the EVM's documented supply; J5 does not power it.
- **Future deck:** T8L two-18650 source is `NEEDS MEASUREMENT`; no charger,
  series/parallel topology, capacity, or safe added-load budget is assumed.
- **External balance port:** measurement only and never powers FpvDeck.

## Before checkout

1. Re-open every product page and save invoice/MPN/board revision.
2. Confirm CM4IO/display cable contents before ordering the adapter twice.
3. Keep SteadyView X and ADV7282 EVM out of the cart until the CSI bridge review.
4. Do not purchase another ELRS TX, generic EasyCAP, HDMI capture dongle, bare Test
   PCB, final battery/charger, or undocumented RF module.
