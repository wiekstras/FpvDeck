# Balance-input ground fault analysis

## Status and boundary

**OPEN SAFETY GATE — NO REAL LIPO.** This document defines tests needed to decide
whether Test PCB Rev A's pack-negative reference concept is acceptable. It is not
an approval, certification, or substitute for a reviewed native schematic.

External balance taps are measurement inputs only. The external pack must never
power FpvDeck. F2 (`1206L010/60WR`) is a DNP experiment between `BNEG_RAW` and
local ground; TP33 and TP34 expose both sides. A wire or zero-ohm bridge is not an
allowed substitute.

R7 (1.00 kΩ) connects `BNEG_RAW` to the ADC's `BNEG_SENSE` input. It makes the
sense path explicit and limits input current, but it is not enough to approve a
ground fault. ADS8688A input bias through R7 creates a calibration term, so test
limits apply to both TP33/TP34 and the ADC code.

## Intended normal state

With no other external ground, the six divider paths draw a modeled 2.152796 mA
from a 6S pack at 4.2 V/cell. The F2 candidate creates a modeled 3.229 mV offset
at its 1.5 Ω typical initial resistance and 21.528 mV at the published 10 Ω
post-trip/reflow maximum. Physical ADC channel 0 measures B− using a
bipolar range. Production firmware **must** correct cumulative taps by subtracting
measured B−, but that correction is not implemented or hardware-validated yet;
raw and corrected values must both pass sanity checks before release.

## Fault matrix

| State | Hazard to test | Required response | Release evidence |
| --- | --- | --- | --- |
| Board unpowered, pack simulator present | current through unpowered ADC clamps | remain inside every absolute maximum; no phantom powering | measured pin/rail currents and thermal record |
| USB/SBC attached | alternate B− return through host ground | no uncontrolled current or bypass of F2 | isolated current measurement in every cable state |
| Earth-referenced scope attached | probe ground shorts intended protection | fixture must prevent unsafe attachment or test must be isolated | signed bench topology and differential-probe procedure |
| Grounded lab supply attached | supply negative creates a second return | no loop current above calculated normal current | current at F2 and every external cable |
| F2 open | measurements may look plausible with floating reference | pack immediately invalid/stale | UI and raw-code test log |
| F2 high resistance | growing common-mode error | warning before B− input/range limit | swept resistance versus correction/error |
| F2 tripped | hot component and stale data | fail safe; no automatic healthy state while hot | trip/cool-down waveform and temperature |
| Partial J1 insertion | B− or taps connect in arbitrary order | no damaging current; never report healthy pack | every insertion-order capture |
| Intermediate tap open | adjacent cells can alias | explicit open/invalid result | all six missing-tap cases |
| Adjacent taps shorted | cell reads zero while pack remains energized | invalid result; components within ratings | current and temperature record |
| Tap-to-local-ground short | pack energy may bypass divider | fixture-limited test only; protection contains fault | reviewed worst-case calculation before test |
| Reversed fixture | negative ADC input/connector stress | no real-battery test; simulator-limited only | limits and measured clamp current |
| MCU/firmware stalled | stale healthy voltage | watchdog expires and Linux marks service offline | HIL watchdog log |

## Required pre-test calculations

Before F2 is populated, record for each matrix row:

1. complete current path, including cable shields and instrument earth;
2. source voltage, source current limit and available energy;
3. maximum pin voltage/current for J1, R1–R6, RN1–RN6, U1 and F2;
4. expected TP33−TP34 polarity and magnitude;
5. abort current, temperature and time;
6. whether the test can leave F2 at its 10 Ω post-trip/reflow maximum;
7. instrument isolation category and probe placement.

Unknown values block that test; they are not filled with assumptions.

## Initial simulator-only procedure

Use six isolated series sources or an approved resistor ladder, limited to 5 mA
and physically unable to connect to a battery. Start with F2 DNP and verify no
alternate TP33–TP34 path. Populate F2 only for a reviewed test, configure physical
ADC channel 0 bipolar, then ramp cumulative taps while monitoring:

- TP33−TP34 with a differential measurement;
- total simulator current and current through F2;
- ADC channel 0 raw code and corrected tap values;
- F2, divider, ADC and connector temperature;
- all attached-cable states one at a time.

Stop immediately for unexpected current, positive B− sense polarity, unstable
correction, a component more than 10 °C above ambient during the low-energy test,
or any credible measurement after F2 is opened.

## Gate closure criteria

The protected reference gate closes only when the native schematic has passed
ERC and independent review, all exact footprints have been checked, worst-case
calculations cover the full 6S voltage and every grounded interface, simulator
tests pass, and a reviewer explicitly approves a real-pack procedure. Closing
this gate does not release the rest of the PCB; routing, DRC and the remaining
release checklist still apply.

## Primary sources

- [TI ADS8688A datasheet](https://www.ti.com/lit/ds/symlink/ads8688a.pdf)
- [Littelfuse 1206L PPTC datasheet](https://www.littelfuse.com/assetdocs/littelfuse-ptc-1206l-datasheet?assetguid=2b6a1515-d4ee-4c83-8bd4-152b4901b8f5)
