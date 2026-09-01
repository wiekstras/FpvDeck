# Build

## Ubuntu 24.04 / Debian-family desktop

Install the host toolchain:

```sh
sudo apt-get update
sudo apt-get install cmake ninja-build g++ ffmpeg v4l-utils \
  gstreamer1.0-libav gstreamer1.0-plugins-good gstreamer1.0-plugins-bad \
  qt6-base-dev qt6-declarative-dev qt6-multimedia-dev qt6-tools-dev-tools \
  qml6-module-qtquick qml6-module-qtquick-controls \
  qml6-module-qtquick-layouts qml6-module-qtquick-window \
  qml6-module-qtmultimedia qml6-module-qttest qml6-module-qtqml \
  qml6-module-qtqml-models qml6-module-qtqml-workerscript \
  qml6-module-qtquick-templates xvfb
```

Build, test, and launch:

```sh
./scripts/check
./scripts/dev
./scripts/demo
```

`scripts/generate-test-video` creates copyright-free PAL-shaped synthetic test
footage locally with FFmpeg. The text drawn into that video represents an OSD
that was already burned in at the aircraft. FPVDeck's colored widgets are QML
elements composited above it.

Set `FPVDECK_DATA_DIR` to relocate the simulator database. Set
`FPVDECK_BUILD_DIR` to relocate the build tree. `fpvdeck --help` lists runtime
arguments.

`./scripts/screenshot-demo` additionally requires `xvfb-run` (provided by the
`xvfb` package above) and regenerates the committed deterministic UI previews.
`./scripts/benchmark-ui` uses the same dependency to collect repeated desktop
first-frame/RSS regression records under `generated/benchmarks/ui/`.

## Raspberry Pi / Prototype 0

On current 64-bit Raspberry Pi OS:

```sh
git clone https://github.com/wiekstras/FpvDeck.git
cd FpvDeck
./scripts/setup-pi
./scripts/fpvdeck-video-list
./scripts/prototype0 --device /dev/video0 --standard PAL
```

`setup-pi` installs only Debian packages and is idempotent. `--dry-run` prints
the package transaction. The real device node and standard must match the output
of `fpvdeck-video-list` and the Air65 configuration.

## macOS

The source architecture is portable, but the macOS dependency/bootstrap script
has not yet been verified. Install Qt 6, CMake, Ninja, and FFmpeg through a
package manager, then configure with the Qt prefix visible to CMake. This is an
open verification task, not a claimed supported path yet.
