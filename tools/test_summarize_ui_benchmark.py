import json
import tempfile
import unittest
from pathlib import Path

from tools.summarize_ui_benchmark import load_record


class UiBenchmarkRecordTest(unittest.TestCase):
    def write_record(self, record: dict[str, object]) -> Path:
        directory = Path(tempfile.mkdtemp(prefix="fpvdeck-benchmark-test-"))
        path = directory / "record.json"
        path.write_text(json.dumps(record), encoding="utf-8")
        return path

    def test_loads_schema_v1_record(self):
        record = {
            "schema_version": 1,
            "demo_state": "fpv",
            "qml_loaded_ms": 100,
            "first_frame_ms": 140,
            "rss_kib": 100_000,
            "timed_out": False,
        }
        self.assertEqual(load_record(self.write_record(record)), record)

    def test_rejects_missing_or_unknown_schema(self):
        with self.assertRaises(ValueError):
            load_record(self.write_record({"schema_version": 1}))
        complete = {
            "schema_version": 2,
            "demo_state": "fpv",
            "qml_loaded_ms": 100,
            "first_frame_ms": 140,
            "rss_kib": 100_000,
            "timed_out": False,
        }
        with self.assertRaises(ValueError):
            load_record(self.write_record(complete))


if __name__ == "__main__":
    unittest.main()
