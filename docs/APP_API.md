# Application and widget API

Applications receive service interfaces from AppManager; they do not instantiate
drivers. API objects are versioned at the C++ boundary when external plugins are
introduced. QML modules bundled with the system currently compile together.

## BatteryService snapshot

`connected`, `cellCount`, `cellVoltages`, `packVoltage`, `delta`, `warning`, and
event time. A future `batteryId` is nullable and may originate from manual choice,
QR, or NFC. FPV's BatteryWidget reads the same service as the full Battery app.

## VideoService snapshot

`standard`, `lockState`, `sourceFormat`, `frameSequence`, `lastFrameTime`, VRX
`channel`, `rssi`, and fault counters. Pixel buffers are never exposed to general
QML plugins; only the trusted video item owns them.

## Overlay provider contract

An app may register declarative widget metadata: stable ID, QML component URL,
minimum/maximum size, default anchor, required service capabilities, update-rate
limit, and safety priority. App widgets cannot create unbounded timers, access
devices, or cover reserved Betaflight OSD safe zones without explicit user action.

## Failure contract

Service values include `quality` (`valid`, `stale`, `unavailable`, `fault`) and a
monotonic timestamp. Apps must render stale/unavailable explicitly rather than
retaining a plausible old value. Calls are nonblocking; long operations return a
request ID and completion/error signal.
