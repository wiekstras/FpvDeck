# Development

FPVDeck is an appliance UI, not a desktop shell. Hardware backends are selected
behind service interfaces; screens consume stable QObject properties and never
open devices directly.

## Keyboard input backend

- Arrows / Enter: reserved for directional navigation as focus handling expands.
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

