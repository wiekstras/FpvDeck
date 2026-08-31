# Software architecture

![FpvDeck software architecture](assets/diagrams/software-architecture.svg)

FPVDeck is a single-purpose Qt 6 application launched directly into a DRM/KMS
session. There is no desktop environment. Systemd owns process lifetime and a
small supervisor restarts noncritical workers independently.

```text
QML apps/widgets
   │ stable QObject APIs, queued signals, immutable snapshots
AppManager ──────────────────────────────────────────────┐
   │                                                     │
Video  Battery  Telemetry  Input  DVR  Radio  Storage  Database services
   │                                                     │
real backend / simulated backend / replay backend        │
   └──────────── isolated drivers and worker processes ──┘
```

VideoService and the compositor are the protected core. Network, history,
battery UI, and settings can restart without taking down capture. DVR receives a
tee and cannot exert backpressure. UI effects are disabled under missed-frame or
temperature pressure.

Qt/QML was chosen over Flutter, Electron/web, and a bespoke OpenGL shell. Qt has
mature embedded KMS/Wayland-less deployment, C++/V4L2 integration, a retained
GPU scene graph, physical focus navigation, and SQLite. Electron is rejected for
runtime/boot/memory cost; Flutter's Linux embedded/video interop adds ecosystem
risk; bespoke rendering would spend effort recreating accessibility, text, and
layout infrastructure.

The repository currently implements BatteryService, VideoService simulator,
DVR simulator, SQLite DatabaseService, application pages, and a file video source.
Telemetry/Input/Radio/Storage interfaces will be added before real backends.
