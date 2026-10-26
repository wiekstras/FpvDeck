#include "apps.h"

void app_scanner_show(void)
{
    lv_obj_t *screen = app_screen_create("Spectrum scanner");
    lv_obj_t *chart = lv_chart_create(screen);
    lv_obj_set_size(chart, 1120, 480);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_MID, 0, -55);
    lv_chart_set_type(chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_chart_set_point_count(chart, 24);
    lv_chart_series_t *series = lv_chart_add_series(
        chart, lv_color_hex(0x51D9D0), LV_CHART_AXIS_PRIMARY_Y);
    for (unsigned i = 0; i < 24; ++i) {
        lv_chart_set_next_value(chart, series, 0);
    }
    lv_obj_t *note = lv_label_create(screen);
    lv_label_set_text(note, "No scan yet · RSSI calibration requires receiver hardware");
    lv_obj_align(note, LV_ALIGN_BOTTOM_MID, 0, -18);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
