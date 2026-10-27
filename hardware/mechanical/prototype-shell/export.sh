#!/usr/bin/env bash
set -euo pipefail
here="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
out="$here/export"
mkdir -p "$out"
openscad -o "$out/fpvdeck-front.stl" -D 'part="front"' "$here/fpvdeck-shell.scad"
openscad -o "$out/fpvdeck-rear.stl" -D 'part="rear"' "$here/fpvdeck-shell.scad"
xvfb-run -a openscad -o "$out/fpvdeck-assembly.png" --imgsize=1600,900 \
  --colorscheme=Tomorrow --viewall --autocenter "$here/fpvdeck-shell.scad"
xvfb-run -a openscad -o "$out/fpvdeck-front.png" --imgsize=1600,900 \
  --colorscheme=Tomorrow --viewall --autocenter --camera=0,0,0,0,0,0,300 \
  -D 'part="front"' "$here/fpvdeck-shell.scad"
xvfb-run -a openscad -o "$out/fpvdeck-rear.png" --imgsize=1600,900 \
  --colorscheme=Tomorrow --viewall --autocenter --camera=0,0,0,0,0,0,300 \
  -D 'part="rear"' "$here/fpvdeck-shell.scad"
xvfb-run -a openscad -o "$out/fpvdeck-product-concept.png" --imgsize=1600,900 \
  --colorscheme=Tomorrow --viewall --autocenter --camera=0,0,0,0,0,0,300 \
  -D 'part="product"' "$here/fpvdeck-shell.scad"
