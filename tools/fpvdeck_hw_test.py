#!/usr/bin/env python3
"""Run FpvDeck controller self-test over its COBS/CRC serial protocol."""

from __future__ import annotations

import argparse
import os
import select
import struct
import termios
import time

VERSION = 1
MSG_REQUEST = 0x40
MSG_REPLY = 0x41
CMD_IDENTIFY = 0x01
CMD_SELF_TEST = 0x02
CMD_ADC_MV = 0x11
CMD_TAP_DUMP = 0x12

CHECKS = (
    (1 << 0, "MCU"),
    (1 << 1, "ADC"),
    (1 << 2, "BALANCE INPUT"),
    (1 << 3, "SD DETECT"),
    (1 << 4, "TEMP SENSOR"),
    (1 << 5, "CVBS BREAKOUT"),
    (1 << 6, "BUTTONS"),
    (1 << 7, "POWER RAILS"),
)


def crc16(data: bytes) -> int:
    crc = 0xFFFF
    for value in data:
        crc ^= value << 8
        for _ in range(8):
            crc = ((crc << 1) ^ 0x1021) & 0xFFFF if crc & 0x8000 else (crc << 1) & 0xFFFF
    return crc


def cobs_encode(data: bytes) -> bytes:
    output = bytearray(b"\x00")
    code_index = 0
    code = 1
    for value in data:
        if value == 0:
            output[code_index] = code
            code_index = len(output)
            output.append(0)
            code = 1
        else:
            output.append(value)
            code += 1
            if code == 0xFF:
                output[code_index] = code
                code_index = len(output)
                output.append(0)
                code = 1
    output[code_index] = code
    return bytes(output) + b"\x00"


def cobs_decode(data: bytes) -> bytes:
    output = bytearray()
    index = 0
    while index < len(data):
        code = data[index]
        if code == 0 or index + code > len(data) + 1:
            raise ValueError("invalid COBS frame")
        index += 1
        output.extend(data[index:index + code - 1])
        index += code - 1
        if code != 0xFF and index < len(data):
            output.append(0)
    return bytes(output)


def encode_request(command: int, argument: int = 0, sequence: int = 1) -> bytes:
    payload = bytes((command, argument))
    raw = struct.pack("<BBHH", VERSION, MSG_REQUEST, sequence, len(payload)) + payload
    return cobs_encode(raw + struct.pack("<H", crc16(raw)))


def decode_reply(encoded: bytes) -> tuple[int, int, bytes]:
    raw = cobs_decode(encoded.removesuffix(b"\x00"))
    if len(raw) < 8 or crc16(raw[:-2]) != struct.unpack_from("<H", raw, len(raw) - 2)[0]:
        raise ValueError("bad response CRC/length")
    version, message, sequence, length = struct.unpack_from("<BBHH", raw)
    if version != VERSION or message != MSG_REPLY or len(raw) != length + 8:
        raise ValueError("unexpected response header")
    return sequence, raw[6], raw[7:6 + length]


class SerialTransport:
    def __init__(self, path: str, timeout: float):
        self.fd = os.open(path, os.O_RDWR | os.O_NOCTTY | os.O_NONBLOCK)
        attributes = termios.tcgetattr(self.fd)
        attributes[0] = 0
        attributes[1] = 0
        attributes[2] = termios.CS8 | termios.CREAD | termios.CLOCAL
        attributes[3] = 0
        attributes[4] = termios.B115200
        attributes[5] = termios.B115200
        termios.tcsetattr(self.fd, termios.TCSANOW, attributes)
        self.timeout = timeout

    def request(self, command: int, argument: int = 0, sequence: int = 1) -> bytes:
        os.write(self.fd, encode_request(command, argument, sequence))
        deadline = time.monotonic() + self.timeout
        received = bytearray()
        while time.monotonic() < deadline:
            ready, _, _ = select.select([self.fd], [], [], max(0.0, deadline - time.monotonic()))
            if not ready:
                break
            received.extend(os.read(self.fd, 256))
            if b"\x00" in received:
                frame = bytes(received[:received.index(0) + 1])
                reply_sequence, reply_command, payload = decode_reply(frame)
                if reply_sequence != sequence or reply_command != command:
                    raise RuntimeError("response does not match request")
                return payload
        raise TimeoutError(f"controller did not reply within {self.timeout:.1f}s")

    def close(self) -> None:
        os.close(self.fd)


