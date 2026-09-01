# Test PCB Rev A bring-up

This procedure applies only after the PCB is marked released in its README and a
second engineer has checked the fab package. Use a non-conductive bench surface,
eye protection and a current-limited supply. Do not connect a LiPo during initial
bring-up.

The separate [balance ground-fault analysis](BALANCE_GROUND_FAULT_ANALYSIS.md)
controls any F2 population or reference-path experiment.

## Equipment

- current-limited 0–6 V bench supply and calibrated DMM;
- oscilloscope and 10× probe;
- ST-LINK/SWD cable and 3.3 V USB-UART;
- precision 0–25.5 V source or protected cumulative-tap simulator limited to 5 mA;
- thermocouple/thermal camera and ESD-safe tools.

## 1. Visual and records inspection

Confirm board revision, fab traveler, polarity marks, pin-1 marks, solder bridges,
connector keys, unpopulated DNP options and component labels. Verify J1 orientation
against the purchased JST drawing and harness using continuity mode only.

Confirm **F2 is not populated**. TP33 (`BNEG_RAW`) and TP34 (local ground) must
remain open-circuit except for ADC input leakage before an approved simulator
fixture intentionally provides a characterized link. Never bridge F2 with solder
or a zero-ohm resistor.

**Pass:** all exact MPNs and orientations match the released BOM/photos. **Fail:**
quarantine the board; do not “try power” to resolve ambiguity.

## 2. Unpowered resistance checks

Leave U1/U2 unpopulated for the first power-only assembly if hand building. Measure
5V_RAW-to-GND, 5V_FUSED-to-GND and 3V3-to-GND in both meter polarities after
capacitors settle. Record values rather than relying on a universal threshold.

Check each balance raw tap to ground. Expected nominal DC resistance is about
41 kΩ once its divider is assembled; adjacent channels should agree within the
network/measurement tolerance. Verify no continuity between adjacent J1 pins.

**Pass:** no hard short (<10 Ω) on a supply and divider channels are consistent.

## 3. Current-limited power-up

Set 5.0 V and 25 mA current limit with MCU/ADC absent. Connect J2 with polarity
verified at the board. Increase limit only after rail measurements are sensible.

Expected:

| Node | Pass band before calibration |
| --- | ---: |
| TP30 / 5V | 4.75–5.25 V |
| TP31 / 3V3 | 3.20–3.40 V |
| TP32 / GND | <5 mV to supply return |

No component should rise more than 10 °C above ambient unloaded. Fit MCU next,
then ADC, repeating the current-limited sequence each time and recording current.

## 4. Flash and communication

Connect SWD with target power off, reapply power at 100 mA limit, read the STM32
device ID, erase, program and verify. Do not disable SWD or readout until production.
Start watchdog/USB/UART firmware and run:

```bash
python3 tools/fpvdeck_hw_test.py --port /dev/ttyACM0
```

**Pass:** identify response version is correct, heartbeats continue for ten minutes,
CRC/error counters remain zero on a clean cable, and disconnect/reconnect recovers.

## 5. ADC identity and reference

Read ADS8688A default/reset behavior over SPI at conservative clock speed. Verify
REFIO and REFCAP against the datasheet's 4.096 V limits after the specified startup
time. Scope SCLK/CS/SDI/SDO and confirm levels stay within DVDD rails.

**Pass:** repeatable register readback, no ADC alarm, reference stable and no supply
overshoot. A plausible conversion alone is not proof of correct SPI framing.

Verify the logical channel identity before calibration: ADC0 = bipolar B− sense,
ADC1 = deck monitor, and ADC2–ADC7 = cumulative taps 1–6. A channel-order mismatch
is a hard fail; do not compensate by relabeling fixture wires.

## 6. Zero and cumulative voltage injection

With no LiPo present, short protected simulator inputs to balance reference and
record 1,000 samples per channel. Then inject 4.200, 8.400, 12.600, 16.800, 21.000
and 25.200 V cumulatively at a 5 mA current limit. Verify both raw test point and
ADC-node voltage; at 25.5 V the ADC node should be approximately 6.19 V.

Perform two-point calibration, repeat the sweep and calculate derived cells.

**Pass:** no ADC input exceeds 10.24 V; derived-cell error is ≤10 mV at room
temperature with p95 noise ≤5 mV. Store raw logs and calibration CRC.

## 7. Fault and connection tests

Using only the protected simulator, test every missing intermediate tap, adjacent
short, partial insertion order, disconnect, overvoltage up to the documented
fixture limit and ADC/MCU power removal. Verify stale data is never presented as a
healthy pack. Test USB-connected and standalone ground states.

The F2 experiment is a separate, reviewed test phase. Populate only after the
ground-fault matrix authorizes it, limit available energy, measure TP33−TP34,
and configure physical ADC channel 0 bipolar before interpreting B− sense.
Compare measured drop with total divider current; unexpected alternate return
current is an immediate fail and power-off condition.

Do not attempt reverse LiPo connection; test reverse fixture behavior with a
strict current limit and the approved fault-analysis procedure.

## 8. Controls, SD detect and outputs

Exercise every button/header input and verify one and only one event per press.
Insert/remove the SD-detect fixture, toggle LEDs/buzzer/fan, stall or disconnect
the fan, and confirm failure-safe off states after heartbeat loss.

## 9. VRX/decoder module interfaces

Before connecting modules, compare each cable pin-to-pin with the FpvDeck interface
table and the module's own documentation. Verify voltage and ground first. Inject
terminated CVBS and confirm it is not routed through an MCU pin. The generic J4/J5
headers are not undocumented commercial-module pinouts.

## 10. Thermal and endurance

Run ADC scanning, USB traffic, fan switching and simulator input for two hours.
Record rail voltage/current, MCU/ADC/LDO case temperature, ADC reference and error
once per minute. Repeat at intended ambient extremes only in controlled equipment.

**Pass:** no reset, CRC storm, thermal alarm, drift outside accuracy target or
component beyond its derated limit.

## Bring-up record

Record serial number, PCB revision, BOM lot, firmware Git SHA, instrument IDs,
calibration certificate dates, photos, all measurements, failures/rework and final
disposition. A board is not cleared for a real LiPo until the fault matrix and
accuracy sweep are signed off.
