# Balance-lead battery measurement

## Measurement model

Each JST-XH signal is cumulative relative to pack negative. Software converts
validated cumulative taps to cells by subtraction. The implemented pure function
rejects nonmonotonic taps, nonfinite values, >8 taps, and derived cells outside a
broad 0.5–5.0 V electrical sanity range. Chemistry warnings are a separate policy.

## Frontend requirements

No tap connects directly to an MCU ADC. Each line needs connector-side fault
current limiting, ESD/transient control selected for the actual maximum pack and
clamp current, precision divider/buffer or an integrated protected AFE, RC filtering,
open-wire detection, and safe behavior for arbitrary hot-plug order. Pack negative
needs its own protection/current path and cannot depend on a signal clamp.

Target after calibration: ±10 mV per derived cell from 0–50 °C, with a stretch
goal of ±5 mV. Validate gain/offset, channel interaction, resistor self-heating,
ADC reference drift, leakage, mux settling, and subtraction error at 1S–8S.

## Candidates

- ADS7066: active, 8-channel, 16-bit, 250 kSPS SAR with internal 2.5 V reference,
  averaging and SPI CRC; 4 mW typical. It needs external attenuation/protection
  and its switched input settling must be proven with the divider/buffer network.
- ADS8688A: active 8-channel 16-bit SAR, constant 1 MΩ AFE, independent ranges,
  internal 4.096 V reference and ±20 V input protection. It is robust but 65 mW,
  larger, costlier, and still needs attenuation for a 33.6 V 8S tap.
- LTC6811: excellent direct cell accuracy (<1.2 mV total error) but designed as a
  powered 12-cell stack monitor. Safe 1S operation and arbitrary checker hot-plug
  are poor fits without additional architecture; rejected for Prototype 2.

No divider values, clamp parts, or connector pinout are frozen. Those require SPICE
fault analysis, ADC EVM testing, and component datasheets before schematic capture.

## Validation matrix

Test 1S through 8S; absent connector; every missing intermediate tap; adjacent
short; reverse connector attempt; connect/disconnect sequences; 0–5 V per cell;
pack common-mode maximum; injected 50/60 Hz and switching noise; ESD to enclosure;
temperature; and calibrated precision sources. A fault must be reported without
turning another valid topology into a plausible but wrong pack.

