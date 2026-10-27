#include "apps.h"
#include "ui_theme.h"

static void open_battery(lv_event_t *e) { (void)e; app_battery_show(); }
static void open_monitor(lv_event_t *e) { (void)e; app_monitor_show(); }
static void open_scanner(lv_event_t *e) { (void)e; app_scanner_show(); }
static void open_finder(lv_event_t *e) { (void)e; app_finder_show(); }
static void open_stream(lv_event_t *e) { (void)e; app_stream_show(); }
static void open_settings(lv_event_t *e) { (void)e; app_settings_show(); }
static void go_home(lv_event_t *e) { (void)e; app_launcher_show(); }
static lv_obj_t *s_battery_warning;

static void warning_deleted(lv_event_t *event)
{
    (void)event;
    s_battery_warning = NULL;
}

void app_set_deck_battery_warning(float voltage)
{
    if (voltage > 3.5f || voltage < 2.5f) {
        if (s_battery_warning != NULL) lv_obj_delete(s_battery_warning);
        return;
    }
    if (s_battery_warning == NULL) {
        s_battery_warning = lv_label_create(lv_screen_active());
        lv_obj_add_event_cb(s_battery_warning, warning_deleted,
                            LV_EVENT_DELETE, NULL);
        lv_obj_set_style_bg_color(s_battery_warning,
                                  lv_color_hex(UI_COLOR_WARNING), 0);
        lv_obj_set_style_text_color(s_battery_warning,
                                    lv_color_hex(UI_COLOR_BG), 0);
        lv_obj_set_style_bg_opa(s_battery_warning, LV_OPA_COVER, 0);
        lv_obj_set_style_pad_hor(s_battery_warning, 18, 0);
        lv_obj_set_style_pad_ver(s_battery_warning, 10, 0);
        lv_obj_set_style_radius(s_battery_warning, 18, 0);
        lv_obj_align(s_battery_warning, LV_ALIGN_TOP_MID, 0, 14);
    }
    lv_label_set_text_fmt(s_battery_warning,
                          LV_SYMBOL_WARNING "  DECK BATTERY  %.2f V", voltage);
}

void app_shutdown_show(float voltage)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    ui_screen_style(screen);
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text_fmt(label,
        LV_SYMBOL_WARNING "\n\nDECK BATTERY CRITICAL\n%.2f V\n\n"
        "Recording stopped. Connect USB power or switch off.", voltage);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_WARNING), 0);
    lv_obj_center(label);
    lv_screen_load(screen);
}

lv_obj_t *app_screen_create(const char *title)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    ui_screen_style(screen);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);

    lv_obj_t *bar = lv_obj_create(screen);
    lv_obj_set_size(bar, lv_pct(100), 72);
    lv_obj_set_style_bg_color(bar, lv_color_hex(UI_COLOR_BG), 0);
    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_set_style_pad_all(bar, 0, 0);

    lv_obj_t *back = lv_button_create(bar);
    lv_obj_set_size(back, 96, 44);
    lv_obj_align(back, LV_ALIGN_LEFT_MID, 20, 0);
    ui_button_style(back, false);
    lv_obj_add_event_cb(back, go_home, LV_EVENT_CLICKED, NULL);
    lv_obj_t *back_label = lv_label_create(back);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT "  HOME");
    lv_obj_set_style_text_font(back_label, &lv_font_montserrat_16, 0);
    lv_obj_center(back_label);

    lv_obj_t *label = lv_label_create(bar);
    lv_label_set_text(label, title);
    ui_label_title(label);
    lv_obj_align(label, LV_ALIGN_LEFT_MID, 140, -7);
    lv_obj_t *context = lv_label_create(bar);
    lv_label_set_text(context, "FPV DECK / FIELD TOOL");
    ui_label_overline(context);
    lv_obj_align(context, LV_ALIGN_LEFT_MID, 141, 18);
    lv_obj_t *chip = ui_chip_create(bar, "HAT LINK", UI_COLOR_MUTED);
    lv_obj_align(chip, LV_ALIGN_RIGHT_MID,
#ifdef FPVDECK_SIMULATOR
                 -132,
#else
                 -22,
#endif
                 0);
    return screen;
}

lv_obj_t *app_make_button(lv_obj_t *parent, const char *text,
                          lv_event_cb_t callback)
{
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_size(button, 250, 112);
    ui_button_style(button, false);
    if (callback != NULL) {
        lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
    }
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_center(label);
    return button;
}

