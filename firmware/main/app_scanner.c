#include "apps.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <stdint.h>

#define SCAN_POINT_COUNT (RX5808_BAND_COUNT * 8U)
#define SCAN_SETTLE_MS 80U

typedef struct {
    scanner_sample_t sample;
    uint8_t band;
    uint8_t channel;
} scan_point_t;

static scan_point_t s_points[SCAN_POINT_COUNT];
static scanner_sample_t s_top[3];
static lv_obj_t *s_chart;
static lv_chart_series_t *s_series;
static lv_obj_t *s_status;
static lv_obj_t *s_top_buttons[3];
static lv_timer_t *s_timer;
static uint16_t s_return_frequency;
static uint32_t s_generation;
static unsigned s_completed;
static bool s_complete;
static bool s_error;
static portMUX_TYPE s_scan_lock = portMUX_INITIALIZER_UNLOCKED;

static bool generation_matches(uint32_t generation)
{
    taskENTER_CRITICAL(&s_scan_lock);
    const bool matches = generation == s_generation;
    taskEXIT_CRITICAL(&s_scan_lock);
    return matches;
}

static void scan_task(void *argument)
{
    const uint32_t generation = (uint32_t)(uintptr_t)argument;
    for (unsigned i = 0; i < SCAN_POINT_COUNT; ++i) {
        if (!generation_matches(generation)) break;
        float a = 0.0f;
        float b = 0.0f;
        if (rx5808_set_both(s_points[i].sample.frequency_mhz) != ESP_OK) {
            taskENTER_CRITICAL(&s_scan_lock);
            s_error = true;
            s_complete = true;
            taskEXIT_CRITICAL(&s_scan_lock);
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(SCAN_SETTLE_MS));
        if (!generation_matches(generation)) break;
        if (signal_read_raw(&a, &b) != ESP_OK) {
            taskENTER_CRITICAL(&s_scan_lock);
            s_error = true;
            s_complete = true;
            taskEXIT_CRITICAL(&s_scan_lock);
            break;
        }
        taskENTER_CRITICAL(&s_scan_lock);
        s_points[i].sample.rssi = a > b ? a : b;
        s_completed = i + 1U;
        taskEXIT_CRITICAL(&s_scan_lock);
    }

    if (generation_matches(generation)) {
        scanner_sample_t samples[SCAN_POINT_COUNT];
        scanner_sample_t strongest[3];
        taskENTER_CRITICAL(&s_scan_lock);
        for (unsigned i = 0; i < SCAN_POINT_COUNT; ++i) {
            samples[i] = s_points[i].sample;
        }
        const bool failed = s_error;
        taskEXIT_CRITICAL(&s_scan_lock);
        if (!failed) {
            scanner_strongest(samples, SCAN_POINT_COUNT, strongest, 3);
            taskENTER_CRITICAL(&s_scan_lock);
            for (unsigned i = 0; i < 3; ++i) s_top[i] = strongest[i];
            s_complete = true;
            taskEXIT_CRITICAL(&s_scan_lock);
        }
        (void)rx5808_set_both(s_return_frequency);
    }
    vTaskDelete(NULL);
}

static void open_result(lv_event_t *event)
{
    const unsigned rank = *(const unsigned *)lv_event_get_user_data(event);
    if (rank >= 3) return;
    taskENTER_CRITICAL(&s_scan_lock);
    const scanner_sample_t selected = s_top[rank];
    taskEXIT_CRITICAL(&s_scan_lock);
    if (selected.rssi < 0.0f) return;
    fpvdeck_settings_t settings;
    if (settings_load(&settings) != ESP_OK) return;
    for (unsigned band = 0; band < RX5808_BAND_COUNT; ++band) {
        for (unsigned channel = 0; channel < 8; ++channel) {
            if (rx5808_channels_mhz[band][channel] == selected.frequency_mhz) {
                settings.band = (rx5808_band_t)band;
                settings.channel = (uint8_t)channel;
                (void)settings_save(&settings);
                app_monitor_show();
                return;
            }
        }
    }
}

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    scan_point_t points[SCAN_POINT_COUNT];
    scanner_sample_t top[3];
    unsigned completed;
    bool complete;
    bool error;
    taskENTER_CRITICAL(&s_scan_lock);
    for (unsigned i = 0; i < SCAN_POINT_COUNT; ++i) points[i] = s_points[i];
    for (unsigned i = 0; i < 3; ++i) top[i] = s_top[i];
    completed = s_completed;
    complete = s_complete;
    error = s_error;
    taskEXIT_CRITICAL(&s_scan_lock);

    for (unsigned i = 0; i < SCAN_POINT_COUNT; ++i) {
        const float value = points[i].sample.rssi;
        int32_t millivolts = value > 0.0f ? (int32_t)(value * 1000.0f) : 0;
        if (millivolts > 2048) millivolts = 2048;
        lv_chart_set_value_by_id(s_chart, s_series, i, millivolts);
    }
    if (error) {
        lv_label_set_text(s_status,
            "RSSI ADC unavailable | connect and verify ADS1115 at 0x4A");
    } else if (!complete) {
        lv_label_set_text_fmt(s_status, "Scanning %u / %u channels…",
                              completed, SCAN_POINT_COUNT);
    } else {
        lv_label_set_text(s_status,
            "Sweep complete | values are raw RSSI volts until HIL calibration");
        for (unsigned i = 0; i < 3; ++i) {
            lv_obj_t *label = lv_obj_get_child(s_top_buttons[i], 0);
            lv_label_set_text_fmt(label, "%u MHz\n%.3f V",
                                  top[i].frequency_mhz, top[i].rssi);
        }
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    taskENTER_CRITICAL(&s_scan_lock);
    ++s_generation;
    taskEXIT_CRITICAL(&s_scan_lock);
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_chart = NULL;
    s_series = NULL;
    s_status = NULL;
}

