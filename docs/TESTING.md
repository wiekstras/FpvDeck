# Testing

Run the local CI-equivalent suite with:

```sh
./scripts/check
```

Current automated coverage includes cumulative balance-tap derivation for 1S–6S,
imbalance/noise and invalid tap cases; Monte Carlo balance-front-end calculations;
MCU protocol CRC/framing/corruption and board-command dispatch; and SQLite
migration plus flight persistence/reopen. Service tests cover touch auto-hide,
flight lock, battery extrema/faults, media seek/playback, storage faults,
deck/MCU/ADC/VRX health, receiver scan/favorites, telemetry loss/corruption,
PAL/NTSC validation, V4L2/file/simulator backend switching, capture loss/reconnect,
recorder-error isolation, and physical-input lockout.

Headless tests launch the complete QML graph in Home, Fly, Fly controls, flight
lock, Battery, Media, touch playback, Receiver, Flights, and Diagnostics states.
The QML suite also starts with an intentionally missing `/dev/video999` to prove
that capture absence does not crash the shell. Python tests cover numeric video-
node ordering and format discovery independently of physical devices.
An additional first-swapped-frame test exercises the schema-versioned benchmark
output and a Python test rejects incomplete/unknown metric records.
The screenshot script runs the same executable with deterministic data. A Python
source audit checks the manufacturer-verified critical ADS8688A and STM32 pad map
without requiring KiCad in CI. It does not replace schematic ERC or design review.
Another source check expands every grouped Test PCB BOM reference and quantity,
then compares the result with all footprints in the committed generated board.

The next layers are synthesized pointer/swipe and golden-diff tests, service
recovery integration tests, generated PAL/NTSC video pipeline assertions, DVR interruption tests,
and hardware-in-the-loop fixtures. Hardware tests must report measurements with
fixture version, firmware version, sample count, median, p95, minimum, maximum,
and environmental notes.