static lv_obj_t *launcher_tile(lv_obj_t *parent, lv_coord_t x, lv_coord_t y,
                               lv_coord_t width, lv_coord_t height,
                               const char *overline, const char *title,
                               const char *detail, bool primary,
                               lv_event_cb_t callback)
{
    lv_obj_t *tile = lv_button_create(parent);
    lv_obj_set_pos(tile, x, y);
    lv_obj_set_size(tile, width, height);
    ui_button_style(tile, primary);
    lv_obj_set_style_pad_all(tile, 24, 0);
    lv_obj_add_event_cb(tile, callback, LV_EVENT_CLICKED, NULL);

    lv_obj_t *top = lv_label_create(tile);
    lv_label_set_text(top, overline);
    ui_label_overline(top);
    if (primary) {
        lv_obj_set_style_text_color(top, lv_color_hex(0x0A5C51), 0);
    }
    lv_obj_align(top, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *name = lv_label_create(tile);
    lv_label_set_text(name, title);
    lv_obj_set_style_text_font(name, &lv_font_montserrat_26, 0);
    lv_obj_align(name, LV_ALIGN_LEFT_MID, 0, primary ? -4 : -2);

    lv_obj_t *description = lv_label_create(tile);
    lv_label_set_text(description, detail);
    lv_obj_set_width(description, width - 48);
    lv_obj_set_style_text_font(description, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(description,
        lv_color_hex(primary ? 0x0A5C51 : UI_COLOR_MUTED), 0);
    lv_obj_align(description, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    lv_obj_t *arrow = lv_label_create(tile);
    lv_label_set_text(arrow, LV_SYMBOL_RIGHT);
    lv_obj_set_style_text_font(arrow, &lv_font_montserrat_20, 0);
    lv_obj_align(arrow, LV_ALIGN_TOP_RIGHT, 0, 0);
    return tile;
}

void app_launcher_show(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    ui_screen_style(screen);

    lv_obj_t *wordmark = lv_label_create(screen);
    lv_label_set_text(wordmark, "FPV / DECK");
    ui_label_title(wordmark);
    lv_obj_set_style_text_color(wordmark, lv_color_hex(UI_COLOR_ACCENT), 0);
    lv_obj_align(wordmark, LV_ALIGN_TOP_LEFT, 32, 23);
    lv_obj_t *strapline = lv_label_create(screen);
    lv_label_set_text(strapline, "ANALOG FIELD CONSOLE");
    ui_label_overline(strapline);
    lv_obj_align(strapline, LV_ALIGN_TOP_LEFT, 32, 54);

    lv_obj_t *status = ui_chip_create(screen,
#ifdef FPVDECK_SIMULATOR
                                      "SIMULATOR / HAT OFFLINE",
#else
                                      "P4 / LOCAL MODE",
#endif
                                      UI_COLOR_WARNING);
    lv_obj_align(status, LV_ALIGN_TOP_RIGHT,
#ifdef FPVDECK_SIMULATOR
                 -142,
#else
                 -32,
#endif
                 24);

    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_pos(content, 32, 94);
    lv_obj_set_size(content, 1216, 594);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 0, 0);

    (void)launcher_tile(content, 0, 0, 586, 330,
        "LIVE VIDEO", "FPV MONITOR",
        "R1  /  5658 MHz\nDual receiver diversity", true, open_monitor);
    (void)launcher_tile(content, 606, 0, 295, 330,
        "PACK HEALTH", "BATTERY",
        "2S-6S balance check\nCalibrated cell detail", false, open_battery);
    (void)launcher_tile(content, 921, 0, 295, 330,
        "RF TOOLS", "SPECTRUM",
        "Sweep channels\nFind strongest signals", false, open_scanner);
    (void)launcher_tile(content, 0, 350, 385, 244,
        "RECOVERY", "QUAD FINDER",
        "RSSI proximity guidance", false, open_finder);
    (void)launcher_tile(content, 405, 350, 385, 244,
        "NETWORK", "SPECTATOR",
        "Local MJPEG preview", false, open_stream);
    (void)launcher_tile(content, 810, 350, 406, 244,
        "SYSTEM", "SETTINGS",
        "Display, video and calibration", false, open_settings);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 100, 0, true);
}
