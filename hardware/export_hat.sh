#!/usr/bin/env bash
set -euo pipefail

repo_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
board="$repo_dir/hardware/fpv-hat/fpv-hat.kicad_pcb"
export_dir="$repo_dir/hardware/fpv-hat/export"
mkdir -p "$export_dir"

kicad-cli pcb export svg --page-size-mode 2 --exclude-drawing-sheet \
  --layers F.Cu,F.Paste,F.Silkscreen,Edge.Cuts,Dwgs.User \
  --output "$export_dir/fpv-hat-top.svg" "$board"
kicad-cli pcb export svg --page-size-mode 2 --exclude-drawing-sheet --mirror \
  --layers B.Cu,B.Paste,B.Silkscreen,Edge.Cuts,Dwgs.User \
  --output "$export_dir/fpv-hat-bottom.svg" "$board"

# KiCad 7 emits insignificant spaces before newlines; normalize them so the
# source-controlled review artifacts remain diff-check clean and deterministic.
sed -i 's/[[:space:]]\+$//' \
  "$export_dir/fpv-hat-top.svg" "$export_dir/fpv-hat-bottom.svg"

rsvg-convert --background-color '#071016' --width 1800 \
  --output "$export_dir/fpv-hat-top.png" "$export_dir/fpv-hat-top.svg"
rsvg-convert --background-color '#071016' --width 1800 \
  --output "$export_dir/fpv-hat-bottom.png" "$export_dir/fpv-hat-bottom.svg"

kicad-cli pcb export step --force --subst-models \
  --output "$export_dir/fpv-hat-placement.step" "$board"
sed -i 's/[[:space:]]\+$//' "$export_dir/fpv-hat-placement.step"

echo "Exported HAT review views to ${export_dir#$repo_dir/}/"
