#include "apps.h"
#include "buzzer.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"

static const char s_band_names[RX5808_BAND_COUNT] = {'A', 'B', 'E', 'F', 'R', 'L'};
static lv_obj_t *s_rssi;
static lv_obj_t *s_lock_label;
static lv_obj_t *s_previous;
static lv_obj_t *s_next;
static lv_timer_t *s_timer;
static fpvdeck_settings_t s_settings;
static float s_local_peak;
static unsigned s_beep_elapsed_ms;
static bool s_tone_on;
static bool s_locked;

static void update_channel_controls(void)
{
    if (s_lock_label == NULL || s_previous == NULL || s_next == NULL) return;
    uint16_t frequency = 0;
    (void)rx5808_channel_frequency(s_settings.band, s_settings.channel,
                                    &frequency);
    lv_label_set_text_fmt(s_lock_label, "%s  %c%u | %u MHz",
                          s_locked ? "UNLOCK" : "LOCK CHANNEL",
                          s_band_names[s_settings.band],
                          s_settings.channel + 1U, frequency);
    if (s_locked) {
        lv_obj_add_state(s_previous, LV_STATE_DISABLED);
        lv_obj_add_state(s_next, LV_STATE_DISABLED);
    } else {
        lv_obj_remove_state(s_previous, LV_STATE_DISABLED);
        lv_obj_remove_state(s_next, LV_STATE_DISABLED);
    }
}

static void tune_selected(void)
{
    uint16_t frequency = 0;
    if (rx5808_channel_frequency(s_settings.band, s_settings.channel,
                                 &frequency)) {
        (void)rx5808_set_both(frequency);
    }
    s_local_peak = 0.0f;
    update_channel_controls();
}

static void change_channel(int delta)
{
    if (s_locked) return;
    s_settings.channel = (uint8_t)((s_settings.channel + 8 + delta) % 8);
    tune_selected();
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

static void toggle_lock(lv_event_t *event)
{
    (void)event;
    s_locked = !s_locked;
    if (s_locked) (void)settings_save(&s_settings);
    s_local_peak = 0.0f;
    update_channel_controls();
}

static void stop_tone(void)
{
    buzzer_off();
    s_tone_on = false;
    s_beep_elapsed_ms = 0;
}

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    float a = 0.0f, b = 0.0f, ignored_peak = 0.0f;
    bool active_a = false;
    uint16_t frequency = 0;
    (void)rx5808_channel_frequency(s_settings.band, s_settings.channel,
                                    &frequency);
    if (signal_latest(&a, &b, &ignored_peak, &active_a)) {
        const float active = active_a ? a : b;
        if (active > s_local_peak) s_local_peak = active;
        lv_label_set_text_fmt(s_rssi,
            "RSSI\n\n%.3f V\n\nPEAK %.3f V\n%c%u | %u MHz\n\nACTIVE RX %c",
            active, s_local_peak, s_band_names[s_settings.band],
            s_settings.channel + 1U, frequency, active_a ? 'A' : 'B');
        if (s_settings.buzzer_enabled) {
            if (s_tone_on) {
                buzzer_off();
                s_tone_on = false;
            } else {
                s_beep_elapsed_ms += 50U;
                if (s_beep_elapsed_ms >= finder_beep_interval_ms(active)) {
                    buzzer_tone(2200, 25);
                    s_tone_on = true;
                    s_beep_elapsed_ms = 0;
                }
            }
        } else {
            stop_tone();
        }
    } else {
        lv_label_set_text_fmt(s_rssi,
            "RSSI\n\n--\n\nPEAK --\n%c%u | %u MHz\n\nWaiting for receiver hardware",
            s_band_names[s_settings.band], s_settings.channel + 1U, frequency);
        stop_tone();
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    stop_tone();
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_rssi = NULL;
    s_lock_label = NULL;
    s_previous = NULL;
    s_next = NULL;
}

void app_finder_show(void)
{
    if (settings_load(&s_settings) != ESP_OK) {
        s_settings.band = RX5808_BAND_R;
        s_settings.channel = 0;
        s_settings.buzzer_enabled = true;
    }
    s_local_peak = 0.0f;
    s_beep_elapsed_ms = 0;
    s_tone_on = false;
    s_locked = true;
    tune_selected();

    lv_obj_t *screen = app_screen_create("Lost-quad finder");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    s_rssi = lv_label_create(screen);
    lv_obj_set_style_text_align(s_rssi, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(s_rssi, &lv_font_montserrat_26, 0);
    lv_obj_set_flex_grow(s_rssi, 1);

    lv_obj_t *controls = lv_obj_create(screen);
    lv_obj_set_size(controls, 1120, 110);
    lv_obj_set_flex_flow(controls, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(controls, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_color(controls, lv_color_hex(0x0D1B24), 0);
    lv_obj_set_style_border_color(controls, lv_color_hex(0x214252), 0);
    s_previous = app_make_button(controls, LV_SYMBOL_LEFT " Channel",
                                 previous_channel);
    lv_obj_set_size(s_previous, 240, 72);
    lv_obj_t *lock = app_make_button(controls, "", toggle_lock);
    lv_obj_set_size(lock, 450, 72);
    s_lock_label = lv_obj_get_child(lock, 0);
    s_next = app_make_button(controls, "Channel " LV_SYMBOL_RIGHT,
                             next_channel);
    lv_obj_set_size(s_next, 240, 72);
    update_channel_controls();

    s_timer = lv_timer_create(refresh, 50, NULL);
    refresh(s_timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
