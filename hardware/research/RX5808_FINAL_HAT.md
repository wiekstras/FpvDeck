# RX5808 and final-HAT research

This document separates published facts, observations from the supplied donor
photo, and measurements still required before fabrication. The checked KiCad
board is a final-form layout candidate, not an order authorization.

## Evidence summary

| Item | Status | Evidence |
| --- | --- | --- |
| RX5808 electrical interface | **CONFIRMED for Spec V1** | [RX5808 Product Specification V1.0](https://www.foxtechfpv.com/product/5.8G%20modules/rx5808/RX5808-Spec-V1.pdf) |
| Published module envelope | **CONFIRMED for Spec V1** | 28 × 23 × 3 mm in the same specification |
| Donor module identity | **LIKELY RX5808-compatible** | shielded modules and donor PCBs are marked `RX5808` in the supplied photo |
| Donor pad geometry/order | **NEEDS MEASUREMENT** | underside/castellations are not visible and the cutting mat is perspective-distorted |
| Waveshare header signals | **CONFIRMED** | [official Waveshare pin table](https://docs.waveshare.com/ESP32-P4-WIFI6-Touch-LCD-5#Pinout_Definition) and schematic |
| rapidFIRE-equivalent fusion | **NOT IMPLEMENTED** | requires more than a single analog selector and decoder |

The user-supplied reference image is stored as
[`hardware/reference/rx5808-donor-boards.jpg`](../reference/rx5808-donor-boards.jpg).
It is useful for identifying the donor construction, but not as dimensional
metrology.

## Published RX5808 V1 contract

The 2010 module specification gives:

- 5.0 V nominal supply, 3.5–5.5 V range;
- 170 mA typical and 180 mA maximum supply current per receiver;
- NTSC/PAL wideband-FM video reception from 5705–5945 MHz in the original
  eight-channel parallel-selection mode;
- approximately -90 dBm typical receive sensitivity;
- 1.0 Vpp nominal composite output into 75 ohms;
- 50-ohm RF input;
- 28 × 23 × 3 mm module envelope;
- twelve contacts: three RF-side contacts and nine control/power/output-side
  contacts.

| Pad | Signal | FpvDeck use |
| ---: | --- | --- |
| 1 | RF ground | RF return / via-fenced ground |
| 2 | antenna | short 50-ohm grounded-coplanar launch |
| 3 | RF ground | RF return / via-fenced ground |
| 4 | CH1 / DATA | shared serial data |
| 5 | CH2 / LE | separate latch enable per receiver |
| 6 | CH3 / CLK | shared serial clock |
| 7 | power ground | ground plane |
| 8 | +5 V | individually ferrite-filtered receiver rail |
| 9 | RSSI | comparator and ADS1115 #3 |
| 10 | 6.5 MHz audio | unused test-only node |
| 11 | composite video | diversity switch input |
| 12 | power ground | ground plane |

The old project model omitted pad 12. The current source model and provisional
footprint include all twelve pads.

### Parallel-select versus serial programming

The RX5808 V1 module sheet labels pads 4–6 as `CH1`, `CH2`, and `CH3` and only
documents the original eight-channel parallel truth table. The underlying
RichWave RTC6715 supports either strap selection or a three-wire serial port,
but serial mode depends on the module's internal `SPI_SE` configuration. The
donor DVR boards clearly controlled more than eight channels, so serial mode is
**LIKELY**, not yet confirmed at the removed-module pads.

FpvDeck maps pad 4 to 3.3 V `RX_DATA`, pad 5 to a separate latch-enable, and
pad 6 to shared `RX_CLK`. Firmware sends the 25-bit word LSB-first (4-bit
address, write bit, 20-bit data) with 3 us phases. Both receivers receive the
same DATA/CLK and are latched independently, which lets firmware tune and test
one receiver before commanding both to the same channel.

Before soldering a salvaged module to the HAT:

1. inspect whether its internal serial-mode strap was already changed by the
   donor manufacturer; do not copy internet resistor-location photos across
   unverified module revisions;
2. power one module from a current-limited 5.0 V supply (250 mA limit), with a
   5.8 GHz antenna attached;
3. drive pads 4–6 only with 3.3 V logic and tune between two known transmitters
   or widely separated channels;
4. confirm frequency movement from video/RSSI response before fitting module B;
5. record underside photographs, continuity results, modification state, and
   measured current in the engineering log.

Failure to tune is a bring-up failure, not evidence that higher GPIO voltage is
needed. The HAT must never drive an ESP32-P4 GPIO with 5 V.

## Provisional land pattern

`hardware/fpv-hat.pretty/RX5808_V1_PROVISIONAL.kicad_mod` follows the V1 drawing:

- 28 × 23 mm body outline;
- 2.54 mm pitch along the nine-pad edge;
- three 2.54 mm-pitch RF contacts at the opposite lower corner;
- 1.6 mm pad width from the published drawing, with carrier lands extended to
  2.2 mm in the solder direction for an inspectable fillet.

The publication is not a modern recommended-land-pattern drawing and the
salvaged modules may be a variant. Before ordering the HAT, measure **each donor
module** with calipers and photograph it square-on beside a steel rule:

1. overall PCB width, length and shield height;
2. pad count and numbering orientation;
3. edge-to-first-pad center on both edges;
4. pad pitch, copper width and copper length;
5. shield overhang and any underside component keep-out;
6. continuity from pads 1/3/7/12 to the shield;
7. pad 8 to the donor board's 5 V rail, with the board unpowered;
8. pad 11 to the donor composite-video route;
9. pad 9 to the donor RSSI route.

Print the KiCad footprint at 1:1 and place all salvaged modules over it. A single
photo cannot close this fabrication gate.

## Receiver power and RF layout

Two receivers can draw 360 mA worst-case from 5 V before the decoder, ADCs,
display host, or Wi-Fi loads are counted. Rev A therefore gives each receiver a
separate BLM21PG221 ferrite, 100 nF local ceramic, and 10 uF local bulk
capacitor. The Waveshare 5 V header path must be load-tested from both USB and
the intended 1S battery before field use.

Both receivers are rotated so their antenna pads face the board edge. The two
SMA center pins are aligned with pad 2 and the generated critical launch is only
about 2.3 mm long. Final trace width and ground-gap values remain stack-up
dependent; `1.0 mm` in the generated file is a visible placeholder, not a
50-ohm impedance claim. The released board needs a four-layer stack-up and a
field-solver-backed grounded-coplanar calculation plus ground-via fencing.

## Diversity: rapidFIRE versus FpvDeck

The [rapidFIRE operator manual](https://m.media-amazon.com/images/I/B1448syfrzL.pdf)
states that its proprietary mode fuses two images and digitally reconstructs
video, while also offering a separate classic-diversity mode. It does not
publish enough implementation detail to reproduce that fusion architecture.

FpvDeck Rev A implements **classic fast diversity**:

1. both RX5808 modules tune together using shared DATA/CLK and separate LE;
2. LMV331 compares the two RSSI voltages with hysteresis;
3. SN74LVC1G3157 selects one composite stream before the decoder;
4. TVP5150AM1 locks to and digitizes the selected stream;
5. ESP32-P4 receives both RSSI readings plus the actual select state.

The [SN74LVC1G3157](https://www.ti.com/product/SN74LVC1G3157) is an appropriate
signal switch on paper: 340 MHz typical bandwidth, break-before-make operation,
approximately 6-ohm typical on resistance, and sub-nanosecond propagation at
3.3 V. Its switching speed is much faster than a video line; receiver lock and
switching policy, not switch propagation, dominate visible disruption.

JP1 defaults to autonomous comparator control. Cutting its 1–2 bridge and
closing 2–3 hands selection to ESP32-P4 GPIO2. This optional mode permits a
future policy using calibrated RSSI, hysteresis, TVP lock state and field
boundaries. It still cannot fuse simultaneous images because only one composite
stream reaches one decoder. The UI and documentation must not call it
rapidFIRE.

The comparator uses the TI DBV pin order (1 IN+, 2 GND, 3 IN-, 4 OUT, 5 VCC).
Its open-collector output is high when RSSI A is stronger. The 3157 DBV truth
table connects pin 1/B2 when select is high, so `VIDEO_A` is deliberately on
pin 1 and `VIDEO_B` on pin 3/B1. Generator assertions lock both mappings.

## Decoder and ESP32-P4 interface

The selected 1 Vpp/75-ohm composite path is attenuated by the specified
37.4-ohm series / 37.4-ohm shunt network before AC coupling into TVP5150 AIP1A.
The [TVP5150AM1](https://www.ti.com/product/TVP5150AM1) produces 8-bit BT.601
YCbCr plus discrete pixel clock, HSYNC and VSYNC. Eleven 33-ohm source series
resistors sit at the decoder before the 40-pin header.

The AV jack is a separate high-impedance branch from `VIDEO_SELECTED`. A 100 nF
input capacitor feeds a 3PEAK TPF131-TR sync-clamp/video driver using its
published SOT-23-6 pinout (2 output, 4 input, 5 ground, 6 supply). Its 6 dB gain
is followed by a 75-ohm source resistor, so a terminated external monitor sees
approximately unity gain while the TVP5150 branch is not loaded by the cable.
The output is DC-coupled as permitted by the datasheet. C26/C27 provide the
required local ceramic and bulk bypass. This branch still requires oscilloscope
validation into a real 75-ohm load before release.

The official Waveshare header exposes the 21 ordinary GPIOs used by the DVP bus,
receiver serial controls, reset, buttons, buzzer and diversity state, plus the
shared GPIO7/GPIO8 I2C bus. GPIO2/header pin 8 is now the optional MCU video
select. USB-HS pins remain untouched.

## Balance reader

J4 is a side-entry JST `S7B-XH-A-1(LF)(SN)` for ground plus six cumulative taps.
The external pack powers nothing. Each tap uses a 100 k / 12 k 1% divider and a
10 nF C0G filter before the ADS1115. At 25.5 V (six cells at 4.25 V), the nominal
ADC node is 2.73 V, inside a 3.3 V-powered ADS1115 and the selected ±4.096 V PGA
range.

The [ADS1115 absolute ratings](https://www.ti.com/lit/ds/symlink/ads1115.pdf)
permit only GND−0.3 V to VDD+0.3 V at an analog input, although continuous pin
current is limited to ±10 mA. The 100 k top resistors limit fault current, but
reverse insertion, ESD and every partial-insertion sequence still require bench
fault testing before the balance port is approved for real packs. No generated
render or passing ERC substitutes for that test.

## Release boundary

The current board now contains all 102 modeled parts, both receiver locations,
both SMAs, diversity comparator/switch, TVP5150, three ADS1115s, JST-XH, AV out,
buzzer, buttons, decoder supplies and test points. Only the two critical RF
launches are routed. Remaining routing, planes, impedance definition, ERC/DRC,
footprint overlay and electrical bring-up remain mandatory before fabrication.
