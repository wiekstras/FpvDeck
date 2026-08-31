# Contributing to FpvDeck

FpvDeck combines flight-adjacent RF hardware, analog video, embedded Linux, MCU
firmware, and a desktop simulator. Contributions are welcome, but claims and
electrical assumptions must be reviewable.

## Before opening a change

1. Read the [architecture](docs/ARCHITECTURE.md), [risk register](docs/RISK_REGISTER.md),
   and the document for the subsystem you are changing.
2. Search existing issues and `TODO.md`.
3. For hardware, cite the manufacturer datasheet and mark unverified facts
   **UNKNOWN** or **NEEDS MEASUREMENT**. Never infer a T8L pinout.
4. Keep flight control independent of Linux and keep the video path free of
   backpressure from apps, networking, and DVR.

## Development loop

```sh
./scripts/check
./scripts/demo
```

Add focused tests for service, protocol, persistence, and recovery behavior.
Hardware-dependent features require a simulated backend so reviewers can exercise
them on a desktop. UI changes that affect showcase states should regenerate
`docs/assets/ui/` with `./scripts/screenshot-demo`.

## Pull requests

- Keep changes coherent and explain measured behavior, risks, and remaining gaps.
- Update `ENGINEERING_LOG.md`, `TODO.md`, and architecture documents when a decision
  or interface changes.
- Do not commit generated build trees, proprietary video, credentials, vendor
  archives, or unredistributable datasheets.
- KiCad changes need ERC evidence, a footprint/pinout source, and a fabrication
  warning until the revision passes review.
- Do not use generated concept art as a mechanical reference.

Bug reports and design proposals that include repeatable steps, logs, source links,
and test fixtures are especially useful.
