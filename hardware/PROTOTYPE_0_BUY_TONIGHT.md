# Prototype 0 — buy tonight

> **PROTOTYPE 0 STATUS: READY TO ORDER**<br>
> Procurement snapshot: **2026-09-01**, delivery country: Netherlands. Re-open
> every product page immediately before checkout because price and stock change.

Already owned:

- ✓ RadioMaster T8L — 2.4 GHz ELRS control donor
- ✓ BetaFPV Air65 analog — real 5.8 GHz video source

Required additional hardware and bring-up tools: **approximately €246.47 including VAT**<br>
Shipping estimate (BerryBase + Kiwi + Rotorama + Informatique): **€23–€35**<br>
Expected total tonight: **approximately €269–€282**<br>
Optional additions: **€3.30**, or **€70.80** if the owned Air65 has neither
a usable battery nor a charger

If a suitable multimeter and fine-wire cutter/stripper are already owned,
subtract **€20.00** from the hardware total. A computer with a microSD writer
and a small flat-blade terminal screwdriver are also required for assembly.

This list buys a functionality prototype, not the final low-latency or mechanical
architecture. The reference chain is:

`Air65 → 5.8 GHz → RC832 Mini → CVBS → UVG-002/V4L2 → Pi 5 → RGB overlay → Touch Display 2`

## Buy tonight

### 1. Compute

**Item:** Raspberry Pi 5, 2 GB<br>
**Exact product:** Raspberry Pi 5 2GB<br>
**Manufacturer:** Raspberry Pi Ltd<br>
**Model / MPN:** SC1642; seller SKU RPI5-2GB<br>
**Quantity:** 1<br>
**Purpose:** Runs capture, Qt/QML compositor, apps and DVR.<br>
**Primary EU seller:** BerryBase, Germany<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/raspberry-pi-5-2gb-ram><br>
**Current price:** €68.90 including German VAT<br>
**Stock status:** 100+ shown; 1–3 working days<br>
**Alternative EU seller:** Kiwi Electronics, Netherlands<br>
**Alternative direct link:** <https://www.kiwi-electronics.com/en/raspberry-pi-boards-363/raspberry-pi-5-2gb-20129><br>
**Alternative status:** exact board, but shown out of stock at this snapshot<br>
**Compatibility:** CONFIRMED with the selected display and USB 2.0 capture.<br>
**Why this exact item:** 2 GB is well above the current ~285 MiB simulator RSS,<br>
while Pi 5 gives useful headroom for software deinterlacing/compositing.<br>
**Required tonight:** YES<br>

### 2. Pi power

**Item:** Official 27 W USB-C supply, EU/white<br>
**Exact product:** Raspberry Pi 27W USB-C Power Supply, white EU<br>
**Manufacturer:** Raspberry Pi Ltd<br>
**Model / MPN:** SC1407<br>
**Quantity:** 1<br>
**Purpose:** Stable 5.1 V/5 A power for Pi, display and capture device.<br>
**Primary EU seller:** Kiwi Electronics, Netherlands<br>
**DIRECT PRODUCT LINK:** <https://www.kiwi-electronics.com/nl/raspberry-pi-27w-usb-c-power-supply-wit-eu-11581><br>
**Current price:** €10.89 excluding NL VAT, approximately €13.18 including VAT<br>
**Stock status:** 250+ shown; same-day dispatch before cutoff<br>
**Alternative EU seller:** BerryBase (seller SKU RPI5NT5AW; confirm EU plug at checkout)<br>
**Alternative direct link:** <https://www.berrybase.de/en/raspberry-pi-27w-usb-c-power-supply-netzteil-weiss><br>
**Compatibility:** CONFIRMED 5.1 V/5 A Pi 5 supply. It does **not** power the VRX.<br>
**Why this exact item:** Avoids Pi undervoltage during USB capture and recording.<br>
**Required tonight:** YES<br>

### 3. Cooling

