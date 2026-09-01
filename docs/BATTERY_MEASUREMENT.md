# Balance-lead battery measurement

## Safety boundary

The external LiPo is a **measurement source only**. It never powers FpvDeck and
is electrically separate from the internal 2×18650 deck supply except for the
temporary measurement reference at the protected balance connector. No tap is
connected to an MCU ADC.

Test PCB Rev A targets keyed 1S–6S JST-XH balance leads. Eight-cell support is a
future requirement, not a Rev A claim.

## Selected Rev A architecture

| Element | Selection | Status |
| --- | --- | --- |
| Connector | JST `B7B-XH-A(LF)(SN)`, 7-way, 2.5 mm, top-entry through-hole | exact vendor drawing/footprint verification required before fabrication release |
| ADC | TI `ADS8688AIDBT`, TSSOP-38, 16-bit, eight-channel, SPI | **SELECTED**; active product, 0–10.24 V range used for taps |
| Divider network | Vishay `ACASA1002U1002P1AT`, four matched 10 kΩ elements, 0.1% absolute, 0.05% ratio, 5 ppm/°C ratio tracking | **SELECTED**; one array per cumulative tap |
| Series resistor | 1.00 kΩ, 0.1%, 25 ppm/°C, ahead of each matched network | exact BOM MPN to be footprint-checked |
| Filter | 10 nF C0G from ADC input to analog ground | selected value; settling/noise to validate |
| Ground protection | low-current resettable/fusible link plus separate B− sense concept | **OPEN DESIGN GATE**; do not release until offset and fault behavior are simulated |

For each cumulative tap, three matched 10 kΩ array elements are placed in series
above the ADC node and one 10 kΩ element below it. A separate 1 kΩ resistor makes
the source-side path nominally 31 kΩ. The ADS8688A is configured for 0–10.24 V.
At a conservative 25.5 V six-cell maximum, the ADC input model predicts 6.192 V,
leaving 4.048 V of input-range headroom. Nominal divider current for the highest
tap is 0.622 mA.

The 31 kΩ series-side path is intentional: TI specifies its powered-off input
overvoltage condition differently above 30 kΩ. This does **not** by itself prove
all hot-plug or power-off cases safe; those remain fault-analysis tests.

## Measurement model

Balance taps are cumulative relative to pack negative. For a 4S pack:

```text
T1 = C1
T2 = C1 + C2
T3 = C1 + C2 + C3
T4 = C1 + C2 + C3 + C4
```

Software derives `C1=T1`, `C2=T2−T1`, and so forth. The implemented pure
function rejects non-monotonic taps, non-finite values, more than eight taps, and
derived cells outside a broad 0.5–5.0 V electrical sanity range. Chemistry
warnings are a separate policy.

## Accuracy budget

Target after per-channel two-point calibration:

- **required:** ±10 mV per derived cell from 0–50 °C;
- **stretch:** p95 ≤5 mV from −20–70 °C;
- quantization is not the dominant error; divider ratio and temperature drift are.

Run the source-controlled estimator with:

```bash
python3 tools/balance_frontend_error.py
```

Current seeded 20,000-case estimate:

| Metric | Estimate |
| --- | ---: |
| Tap absolute error p95 | 2.757 mV |
| Tap absolute error p99 | 4.259 mV |
| Derived-cell error p95 | 3.836 mV |
| Derived-cell error p99 | 6.187 mV |

The Monte Carlo model includes 0.1% initial resistance, 5 ppm/°C divider ratio
tracking, ADS8688A 0.85–1.15 MΩ input impedance and 25 ppm/°C maximum input
impedance drift, 4 ppm/°C maximum gain drift, quantization, offset drift, and
adjacent-channel subtraction. It assumes a 25 °C two-point calibration at 0 and
25.5 V. It does not yet include PCB leakage, humidity, thermoelectric effects,
connector resistance, RC dielectric absorption, reference long-term drift, or
ground-protection drop. Therefore these are design estimates, not specifications.

## Calibration

1. Warm the board for five minutes at room temperature.
2. Short each input at the protected simulator header to the local balance
   reference; record zero codes.
3. Apply a calibrated cumulative reference near 25.2 V through the bench adapter;
   record high codes. Never generate this from a real LiPo during calibration.
4. Store slope/intercept and board serial in MCU flash with CRC and calibration
   timestamp. Keep factory defaults separately recoverable.
5. Validate at 4.200, 8.400, 12.600, 16.800, 21.000 and 25.200 V, then calculate
   adjacent-channel cell error.

## Safe testing without a LiPo

Preferred method: a six-channel isolated battery-cell simulator or six isolated
bench outputs in series, current-limited to 5 mA. A lower-cost resistor ladder may
be driven from a current-limited 0–26 V supply, but it tests cumulative voltage,
not independently variable cells.

The current placed Rev A source does **not** claim a separate simulator connector.
Use a dedicated keyed, current-limited harness into J1, with no battery present,
and probe the raw/ADC test points. A future `BAL_SIM` connector is permitted only
after a backfeed analysis proves that it cannot coexist electrically with a real
pack. No fixture output may connect to a real battery; use conspicuous labels and
no gender arrangement that can be joined to a battery lead by mistake.

## Hot-plug and wiring behavior

- A keyed JST-XH housing reduces, but does not eliminate, partial insertion.
- Firmware does not declare a pack until cumulative taps are monotonic and stable
  for at least 100 ms.
- A missing intermediate tap produces `INVALID TAP ORDER` or `OPEN TAP`; it must
  not be silently interpreted as a lower cell count.
- The last valid measurement is marked stale immediately on disconnect and is
  removed from the FPV overlay after a short UI transition.
- The ground-protection implementation is still a release gate. The PCB must have
  DNP/bypass options and test points so ground offset and trip behavior can be
  characterized without a redesign.

## Validation matrix

Test 1S–6S, no connector, every missing intermediate tap, adjacent short, reversed
test-fixture wiring, all connect/disconnect sequences, 0–4.35 V/cell, 25.5 V top
tap, ADC unpowered, MCU unpowered, USB attached, 50/60 Hz and converter noise,
ESD to enclosure, −20–70 °C, and calibrated source sweeps. Record raw codes as
well as filtered results. A fault must be reported without producing a plausible
but incorrect pack.

## Primary sources

- [TI ADS8688A product page](https://www.ti.com/product/ADS8688A)
- [TI ADS8688A datasheet](https://www.ti.com/lit/ds/symlink/ads8688a.pdf)
- [Vishay ACAS precision array datasheet](https://www.vishay.com/docs/28959/acas0612.pdf)
- [JST XH series](https://www.jst-mfg.com/product/index.php?series=277)
