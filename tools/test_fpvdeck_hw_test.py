import unittest

from tools.fpvdeck_hw_test import cobs_decode, cobs_encode, crc16, decode_reply


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


if __name__ == "__main__":
    unittest.main()
