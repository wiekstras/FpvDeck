# Prototype 0 setup

This guide starts after the items in [Buy Tonight](../hardware/PROTOTYPE_0_BUY_TONIGHT.md)
arrive. Read [the exact wiring table](../hardware/prototype-0/WIRING.md) first.

## 1. Prepare the Pi

1. Use Raspberry Pi Imager to write current 64-bit Raspberry Pi OS with Desktop
   to the 64 GB card. Configure a user, Wi-Fi and SSH in Imager if desired.
2. With all power disconnected, fit the official Active Cooler.
3. Connect Touch Display 2 using its supplied Pi 5 22-to-15-way FFC and GPIO
   power cable. Verify GPIO pin 2 is red/+5 V and pin 6 is black/ground.
4. Insert the microSD and connect the official 27 W USB-C supply.
5. Boot, update the OS, and rotate display/touch together to landscape in Raspberry
   Pi Control Centre. Verify tap, drag, swipe, long press and the on-screen keyboard.

## 2. Install FpvDeck

```bash
git clone https://github.com/wiekstras/FpvDeck.git
cd FpvDeck
./scripts/setup-pi
```

The setup is safe to re-run. After it completes:

```bash
./scripts/dev
```

Use the simulator first to verify the build, touch navigation and landscape
orientation before connecting RF/video hardware.

## 3. Connect receiver and capture

1. Keep both wall supplies unplugged.
2. Attach the supplied antenna to RC832 `ANT`.
3. Connect the supplied RC832 AV cable from `AV OUT`; connect its yellow composite
   RCA plug to UVG-002 yellow composite input.
4. Plug UVG-002 into a Pi USB-A port.
5. Build the verified 12 V connection exactly as described in
   [WIRING.md](../hardware/prototype-0/WIRING.md). Measure polarity before power.
6. Apply receiver power. Do not power RC832 from the Pi.

## 4. Verify raw capture

```bash
./scripts/fpvdeck-video-list
v4l2-ctl --device /dev/video0 --list-inputs
./scripts/test-video --device /dev/video0 --input N --standard PAL
```

The node may be `/dev/video1` or another number. Select the node whose description
is USBTV007/`usbtv` or `AV TO USB2.0`/`uvcvideo` (wording depends on the
received Gembird chipset) and whose formats include the video-capture stream. If access is
denied, add the current user to `video`, log out/in, and retry:

```bash
sudo usermod -aG video "$USER"
```

Replace `N` with the input index whose driver-reported name is composite. Do not
copy an index from this guide: input numbering is evidence captured from the
received unit and driver. Omit `--input` only if the composite input is already
selected.

## 5. Tune and test the Air65

1. Remove the props.
2. Determine the configured VTX frequency in Betaflight as described in
   [AIR65_TESTING.md](AIR65_TESTING.md).
3. Set RC832 `FR` (band) and `CH` to the same frequency.
4. Power Air65 with its normal 1S battery.
5. Confirm raw video and Betaflight OSD in `test-video` before involving FpvDeck.

## 6. Launch real hardware mode

```bash
./scripts/prototype0 --device /dev/video0 --input N --standard PAL
```

Prototype 0 mode uses real V4L2 video and real touch while battery, telemetry,
T8L diagnostics and direct VRX control remain simulated. With no capture device,
the command prints a warning and falls back to the synthetic feed so the UI still
starts.
If UVG-002 is unplugged and reconnected at the same device path, FpvDeck watches
Qt's device list and attempts to resume the camera without restarting the shell.

Verify in order:

1. Air65 video moves and its monochrome Betaflight OSD remains visible.
2. FpvDeck's colour overlay is above the feed.
3. Tap video to show/hide controls; allow auto-hide to run.
4. Open apps and use flight lock.
5. Open Diagnostics; `CVBS CAPTURE` should show the UVG-002 capture node and `PASS`.
6. Start and stop DVR, then inspect the output before trusting longer recordings.
7. Disconnect the Air65 battery, confirm UI survival, reconnect and measure recovery.

## 7. Optional boot service

Do not enable auto-start until manual bring-up is reliable. Prototype 0 deliberately
keeps shell access. A systemd unit remains a follow-up after capture node persistence,
clean shutdown and log retrieval are verified on the actual Pi.

## Troubleshooting

- **No `/dev/video*`:** record `lsusb`, run both `modinfo usbtv` and
  `modinfo uvcvideo`, reseat UVG-002 and update Raspberry Pi OS. UTV007 commonly
  uses `usbtv` IDs such as `1b71:3002`; UTVF007 commonly appears as UVC
  `534d:0021`. Do not force either driver onto a different received ID.
- **UTVF007 gives a stable picture but hides RF noise:** this family is known to
  squelch weak/noisy composite input. That is acceptable for Prototype 0
  functionality, but record it as a limitation; it cannot validate final FPV
  weak-signal behavior.
- **Node exists but black:** select composite input and the correct PAL/NTSC norm
  with `v4l2-ctl`; verify yellow RCA, channel frequency and Air65 power.
- **Rolling/monochrome:** wrong standard or marginal RF. Try the correct norm, not
  arbitrary format conversions.
- **Touch rotated:** rotate the display in Raspberry Pi Control Centre so libinput
  applies the matching touch transform.
- **Pi undervoltage:** use the official 27 W supply; RC832 must retain its own 12 V PSU.
