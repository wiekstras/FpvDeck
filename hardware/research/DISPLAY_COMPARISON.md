# Touch display comparison

Research snapshot: 2026-09-01. Touch is a **hard product requirement**. Prices are
single-unit planning prices and must be refreshed before purchase. No vendor in
this table publishes end-to-end display latency, so latency statements are
architectural assessments rather than measurements.

## Decision

**Prototype 1 selection: Waveshare 6.25inch DSI LCD (B), SKU 35000.** This is the
current Type B product, not the discontinued SKU 28918 with the almost identical
name. It is intentionally slightly larger than the earlier 4–6 inch target: in
landscape its 159.18 mm cover lens provides a materially larger FPV image while
remaining plausible above the gimbals for a pinch-grip transmitter layout.

This selection does not freeze the production panel. Prototype exit gates are
measured scanout/response latency, landscape scan direction, touch latency,
sunlight usability, backlight power, EMI, thermals, and a physical pinch-grip
mock-up. Critical flight actions remain available on physical controls.

## Comparison

| Candidate | Display / mechanics | Touch / Linux | Interface, support and latency concerns | Availability / price | Decision |
| --- | --- | --- | --- | --- | --- |
| **Waveshare 6.25inch DSI LCD (B), SKU 35000** | 6.25-inch IPS; 720×1560 portrait native; 178°; 500 cd/m²; up to 60 Hz; optical-bonded 1.1 mm, 6H cover; cover 74.70×159.18 mm; active area 66.81×144.75 mm | 5-point projected capacitive; I²C touch in the DSI connector; exact controller IC **UNKNOWN**; current Raspberry Pi OS is advertised driver-free | Native Pi DSI; documented CM4 IO Board connection; no HDMI scaler. Landscape rotation and panel scan direction need measurement. Panel response/buffering latency **UNKNOWN** | Current Waveshare catalog item, US$59.99 list / US$58.79 qty 1–9 | **SELECTED for Prototype 1**: largest credible low-risk Pi DSI touch module with adequate brightness and official CAD resources |
| Raspberry Pi Touch Display 2, 5-inch, SC1631 | 5-inch; 720×1280; 24-bit; active area 62.1×110.4 mm; outline about 91.5×143.5×16 mm; anti-glare | Five-finger capacitive; first-party Raspberry Pi OS integration; touch and display over DSI | Native DSI and very low software-integration risk. Brightness, response time and measured latency are not published in the product brief | Current, US$40 list; production stated through at least Jan 2030 | **BENCH FALLBACK**: excellent support, but visibly smaller than requested |
| Waveshare 5inch DSI LCD (D) | 5-inch IPS; 720×1280; 178°; optical bonding; published brightness not found | 5-point capacitive, I²C, documented Pi support | Native DSI, up to 60 Hz; no HDMI bridge. Same unanswered scan/response latency questions | Current module, commonly stocked around US$50–70 | Rejected as baseline: smaller with no compensating brightness/support advantage |
| Focus LCDs E55RB-I-MW400-C | 5.5-inch IPS; 1080×1920; 400 cd/m²; 79.04×137.67 mm; -20…70 °C; HX8399-C | Capacitive GT911 over I²C; upstream Linux has a Goodix-family driver, but complete panel init/DRM integration is project work | Raw 4-lane MIPI DSI panel, 3.3 V logic and separate 14-LED backlight design. More driver, carrier and power risk; latency not specified | Recommended for new designs; US$128.81 list, limited visible unit stock | Rejected for Prototype 1: smaller/dimmer and much higher integration risk |
| Winstar WF50DSYA3MNN0 | 5-inch IPS; 720×1280; 1100 cd/m²; -20…70 °C | **No touch in selected configuration** | Raw 4-lane DSI/ILI9881-class integration; custom init, backlight and touch stack required | Sample/inquiry procurement | Rejected: violates touchscreen requirement despite excellent nominal brightness |
| Waveshare 7inch DSI LCD (E) | 7-inch IPS; 1280×800; 178°; optical bonding | 5-point capacitive I²C; Pi-oriented support | Native DSI, but substantially larger and higher pixel load. Enclosure/gimbal clearance and grip reach are higher risk | Current catalog module | Rejected for donor baseline: likely compromises pinch-grip reach and mass; reconsider after an ergonomics buck |
| Waveshare 6.25inch HDMI LCD | 6.25-inch IPS; 720×1560; optical-bonded touch | USB touch | HDMI bridge/scaler adds unquantified buffering and cable/connector bulk | Current | Rejected: DSI Type B offers the format without the HDMI conversion path |

## Compute/display compatibility

The touchscreen requirement does **not** justify changing Prototype 1 away from
Raspberry Pi CM4. The selected display has a documented CM4 IO Board hookup, and
CM4 retains the project's strongest documented combination of:

- Pi DSI display and I²C touch support;
- a documented ADV728x-M CSI-2 decoder path;
- hardware H.264 encode useful for pre-overlay DVR;
- eMMC variants and a mature carrier ecosystem.

CM5 offers more CPU/GPU performance, but its codec path is not a sufficient reason
to abandon CM4 for this latency/DVR prototype. Production SoM selection remains
open until decoder, compositor, encoder and display are measured as one pipeline.

## Pinch-grip mechanical constraints

The screen must sit above, rather than between, the gimbal finger envelopes. The
landscape cover width is 159.18 mm and active width is 144.75 mm. These are
**CONFIRMED from the vendor drawing**; they are not an enclosure width. Before an
enclosure can be frozen, build a foam or printed buck and measure:

1. thumb and forefinger envelopes at full gimbal travel;
2. clearance from knuckles to the lower display bezel;
3. display viewing angle while neck-strapped and hand-held;
4. reach to shoulder switches and flight-safe buttons;
5. display/PCB thickness from the official STEP model;
6. antenna clearance and center-of-mass shift.

An initial ergonomic gate is no contact with the display housing over the full
stick range for both thumb and pinch users. Exact T8L locations remain **NEEDS
MEASUREMENT** and are tracked in the mechanical checklist.

## Purchase and validation notes

- Buy exact part **6.25inch DSI LCD (B), SKU 35000**; do not substitute SKU 28918.
- Include the correct CM4-compatible DSI cable and vendor-specified 5 V lead.
- Do not route DSI on Test PCB Rev A; connect directly to the CM4 IO Board.
- Measure touch-to-photon and CVBS-to-photon latency with DVR off/on.
- Verify multi-touch, edge accuracy, glove behavior, brightness control, boot
  rotation and resume after display sleep.

## Primary sources

- [Waveshare 6.25inch DSI LCD (B), SKU 35000](https://www.waveshare.com/6.25inch-dsi-lcd-b.htm)
- [Waveshare Type B documentation and CAD](https://docs.waveshare.com/6.25inch_DSI_LCD_B/Resources-And-Documents)
- [Waveshare Type B dimension drawing](https://files.waveshare.com/wiki/6.25inch_DSI_LCD_B/6.25inch_DSI_LCD_(B).pdf)
- [Discontinued Waveshare SKU 28918 notice](https://www.waveshare.com/6.25inch-dsi-lcd.htm)
- [Raspberry Pi Touch Display 2 documentation](https://www.raspberrypi.com/documentation/accessories/touch-display-2.html)
- [Focus LCDs E55RB-I-MW400-C](https://focuslcds.com/product/e55rb-i-mw400-c/)
- [Winstar WF50DSYA3MNN0](https://www.winstar.com.tw/products/tft-lcd/ips-tft/vertical-display.html)
