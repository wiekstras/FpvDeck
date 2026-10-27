#include "apps.h"
#include "deck_power.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"

#include <stdint.h>
#ifdef FPVDECK_SIMULATOR
#include "sim_video.h"
#endif

static const char s_band_names[RX5808_BAND_COUNT] = {'A', 'B', 'E', 'F', 'R', 'L'};
static lv_obj_t *s_screen;
static lv_obj_t *s_overlay;
static lv_obj_t *s_band_picker;
static lv_timer_t *s_timer;
static fpvdeck_settings_t s_settings;

static void tune(void)
{
    uint16_t frequency = 0;
    if (rx5808_channel_frequency(s_settings.band, s_settings.channel,
                                 &frequency)) {
        (void)rx5808_set_both(frequency);
        (void)settings_save(&s_settings);
    }
}

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    float a = 0.0f, b = 0.0f, peak = 0.0f, deck = 0.0f;
    bool active_a = false;
    uint16_t frequency = 0;
    (void)rx5808_channel_frequency(s_settings.band, s_settings.channel,
                                    &frequency);
    const bool rssi_ok = signal_latest(&a, &b, &peak, &active_a);
    const bool deck_ok = deck_power_latest(&deck);
    (void)peak;
    if (rssi_ok && deck_ok) {
        lv_label_set_text_fmt(s_overlay,
            "%c%u  %u MHz   RSSI A %.3f V  B %.3f V   RX %c   DECK %.2f V",
            s_band_names[s_settings.band], s_settings.channel + 1U, frequency,
            a, b, active_a ? 'A' : 'B', deck);
    } else if (rssi_ok) {
        lv_label_set_text_fmt(s_overlay,
            "%c%u  %u MHz   RSSI A %.3f V  B %.3f V   RX %c   DECK —",
            s_band_names[s_settings.band], s_settings.channel + 1U, frequency,
            a, b, active_a ? 'A' : 'B');
    } else {
        lv_label_set_text_fmt(s_overlay,
            "%c%u  %u MHz   RSSI A —  B —   RX —   DECK —",
            s_band_names[s_settings.band], s_settings.channel + 1U, frequency);
    }
}

static void change_channel(int delta)
{
    s_settings.channel = (uint8_t)((s_settings.channel + 8 + delta) % 8);
    tune();
    refresh(s_timer);
}

static void previous_channel(lv_event_t *event)
{
    (void)event;
    change_channel(-1);
}

static void next_channel(lv_event_t *event)
{
    (void)event;
    change_channel(1);
}

static void monitor_key(lv_event_t *event)
{
    const uint32_t key = *(const uint32_t *)lv_event_get_param(event);
    if (key == LV_KEY_PREV) change_channel(-1);
    if (key == LV_KEY_NEXT) change_channel(1);
}

static void restore_monitor_focus(void)
{
    lv_group_t *group = lv_group_get_default();
    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, s_screen);
    lv_group_focus_obj(s_screen);
}

static void select_band(lv_event_t *event)
{
    s_settings.band = (rx5808_band_t)(uintptr_t)lv_event_get_user_data(event);
    tune();
    lv_obj_delete(s_band_picker);
    s_band_picker = NULL;
    restore_monitor_focus();
    refresh(s_timer);
}

static void open_band_picker(lv_event_t *event)
{
    (void)event;
    lv_indev_t *indev = lv_indev_active();
    if (indev != NULL && lv_indev_get_key(indev) != LV_KEY_ENTER) return;
    if (s_band_picker != NULL) return;

    lv_group_remove_obj(s_screen);
    s_band_picker = lv_obj_create(s_screen);
    lv_obj_set_size(s_band_picker, 760, 430);
    lv_obj_center(s_band_picker);
    lv_obj_set_style_bg_color(s_band_picker, lv_color_hex(0x0D1B24), 0);
    lv_obj_set_style_radius(s_band_picker, 22, 0);
    lv_obj_set_flex_flow(s_band_picker, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(s_band_picker, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
    for (unsigned band = 0; band < RX5808_BAND_COUNT; ++band) {
        char name[16];
        lv_snprintf(name, sizeof(name), "Band %c", s_band_names[band]);
        lv_obj_t *button = app_make_button(s_band_picker, name, NULL);
        lv_obj_set_size(button, 210, 90);
        lv_obj_add_event_cb(button, select_band, LV_EVENT_CLICKED,
                            (void *)(uintptr_t)band);
    }
    lv_group_focus_next(lv_group_get_default());
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_overlay = NULL;
    s_band_picker = NULL;
    s_screen = NULL;
}

void app_monitor_show(void)
{
    if (settings_load(&s_settings) != ESP_OK) {
        s_settings.band = RX5808_BAND_R;
        s_settings.channel = 0;
    }
    s_screen = app_screen_create("FPV monitor");
    lv_obj_add_event_cb(s_screen, screen_deleted, LV_EVENT_DELETE, NULL);
    lv_obj_add_event_cb(s_screen, monitor_key, LV_EVENT_KEY, NULL);
    lv_obj_add_event_cb(s_screen, open_band_picker, LV_EVENT_LONG_PRESSED, NULL);
    lv_obj_t *video = lv_obj_create(s_screen);
    lv_obj_set_width(video, lv_pct(100));
    lv_obj_set_flex_grow(video, 1);
    lv_obj_set_style_bg_color(video, lv_color_hex(0x101A1D), 0);
    lv_obj_set_style_border_width(video, 0, 0);
#ifdef FPVDECK_SIMULATOR
    sim_video_populate(video);
#else
    lv_obj_t *hint = lv_label_create(video);
    lv_label_set_text(hint, "VIDEO INPUT\nwaiting for TVP5150 lock");
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(hint);
#endif

    lv_obj_t *previous = lv_button_create(video);
    lv_obj_set_size(previous, 80, 80);
    lv_obj_align(previous, LV_ALIGN_LEFT_MID, 18, 0);
    lv_obj_add_event_cb(previous, previous_channel, LV_EVENT_CLICKED, NULL);
    lv_obj_t *previous_label = lv_label_create(previous);
    lv_label_set_text(previous_label, LV_SYMBOL_LEFT);
    lv_obj_center(previous_label);
    lv_obj_t *next = lv_button_create(video);
    lv_obj_set_size(next, 80, 80);
    lv_obj_align(next, LV_ALIGN_RIGHT_MID, -18, 0);
    lv_obj_add_event_cb(next, next_channel, LV_EVENT_CLICKED, NULL);
    lv_obj_t *next_label = lv_label_create(next);
    lv_label_set_text(next_label, LV_SYMBOL_RIGHT);
    lv_obj_center(next_label);

    s_overlay = lv_label_create(video);
    lv_obj_set_style_bg_color(s_overlay, lv_color_hex(0x071016), 0);
    lv_obj_set_style_bg_opa(s_overlay, LV_OPA_80, 0);
    lv_obj_set_style_pad_all(s_overlay, 14, 0);
    lv_obj_align(s_overlay, LV_ALIGN_BOTTOM_MID, 0, -20);
    s_timer = lv_timer_create(refresh, 250, NULL);
    tune();
    refresh(s_timer);
    restore_monitor_focus();
    lv_screen_load_anim(s_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
