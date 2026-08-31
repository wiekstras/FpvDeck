# Simulator

The same FPVDeck executable runs simulated and real backends. Desktop development
currently instantiates simulated battery, VRX/video, DVR, input, and system state.

The `F10` panel injects video lock, weak signal, vertical roll, signal loss,
black frames, PAL/NTSC selection, channel changes, battery imbalance, under- and
overvoltage, disconnect, and invalid balance-tap ordering. DVR storage-full can
be injected from the DVR app.

Synthetic input video is generated at 720×576 and 50 frames/s. It intentionally
contains white pseudo-OSD text inside the encoded pixels. Colored translucent UI
remains a separate QML layer. This verifies the composition model but does not
claim analog timing fidelity.

Future fault backends still required: telemetry packet corruption/reconnect,
MCU disconnect/reconnect, temperature warning, decoder resolution changes, file
recorder I/O failure, and a controllable analog-noise shader calibrated against
captured receiver output.

