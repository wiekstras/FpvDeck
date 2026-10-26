# Hardware design

## Stack

1. Waveshare ESP32-P4-WIFI6-Touch-LCD-5.
2. Rear 2×20 HAT, currently Rev A engineering draft.
3. Protected flat 1S LiPo, 3000–4000 mAh, after connector/polarity validation.
4. Future measured 3D-printed enclosure.

![HAT component map](assets/diagrams/hat-component-map.svg)

## HAT blocks

- RX A is required; RX B and its SMA path are optional/DNP.
- LMV331 compares RSSI A/B. Its open-drain output both selects the
  SN74LVC1G3157 and reports `ACTIVE_RX` to the ESP32-P4.
- TVP5150AM1 accepts selected CVBS and emits 8-bit BT.601 YCbCr with discrete
  synchronization. Eleven 33 Ω source resistors isolate DVP edges.
- ME6211C18 makes 1.8 V; ferrites split analog and digital decoder rails.
- ADS1115 at 0x48/0x49 reads balance taps; the nominal internal-cell channel on
  0x49 is reserved/DNP because raw BAT is not exposed on the header. The host's
  existing GPIO20 BAT_ADC reads deck voltage. ADS1115 0x4A reads receiver RSSI.
- Three buttons are active-low to internal pull-ups. A transistor drives the
  buzzer so the GPIO does not source its current.
- AV out is a review/DNP branch until the TPF131 input scaling is simulated and
  measured. It must not load the decoder input.

## Board status

[`hardware/gen_hat.py`](../hardware/gen_hat.py) owns the checked parts/net
model. It emits the schematic, audit netlist, BOM, placement-board container,
and component view. The current PCB deliberately has no fabricated RF geometry
or guessed RX5808 pads. See [release gates](../hardware/READINESS.md).

## Layout rules for the routed revision

- Four layers: signal/GND/power/signal; uninterrupted ground below RF and DVP.
- SMA-to-RX path grounded coplanar, 50 Ω, under 10 mm, with via fence.
- Keep both RX5808s and antennas away from DSI, DVP, ESP32-C6, and converters.
- Place TVP input termination at AIP1A and crystal network at pins 5/6.
- Match DVP data/sync paths within the budget derived from the actual stack-up.
- Put tap-side protection adjacent to J4; no tap trace enters digital areas first.
- Add readable probe labels and test points for every cumulative tap and rail.
