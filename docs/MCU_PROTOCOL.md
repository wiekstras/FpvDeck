# MCU protocol v1

Transport is USB CDC for Prototype 1 with UART debug/fallback. Messages are binary,
COBS framed, and terminated by `0x00`. Every decoded frame is:

| Offset | Field | Encoding |
| ---: | --- | --- |
| 0 | protocol version | `uint8`, currently 1 |
| 1 | message type | `uint8` |
| 2 | sequence | little-endian `uint16` |
| 4 | payload length | little-endian `uint16`, maximum 1024 |
| 6 | payload | type-specific bytes |
| 6+N | CRC | CRC-16/CCITT-FALSE over header+payload, init `0xffff`, poly `0x1021` |

Unknown message types are ignored and counted. Unsupported major protocol versions
receive a version error; no decoder guesses a layout. Requests time out, sequences
allow correlation, and idempotent controls may be retried. Safety outputs revert to
defined states on heartbeat loss.

Initial reserved types:

| Type | Name | Direction |
| ---: | --- | --- |
| `0x01` | Hello / device ID and capabilities | MCU → Linux |
| `0x02` | Heartbeat | both |
| `0x10` | Input event | MCU → Linux |
| `0x11` | Battery snapshot | MCU → Linux |
| `0x12` | Thermal/power snapshot | MCU → Linux |
| `0x20` | Output command | Linux → MCU |
| `0x30` | Shutdown request | MCU → Linux |
| `0x31` | Shutdown ready | Linux → MCU |
| `0x40` | Board bring-up request | Linux → MCU |
| `0x41` | Board bring-up reply | MCU → Linux |
| `0x7e` | Error | both |

Core runtime payloads `0x01`–`0x31` are not yet frozen. The board bring-up subset
is now executable and versioned in `firmware/controller/include/fpvdeck/board.h`.

## Board bring-up payload

Request payload:

| Offset | Field |
| ---: | --- |
| 0 | command ID |
| 1 | optional channel/value byte |

Reply payload:

| Offset | Field |
| ---: | --- |
| 0 | echoed command ID |
| 1 | status: 0 OK, 1 bad command, 2 bad argument, 3 not ready, 4 hardware fault |
| 2 | command-specific fixed-width little-endian data |

Implemented commands include identify, self-test bitmasks, raw ADC, calibrated ADC
millivolts, six-tap dump, two temperatures, LEDs, buzzer, fan, buttons, SD detect,
and the 5 V/3.3 V/deck rails. Requests and replies retain the outer CRC and sequence
number. Tests cover the dispatcher and framing corruption.

The physical ADS8688A map is fixed in `fpvdeck_adc_channel_t`: channel 0 is
signed/bipolar pack-negative sense, channel 1 is deck monitoring, and channels
2–7 are cumulative balance taps 1–6. `TAP_DUMP` returns only those six taps in
order, insulating the host self-test from confusing B− or deck voltage with a
cell input.

`tools/fpvdeck_hw_test.py --port /dev/ttyACM0` is the Linux-side bring-up client;
`--simulate` exercises its output before hardware exists.
