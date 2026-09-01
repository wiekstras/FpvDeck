# Application and widget API

Applications receive service interfaces from AppManager; they do not instantiate
drivers. API objects are versioned at the C++ boundary when external plugins are
introduced. QML modules bundled with the system currently compile together.

## BatteryService snapshot

`connected`, `chemistry`, `cellCount`, `cellVoltages`, `packVoltage`, `delta`,
`weakestCell`, `weakestVoltage`, `strongestCell`, `strongestVoltage`, `health`,
`warning`, and event time. A future `batteryId` is nullable and may originate from manual choice,
QR, or NFC. FPV's BatteryWidget reads the same service as the full Battery app.

## VideoService snapshot

`standard`, `lockState`, `sourceFormat`, `frameSequence`, `lastFrameTime`, VRX
`channel`, `rssi`, and fault counters. Pixel buffers are never exposed to general
QML plugins; only the trusted video item owns them.

## InteractionService

Owns `controlsVisible`, `flightLocked`, `autoHideMs`, and last touch position.
Navigation is requested through the service; a locked request emits rejection
instead of silently navigating. UI pages do not independently implement flight
lock or auto-hide timers.

## StorageService and MediaService

Storage publishes removable-card presence, health, free space, and immutable
media metadata. Media owns open/play/pause/seek/skip state and aspect metadata.
Playback uses preserve-aspect rendering. A future real Storage backend owns mount
and safe-eject policy; Media never shells out or mounts a device.

## SystemService

Publishes internal deck battery/charging, MCU/ADC/VRX health, temperatures,
brightness, uptime, and developer touch-debug state. External balance-pack values
never enter this service; that separation prevents UI or backend code from
confusing the measurement-only port with the internal energy source.

## Overlay provider contract

An app may register declarative widget metadata: stable ID, QML component URL,
minimum/maximum size, default anchor, required service capabilities, update-rate
limit, and safety priority. App widgets cannot create unbounded timers, access
devices, or cover reserved Betaflight OSD safe zones without explicit user action.

## Failure contract

Service values include `quality` (`valid`, `stale`, `unavailable`, `fault`) and a
monotonic timestamp in the real backend contract. Apps must render
stale/unavailable explicitly rather than
retaining a plausible old value. Calls are nonblocking; long operations return a
request ID and completion/error signal.
