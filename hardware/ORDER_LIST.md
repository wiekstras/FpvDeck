# First bench prototype order list

This legacy global list is retained for design context. EU purchasing uses the
current compatibility-gated [EU order list](ORDER_LIST_EU.md) and
[buy-now list](BUY_NOW.md). Recheck stock, VAT, and exact MPN at checkout.

## Already owned

- **1× RadioMaster T8L.** It supplies the independent 2.4 GHz ELRS control radio,
  its RF path, two Hall gimbals, useful controls/controller electronics, and the
  initial two-18650 arrangement subject to verification. It supplies **no analog
  VRX, decoder, display, media reader, or balance checker**.

## Compute — order now

- **1× Raspberry Pi CM4, `CM4104032` (4 GB / 32 GB eMMC)** — about US$85. This is
  the documented Prototype 1 CSI/DSI/H.264 platform. Any authentic CM4 eMMC model
  with at least 2 GB RAM is compatible if this exact RAM variant is unavailable.
- **1× official Raspberry Pi Compute Module 4 IO Board** — about US$45. It exposes
  both 22-pin CSI and DSI ports and avoids a custom high-speed carrier initially.
- **1× regulated CM4IO supply** — use the official barrel-jack requirements or a
  current-limited lab supply; budget US$15–25.
- **1× USB A-to-micro-B data cable** for eMMC flashing and **1× Ethernet cable**.

## Display — order now

- **1× Waveshare `6.25inch DSI LCD (B)`, SKU 35000** — US$59.99 list. It includes
  12 cm DSI cable, two 15-pin FPC cables, a PH2.0 four-wire 5 V lead and screws.
  This is the selected 720×1560, 500 cd/m², optical-bonded five-touch display.
  Do not buy discontinued SKU 28918.

## Video

- **1× Skyzone SteadyView X ground-station kit — WAIT FOR CHAIN GATE.** It provides
  the dedicated 5.8 GHz RF-to-CVBS stage: standalone 1 Vpp/75 Ω CVBS, two SMA
  antenna ports and 6.5–26 V input. The EU kits identified in 2026 include one
  RHCP patch and one omni antenna, so do not separately buy antennas initially.
- **1× Analog Devices `EVAL-ADV7282AMEBZ` — WAIT FOR STOCK + BRIDGE.** Budget
  US$250–350. It evaluates the ADV7282A-M, which ADI says
  is pin/software-compatible with ADV7282-M; Raspberry Pi tested its driver with
  the older `EVAL-ADV7282MEBZ`. Both silicon families are NRND and are prototype
  tools, not production selections. The EVM routes CSI D0± and CLK± to SMA ports,
  not a camera FFC. Obtain UG-1175 and the schematic before buying/building the
  separate controlled-impedance SMA-to-CM4IO fixture. If an exact older
  `EVAL-ADV7282MEBZ` is genuinely available, it is the closest Pi-tested baseline.
- **2× 75 Ω BNC/coax patch leads**, **1× BNC-to-RCA adapter**, and **1× 75 Ω
  feed-through terminator** — budget US$35. Match the actual EVM/VRX connectors.

Do not substitute a USB EasyCAP or HDMI capture device as the latency baseline.

## Controller / balance development — order now

- **1× ST `NUCLEO-G0B1RE`** — about US$25. This lets the protocol, ADC SPI and
  power-state firmware progress while the custom board is under review.
- **1× TI `ADS8688EVM-PDK`** if available and below the US$250 allowance. This is
  useful for validating register configuration and the input-impedance model, but
  it is optional if Test PCB Rev A is released promptly.
- **1× ST `STLINK-V3MINIE`** plus compatible SWD cable — about US$15–25.
- **1× JST `B7B-XH-A(LF)(SN)`** and several mating XH housings/crimp contacts for
  physical fit testing. Never make a male-to-male lead that could backfeed a pack.
- Precision bench source or battery simulator capable of 0–25.5 V at a 5 mA
  current limit. An ordinary bench supply plus resistor ladder is adequate for
  initial cumulative-tap tests; it does not replace independent-cell tests.

The decoder EVM and VRX are not unconditional purchases. The display, CM4, MCU,
and media reader remain useful without them, while the analog-video pair waits
for a reviewed EVM-SMA-to-CM4IO CSI bridge.

## Test PCB

- **Bare PCB: WAIT.** The current KiCad project is real and exportable but remains
  unrouted and release-gated. Do not upload its generated Gerbers to a fab.
- Components may be pre-ordered in small quantities if desired. Recommended spare
  quantities: 2× ADS8688A, 3× STM32G0B1, 10× Vishay matched arrays, 20× each
  precision resistor/filter capacitor, 2× every connector.
- Use the exact [Test PCB BOM](bom/TEST_PCB_REV_A.md), not a screenshot or this
  prose list, for purchasing.

## Power — bench only now

- 0–15 V laboratory supply with adjustable current limit and readable current.
- Inline 0.5 A fuse holder, 22 AWG silicone wire, 4 mm safety leads, and shrouded
  adapters. Budget US$30 excluding the supply.
- Do **not** buy final 18650 cells, charger PCB or T8L power adapters yet. The T8L
  input and final protected pack topology still require measurement and safety
  review.

## Debugging

- Required: multimeter with millivolt resolution, current-limited supply, SWD
  debugger and USB-UART adapter with selectable 3.3 V I/O.
- Strongly recommended: 8-channel logic analyzer at 24 MS/s or better, 100 MHz
  oscilloscope, differential probe for switching-power work, and thermometer or
  thermal camera.
- Latency fixture later: two fast photodiodes, transimpedance/front-end parts,
  high-brightness LED and microcontroller trigger board.

## Mechanical / temporary assembly

- M3 nylon standoffs, non-conductive baseplate, hook-and-loop straps, printed/foam
  display buck, short 75 Ω coax, and FFC strain relief — budget US$25–40.
- Do not attach the large display to the T8L until the pinch-grip finger envelope
  has been checked with a non-powered mock-up.

## Optional

- 128 GB SanDisk High Endurance microSD for removable-media/DVR tests — US$20.
- Saleae Logic 8 or equivalent known-good analyzer.
- TBS Fusion/rapidFIRE as an RF/latency A/B benchmark if already available; neither
  is required to start.
