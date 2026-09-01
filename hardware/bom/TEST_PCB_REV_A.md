# Test PCB Rev A BOM

The machine-readable source is [TEST_PCB_REV_A.csv](TEST_PCB_REV_A.csv). It lists
every footprint currently generated on the board, exact non-commodity MPNs,
ratings, sources, alternatives, and assembly status.

## Cost snapshot

| Group | Approximate single-board cost |
| --- | ---: |
| ADC + MCU + LDO | US$30.25 |
| Six matched divider arrays | US$18.66 |
| connector-side resistors and filters | US$1.68 |
| reference/supply capacitors | US$2.05 |
| connectors, fuse and test points | US$15–20 |
| bare 4-layer PCB allowance | US$15–40 |
| **estimated assembled parts + PCB** | **US$83–113** |

Shipping, VAT, assembly, spare parts and debug equipment are excluded. Buy at
least two ADCs/MCUs and ten divider arrays if hand assembling.

## Important status distinction

- `Required` means the component belongs on the current electrical architecture.
- `Optional` means the feature can be omitted during initial assembly.
- `WAIT` means **do not order yet**. The bare PCB remains `WAIT` until the board
  release gates in its README close.

The BOM does not convert an unrouted board into a manufacturing release. Exact
footprint and lifecycle information must be refreshed at purchase.
