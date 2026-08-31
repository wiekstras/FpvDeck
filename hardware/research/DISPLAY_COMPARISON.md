# Display comparison

| Candidate | Key specification | Integration / latency | Availability/cost | Decision |
| --- | --- | --- | --- | --- |
| Waveshare 6.25-inch DSI LCD | 6.25-inch IPS, 720×1560, optical bonding, 5-point touch | documented Pi DSI product; brightness, landscape scan behavior and latency **UNKNOWN** | current catalog module; price recheck at purchase | **larger Prototype candidate** |
| Focus LCDs E55RB-I-MW400-C | 5.5-inch IPS, 1080×1920, 4-lane DSI, 400 cd/m², HX8399-A, GT911 touch | raw panel integration and driver work; likely insufficient outdoor luminance | recommended for new designs, listed US$128.81 | reject as baseline; size/fit reference |
| Winstar WF50DSYA3MNN0 | 5.0-inch IPS, 720×1280, 4-lane DSI, 1100 cd/m², -20–70 °C, no touch | raw ILI9881C-class panel; driver/init/backlight carrier required; latency unknown | manufacturer catalog/sample inquiry, planning €35–70 | **bright fallback; smaller than desired** |
| Raspberry Pi Touch Display 2 5-inch | 5.0-inch, 720×1280, 24-bit, five-touch, DSI | first-party Pi overlay/support; brightness/latency not published in brief | current, about US$40 | **low-risk bench fallback** |
| Waveshare 5-inch DSI LCD (D) | 5.0-inch IPS, 720×1280, optical bond, touch, 60 Hz | documented Pi support; brightness/latency unspecified | broadly sold, about €55–75 | Prototype alternative |
| Crystalfontz CFAF7201280A0-050TN | 5.0-inch IPS, 720×1280, 4-lane DSI, 300 cd/m² | raw panel and adapter options; insufficient outdoor brightness | current, about US$35–55 | reject for final; lab fallback |
| Waveshare 5-inch HDMI 720×1280 | IPS, 350 cd/m², HDMI controller | extra bridge/controller and unknown buffering | readily available | reject for latency/brightness |

**Revised product direction:** target a 5.5–6.25-inch-class display mounted above
the gimbal finger envelopes. No larger production panel is selected yet. The
6.25-inch Waveshare module is a mechanical/software prototype candidate, not an
outdoor selection, until brightness, scan direction, power and latency are measured.

The 5-inch Winstar panel's 1100-nit peak does not by itself guarantee sunlight
readability. Cover-glass reflectance, optical bonding, polarizer orientation,
backlight thermal derating, power, and landscape viewing must be tested. Touch is
optional and cannot be required for flight use.

Sources: [Winstar panel](https://www.winstar.com.tw/products/tft-lcd/ips-tft/vertical-display.html),
[Winstar specification](https://www.winstar.com.tw/uploads/files/d4caedc6f9470d719c79eafcd5d14224.pdf),
[Raspberry Pi Touch Display 2](https://www.raspberrypi.com/documentation/accessories/touch-display-2.html),
[Waveshare DSI D](https://www.waveshare.com/wiki/5inch_DSI_LCD_%28D%29),
[Waveshare 6.25-inch DSI](https://www.waveshare.com/6.25inch-dsi-lcd.htm),
[Focus LCDs 5.5-inch](https://focuslcds.com/product/e55rb-i-mw400-c/),
[Crystalfontz](https://www.crystalfontz.com/product/cfaf7201280a0050tn-720x1280-mipi-dsi-tft-display).
