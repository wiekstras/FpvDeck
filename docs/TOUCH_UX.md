# Touch and control interaction

FpvDeck is touchscreen-first for applications and remains physically operable
for flight-critical actions. The UI should feel like a dedicated radio, not a
desktop compressed onto a display.

## Input hierarchy

1. Gimbals, flight switches, and the ELRS path are never application input.
2. A dedicated physical DVR action, Back, Select, Menu, and directional controls
   remain available while flying.
3. Touch is the fastest path through Home, Battery, Media, Receiver, Diagnostics,
   and Settings.
4. Gestures are conveniences only. Every important action has a visible control
   or physical path.

The current desktop keyboard backend maps arrows to forward/reverse focus cycling
and Enter to activate. A spatial focus graph is still required before the final
physical control layout is frozen.

## Design tokens

The source of truth is `software/ui/theme/Theme.qml`.

- Minimum normal touch target: 52 px at the 1280×720 reference canvas; never
  below a 44 px equivalent after scaling.
- Spacing: 8, 12, 16, 24, and 32 px tiers.
- Primary text: near-white; muted state: cool gray; actionable state: cyan;
  success: green; warning: amber; danger: red. Text/icon/shape accompany color.
- Panels use dark translucent surfaces, a restrained cyan edge, and 14–20 px
  radii. Flight overlays are smaller and more transparent than app panels.
- Transitions are normally 120–180 ms. Video visibility and frame pacing take
  precedence over animation completion.

Reusable components include `TouchButton`, `StatusChip`, `AppTile`, `TopBar`,
`OverlayCard`, `CellVoltageRow`, `MediaTile`, `Toast`, and `BootSplash`.

## Fly screen

The video remains nearly full-screen. A tap reveals a temporary control layer;
another tap or the physical Back action dismisses it. Controls auto-hide after
inactivity. DVR, receiver, brightness, external-battery detail, Media, and flight
lock are visible actions rather than gesture-only functions.

Flight lock prevents accidental navigation away from live video. Enabling it is
one deliberate action; leaving Fly requires unlocking first. Signal loss and
critical system faults remain visible while the controls are hidden.

The compact external-pack overlay exists only while a balance pack is connected.
Tapping it opens Battery Checker. It must never imply that the external pack
powers or charges FpvDeck.

## Gesture policy

- Tap: activate or reveal FPV controls.
- Long press: reserved for future configuration/context; never destructive.
- Edge swipe: future quick panel, only after pointer-driven tests exist.
- Pinch: not required by the UI. The user requirement for pinch grip means the
  enclosure/display must leave both gimbal finger envelopes unobstructed.

No destructive or flight-ending operation may be single-gesture. Confirmation
dialogs use plain language, large separated actions, and a safe default.

## Environmental rules

- Default theme is dark to preserve night vision, with a quick brightness path.
- Status never depends on color alone.
- Dense tables become large rows/cards; fine scroll bars are not controls.
- Glove operation is supported by physical navigation. Capacitive glove behavior
  of the selected display is **NEEDS MEASUREMENT**.
- Touch controller behavior with water, RF emission, ground bounce, and the
  portrait-native panel mounted landscape must be tested on the bench.

## Automated evidence

Service tests cover auto-hide, flight lock, fault states, storage/media state,
and receiver selection. Deterministic full-page QML startup tests and screenshots
cover the main visual states. Synthesized pointer/swipe tests and maintainable
golden-diff thresholds remain an explicit TODO; the existing tests must not be
described as full touch-gesture coverage.
