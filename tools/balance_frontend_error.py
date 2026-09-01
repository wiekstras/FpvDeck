#!/usr/bin/env python3
"""Estimate calibrated balance-front-end error for Test PCB Rev A.

The model includes divider tolerance/TCR, ADS8688A input impedance and its
voltage-dependent bias current, ADC quantization, gain drift, offset drift, and
the subtraction of adjacent cumulative taps. It is an engineering estimator,
not a substitute for validation against calibrated equipment.
"""

from __future__ import annotations

import argparse
import json
import math
import random
from dataclasses import asdict, dataclass

ADC_COUNTS = 65535


@dataclass(frozen=True)
class Frontend:
    # 1 kΩ series protection plus three 10 kΩ elements over one matched 10 kΩ
    # element in ACASA1002U1002P1AT. The 31 kΩ path is above TI's 30 kΩ
    # powered-off OVP condition.
    r_top_ohm: float = 31_000.0
    r_bottom_ohm: float = 10_000.0
    resistor_tolerance: float = 0.001
    resistor_tcr_ppm: float = 5.0
    adc_range_v: float = 10.24
    adc_input_ohm_nominal: float = 1_000_000.0
    adc_input_ohm_min: float = 850_000.0
    adc_input_ohm_max: float = 1_150_000.0
    adc_input_tcr_ppm: float = 25.0
    adc_bias_v: float = 2.50
    adc_gain_drift_ppm: float = 4.0
    adc_offset_drift_lsb: float = 1.0
    calibration_high_v: float = 25.50
    calibration_temp_c: float = 25.0


def divider_output(
    input_v: float,
    r_top_ohm: float,
    r_bottom_ohm: float,
    adc_input_ohm: float,
    adc_bias_v: float = 2.50,
) -> float:
    """Solve the divider including ADS8688A's resistive path to its bias node."""
    numerator = input_v / r_top_ohm + adc_bias_v / adc_input_ohm
    denominator = 1.0 / r_top_ohm + 1.0 / r_bottom_ohm + 1.0 / adc_input_ohm
    return numerator / denominator


def pack_return_current_a(tap_voltages_v: list[float], model: Frontend) -> float:
    """Estimate normal current returning through the common pack-negative link.

    This covers the nominal divider/ADC input network only. It intentionally does
    not model an externally grounded fault, ESD, partial insertion, or a tripped
    PPTC; those cases require circuit fault analysis and bench validation.
    """
    current = 0.0
    for tap_v in tap_voltages_v:
        pin_v = divider_output(
            tap_v,
            model.r_top_ohm,
            model.r_bottom_ohm,
            model.adc_input_ohm_nominal,
            model.adc_bias_v,
        )
        current += (tap_v - pin_v) / model.r_top_ohm
    return current


def reference_drop_mv(current_a: float, resistance_ohm: float) -> float:
    if resistance_ohm < 0:
        raise ValueError("reference resistance cannot be negative")
    return current_a * resistance_ohm * 1000.0


def quantized_code(voltage_v: float, range_v: float, gain: float = 1.0, offset_lsb: float = 0.0) -> int:
    ideal = voltage_v / range_v * ADC_COUNTS * gain + offset_lsb
    return max(0, min(ADC_COUNTS, round(ideal)))


def calibration_from_codes(code_zero: int, code_high: int, high_v: float) -> tuple[float, float]:
    if code_high == code_zero:
        raise ValueError("calibration points collapsed to one ADC code")
    slope = high_v / (code_high - code_zero)
    return slope, -code_zero * slope


@dataclass
class Channel:
    r_top: float
    r_bottom: float
    r_top_tcr: float
    r_bottom_tcr: float
    adc_input: float
    adc_input_tcr: float
    gain_tcr: float
    offset_direction: float
    slope: float = 0.0
    intercept: float = 0.0


def _random_channel(model: Frontend, rng: random.Random) -> Channel:
    common_tcr = rng.uniform(-10.0, 10.0)
    relative_tcr = rng.uniform(-model.resistor_tcr_ppm, model.resistor_tcr_ppm)
    return Channel(
        r_top=model.r_top_ohm * (1 + rng.uniform(-model.resistor_tolerance, model.resistor_tolerance)),
        r_bottom=model.r_bottom_ohm * (1 + rng.uniform(-model.resistor_tolerance, model.resistor_tolerance)),
        r_top_tcr=common_tcr + relative_tcr / 2,
        r_bottom_tcr=common_tcr - relative_tcr / 2,
        adc_input=rng.uniform(model.adc_input_ohm_min, model.adc_input_ohm_max),
        adc_input_tcr=rng.uniform(-model.adc_input_tcr_ppm, model.adc_input_tcr_ppm),
        gain_tcr=rng.uniform(-model.adc_gain_drift_ppm, model.adc_gain_drift_ppm),
        offset_direction=rng.uniform(-1.0, 1.0),
    )


