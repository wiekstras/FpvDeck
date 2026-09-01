import contextlib
import io
import struct
import unittest

from tools.fpvdeck_hw_test import (
    CMD_ADC_MV,
    CMD_IDENTIFY,
    CMD_SELF_TEST,
    CMD_TAP_DUMP,
    cobs_decode,
    cobs_encode,
    crc16,
    decode_reply,
    run_simulated,
    run_transport,
)


class FakeTransport:
    def __init__(self):
        self.requests = []

    def request(self, command, argument=0, sequence=1):
        self.requests.append((command, argument, sequence))
        if command == CMD_IDENTIFY:
            return bytes((0, 1, 0, 0, 1, 0, 0, 0, 0, 0))
        if command == CMD_SELF_TEST:
            return bytes((0,)) + struct.pack("<III", 0xFF, 0, 0)
        if command == CMD_ADC_MV:
            value = -3 if argument == 0 else 7400
            return bytes((0, argument)) + struct.pack("<i", value)
        if command == CMD_TAP_DUMP:
            return bytes((0, 6)) + b"".join(struct.pack("<i", 4200 * cell) for cell in range(1, 7))
        raise AssertionError(f"unexpected command {command}")


class HardwareTestProtocolTest(unittest.TestCase):
    def test_crc_vector(self):
        self.assertEqual(crc16(b"123456789"), 0x29B1)

    def test_cobs_round_trip(self):
        payload = b"\x00abc\x00\xff"
        encoded = cobs_encode(payload)
        self.assertEqual(cobs_decode(encoded[:-1]), payload)

    def test_bad_reply_is_rejected(self):
        with self.assertRaises(ValueError):
            decode_reply(b"\x01\x00")

    def test_self_test_uses_bneg_deck_and_tap_dump_channels(self):
        transport = FakeTransport()
        with contextlib.redirect_stdout(io.StringIO()):
            self.assertEqual(run_transport(transport), 0)
        self.assertIn((CMD_ADC_MV, 0, 10), transport.requests)
        self.assertIn((CMD_ADC_MV, 1, 11), transport.requests)
        self.assertIn((CMD_TAP_DUMP, 0, 12), transport.requests)
        self.assertNotIn((CMD_ADC_MV, 2, 12), transport.requests)

    def test_passive_cvbs_breakout_is_not_reported_as_automatic_pass(self):
        output = io.StringIO()
        with contextlib.redirect_stdout(output):
            self.assertEqual(run_simulated(set()), 0)
        self.assertIn("CVBS BREAKOUT      INTERACTIVE", output.getvalue())
        self.assertNotIn("VRX LINK", output.getvalue())


if __name__ == "__main__":
    unittest.main()
