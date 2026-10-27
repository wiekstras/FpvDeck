# HAT Rev A readiness

**Status: NOT READY FOR FABRICATION.**

The design is sufficiently concrete for firmware and schematic review, but no
Gerbers or placement files are released.

## Closed

- [x] Official Waveshare 40-pin signal map captured in one firmware header.
- [x] Official Waveshare overall, active-area, board and 112.003 × 57 mm
      mounting-pattern dimensions captured in parametric enclosure CAD.
- [x] Source-controlled mechanical PCB places two edge SMAs, a side-entry
      JST-XH balance connector and three buttons, with SVG/STEP review exports.
- [x] TVP5150AM1 pin assignment and register plan traced to TI SLES209E.
- [x] ADS1115 DGS pin order corrected against TI SBAS444E and a local 100 nF
      VDD bypass added for each ADC; generator assertions now lock those pins.
- [x] Added TVP5150 100 nF local bypasses for PLL_AVDD, CH_AVDD, DVDD and
      IO_DVDD from TI Figure 6-1, plus ME6211 1 µF input/output capacitors.
- [x] Checked source model emits BOM, audit netlist and KiCad-readable schematic.
- [x] RX A required / RX B DNP assembly strategy established.
- [x] External pack is measurement-only in every active document.

## Blocking gates

- [ ] Measure at least three actual RX5808 modules; verify pin order, body, pads and antenna node.
- [ ] Import/measure the exact host 40-pin header offset and keep-outs from the
      official STEP; confirm mating height, battery envelope and antenna clearance.
- [ ] Replace the placement-only PCB container with a four-layer routed board.
- [ ] Simulate and bench TVP input termination, diversity switching and AV buffer branch.
- [ ] Select/validate ESD and miswire protection for every balance pin.
- [ ] Select a crystal whose combined tolerance meets TI's ±50 ppm requirement;
      calculate load capacitors from measured stray capacitance and verify margin.
- [ ] Select and validate the recommended CVBS anti-aliasing filter and input ESD.
- [ ] Remove or justify the DNP HAT deck-cell divider; raw BAT is not available
      at the 40-pin header and must not be acquired with an unreviewed flying wire.
- [ ] Add/verify the remaining switch, comparator and video-buffer decoupling
      from each manufacturer application circuit.
- [ ] Independent schematic/pin/footprint review.
- [ ] ERC clean; PCB DRC clean; controlled-impedance stack-up documented.
- [ ] 1:1 footprint print checked against every physical component.