def _at_temperature(channel: Channel, temperature_c: float, model: Frontend) -> tuple[float, float, float, float, float]:
    delta_c = temperature_c - model.calibration_temp_c
    r_top = channel.r_top * (1 + channel.r_top_tcr * 1e-6 * delta_c)
    r_bottom = channel.r_bottom * (1 + channel.r_bottom_tcr * 1e-6 * delta_c)
    adc_input = channel.adc_input * (1 + channel.adc_input_tcr * 1e-6 * delta_c)
    gain = 1 + channel.gain_tcr * 1e-6 * delta_c
    offset = channel.offset_direction * model.adc_offset_drift_lsb * abs(delta_c) / 50.0
    return r_top, r_bottom, adc_input, gain, offset


def _read_code(channel: Channel, input_v: float, temperature_c: float, model: Frontend) -> int:
    r_top, r_bottom, adc_input, gain, offset = _at_temperature(channel, temperature_c, model)
    pin_v = divider_output(input_v, r_top, r_bottom, adc_input, model.adc_bias_v)
    return quantized_code(pin_v, model.adc_range_v, gain, offset)


def _calibrate(channel: Channel, model: Frontend) -> None:
    code_zero = _read_code(channel, 0.0, model.calibration_temp_c, model)
    code_high = _read_code(channel, model.calibration_high_v, model.calibration_temp_c, model)
    channel.slope, channel.intercept = calibration_from_codes(code_zero, code_high, model.calibration_high_v)


def _read_calibrated(channel: Channel, input_v: float, temperature_c: float, model: Frontend) -> float:
    code = _read_code(channel, input_v, temperature_c, model)
    return channel.slope * code + channel.intercept


def _percentile(values: list[float], percentile: float) -> float:
    ordered = sorted(values)
    index = min(len(ordered) - 1, max(0, math.ceil(percentile * len(ordered)) - 1))
    return ordered[index]


def simulate(model: Frontend, iterations: int = 20_000, seed: int = 0xF9D, temperature_min_c: float = -20.0, temperature_max_c: float = 70.0) -> dict[str, float | int]:
    if iterations < 1:
        raise ValueError("iterations must be positive")
    rng = random.Random(seed)
    tap_errors_mv: list[float] = []
    cell_errors_mv: list[float] = []
    for _ in range(iterations):
        lower = _random_channel(model, rng)
        upper = _random_channel(model, rng)
        _calibrate(lower, model)
        _calibrate(upper, model)
        cell_v = rng.uniform(3.0, 4.35)
        lower_input = rng.uniform(0.0, model.calibration_high_v - cell_v)
        upper_input = lower_input + cell_v
        temperature = rng.uniform(temperature_min_c, temperature_max_c)
        lower_measured = _read_calibrated(lower, lower_input, temperature, model)
        upper_measured = _read_calibrated(upper, upper_input, temperature, model)
        tap_errors_mv.extend((abs(lower_measured - lower_input) * 1000, abs(upper_measured - upper_input) * 1000))
        cell_errors_mv.append(abs((upper_measured - lower_measured) - cell_v) * 1000)

    pin_at_max = divider_output(
        model.calibration_high_v,
        model.r_top_ohm,
        model.r_bottom_ohm,
        model.adc_input_ohm_nominal,
        model.adc_bias_v,
    )
    return_current = pack_return_current_a([4.2 * cell for cell in range(1, 7)], model)
    return {
        "iterations": iterations,
        "adc_pin_at_25_5_v": round(pin_at_max, 6),
        "adc_headroom_v": round(model.adc_range_v - pin_at_max, 6),
        "divider_current_at_25_5_v_ma": round(model.calibration_high_v / (model.r_top_ohm + model.r_bottom_ohm) * 1000, 6),
        "six_tap_return_current_ma": round(return_current * 1000, 6),
        "reference_drop_at_1_5_ohm_mv": round(reference_drop_mv(return_current, 1.5), 6),
        "reference_drop_at_10_ohm_mv": round(reference_drop_mv(return_current, 10.0), 6),
        "tap_error_p95_mv": round(_percentile(tap_errors_mv, 0.95), 3),
        "tap_error_p99_mv": round(_percentile(tap_errors_mv, 0.99), 3),
        "tap_error_max_mv": round(max(tap_errors_mv), 3),
        "cell_error_p95_mv": round(_percentile(cell_errors_mv, 0.95), 3),
        "cell_error_p99_mv": round(_percentile(cell_errors_mv, 0.99), 3),
        "cell_error_max_mv": round(max(cell_errors_mv), 3),
    }


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--iterations", type=int, default=20_000)
    parser.add_argument("--seed", type=int, default=0xF9D)
    parser.add_argument("--json", action="store_true")
    args = parser.parse_args()
    model = Frontend()
    result = simulate(model, args.iterations, args.seed)
    if args.json:
        print(json.dumps({"model": asdict(model), "result": result}, indent=2))
    else:
        print("FpvDeck Test PCB Rev A balance frontend estimate")
        for key, value in result.items():
            print(f"{key:32} {value}")
        print("Model assumptions are documented in docs/BATTERY_MEASUREMENT.md.")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
