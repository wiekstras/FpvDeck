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
| `0x7e` | Error | both |

Type payloads are not yet frozen and must not be invented in firmware. The C++
codec and corruption tests in `software/core` are the current executable framing
specification. Golden byte vectors will be shared with MCU C tests before the
first hardware protocol release.