def print_result(label: str, state: str, detail: str = "") -> None:
    print(f"{label:<18} {state:<12}{detail}")


def run_simulated(faults: set[str]) -> int:
    failed = 0
    print("FpvDeck Test PCB Rev A — SIMULATED SELF TEST")
    for _, label in CHECKS:
        key = label.lower().replace(" ", "-")
        if key in faults:
            print_result(label, "FAIL", "injected fault")
            failed += 1
        elif label in ("BALANCE INPUT", "BUTTONS", "CVBS BREAKOUT"):
            print_result(label, "INTERACTIVE", "follow bring-up prompts on real hardware")
        else:
            print_result(label, "PASS")
    for channel in range(1, 7):
        key = f"balance-ch{channel}"
        if key in faults:
            print_result(f"BALANCE CH{channel}", "FAIL", "injected fault")
            failed += 1
        else:
            print_result(f"BALANCE CH{channel}", "PASS", f"{channel * 4.200:6.3f} V cumulative")
    return 1 if failed else 0


def run_transport(transport: object) -> int:
    identify = transport.request(CMD_IDENTIFY, sequence=1)
    if len(identify) < 9 or identify[0] != 0:
        raise RuntimeError("identify failed")
    print(f"FpvDeck controller HW {identify[1]}.{identify[2]} FW {identify[3]}.{identify[4]}.{identify[5]}")
    payload = transport.request(CMD_SELF_TEST, sequence=2)
    if len(payload) < 13:
        raise RuntimeError("short self-test response")
    status = payload[0]
    passed, interactive, failed = struct.unpack_from("<III", payload, 1)
    for bit, label in CHECKS:
        if failed & bit:
            print_result(label, "FAIL")
        elif interactive & bit:
            print_result(label, "INTERACTIVE")
        elif passed & bit:
            print_result(label, "PASS")
        else:
            print_result(label, "NOT TESTED")

    for channel, label, sequence in ((0, "B- OFFSET", 10), (1, "DECK MON", 11)):
        adc = transport.request(CMD_ADC_MV, channel, sequence=sequence)
        if len(adc) < 6 or adc[0] != 0 or adc[1] != channel:
            raise RuntimeError(f"{label.lower()} ADC request failed")
        millivolts = struct.unpack_from("<i", adc, 2)[0]
        print_result(label, "PASS", f"{millivolts / 1000:7.3f} V")

    taps = transport.request(CMD_TAP_DUMP, sequence=12)
    if len(taps) < 26 or taps[0] != 0 or taps[1] != 6:
        raise RuntimeError("tap dump failed or returned wrong channel count")
    for tap in range(6):
        millivolts = struct.unpack_from("<i", taps, 2 + tap * 4)[0]
        print_result(f"BALANCE CH{tap + 1}", "PASS", f"{millivolts / 1000:6.3f} V cumulative")
    return 1 if status != 0 or failed else 0


def run_serial(path: str, timeout: float) -> int:
    transport = SerialTransport(path, timeout)
    try:
        return run_transport(transport)
    finally:
        transport.close()


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    source = parser.add_mutually_exclusive_group(required=True)
    source.add_argument("--simulate", action="store_true")
    source.add_argument("--port", help="USB CDC or UART device, e.g. /dev/ttyACM0")
    parser.add_argument("--timeout", type=float, default=2.0)
    parser.add_argument("--fault", action="append", default=[], help="simulated check key to fail")
    args = parser.parse_args()
    return run_simulated(set(args.fault)) if args.simulate else run_serial(args.port, args.timeout)


if __name__ == "__main__":
    raise SystemExit(main())