**Item:** Official Raspberry Pi 5 Active Cooler<br>
**Exact product:** Raspberry Pi Active Cooler<br>
**Manufacturer:** Raspberry Pi Ltd<br>
**Model / MPN:** SC1148<br>
**Quantity:** 1<br>
**Purpose:** Prevents thermal throttling during capture/compositing/DVR.<br>
**Primary EU seller:** BerryBase<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/en/raspberry-pi-active-cooler-luefter-fuer-raspberry-pi-5?c=298><br>
**Current price:** €5.90 including VAT<br>
**Stock status:** 100+ shown; 1–3 working days<br>
**Alternative EU seller:** Kiwi Electronics<br>
**Alternative direct link:** <https://www.kiwi-electronics.com/nl/merk-raspberry-pi/raspberry-pi-active-cooler-11585><br>
**Alternative price/stock:** €4.49 excluding NL VAT; 250+ shown<br>
**Compatibility:** CONFIRMED for Pi 5.<br>
**Why this exact item:** Supported fan control and no enclosure fit uncertainty.<br>
**Required tonight:** YES<br>

### 4. System storage

**Item:** 64 GB microSD card<br>
**Exact product:** SanDisk Ultra microSDXC A1 140 MB/s Class 10 + adapter<br>
**Manufacturer:** Western Digital / SanDisk<br>
**Model / MPN:** SDSQUAB-064G-GN6MA; seller product 619659200541<br>
**Quantity:** 1<br>
**Purpose:** Raspberry Pi OS, FpvDeck and short DVR tests.<br>
**Primary EU seller:** BerryBase<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/sandisk-ultra-microsdhc-a1-140mb-s-class-10-speicherkarte-adapter-64gb><br>
**Current price:** €19.50 including VAT<br>
**Stock status:** 58 shown; 1–3 working days<br>
**Alternative EU seller:** Kiwi Electronics (pre-imaged Raspberry Pi-branded A2 card)<br>
**Alternative direct link:** <https://www.kiwi-electronics.com/nl/64gb-microsd-met-raspberry-pi-os-7377><br>
**Alternative price/stock:** €32.99 excluding NL VAT; 26 shown<br>
**Compatibility:** CONFIRMED boot storage; not claimed as long-term DVR endurance media.<br>
**Why this exact item:** Cheapest credible 64 GB boot volume; replace with High<br>
Endurance media after recording behavior is proven.<br>
**Required tonight:** YES<br>

### 5. Touchscreen

**Item:** 5-inch capacitive DSI touchscreen<br>
**Exact product:** Raspberry Pi Touch Display 2, 5 inch<br>
**Manufacturer:** Raspberry Pi Ltd<br>
**Model / MPN:** SC1975; Kiwi KW-3549<br>
**Quantity:** 1<br>
**Purpose:** Real landscape touch UX at 1280×720 logical orientation.<br>
**Primary EU seller:** Kiwi Electronics, Netherlands<br>
**DIRECT PRODUCT LINK:** <https://www.kiwi-electronics.com/nl/raspberry-pi-touch-display-2-5-inch-20517><br>
**Current price:** €36.49 excluding NL VAT, approximately €44.15 including 21% VAT<br>
**Stock status:** 10 shown; same-day dispatch before cutoff<br>
**Alternative EU seller:** Welectron, Germany<br>
**Alternative direct link:** <https://www.welectron.com/Official-Raspberry-Pi-Touch-Display-2-5-Portrait><br>
**Alternative price/stock:** €43.90 including 19% VAT; 50+ shown<br>
**Compatibility:** CONFIRMED. The box includes the Pi 5 22-to-15-way FFC, GPIO<br>
power lead and mounting screws; DSI carries video and touch.<br>
**Why this exact item:** Cheap for a documented five-point capacitive DSI display,<br>
with no HDMI bridge or separate USB touch cable. It is Prototype 0 hardware, not
the final large outdoor display.<br>
**Required tonight:** YES<br>

### 6. Analog video receiver

