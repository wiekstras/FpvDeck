#!/usr/bin/env python3
"""Summarize FpvDeck first-frame benchmark records as a compact table."""

from __future__ import annotations

import argparse
import json
import math
import statistics
from collections import defaultdict
from pathlib import Path


def load_record(path: Path) -> dict[str, object]:
    record = json.loads(path.read_text(encoding="utf-8"))
    required = {"schema_version", "demo_state", "qml_loaded_ms", "first_frame_ms", "rss_kib", "timed_out"}
    missing = required.difference(record)
    if missing:
        raise ValueError(f"{path}: missing fields {sorted(missing)}")
    if record["schema_version"] != 1:
        raise ValueError(f"{path}: unsupported schema {record['schema_version']}")
    return record


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("records", nargs="+", type=Path)
    args = parser.parse_args()
    grouped: dict[str, list[dict[str, object]]] = defaultdict(list)
    for path in args.records:
        record = load_record(path)
        if record["timed_out"]:
            raise ValueError(f"{path}: first-frame benchmark timed out")
        grouped[str(record["demo_state"])].append(record)

    def p95(values: list[int]) -> int:
        ordered = sorted(values)
        return ordered[max(0, math.ceil(len(ordered) * 0.95) - 1)]

    print("state                 n   QML median/p95   frame median/p95   RSS median")
    print("--------------------  --  ----------------  -----------------  ----------")
    for state, records in grouped.items():
        qml = [int(record["qml_loaded_ms"]) for record in records]
        frame = [int(record["first_frame_ms"]) for record in records]
        rss = [int(record["rss_kib"]) for record in records if int(record["rss_kib"]) >= 0]
        rss_text = "n/a" if not rss else f"{statistics.median(rss) / 1024:.1f} MiB"
        print(
            f"{state:20}  {len(records):2}  {statistics.median(qml):6.0f}/{p95(qml):4} ms"
            f"       {statistics.median(frame):6.0f}/{p95(frame):4} ms  {rss_text:>10}"
        )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
