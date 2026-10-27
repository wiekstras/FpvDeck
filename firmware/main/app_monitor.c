#include "apps.h"
#include "deck_power.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"
#include "ui_theme.h"

#include <stdint.h>
#ifdef FPVDECK_SIMULATOR
#include "sim_video.h"
#endif

static const char s_band_names[RX5808_BAND_COUNT] = {'A', 'B', 'E', 'F', 'R', 'L'};
static lv_obj_t *s_screen;
static lv_obj_t *s_channel;
static lv_obj_t *s_rssi;
static lv_obj_t *s_receiver;
static lv_obj_t *s_deck;
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

    lv_label_set_text_fmt(s_channel, "%c%u\n%u MHz",
                          s_band_names[s_settings.band],
                          s_settings.channel + 1U, frequency);
    if (rssi_ok) {
        lv_label_set_text_fmt(s_rssi, "RSSI A / B\n%.3f / %.3f V", a, b);
        lv_label_set_text_fmt(s_receiver, "DIVERSITY\nRX %c ACTIVE",
                              active_a ? 'A' : 'B');
        lv_obj_set_style_text_color(s_receiver,
                                    lv_color_hex(UI_COLOR_ACCENT), 0);
    } else {
        lv_label_set_text(s_rssi, "RSSI A / B\n-- / --");
        lv_label_set_text(s_receiver, "DIVERSITY\nNO SIGNAL");
        lv_obj_set_style_text_color(s_receiver,
                                    lv_color_hex(UI_COLOR_WARNING), 0);
    }
    if (deck_ok) lv_label_set_text_fmt(s_deck, "DECK POWER\n%.2f V", deck);
    else lv_label_set_text(s_deck, "DECK POWER\n--");
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
    ui_surface_style(s_band_picker);
    lv_obj_set_style_border_color(s_band_picker, lv_color_hex(UI_COLOR_ACCENT), 0);
    lv_obj_set_style_pad_all(s_band_picker, 24, 0);

    lv_obj_t *title = lv_label_create(s_band_picker);
    lv_label_set_text(title, "SELECT RECEIVER BAND");
    ui_label_title(title);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 8, 4);

    lv_obj_t *grid = lv_obj_create(s_band_picker);
    lv_obj_set_size(grid, 700, 320);
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, -4);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
    for (unsigned band = 0; band < RX5808_BAND_COUNT; ++band) {
        char name[16];
        lv_snprintf(name, sizeof(name), "BAND %c", s_band_names[band]);
        lv_obj_t *button = app_make_button(grid, name, NULL);
        lv_obj_set_size(button, 205, 84);
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
    s_channel = NULL;
    s_rssi = NULL;
    s_receiver = NULL;
    s_deck = NULL;
    s_band_picker = NULL;
    s_screen = NULL;
}

static lv_obj_t *overlay_metric(lv_obj_t *parent, lv_coord_t width)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_obj_set_width(label, width);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_obj_set_style_text_line_space(label, 7, 0);
    return label;
}

static lv_obj_t *channel_button(lv_obj_t *parent, lv_align_t align,
                                const char *symbol, lv_event_cb_t callback)
{
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_size(button, 64, 72);
    lv_obj_align(button, align, align == LV_ALIGN_LEFT_MID ? 20 : -20, -18);
    ui_button_style(button, false);
    lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, symbol);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_center(label);
    return button;
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
    lv_obj_set_style_bg_color(video, lv_color_hex(0x0B1215), 0);
    lv_obj_set_style_border_width(video, 0, 0);
    lv_obj_set_style_pad_all(video, 0, 0);
    lv_obj_remove_flag(video, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(video, LV_SCROLLBAR_MODE_OFF);
#ifdef FPVDECK_SIMULATOR
    sim_video_populate(video);
#else
    lv_obj_t *hint = lv_label_create(video);
    lv_label_set_text(hint, "VIDEO INPUT\nWAITING FOR TVP5150 LOCK");
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    ui_label_overline(hint);
    lv_obj_center(hint);
#endif

    (void)channel_button(video, LV_ALIGN_LEFT_MID, LV_SYMBOL_LEFT,
                         previous_channel);
    (void)channel_button(video, LV_ALIGN_RIGHT_MID, LV_SYMBOL_RIGHT,
                         next_channel);

    lv_obj_t *rail = lv_obj_create(video);
    lv_obj_set_size(rail, 890, 88);
    lv_obj_align(rail, LV_ALIGN_BOTTOM_MID, 0, -18);
    ui_surface_style(rail);
    lv_obj_set_style_bg_color(rail, lv_color_hex(UI_COLOR_BG), 0);
    lv_obj_set_style_pad_hor(rail, 22, 0);
    lv_obj_set_style_pad_ver(rail, 14, 0);
    lv_obj_set_flex_flow(rail, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(rail, LV_FLEX_ALIGN_SPACE_BETWEEN,
                          LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    s_channel = overlay_metric(rail, 138);
    s_rssi = overlay_metric(rail, 215);
    s_receiver = overlay_metric(rail, 190);
    s_deck = overlay_metric(rail, 150);

    s_timer = lv_timer_create(refresh, 250, NULL);
    tune();
    refresh(s_timer);
    restore_monitor_focus();
    lv_screen_load_anim(s_screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
}