**Item:** 5.8 GHz analog FPV receiver kit<br>
**Exact product:** RC832 Mini receiver 4.9–5.8 GHz<br>
**Manufacturer:** UNKNOWN OEM; Rotorama is the identified EU vendor<br>
**Model / MPN:** RC832 Mini, Rotorama product 27309-1<br>
**Quantity:** 1<br>
**Purpose:** Receives Air65 5.8 GHz RF and outputs CVBS.<br>
**Primary EU seller:** Rotorama s.r.o., Czech Republic<br>
**DIRECT PRODUCT LINK:** <https://www.rotorama.com/product/rc832-mini-5ghz><br>
**Current price:** €55.49; checkout determines local VAT<br>
**Stock status:** In Stock; vendor states 2–3 working days within the EU and<br>
€6.49 GLS shipping to Holland<br>
**Alternative EU seller:** Fruugo NL marketplace, RC832S kit<br>
**Alternative direct link:** <https://www.fruugo.nl/fpv-rc832-rc832s-receiver-58g-48ch-600mw-aerial-photography-58ghz-av-receiver-system-with-av-and/p-464528452-976977969?language=en><br>
**Alternative price/stock:** €57.63 + €12.99, in stock; marketplace/longer-chain<br>
fallback only<br>
**Compatibility:** CONFIRMED at signal level: 56 channels include standard A/B/E/F/R,<br>
PAL/NTSC 1.0 Vp-p into 75 Ω, 3.5 mm AV out, 12 V/130 mA, RP-SMA. The Rotorama
kit includes receiver, linear antenna, power cable and video cable.<br>
**Why this exact item:** It is the least-complex documented EU-stock receiver<br>
that arrives with the otherwise easy-to-miss antenna and proprietary AV lead.<br>
**Required tonight:** YES<br>

### 7. Composite USB capture

**Item:** Linux-supported PAL/NTSC composite capture<br>
**Exact product:** Gembird USB Audio/Video Grabber<br>
**Manufacturer:** Gembird Europe B.V.<br>
**Model / MPN:** UVG-002 / EAN 8716309079983<br>
**Quantity:** 1<br>
**Purpose:** Converts receiver CVBS to a V4L2 stream for the Pi.<br>
**Primary EU seller:** Informatique, Netherlands<br>
**DIRECT PRODUCT LINK:** <https://www.informatique.nl/490730/gembird-usb-audio-video-grabber.html><br>
**Current price:** €12.95 including NL VAT; €4.50 shipping shown<br>
**Stock status:** In stock in its warehouse; before-22:00 orders stated to ship today<br>
**Alternative EU seller:** bol.com Netherlands, sold by Kabeldirect.nl<br>
**Alternative direct link:** <https://www.bol.com/nl/nl/p/gembird-uvg-002-usb-videograbber-geschikt-voor-windows/9200000036282031/><br>
**Alternative price/stock:** €19.70 including NL VAT and delivery; in stock,<br>
next-business-day delivery stated at this snapshot<br>
**Compatibility:** CONFIRMED interfaces: yellow RCA composite input, USB-A 2.0,<br>
PAL 720×576/25 and NTSC 720×480/30. **SUPPORTED FAMILY, RECEIPT-GATED UNIT:**
Gembird documents UTV007 or UTVF007 silicon. UTV007 uses Linux's `usbtv` V4L2
driver; deployed UTVF007 units enumerate as UVC `534d:0021` and use `uvcvideo`.
Both routes expose V4L2 on Raspberry Pi, but the received USB ID, driver, format
and weak-signal behavior remain mandatory receipt tests because Gembird does not
guarantee which chipset is in a box. Return the unit if neither documented path
appears—never bind a driver to an unknown ID.<br>
**Why this exact item:** It is a named, returnable Dutch-stock €13 product with a<br>
manufacturer-declared chipset family, not an anonymous capture stick. It meets
Prototype 0's speed/cost goal; capture latency, deinterlacing and UTVF007's known
weak-signal squelch behavior still need measurement.<br>
**Required tonight:** YES<br>

### 8. Receiver power

