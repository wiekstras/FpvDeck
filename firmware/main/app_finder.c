#include "apps.h"
#include "buzzer.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"
#include "ui_theme.h"

static const char s_band_names[RX5808_BAND_COUNT] = {'A', 'B', 'E', 'F', 'R', 'L'};
static lv_obj_t *s_rssi_value;
static lv_obj_t *s_peak_value;
static lv_obj_t *s_receiver_value;
static lv_obj_t *s_signal_bar;
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
    lv_label_set_text_fmt(s_lock_label, "%s  /  %c%u  /  %u MHz",
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
    if (signal_latest(&a, &b, &ignored_peak, &active_a)) {
        const float active = active_a ? a : b;
        if (active > s_local_peak) s_local_peak = active;
        lv_label_set_text_fmt(s_rssi_value, "%.3f V", active);
        lv_label_set_text_fmt(s_peak_value, "%.3f V", s_local_peak);
        lv_label_set_text_fmt(s_receiver_value, "RX %c", active_a ? 'A' : 'B');
        int32_t bar_value = (int32_t)(active * 1000.0f);
        if (bar_value < 0) bar_value = 0;
        if (bar_value > 2048) bar_value = 2048;
        lv_bar_set_value(s_signal_bar, bar_value, LV_ANIM_OFF);

        if (s_settings.buzzer_enabled) {
            if (s_tone_on) {
                buzzer_off();
                s_tone_on = false;
            } else {
                s_beep_elapsed_ms += 100U;
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
        lv_label_set_text(s_rssi_value, "--");
        lv_label_set_text(s_peak_value, "--");
        lv_label_set_text(s_receiver_value, "OFFLINE");
        lv_bar_set_value(s_signal_bar, 0, LV_ANIM_OFF);
        stop_tone();
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    stop_tone();
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_rssi_value = NULL;
    s_peak_value = NULL;
    s_receiver_value = NULL;
    s_signal_bar = NULL;
    s_lock_label = NULL;
    s_previous = NULL;
    s_next = NULL;
}

static lv_obj_t *finder_metric(lv_obj_t *parent, const char *caption,
                               lv_coord_t x, lv_coord_t width,
                               lv_obj_t **value)
{
    lv_obj_t *card = ui_metric_create(parent, caption, value);
    lv_obj_set_pos(card, x, 0);
    lv_obj_set_size(card, width, 122);
    return card;
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
    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 0, 0);

    lv_obj_t *signal = lv_obj_create(content);
    lv_obj_set_pos(signal, 24, 18);
    lv_obj_set_size(signal, 1232, 372);
    ui_surface_style(signal);
    lv_obj_set_style_border_color(signal, lv_color_hex(UI_COLOR_ACCENT), 0);
    lv_obj_set_style_pad_all(signal, 30, 0);
    lv_obj_t *caption = lv_label_create(signal);
    lv_label_set_text(caption, "PROXIMITY SIGNAL");
    ui_label_overline(caption);
    s_rssi_value = lv_label_create(signal);
    lv_label_set_text(s_rssi_value, "--");
    lv_obj_set_style_text_font(s_rssi_value, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(s_rssi_value, lv_color_hex(UI_COLOR_ACCENT), 0);
    lv_obj_align(s_rssi_value, LV_ALIGN_LEFT_MID, 0, -18);
    lv_obj_t *hint = lv_label_create(signal);
    lv_label_set_text(hint, "Move slowly. Faster tones indicate a stronger return signal.");
    ui_label_muted(hint);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_LEFT, 0, -42);
    s_signal_bar = lv_bar_create(signal);
    lv_obj_set_size(s_signal_bar, 1172, 18);
    lv_bar_set_range(s_signal_bar, 0, 2048);
    ui_bar_style(s_signal_bar, UI_COLOR_ACCENT);
    lv_obj_align(s_signal_bar, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    lv_obj_t *metrics = lv_obj_create(content);
    lv_obj_set_pos(metrics, 24, 410);
    lv_obj_set_size(metrics, 1232, 122);
    lv_obj_set_style_bg_opa(metrics, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(metrics, 0, 0);
    lv_obj_set_style_pad_all(metrics, 0, 0);
    (void)finder_metric(metrics, "PEAK HOLD", 0, 390, &s_peak_value);
    (void)finder_metric(metrics, "ACTIVE RECEIVER", 410, 390, &s_receiver_value);
    lv_obj_t *tone_value;
    (void)finder_metric(metrics, "AUDIO GUIDANCE", 820, 412, &tone_value);
    lv_label_set_text(tone_value, s_settings.buzzer_enabled ? "ENABLED" : "MUTED");

    lv_obj_t *controls = lv_obj_create(content);
    lv_obj_set_pos(controls, 24, 550);
    lv_obj_set_size(controls, 1232, 80);
    lv_obj_set_flex_flow(controls, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(controls, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(controls, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(controls, 0, 0);
    lv_obj_set_style_pad_all(controls, 0, 0);
    s_previous = app_make_button(controls, LV_SYMBOL_LEFT "  CHANNEL",
                                 previous_channel);
    lv_obj_set_size(s_previous, 250, 64);
    lv_obj_t *lock = app_make_button(controls, "", toggle_lock);
    lv_obj_set_size(lock, 650, 64);
    s_lock_label = lv_obj_get_child(lock, 0);
    s_next = app_make_button(controls, "CHANNEL  " LV_SYMBOL_RIGHT,
                             next_channel);
    lv_obj_set_size(s_next, 250, 64);
    update_channel_controls();

    s_timer = lv_timer_create(refresh, 100, NULL);
    refresh(s_timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
}
