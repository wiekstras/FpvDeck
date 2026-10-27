# User interface

The landscape UI is touch-first with three redundant buttons: up, OK, and down.
Minimum interactive target is 52 px; primary tiles are 250×112 px. Dark surfaces,
teal focus, text labels and shape changes avoid color-only status.

## Applications

- **Battery:** total, six large cell rows, delta, lowest/highest, warnings.
- **FPV monitor:** video-dominant screen with one compact telemetry strip.
- **Scanner:** tap-selectable RSSI bars and strongest three channels.
- **Finder:** glanceable RSSI, peak hold and optional proportional buzzer.
- **Stream:** explicit status for local spectator SoftAP.
- **Settings:** brightness, video mode, buzzer, Wi-Fi and calibration.

Buttons are registered as an LVGL keypad: Up/Down move focus and OK activates
the focused control. Streaming or a settings failure must not block the monitor
capture task. App-specific channel/long-press behavior and physical usability
remain HIL tasks. Production screens show explicit empty states rather than
plausible-looking fabricated readings whenever their hardware source is absent.
Battery and RSSI views already consume live snapshots; video/display handoff is
still the primary live-binding gate.

## Concept imagery

![Front concept](assets/renders/fpvdeck-field-tool-concept.png)

![Exploded concept](assets/renders/fpvdeck-exploded-concept.png)

Both images are generated concepts, not dimensional CAD or routing evidence.