**Item:** Protected regulated 12 V wall supply<br>
**Exact product:** PCEnergy 12 V DC 1 A plug-in PSU, 5.5×2.1 mm P1J<br>
**Manufacturer:** P+C Schwick / PCEnergy<br>
**Model / MPN:** PCE12-12-1-WM<br>
**Quantity:** 1<br>
**Purpose:** Dedicated supply for the 12 V/130 mA receiver.<br>
**Primary EU seller:** BerryBase<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/en/steckernetzteil-12v-dc-1a-12w-hohlstecker-5-5x2-1mm?c=393><br>
**Current price:** €5.90 including VAT<br>
**Stock status:** 53+ shown; 1–3 working days<br>
**Alternative EU seller:** BerryBase Goobay 64977 exact-output alternative<br>
**Alternative direct link:** <https://www.berrybase.de/schaltnetzteil-12v-dc-1a-1000ma-12w-mit-hohlstecker-5-5-x-2-1-mm><br>
**Alternative price/stock:** €9.50; manufacturer stock, 5–10 days<br>
**Compatibility:** Voltage/current CONFIRMED; exact receiver barrel dimensions<br>
are **NEEDS MEASUREMENT** and the one-page receiver manual omits polarity. Use
item 9 and the receiver's factory red/black power lead. Its wire purpose and
barrel continuity must agree before first power; never trial-fit an unverified
barrel supply into the receiver.<br>
**Why this exact item:** Protected, certified low-voltage supply with ample margin.<br>
**Required tonight:** YES<br>

### 9. Safe power breakout

**Item:** 5.5×2.1 mm female barrel to screw terminals<br>
**Exact product:** Goobay DC coupling, terminal block, 2-pin<br>
**Manufacturer:** Wentronic / Goobay<br>
**Model / MPN:** 76743<br>
**Quantity:** 1<br>
**Purpose:** Exposes the known centre-positive wall supply on marked screw<br>
terminals for the receiver's modified supplied power lead.<br>
**Primary EU seller:** BerryBase<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/en/dc-kupplung-fuer-hohlstecker-5-5x2-1mm-schraubmontage-terminal-block-2-pin><br>
**Current price:** €0.50 including VAT<br>
**Stock status:** 46+ shown; 1–3 working days<br>
**Alternative EU seller:** Kiwi Electronics, Netherlands (`PC-GP2.1-TB`)<br>
**Alternative direct link:** <https://www.kiwi-electronics.com/nl/terminal-block-naar-2-1mm-dc-barrel-jack-female-747><br>
**Alternative price/stock:** €1.29 excluding NL VAT; 24 shown in stock<br>
**Compatibility:** CONFIRMED positive-inside/negative-outside marking; receiver<br>
cable polarity must still be continuity-checked before power.<br>
**Why this exact item:** Makes the unknown receiver jack dimension irrelevant.<br>
**Required tonight:** YES<br>

### 10. Electrical safety meter

**Item:** Digital multimeter with continuity and DC-voltage modes<br>
**Exact product:** UNI-T UT131B palm-size digital multimeter<br>
**Manufacturer:** UNI-Trend Technology<br>
**Model / MPN:** UT131B<br>
**Quantity:** 1<br>
**Purpose:** Maps the supplied RC832 barrel lead and verifies the assembled 12 V<br>
supply polarity before first power.<br>
**Primary EU seller:** BerryBase, Germany<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/en/uni-t-ut131b-digitales-multimeter-palm-size-mit-batteriepruefung><br>
**Current price:** €14.50 including VAT<br>
**Stock status:** Available at the procurement snapshot; recheck at checkout<br>
**Alternative EU seller:** Reichelt, Germany<br>
**Alternative direct link:** <https://www.reichelt.com/nl/nl/shop/product/multimeter_digitaal_2000_counts_palm_size-224311><br>
**Compatibility:** CONFIRMED functions required: continuity and low-voltage DC.<br>
**Why this exact item:** The Mini manual omits polarity; a meter is cheaper than<br>
risking the receiver and remains useful throughout hardware bring-up.<br>
**Required tonight:** YES, unless an equivalent working meter is already owned<br>

### 11. Cable cutter / stripper

