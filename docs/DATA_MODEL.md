# Flight and battery data model

SQLite is local, authoritative storage. Foreign keys are enabled; WAL plus FULL
synchronous mode is used today. Schema changes are ordered migrations recorded in
`schema_migrations`; the test suite creates, migrates, writes, closes, reopens and
reads a database.

## Core relationships

```text
aircraft 1 ── * flights * ── 0..1 batteries
                    │
                    ├── * telemetry_samples
                    └── * recordings

batteries 1 ── * battery_observations
```

Flights store UTC start/end, aircraft, battery, receiver channel, minimum RSSI/LQ,
minimum pack voltage, maximum cell delta, maximum distance/altitude, notes, DVR
path compatibility field, application version, and aircraft firmware. High-rate
history is normalized into telemetry samples keyed by flight and monotonic offset.

Battery identity is a stable optional UID independent of display name. Brand,
chemistry, cell count, capacity, purchase date, cycle count, estimated health and
notes are stored. Average flight duration, flight count, minimum observed voltage,
and maximum imbalance are derived from associated records rather than duplicated.
`battery_observations` preserves checker-only measurements even without a flight.

Recordings have stream kind (`pre-overlay` or `composited`), container, timestamps,
size, recovery flag and optional flight. GPS coordinates remain nullable; V1 does
not require location. NFC/QR resolve to `battery_uid` later without schema change.

## Durability rules

- Flight start is committed before DVR start; end/summary is a later transaction.
- Samples are inserted in bounded batches, never on the UI thread.
- Files are created with a temporary/in-progress suffix and short segments. The
  database references a final path only after durable close/rename.
- Boot recovery scans unfinished flights/recordings and records what was inferred;
  it never fabricates an end time without marking recovery metadata.
- Export/backup reads a consistent SQLite snapshot and never pauses live video.

