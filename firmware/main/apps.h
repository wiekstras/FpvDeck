#pragma once

#include "lvgl.h"

void app_launcher_show(void);
void app_battery_show(void);
void app_monitor_show(void);
void app_scanner_show(void);
void app_finder_show(void);
void app_stream_show(void);
void app_settings_show(void);
void app_set_deck_battery_warning(float voltage);
void app_shutdown_show(float voltage);

lv_obj_t *app_screen_create(const char *title);
lv_obj_t *app_make_button(lv_obj_t *parent, const char *text,
                          lv_event_cb_t callback);
