# OpenPocket / FPVDeck

OpenPocket is an open-source, low-latency analog FPV handheld computer built
around reusable RadioMaster T8L controls and RF hardware. FPVDeck is its local,
appliance-style Linux application platform.

The project is intentionally staged. The first milestone is a fully simulated
desktop application; bench hardware follows only after the latency-critical
video chain has been measured. No flight-control function depends on Linux.

## Status

Early engineering prototype. Hardware selections are provisional until their
latency and electrical interfaces are verified. Never fabricate from files in
this repository unless the relevant revision is explicitly marked released.

## Build the desktop simulator

Ubuntu/Debian prerequisites are documented in [docs/BUILD.md](docs/BUILD.md).
Once installed:

```sh
./scripts/dev
```

Keyboard controls:

| Key | Action |
| --- | --- |
| Arrow keys | Navigate |
| Enter | Select |
| Escape | Back |
| M | Open/close application menu |
| R | Toggle DVR recording |
| B | Toggle battery overlay |
| L | Toggle simulated signal loss |
| F10 | Open development simulator controls |

## Safety

- The battery-checker input is not yet a released circuit. Do not connect a pack
  to any unreviewed prototype.
- No unknown T8L pin is to be driven. Follow `docs/REVERSE_ENGINEERING.md`.
- FPVDeck overlays are secondary information. Betaflight OSD remains in the
  received analog image and is not parsed or modified.

## Repository map

```text
docs/                 Architecture, validation, and operating documentation
hardware/research/    Component comparisons and selection evidence
hardware/pcb/         KiCad projects by hardware revision
hardware/bom/         Prototype and production-oriented BOMs
firmware/controller/  Real-time controller firmware
protocols/            Versioned Linux↔MCU wire protocol
software/             Qt/QML application and service backends
scripts/              Developer and verification entry points
tools/                 Test generators and engineering utilities
```

The project is MIT licensed. Hardware design licensing will be frozen before the
first fabrication release.
