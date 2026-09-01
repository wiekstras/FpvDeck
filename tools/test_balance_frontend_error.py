import unittest

from tools.balance_frontend_error import Frontend, calibration_from_codes, divider_output, simulate


class BalanceFrontendErrorTest(unittest.TestCase):
    def test_divider_keeps_six_cell_pack_inside_adc_range(self):
        model = Frontend()
        pin = divider_output(
            25.5,
            model.r_top_ohm,
            model.r_bottom_ohm,
            model.adc_input_ohm_nominal,
            model.adc_bias_v,
        )
        self.assertGreater(pin, 6.0)
        self.assertLess(pin, model.adc_range_v - 1.0)

    def test_two_point_calibration(self):
        slope, intercept = calibration_from_codes(1000, 51000, 25.0)
        self.assertAlmostEqual(slope * 1000 + intercept, 0.0)
        self.assertAlmostEqual(slope * 51000 + intercept, 25.0)

    def test_seeded_error_budget_meets_prototype_target(self):
        result = simulate(Frontend(), iterations=3000, seed=1234)
        self.assertLess(result["cell_error_p95_mv"], 5.0)
        self.assertGreater(result["adc_headroom_v"], 1.0)


if __name__ == "__main__":
    unittest.main()
