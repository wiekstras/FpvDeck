# Simulator

The same FPVDeck executable runs simulated and real backends. Desktop development
currently instantiates simulated battery, dedicated 5.8 GHz Radio/VRX, separate
video/decoder, DVR, input, removable
storage, media playback, deck health, and system state.

`./scripts/demo` opens the deterministic showcase Home state. The Home, FPV,
Balance, Media, Flights, and Diagnostics routes can be selected at startup with
`--demo-state`; supported states are `home`, `fpv`, `fpv-controls`, `fpv-lock`,
`battery`, `media`, `media-playback`, `receiver`, `flights`, and `diagnostics`.
This seeds temporary/repeatable state when requested.
The normal `./scripts/dev` path retains the developer database and opens FPV.

The `F10` panel injects video lock, weak signal, vertical roll, signal loss,
black frames, PAL/NTSC selection, channel changes, balance lead events,
imbalance, under/overvoltage, deck battery drain/charging, storage insertion,
removal/corruption/full, T8L/ELRS/MCU/ADC/VRX/decoder disconnects, and temperature warnings. The
fault strip is deliberately visible only in development mode.

Mouse input is treated as a single touch point. Enable **Show touch points** in
Settings to display a transient pointer marker. Multi-touch and pinch are not
used for critical actions; the product requirement for pinch grip refers to
physical gimbal ergonomics, not a gesture dependency.

Synthetic input video is generated at 720×576 and 50 frames/s. It intentionally
contains white pseudo-OSD text inside the encoded pixels. Colored translucent UI
remains a separate QML layer. This verifies the composition model but does not
claim analog timing fidelity.

Representative repository screenshots are generated from the executable, rather
than drawn mockups:

```sh
./scripts/screenshot-demo
```

The command uses Xvfb so the Qt Multimedia video texture is included, writes eight
1280×720 captures to `docs/assets/ui/`, and uses an in-memory SQLite database.

Future simulation work still required: pointer-driven QML test automation,
decoder resolution-change assertions, real test-file recorder interruption and
recovery, and an analog-noise shader calibrated against captured receiver output.
