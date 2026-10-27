# HAT Rev A readiness

**Status: NOT READY FOR FABRICATION.**

The design is sufficiently concrete for firmware and schematic review, but no
Gerbers or placement files are released.

## Closed

- [x] Official Waveshare 40-pin signal map captured in one firmware header.
- [x] TVP5150AM1 pin assignment and register plan traced to TI SLES209E.
- [x] ADS1115 DGS pin order corrected against TI SBAS444E and a local 100 nF
      VDD bypass added for each ADC; generator assertions now lock those pins.
- [x] Checked source model emits BOM, audit netlist and KiCad-readable schematic.
- [x] RX A required / RX B DNP assembly strategy established.
- [x] External pack is measurement-only in every active document.

## Blocking gates

- [ ] Measure at least three actual RX5808 modules; verify pin order, body, pads and antenna node.
- [ ] Measure host board keep-outs, header mating height, battery envelope and antenna clearance.
- [ ] Replace the placement-only PCB container with a four-layer routed board.
- [ ] Simulate and bench TVP input termination, diversity switching and AV buffer branch.
- [ ] Select/validate ESD and miswire protection for every balance pin.
- [ ] Verify crystal load/circuit and oscillator margin.
- [ ] Remove or justify the DNP HAT deck-cell divider; raw BAT is not available
      at the 40-pin header and must not be acquired with an unreviewed flying wire.
- [ ] Add/verify the remaining TVP5150, LDO, switch, comparator and video-buffer
      decoupling from each manufacturer application circuit.
- [ ] Independent schematic/pin/footprint review.
- [ ] ERC clean; PCB DRC clean; controlled-impedance stack-up documented.
- [ ] 1:1 footprint print checked against every physical component.
