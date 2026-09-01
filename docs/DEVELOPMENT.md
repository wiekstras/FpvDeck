# Development

FPVDeck is an appliance UI, not a desktop shell. Hardware backends are selected
behind service interfaces; screens consume stable QObject properties and never
open devices directly.

## Video backends

- `simulated`: generated source used when no explicit input is provided.
- `file`: explicit `--video PATH`, used by deterministic tests and demos.
- `v4l2`: explicit `--video-device /dev/videoN`; optional `--video-standard
  PAL|NTSC` keeps diagnostics aligned with the forced capture norm. Qt's Linux
  camera backend owns capture and publishes frames to the existing FPV compositor.

Use `./scripts/fpvdeck-video-list` and `./scripts/test-video` before debugging
the full UI. A missing device produces an explicit `lost` service state rather
than terminating the application. `./scripts/prototype0` provides friendly
auto-detection and simulator fallback for bench work.

## Keyboard input backend

- Arrows: cycle the current screen's large actionable controls; Enter activates
  the focused control. This is a desktop stand-in for the dedicated navigation
  cluster, not a connection to any gimbal or flight switch.
- Escape: close overlay or return to FPV.
- `M`: application menu.
- `R`: DVR start/stop.
- `B`: battery overlay.
- `L`: video signal loss/recovery.
- `F10`: development-only simulator control panel.

Critical flight controls will never be translated into UI navigation. A future
MCU input event includes its control class, and InputService will accept menu
events only from a physically separate navigation allowlist.

## Definition of done

A change is not complete because it compiles. Relevant automated tests must pass,
the simulator path must remain usable, failure behavior must be considered, and
hardware claims must carry a source or a `NEEDS MEASUREMENT` marker.
