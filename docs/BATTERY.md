# Battery checker

## Electrical model

JST-XH pins carry pack negative followed by cumulative taps 1–6. Each tap uses
100 kΩ high-side and 12 kΩ low-side resistors:

```text
tap = adc_voltage × (100k + 12k) / 12k × calibration[tap]
cell1 = tap1
cellN = tapN - tap(N-1)
```

At 25.5 V, the nominal ADC node is 2.732 V, inside the ADS1115 ±4.096 V range.
Theoretical ADC LSB referred to the tap is about 1.17 mV before component and
calibration errors. Subtracted cells include error from two cumulative channels.

## UI rules

- highest cumulative tap above 1.0 V determines connected cell count;
- below 3.50 V/cell is warning;
- above 4.25 V/cell is error;
- 3.80–3.85 V/cell is identified as storage range;
- calibration is stored per cumulative tap.

## Safety gates

The divider is not by itself a field-safe input. Fabrication remains blocked
until reverse insertion, partial insertion, ESD, tap-to-tap fault current,
resistor voltage rating, creepage, and connector shrouding are reviewed. Initial
testing uses current-limited precision sources, never a flight pack.

The external pack powers nothing. The internal 1S supply is a separate input on
the Waveshare host.

## Deck battery

The official Waveshare schematic already routes internal `BAT` through 200 kΩ
over 100 kΩ to ESP32-P4 GPIO20 (`BAT_ADC`). Firmware averages eight calibrated
oneshot samples and applies the 3:1 divider ratio. Below 3.50 V it shows a warning;
at or below 3.30 V it replaces the UI with a critical-power screen. A reading
below 2.50 V is treated as no battery (for example USB-only development), not an
empty cell.

Raw `BAT` is not on the 40-pin header. The reserved ADS1115 #2 deck-cell channel
and HAT divider are therefore DNP. No flying wire to the host battery circuit is
approved by this design.
