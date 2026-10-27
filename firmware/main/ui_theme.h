#pragma once

#include "lvgl.h"

#include <stdbool.h>

#define UI_COLOR_BG          0x061014
#define UI_COLOR_SURFACE     0x0B1A20
#define UI_COLOR_SURFACE_HI  0x11262E
#define UI_COLOR_BORDER      0x1D3942
#define UI_COLOR_TEXT        0xF2F7F7
#define UI_COLOR_MUTED       0x89A3AA
#define UI_COLOR_ACCENT      0x45E0C1
#define UI_COLOR_BLUE        0x45A9FF
#define UI_COLOR_SUCCESS     0x5BD68A
#define UI_COLOR_WARNING     0xFFB45C
#define UI_COLOR_DANGER      0xFF6378

/*
 * The production display is 1280x720 landscape. These helpers deliberately
 * use flat fills, thin borders and static object trees: all are cheap for the
 * ESP32-P4/LVGL renderer and avoid software shadows, blur and gradients.
 */
void ui_screen_style(lv_obj_t *screen);
void ui_surface_style(lv_obj_t *object);
void ui_button_style(lv_obj_t *button, bool primary);
void ui_label_title(lv_obj_t *label);
void ui_label_overline(lv_obj_t *label);
void ui_label_muted(lv_obj_t *label);
void ui_bar_style(lv_obj_t *bar, uint32_t indicator_color);

lv_obj_t *ui_chip_create(lv_obj_t *parent, const char *text,
                         uint32_t accent_color);
lv_obj_t *ui_metric_create(lv_obj_t *parent, const char *caption,
                           lv_obj_t **value_label);
