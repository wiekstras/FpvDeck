# Testing

Run the local CI-equivalent suite with:

```sh
./scripts/check
```

Current automated coverage includes cumulative balance-tap derivation for 1S–6S,
imbalance/noise and invalid tap cases; MCU protocol CRC/framing/corruption; and
SQLite migration plus flight persistence/reopen. Service tests cover battery
faults, telemetry loss/corruption, PAL/NTSC validation, and input allowlist/lockout.
A headless QML smoke test proves the complete application graph loads and exits.

The next layers are QML smoke/screenshot tests, service recovery integration
tests, generated PAL/NTSC video pipeline assertions, DVR interruption tests,
and hardware-in-the-loop fixtures. Hardware tests must report measurements with
fixture version, firmware version, sample count, median, p95, minimum, maximum,
and environmental notes.
