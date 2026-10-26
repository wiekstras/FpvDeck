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

Buttons always provide launcher navigation. Streaming or a settings failure must
not block the monitor capture task. The firmware currently has deterministic UI
states; live data binding and physical touch/button usability remain HIL tasks.

## Concept imagery

![Front concept](assets/renders/fpvdeck-field-tool-concept.png)

![Exploded concept](assets/renders/fpvdeck-exploded-concept.png)

Both images are generated concepts, not dimensional CAD or routing evidence.
