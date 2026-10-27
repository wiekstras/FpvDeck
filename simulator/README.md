# macOS / desktop simulator

The simulator compiles the production LVGL launcher and application screens
against LVGL's SDL2 display, mouse/touch and keyboard drivers. Hardware service
calls are replaced at link time by deterministic receiver, balance-pack, deck
battery and decoder simulations.

## macOS quick start

```sh
brew install cmake sdl2 pkg-config
git clone git@github.com:wiekstras/FpvDeck.git
cd FpvDeck
./scripts/simulator
```

A mouse click is a touch. The mouse wheel scrolls. Arrow keys move focus and
Enter activates a focused control. Click the amber **SIM** button to inject:

- balance lead connection/removal, 2S–6S packs and imbalance;
- video loss and analog noise;
- low internal Deck battery.

The monitor screen contains synthetic moving FPV imagery and a deliberately
separate Betaflight-style monochrome OSD underneath FpvDeck's RGB status card.
No hardware is required.

Open a particular screen directly when reviewing layouts:

```sh
./scripts/simulator --screen monitor
./scripts/simulator --screen battery
./scripts/simulator --screen scanner
```

Valid screen names are `launcher`, `battery`, `monitor`, `scanner`, `finder`,
`stream`, and `settings`.

Regenerate the source-controlled launcher, Battery, Monitor, and Scanner images:

```sh
./scripts/simulator-screenshots
```

The script uses macOS `sips` (built in) or `ffmpeg` for BMP-to-PNG conversion.
For one raw capture, use `./scripts/simulator --screen monitor --screenshot
/tmp/monitor.bmp`.

Run the headless suite, which starts every screen in an isolated process, with:

```sh
./scripts/test-simulator
```

## Scope

This validates navigation, layout, touch sizing, simulated services, channel
tuning and failure-state presentation. It does not simulate PPA timing, DVP
electrical behavior, analog receiver characteristics or panel latency.