**Item:** Small wire cutter and stripper<br>
**Exact product:** PCWork PCW04A 2-in-1 side cutter / wire stripper<br>
**Manufacturer:** P+C Schwick / PCWork<br>
**Model / MPN:** PCW04A / EAN 4260766895340<br>
**Quantity:** 1<br>
**Purpose:** Removes the undocumented red connector from the **supplied RC832<br>
power accessory lead** and strips its two conductors for the terminal block.<br>
**Primary EU seller:** BerryBase, Germany<br>
**DIRECT PRODUCT LINK:** <https://www.berrybase.de/en/pcwork-pcw04a-2in1-side-cutter-wire-stripper><br>
**Current price:** €5.50 including VAT<br>
**Stock status:** Available; 1–3 working days shown at the snapshot<br>
**Alternative EU seller:** Any local electronics/hardware shop<br>
**Alternative direct link:** Not applicable; use an already-owned cutter/stripper<br>
that is suitable for small stranded wire.<br>
**Compatibility:** Seller specifies a 0.8–2.6 mm cable-diameter range. The<br>
received lead is a mandatory fit check; use an already-owned fine-wire stripper
instead if it falls outside the tool's range, and never nick conductor strands.<br>
**Why this exact item:** It shares the BerryBase shipment and avoids buying an<br>
unverified connector. The modification affects only the replaceable receiver
accessory cable—never the receiver or RadioMaster T8L.<br>
**Required tonight:** YES, unless equivalent tools are already owned<br>

## Conditional tool

If the computer used to flash the card has no SD slot, add the Goobay 38656 USB
microSD reader (€3.30, shown available):
<https://www.berrybase.de/en/microsd-card-reader-with-usb-2.0-port-black/>.

## Air65 power pre-check

The Air65 aircraft is already owned, but a bare Air65 does not itself guarantee
that a charged flight battery and charger are on hand. Before checkout, confirm
you have **at least one serviceable BT2.0 1S LiHV pack and a charger explicitly
supporting 4.35 V LiHV**. If not, add:

- **Battery:** BetaFPV LAVA 1S 260 mAh 80C LiHV, BT2.0, five-pack,
  manufacturer order number `BET-LAVA-1S-260MAH-80C`; €24.90 including German
  VAT, more than 10 available at FPV24:
  <https://www.fpv24.com/en/betafpv/betafpv-lipo-akku-lava-bt20-260mah-1s-80c-5-stueck>.
- **Charger:** VIFLY WhoopStor V3, model/order number
  `VFL-WHOOPSTOR-3-WHITE`; €45.90 including German VAT, more than 10 available
  at FPV24:
  <https://www.fpv24.com/en/vifly/vifly-whoopstor-v3-6-port-1s-lihv-lipo-battery-charger-discharger>.
  It accepts BT2.0 and supports 4.35 V LiHV. Its power adapter is **not included**;
  use a suitable existing USB-C PD supply or other input meeting the documented
  9–20 V USB-C / 6–26 V DC input and power requirements.

These are **Required tonight: NO** when your existing Air65 battery/charging
setup is already complete. Never charge LiHV packs in ordinary 4.20 V LiPo mode
unless deliberately accepting an undercharge, and never select LiHV mode for a
non-LiHV pack.

## Checkout split

- **BerryBase:** items 1, 3, 4, 8, 9, 10 and 11; add the conditional card reader
  only if needed.
- **Kiwi Electronics:** items 2 and 5.
- **Rotorama:** item 6; its included antenna, power lead and AV lead are required.
- **Informatique:** item 7.

Do not substitute a generic AV lead, receiver antenna or USB capture device at
checkout. Those substitutions reopen compatibility checks. Recheck stock, price,
plug type and delivery country on every final cart page.

## Intentionally not ordered

- No second ELRS transmitter: the T8L already supplies radio control.
- No premium/final touchscreen, CM4, decoder EVM or custom PCB.
- No diversity VRX yet; first prove the whole application path.
- No balance-check hardware; it stays simulated in Prototype 0.
- No enclosure or integrated T8L power modification.

When these arrive, continue with [Prototype 0 setup](../docs/PROTOTYPE_0_SETUP.md).
