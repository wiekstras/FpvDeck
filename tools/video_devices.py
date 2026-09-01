#!/usr/bin/env python3
"""Enumerate Linux V4L2 devices without hiding missing-driver failures."""

from __future__ import annotations

import argparse
import glob
import json
import os
import subprocess
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Callable


@dataclass(frozen=True)
class VideoDevice:
    path: str
    name: str
    formats: tuple[str, ...]
    error: str = ""


def _device_number(path: str) -> tuple[int, str]:
    suffix = Path(path).name.removeprefix("video")
    return (int(suffix) if suffix.isdigit() else 1_000_000, path)


def discover_devices(
    dev_root: str = "/dev",
    sys_root: str = "/sys/class/video4linux",
    runner: Callable[..., subprocess.CompletedProcess[str]] = subprocess.run,
) -> list[VideoDevice]:
    devices: list[VideoDevice] = []
    for path in sorted(glob.glob(os.path.join(dev_root, "video*")), key=_device_number):
        node = Path(path).name
        name_path = Path(sys_root) / node / "name"
        try:
            name = name_path.read_text(encoding="utf-8").strip()
        except OSError:
            name = "Unknown V4L2 device"

        try:
            result = runner(
                ["v4l2-ctl", "--device", path, "--list-formats-ext"],
                check=False,
                capture_output=True,
                text=True,
            )
        except FileNotFoundError:
            devices.append(VideoDevice(path, name, (), "v4l2-ctl is not installed"))
            continue

        formats = tuple(
            line.strip()
            for line in result.stdout.splitlines()
            if line.strip().startswith(("[", "Size:", "Interval:"))
        )
        error = result.stderr.strip() if result.returncode else ""
        devices.append(VideoDevice(path, name, formats, error))
    return devices


def select_capture_device(
    devices: list[VideoDevice], preferred_name: str = ""
) -> VideoDevice | None:
    """Choose a node that reports capture formats, preferring a known device name."""
    usable = [device for device in devices if device.formats and not device.error]
    if preferred_name:
        needle = preferred_name.casefold()
        for device in usable:
            if needle in device.name.casefold():
                return device
    return usable[0] if usable else None


def main() -> int:
    parser = argparse.ArgumentParser(description="List V4L2 capture devices and formats")
    parser.add_argument("--json", action="store_true", help="emit machine-readable JSON")
    parser.add_argument(
        "--path-only", action="store_true", help="print one usable capture node path"
    )
    parser.add_argument(
        "--match", default="", help="prefer a device whose name contains this text"
    )
    parser.add_argument("--dev-root", default="/dev", help=argparse.SUPPRESS)
    parser.add_argument("--sys-root", default="/sys/class/video4linux", help=argparse.SUPPRESS)
    args = parser.parse_args()

    devices = discover_devices(args.dev_root, args.sys_root)
    if args.path_only:
        selected = select_capture_device(devices, args.match)
        if selected:
            print(selected.path)
        return 0 if selected else 1
    if args.json:
        print(json.dumps([asdict(device) for device in devices], indent=2))
    elif not devices:
        print("No V4L2 video devices found.")
    else:
        for device in devices:
            print(f"{device.path}\n  {device.name}")
            if device.error:
                print(f"  ERROR: {device.error}")
            elif not device.formats:
                print("  No capture formats reported")
            else:
                for line in device.formats:
                    print(f"  {line}")
    return 0 if devices else 1


if __name__ == "__main__":
    raise SystemExit(main())