void app_scanner_show(void)
{
    fpvdeck_settings_t settings;
    if (settings_load(&settings) != ESP_OK) {
        settings.band = RX5808_BAND_R;
        settings.channel = 0;
    }
    (void)rx5808_channel_frequency(settings.band, settings.channel,
                                    &s_return_frequency);

    taskENTER_CRITICAL(&s_scan_lock);
    ++s_generation;
    const uint32_t generation = s_generation;
    s_completed = 0;
    s_complete = false;
    s_error = false;
    for (unsigned band = 0; band < RX5808_BAND_COUNT; ++band) {
        for (unsigned channel = 0; channel < 8; ++channel) {
            const unsigned index = band * 8U + channel;
            s_points[index] = (scan_point_t){
                .sample = {rx5808_channels_mhz[band][channel], 0.0f},
                .band = (uint8_t)band,
                .channel = (uint8_t)channel,
            };
        }
    }
    for (unsigned i = 0; i < 3; ++i) s_top[i] = (scanner_sample_t){0, -1.0f};
    taskEXIT_CRITICAL(&s_scan_lock);

    lv_obj_t *screen = app_screen_create("Spectrum scanner");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    s_chart = lv_chart_create(screen);
    lv_obj_set_size(s_chart, 1120, 350);
    lv_obj_set_style_bg_color(s_chart, lv_color_hex(0x0D1B24), 0);
    lv_obj_set_style_border_color(s_chart, lv_color_hex(0x214252), 0);
    lv_chart_set_type(s_chart, LV_CHART_TYPE_BAR);
    lv_chart_set_range(s_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 2048);
    lv_chart_set_point_count(s_chart, SCAN_POINT_COUNT);
    s_series = lv_chart_add_series(s_chart, lv_color_hex(0x51D9D0),
                                   LV_CHART_AXIS_PRIMARY_Y);
    for (unsigned i = 0; i < SCAN_POINT_COUNT; ++i) {
        lv_chart_set_next_value(s_chart, s_series, 0);
    }
    s_status = lv_label_create(screen);
    lv_label_set_text(s_status, "Starting receiver sweep…");
    lv_obj_set_style_text_color(s_status, lv_color_hex(0xEAF6F7), 0);

    lv_obj_t *results = lv_obj_create(screen);
    lv_obj_set_size(results, 1120, 125);
    lv_obj_set_flex_flow(results, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(results, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(results, lv_color_hex(0x0D1B24), 0);
    lv_obj_set_style_border_color(results, lv_color_hex(0x214252), 0);
    static const unsigned ranks[3] = {0, 1, 2};
    for (unsigned i = 0; i < 3; ++i) {
        s_top_buttons[i] = app_make_button(results, "Scanning…", NULL);
        lv_obj_set_size(s_top_buttons[i], 310, 84);
        lv_obj_add_event_cb(s_top_buttons[i], open_result, LV_EVENT_CLICKED,
                            (void *)&ranks[i]);
    }
    s_timer = lv_timer_create(refresh, 100, NULL);
    refresh(s_timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
    if (xTaskCreate(scan_task, "rx_sweep", 4096,
                    (void *)(uintptr_t)generation, 3, NULL) != pdPASS) {
        taskENTER_CRITICAL(&s_scan_lock);
        s_error = true;
        s_complete = true;
        taskEXIT_CRITICAL(&s_scan_lock);
    }
}
