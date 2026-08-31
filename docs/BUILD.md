# Build

## Ubuntu 24.04 / Debian-family desktop

Install the host toolchain:

```sh
sudo apt-get update
sudo apt-get install cmake ninja-build g++ ffmpeg \
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

## macOS

The source architecture is portable, but the macOS dependency/bootstrap script
has not yet been verified. Install Qt 6, CMake, Ninja, and FFmpeg through a
package manager, then configure with the Qt prefix visible to CMake. This is an
open verification task, not a claimed supported path yet.
