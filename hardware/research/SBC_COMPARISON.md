# Compute platform comparison

| Candidate | Video/display and codec | Linux/support | Power/complexity | Availability/cost | Decision |
| --- | --- | --- | --- | --- | --- |
| Raspberry Pi CM4, BCM2711 | 2/4-lane CSI and DSI; H.264 1080p30 encode; OpenGL ES 3 | Raspberry Pi documents ADV728x-M; exceptional community | moderate, simple prototype carrier ecosystem | production stated through Jan 2034; roughly €35–90 by RAM/eMMC | **Prototype 1 selected** |
| Raspberry Pi CM5, BCM2712 | two flexible 4-lane CSI/DSI; stronger CPU/GPU; no CM4-style documented H.264 encode assumption | current Pi stack; ADV728x path needs CM5-specific validation | higher peak/thermal; carrier compatible conceptually, not electrically identical | current/available; roughly €55–130 | reject as baseline; benchmark |
| NXP i.MX 8M Plus SoM | dual 4-lane CSI, DSI/LVDS/HDMI; 1080p60 H.264/H.265 encode; 2D/3D GPU | NXP BSP/mainline mix; decoder porting work | industrial carrier/BSP effort; good codec fit | active; SoMs often €90–180 | **production lead** |
| NXP i.MX 8M Mini SoM | CSI/DSI and 1080p60 H.264 encode | mature embedded vendors | lower compute than Plus, still custom BSP | active; midrange | credible fallback |
| Rockchip RK3566/3568 SoM | CSI/DSI/LVDS variants, capable VPU/GPU | vendor kernels vary; exact interlaced/bridge support risky | attractive power/cost but integration variance | many modules, lifecycle varies | reject for Prototype 1 |
| TI AM62x SoM | CSI/DSI variants, industrial focus; multimedia differs by SKU | strong industrial docs/Yocto | good lifecycle, weaker DVR fit without exact codec SKU | industrial cost | retain for low-power study |

Selection is coupled to decoder and panel driver evidence. A CSI connector alone
does not prove the receiver accepts the decoder's data type/timing, and a VPU block
does not prove a maintained V4L2 encoder path.

Sources: [CM4 brief](https://datasheets.raspberrypi.com/cm4/cm4-product-brief.pdf),
[CM5 datasheet](https://pip.raspberrypi.com/categories/944-raspberry-pi-compute-module-5),
[i.MX 8M Plus](https://www.nxp.com/products/i.MX8MPLUS).

