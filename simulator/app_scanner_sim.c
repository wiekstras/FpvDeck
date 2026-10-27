#include "apps.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"

#include <stdint.h>
#include <string.h>

#define POINT_COUNT (RX5808_BAND_COUNT * 8U)

static scanner_sample_t s_samples[POINT_COUNT];
static scanner_sample_t s_top[3];
static lv_obj_t *s_chart;
static lv_chart_series_t *s_series;
static lv_obj_t *s_status;
static lv_obj_t *s_buttons[3];
static lv_timer_t *s_timer;
static unsigned s_index;

static void open_result(lv_event_t *event)
{
    const unsigned rank = *(const unsigned *)lv_event_get_user_data(event);
    fpvdeck_settings_t settings;
    if (rank >= 3 || settings_load(&settings) != ESP_OK) return;
    for (unsigned band = 0; band < RX5808_BAND_COUNT; ++band) {
        for (unsigned channel = 0; channel < 8; ++channel) {
            if (rx5808_channels_mhz[band][channel] == s_top[rank].frequency_mhz) {
                settings.band = (rx5808_band_t)band;
                settings.channel = (uint8_t)channel;
                (void)settings_save(&settings);
                app_monitor_show();
                return;
            }
        }
    }
}

static void scan_tick(lv_timer_t *timer)
{
    (void)timer;
    if (s_index < POINT_COUNT) {
        float a = 0.0f, b = 0.0f;
        (void)rx5808_set_both(s_samples[s_index].frequency_mhz);
        (void)signal_read_raw(&a, &b);
        s_samples[s_index].rssi = a > b ? a : b;
        lv_chart_set_value_by_id(s_chart, s_series, s_index,
                                 (int32_t)(s_samples[s_index].rssi * 1000.0f));
        ++s_index;
        lv_label_set_text_fmt(s_status, "Scanning %u / %u channels…",
                              s_index, POINT_COUNT);
        return;
    }
    scanner_strongest(s_samples, POINT_COUNT, s_top, 3);
    lv_label_set_text(s_status, "Simulation sweep complete · tap a peak to monitor");
    for (unsigned i = 0; i < 3; ++i) {
        lv_label_set_text_fmt(lv_obj_get_child(s_buttons[i], 0), "%u MHz\n%.3f V",
                              s_top[i].frequency_mhz, s_top[i].rssi);
    }
    lv_timer_pause(s_timer);
}

static void deleted(lv_event_t *event)
{
    (void)event;
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
}

void app_scanner_show(void)
{
    memset(s_samples, 0, sizeof(s_samples));
    for (unsigned b = 0; b < RX5808_BAND_COUNT; ++b) {
        for (unsigned c = 0; c < 8; ++c)
            s_samples[b * 8U + c].frequency_mhz = rx5808_channels_mhz[b][c];
    }
    s_index = 0;
    lv_obj_t *screen = app_screen_create("Spectrum scanner");
    lv_obj_add_event_cb(screen, deleted, LV_EVENT_DELETE, NULL);
    s_chart = lv_chart_create(screen);
    lv_obj_set_size(s_chart, 1120, 350);
    lv_chart_set_type(s_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 2048);
    lv_chart_set_point_count(s_chart, POINT_COUNT);
    s_series = lv_chart_add_series(s_chart, lv_color_hex(0x51D9D0),
                                   LV_CHART_AXIS_PRIMARY_Y);
    s_status = lv_label_create(screen);
    lv_label_set_text(s_status, "Starting simulated receiver sweep…");
    lv_obj_t *results = lv_obj_create(screen);
    lv_obj_set_size(results, 1120, 125);
    lv_obj_set_flex_flow(results, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(results, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    static const unsigned ranks[] = {0, 1, 2};
    for (unsigned i = 0; i < 3; ++i) {
        s_buttons[i] = app_make_button(results, "Scanning…", NULL);
        lv_obj_set_size(s_buttons[i], 310, 84);
        lv_obj_add_event_cb(s_buttons[i], open_result, LV_EVENT_CLICKED,
                            (void *)&ranks[i]);
    }
    s_timer = lv_timer_create(scan_tick, 55, NULL);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}

