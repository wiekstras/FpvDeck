# Prototype 0 wiring

These instructions apply only to the exact [Prototype 0 reference
hardware](../PROTOTYPE_0_BUY_TONIGHT.md). Disconnect all power before changing
FFC, GPIO or receiver wiring.

## 1. Pi 5 → Touch Display 2

| From | To | Cable |
|---|---|---|
| Pi 5 either `CAM/DISP` connector | Touch Display 2 DSI connector | supplied 22-way-to-15-way FFC |
| Pi GPIO physical pin 2, +5 V | Display `J1` through red conductor | supplied three-wire GPIO power lead |
| Pi GPIO physical pin 6, ground | Display `J1` through black conductor | supplied three-wire GPIO power lead |
| Official 27 W supply USB-C | Pi 5 USB-C power input | captive PSU cable |

For the FFC, the wide 15-way end enters the display with contacts facing away
from the display. The small 22-way end enters the Pi with contacts facing the
Ethernet/USB-A connectors. Follow Raspberry Pi's illustrated instructions; an
incorrect GPIO power connection can damage the display.

## 2. Analog RF and CVBS

| From | To | Cable |
|---|---|---|
| supplied linear RP-SMA antenna | RC832 Mini `ANT` RP-SMA connector | direct; finger-tight only |
| RC832 Mini `AV OUT` 3.5 mm | yellow RCA composite input on Gembird UVG-002 | **supplied RC832 video cable** |
| Gembird UVG-002 USB-A | Pi 5 USB-A 2.0 port | captive USB plug |

The RC832 3.5 mm conductor assignment is not published in the one-page manual.
That is why the selected receiver's supplied cable is mandatory. Do not replace
it with a generic TRRS-to-RCA lead. Connect the antenna before applying receiver
power.

## 3. Receiver power

| From | To | Value |
|---|---|---|
| PCE12-12-1-WM barrel plug | Goobay 76743 female barrel adapter | 12 V DC, max 1 A |
| Adapter `+` terminal | factory-red conductor of the modified supplied receiver lead, after continuity cross-check | +12 V |
| Adapter `−` terminal | factory-black conductor of the modified supplied receiver lead, after continuity cross-check | 0 V |
| Barrel plug on supplied receiver lead | RC832 `DC12V` | receiver-supplied mating plug |

Before connecting the receiver:

1. Leave the wall supply unplugged from mains.
2. On the **loose receiver accessory power lead only**, cut off the undocumented
   red two-pole connector, leaving the maximum useful wire length. Do not cut or
   open the receiver and do not modify the T8L.
3. Strip approximately 6 mm from each conductor. With the lead disconnected from
   everything, use continuity mode to map each exposed conductor to the barrel
   plug centre or sleeve. Record the wire colour and map. The vendor's product
   image shows the supplied factory lead as red/black, but the receiver manual
   does not state jack polarity. Proceed only if red maps to barrel centre and
   black maps to sleeve. If the lead is unmarked, uses other colours, or gives a
   different map, stop and obtain written polarity confirmation from Rotorama.
4. Plug only the wall supply into the 76743 and measure its terminals: `+` to `−`
   must read approximately +12 V DC.
5. Unplug the wall supply. Land the verified factory-red/centre conductor in the
   adapter `+` terminal and the factory-black/sleeve conductor in `−`.
6. Inspect for loose strands and shorts. Reconnect the wall supply and measure
   approximately +12 V centre-to-sleeve at the receiver lead's barrel plug before
   inserting it. Unplug immediately if polarity or voltage is wrong.
7. Unplug mains again. Connect the antenna and AV lead, then insert the receiver's
   supplied barrel plug into `DC12V`.
8. Apply mains power and confirm the receiver display starts normally.

**Never** power the receiver from Pi GPIO. Wire colour is accepted only when the
factory red/black purpose and the continuity map agree; never infer polarity from
colour alone. The Air65 and external RC832 supply do not connect electrically.

## 4. What does not connect

- The T8L has no video cable to FpvDeck. Its link to Air65 is 2.4 GHz ELRS RF.
- The Air65 has no physical cable to the receiver during normal testing.
- Do not connect the T8L 18650 pack to Pi or VRX in Prototype 0.
- Do not connect an external LiPo balance lead; Battery Checker remains simulated.
