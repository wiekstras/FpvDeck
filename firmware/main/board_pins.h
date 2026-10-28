#pragma once

/*
 * Single source of truth for FpvDeck HAT pin assignments.
 * Source: Waveshare ESP32-P4-WIFI6-Touch-LCD-5 schematic, sheet 1/2,
 * connector J3; cross-checked against the official 40-pin table on 2026-10-26.
 * Pins 21/23 (USB-FS capable) and 25/27 (dedicated USB-HS) are not used.
 */

#define PIN_I2C_SDA       7   /* J3 pin 4,  schematic sheet 1/2: SDA/GPIO7 */
#define PIN_I2C_SCL       8   /* J3 pin 6,  schematic sheet 1/2: SCL/GPIO8 */

#define PIN_DVP_D0       37   /* J3 pin 7,  schematic sheet 1/2: GPIO37 */
#define PIN_VIDEO_SELECT_MCU 2 /* J3 pin 8, schematic sheet 1/2: GPIO2; JP1 option */
#define PIN_DVP_D1       38   /* J3 pin 9,  schematic sheet 1/2: GPIO38 */
#define PIN_DVP_D2        5   /* J3 pin 11, schematic sheet 1/2: GPIO5 */
#define PIN_DVP_D3        3   /* J3 pin 12, schematic sheet 1/2: GPIO3 */
#define PIN_DVP_D4        4   /* J3 pin 14, schematic sheet 1/2: GPIO4 */
#define PIN_DVP_D5       21   /* J3 pin 15, schematic sheet 1/2: GPIO21 */
#define PIN_DVP_D6       28   /* J3 pin 16, schematic sheet 1/2: GPIO28 */
#define PIN_DVP_D7       22   /* J3 pin 17, schematic sheet 1/2: GPIO22 */
#define PIN_DVP_PCLK     29   /* J3 pin 20, schematic sheet 1/2: GPIO29 */
#define PIN_DVP_HSYNC    30   /* J3 pin 22, schematic sheet 1/2: GPIO30 */
#define PIN_DVP_VSYNC    31   /* J3 pin 24, schematic sheet 1/2: GPIO31 */

#define PIN_RX_DATA      34   /* J3 pin 28, schematic sheet 1/2: GPIO34 */
#define PIN_RX_CLK       35   /* J3 pin 30, schematic sheet 1/2: GPIO35 */
#define PIN_RX_LE_A      32   /* J3 pin 31, schematic sheet 1/2: GPIO32 */
#define PIN_RX_LE_B      49   /* J3 pin 32, schematic sheet 1/2: GPIO49 */
#define PIN_ACTIVE_RX    50   /* J3 pin 34, schematic sheet 1/2: actual VIDEO_SELECT */
#define PIN_TVP_RESETB   46   /* J3 pin 35, schematic sheet 1/2: GPIO46 */
#define PIN_BUTTON_UP    51   /* J3 pin 36, schematic sheet 1/2: GPIO51 */
#define PIN_BUTTON_OK    47   /* J3 pin 37, schematic sheet 1/2: GPIO47 */
#define PIN_BUTTON_DOWN  52   /* J3 pin 38, schematic sheet 1/2: GPIO52 */
#define PIN_BUZZER       48   /* J3 pin 39, schematic sheet 1/2: GPIO48 */

/* Host-board net, not a 40-pin HAT signal: sheet 1/2 BAT_ADC -> GPIO20. */
#define PIN_DECK_BATTERY_ADC 20

#define HAT_HEADER_5V_PIN_A   1
#define HAT_HEADER_5V_PIN_B   3
#define HAT_HEADER_3V3_PIN_A  2
#define HAT_HEADER_3V3_PIN_B 18
