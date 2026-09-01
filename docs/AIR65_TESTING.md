# Air65 analog video test procedure

The owned BetaFPV Air65 is Prototype 0's real analog source. BetaFPV confirms
that the current analog model integrates its camera, analog OSD, 2.4 GHz ELRS
receiver and 5.8 GHz 25–400 mW VTX. Control and video remain separate paths.

## Safety

1. Remove all four propellers before any bench test.
2. Place the Air65 on a non-conductive surface with airflow around its electronics.
3. Use its normal charged 1S BT2.0 flight battery. Do not improvise bench wiring.
4. Keep VTX tests short. Use the lowest usable configured VTX power and allow the
   quad to cool between long stationary sessions.
5. Fit the RC832 antenna before powering the receiver.

## Determine the exact channel

Do not guess the factory channel. With the props removed, connect Betaflight
Configurator and inspect **Video Transmitter**. Record band, channel, frequency
and power. If the UI cannot identify it, save the existing configuration and use
the Betaflight CLI `vtx`/`vtxtable` information without changing unrelated flight
settings. Raceband values are supported by the RC832 Mini, including R1 5658 MHz
through R8 5917 MHz.

## Test sequence

1. Assemble and power the Pi/display, receiver and USB capture with the Air65 off.
2. Set RC832 `FR` and `CH` to the Air65's recorded band/channel.
3. Run `./scripts/fpvdeck-video-list`; confirm the UVG-002 node. Depending on the
   received chipset it may identify as USBTV007/`usbtv` or UVC `AV TO USB2.0`.
4. Inspect `v4l2-ctl --device /dev/videoN --list-inputs`, then run
   `./scripts/test-video --device /dev/videoN --input N --standard PAL` using the
   actual capture node and driver-reported composite input index. If the configured
   camera standard is NTSC, select `--standard NTSC`.
5. Power the prop-less Air65 from its normal 1S pack.
6. Verify raw video and the embedded Betaflight OSD in the independent preview.
7. Run `./scripts/prototype0 --device /dev/videoN --input N --standard PAL`.
8. Verify video, FpvDeck RGB status chips, touch, menu auto-hide and flight lock.
9. Start/stop a short DVR recording and play it back.
10. Unplug only the Air65 battery and confirm FpvDeck remains responsive and shows
    loss/no-video behavior; reconnect and time recovery.
11. For weak-signal behavior, increase separation gradually while staying legal
    and keeping the stationary quad thermally safe. Do not fly for initial tests.

Record the receiver channel, reported capture format/FPS, dropped frames, recovery
time and rough glass-to-glass latency in the engineering log. UVG-002 latency is
a Prototype 0 baseline and is not a final product acceptance value.
