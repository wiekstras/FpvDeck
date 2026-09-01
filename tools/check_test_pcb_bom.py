#!/usr/bin/env python3
"""Verify Test PCB Rev A footprint references against its orderable CSV BOM."""

from __future__ import annotations

import csv
import re
from pathlib import Path

PROJECT = Path(__file__).resolve().parents[1]
BOARD = PROJECT / "hardware/pcb/test-board-rev-a/test-board-rev-a.kicad_pcb"
BOM = PROJECT / "hardware/bom/TEST_PCB_REV_A.csv"
NON_FOOTPRINT_REFERENCES = {"PCB1"}


def board_references(path: Path = BOARD) -> set[str]:
    references = set(re.findall(r'\(fp_text reference "([A-Z]+[0-9]+)"', path.read_text(encoding="utf-8")))
    if not references:
        raise ValueError(f"{path}: no footprint references found")
    return references


def expand_reference_token(token: str) -> list[str]:
    match = re.fullmatch(r"([A-Z]+)([0-9]+)(?:-([A-Z]*)([0-9]+))?", token)
    if not match:
        raise ValueError(f"invalid BOM reference token: {token}")
    prefix, start_text, end_prefix, end_text = match.groups()
    if end_text is None:
        return [f"{prefix}{int(start_text)}"]
    if end_prefix not in ("", prefix):
        raise ValueError(f"mixed-prefix BOM range: {token}")
    start, end = int(start_text), int(end_text)
    if end < start:
        raise ValueError(f"descending BOM range: {token}")
    return [f"{prefix}{number}" for number in range(start, end + 1)]


def bom_references(path: Path = BOM) -> tuple[set[str], list[str]]:
    references: set[str] = set()
    errors: list[str] = []
    with path.open(newline="", encoding="utf-8") as source:
        for line, row in enumerate(csv.DictReader(source), start=2):
            expanded: list[str] = []
            for token in row["Reference"].split():
                expanded.extend(expand_reference_token(token))
            duplicates = references.intersection(expanded)
            if duplicates:
                errors.append(f"line {line}: duplicate references {sorted(duplicates)}")
            references.update(expanded)
            declared = int(row["Qty"])
            if declared != len(expanded):
                errors.append(
                    f"line {line}: {row['Reference']} declares Qty {declared}, expands to {len(expanded)}"
                )
    return references, errors


def check(board_path: Path = BOARD, bom_path: Path = BOM) -> list[str]:
    board = board_references(board_path)
    bom, errors = bom_references(bom_path)
    footprint_bom = bom - NON_FOOTPRINT_REFERENCES
    missing = board - footprint_bom
    extra = footprint_bom - board
    if missing:
        errors.append(f"board references missing from BOM: {sorted(missing)}")
    if extra:
        errors.append(f"BOM references absent from board: {sorted(extra)}")
    return errors


def main() -> int:
    errors = check()
    if errors:
        for error in errors:
            print(f"ERROR: {error}")
        return 1
    print(f"Test PCB BOM matches {len(board_references())} generated footprints")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
