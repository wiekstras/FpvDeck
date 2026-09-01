"""Regression checks for manufacturer-verified Test PCB pin assignments.

These tests parse the generator without importing pcbnew, so CI can catch a
source-level mapping regression. They complement, but do not replace, schematic
ERC and an independent datasheet/footprint review.
"""

import ast
import unittest
from pathlib import Path


SOURCE = (
    Path(__file__).resolve().parents[1]
    / "hardware"
    / "pcb"
    / "test-board-rev-a"
    / "generate_board.py"
)


def literal_assignment(name: str) -> dict[str, str]:
    module = ast.parse(SOURCE.read_text(encoding="utf-8"))
    for node in module.body:
        if isinstance(node, ast.Assign):
            if any(isinstance(target, ast.Name) and target.id == name for target in node.targets):
                value = ast.literal_eval(node.value)
                return value
    raise AssertionError(f"{name} not found in {SOURCE}")


class TestBoardPinMapTest(unittest.TestCase):
    def test_pack_negative_link_remains_dnp(self) -> None:
        source = SOURCE.read_text(encoding="utf-8")
        self.assertIn('add_fp("F2", "1206L010/60WR DNP"', source)
        self.assertIn('assign("F2", {"1": "BNEG_RAW", "2": "GND"})', source)
        self.assertIn('assign("R7", {"1": "BNEG_RAW", "2": "BNEG_SENSE"})', source)

    def test_vrx_and_decoder_headers_are_cvbs_only(self) -> None:
        source = SOURCE.read_text(encoding="utf-8")
        self.assertIn('assign("J4", {"2": "GND", "3": "VRX_VIDEO"})', source)
        self.assertIn('assign("J5", {"2": "GND", "3": "VRX_VIDEO"})', source)
        self.assertNotIn('"VRX_RSSI"', source)
        self.assertNotIn('"VRX_TX"', source)
        self.assertNotIn('"VRX_RX"', source)

    def test_ads8688a_dbt_map(self) -> None:
        mapping = literal_assignment("adc_mapping")
        expected_analog = {
            "5": "ADC_REFIO",
            "6": "GND",
            "7": "ADC_REFCAP",
            "8": "GND",
            "9": "+5V_FUSED",
            "12": "BNEG_SENSE",
            "13": "GND",
            "14": "DECK_MON",
            "15": "GND",
            "16": "TAP1_ADC",
            "17": "GND",
            "18": "TAP2_ADC",
            "19": "GND",
            "20": "GND",
            "21": "TAP3_ADC",
            "22": "GND",
            "23": "TAP4_ADC",
            "24": "GND",
            "25": "TAP5_ADC",
            "26": "GND",
            "27": "TAP6_ADC",
            "28": "GND",
            "29": "GND",
            "30": "+5V_FUSED",
            "31": "GND",
            "32": "GND",
            "33": "GND",
            "34": "+3V3",
        }
        for pad, net in expected_analog.items():
            self.assertEqual(mapping.get(pad), net, f"ADS8688A DBT pad {pad}")

    def test_stm32g0b1_lqfp48_critical_map(self) -> None:
        mapping = literal_assignment("mcu_mapping")
        expected = {
            "4": "+3V3", "5": "+3V3", "6": "+3V3", "7": "GND",
            "10": "NRST", "13": "UART_TX", "14": "UART_RX",
            "16": "SPI_SCLK", "17": "SPI_MISO", "18": "SPI_MOSI",
            "33": "USB_DM", "34": "USB_DP", "35": "SWDIO", "36": "SWCLK",
            "47": "I2C_SCL", "48": "I2C_SDA",
        }
        for pad, net in expected.items():
            self.assertEqual(mapping.get(pad), net, f"STM32G0B1 LQFP48 pad {pad}")


if __name__ == "__main__":
    unittest.main()
