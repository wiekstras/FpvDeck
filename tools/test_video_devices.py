import subprocess
import tempfile
import unittest
from pathlib import Path

from tools.video_devices import VideoDevice, discover_devices, select_capture_device


class VideoDeviceDiscoveryTest(unittest.TestCase):
    def test_orders_nodes_and_reports_formats(self):
        with tempfile.TemporaryDirectory() as temp:
            root = Path(temp)
            dev = root / "dev"
            sys = root / "sys"
            dev.mkdir()
            for number, name in ((10, "Metadata"), (2, "usbtv")):
                (dev / f"video{number}").touch()
                node = sys / f"video{number}"
                node.mkdir(parents=True)
                (node / "name").write_text(name, encoding="utf-8")

            def runner(command, **_kwargs):
                self.assertEqual(command[0], "v4l2-ctl")
                return subprocess.CompletedProcess(
                    command, 0, "[0]: 'YUYV'\n  Size: Discrete 720x576\n", ""
                )

            devices = discover_devices(str(dev), str(sys), runner)
            self.assertEqual([item.path for item in devices], [str(dev / "video2"), str(dev / "video10")])
            self.assertEqual(devices[0].name, "usbtv")
            self.assertIn("Size: Discrete 720x576", devices[0].formats)

    def test_no_devices_is_not_an_error_in_library(self):
        with tempfile.TemporaryDirectory() as temp:
            self.assertEqual(discover_devices(temp, temp), [])

    def test_selects_formatted_node_and_prefers_name(self):
        devices = [
            VideoDevice("/dev/video0", "Metadata", ()),
            VideoDevice("/dev/video1", "Other Capture", ("[0]: 'YUYV'",)),
            VideoDevice("/dev/video2", "USBTV007 usbtv", ("[0]: 'YUYV'",)),
        ]
        self.assertEqual(select_capture_device(devices).path, "/dev/video1")
        self.assertEqual(select_capture_device(devices, "usbtv").path, "/dev/video2")

    def test_rejects_nodes_without_capture_formats(self):
        self.assertIsNone(select_capture_device([VideoDevice("/dev/video0", "Metadata", ())]))


if __name__ == "__main__":
    unittest.main()
