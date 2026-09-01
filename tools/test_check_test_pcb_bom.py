import tempfile
import unittest
from pathlib import Path

from tools.check_test_pcb_bom import check, expand_reference_token


class TestPcbBomCheckTest(unittest.TestCase):
    def test_reference_range_expansion(self):
        self.assertEqual(expand_reference_token("TP30-TP34"), [f"TP{value}" for value in range(30, 35)])
        self.assertEqual(expand_reference_token("J2"), ["J2"])
        with self.assertRaises(ValueError):
            expand_reference_token("R6-R1")

    def test_detects_missing_and_quantity_mismatch(self):
        root = Path(tempfile.mkdtemp(prefix="fpvdeck-bom-test-"))
        board = root / "board.kicad_pcb"
        bom = root / "bom.csv"
        board.write_text('(fp_text reference "R1")\n(fp_text reference "R2")\n', encoding="utf-8")
        bom.write_text(
            "Reference,Qty\n"
            "R1,2\n",
            encoding="utf-8",
        )
        errors = check(board, bom)
        self.assertTrue(any("declares Qty" in error for error in errors))
        self.assertTrue(any("R2" in error for error in errors))

    def test_repository_bom_matches_generated_board(self):
        self.assertEqual(check(), [])


if __name__ == "__main__":
    unittest.main()
