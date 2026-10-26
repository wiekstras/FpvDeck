#include "apps.h"

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
        lv_obj_set_style_bg_color(s_battery_warning, lv_color_hex(0xB45309), 0);
        lv_obj_set_style_bg_opa(s_battery_warning, LV_OPA_COVER, 0);
        lv_obj_set_style_pad_all(s_battery_warning, 12, 0);
        lv_obj_set_style_radius(s_battery_warning, 10, 0);
        lv_obj_align(s_battery_warning, LV_ALIGN_TOP_MID, 0, 14);
    }
    lv_label_set_text_fmt(s_battery_warning,
                          LV_SYMBOL_WARNING "  LOW DECK BATTERY  %.2f V", voltage);
}

void app_shutdown_show(float voltage)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x050708), 0);
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text_fmt(label,
        LV_SYMBOL_WARNING "\n\nDECK BATTERY CRITICAL\n%.2f V\n\n"
        "Recording stopped. Connect USB power or switch off.", voltage);
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xFFB24A), 0);
    lv_obj_center(label);
    lv_screen_load(screen);
}

lv_obj_t *app_screen_create(const char *title)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x071016), 0);
    lv_obj_t *bar = lv_obj_create(screen);
    lv_obj_set_size(bar, lv_pct(100), 76);
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x0D1B24), 0);
    lv_obj_set_style_border_width(bar, 0, 0);
    lv_obj_t *back = lv_button_create(bar);
    lv_obj_set_size(back, 92, 52);
    lv_obj_align(back, LV_ALIGN_LEFT_MID, 0, 0);
    lv_obj_add_event_cb(back, go_home, LV_EVENT_CLICKED, NULL);
    lv_obj_t *back_label = lv_label_create(back);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT " Home");
    lv_obj_center(back_label);
    lv_obj_t *label = lv_label_create(bar);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_24, 0);
    lv_obj_center(label);
    lv_obj_set_flex_flow(screen, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(screen, 0, 0);
    return screen;
}

lv_obj_t *app_make_button(lv_obj_t *parent, const char *text,
                          lv_event_cb_t callback)
{
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_size(button, 250, 112);
    lv_obj_set_style_radius(button, 20, 0);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x123647), 0);
    lv_obj_add_event_cb(button, callback, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_center(label);
    return button;
}

void app_launcher_show(void)
{
    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x071016), 0);
    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "FPV DECK");
    lv_obj_set_style_text_color(title, lv_color_hex(0x51D9D0), 0);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_26, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 36, 24);
    lv_obj_t *status = lv_label_create(screen);
    lv_label_set_text(status, "FIELD TOOL  •  HAT SELF-TEST PENDING");
    lv_obj_align(status, LV_ALIGN_TOP_RIGHT, -36, 31);

    lv_obj_t *grid = lv_obj_create(screen);
    lv_obj_set_size(grid, 1160, 570);
    lv_obj_align(grid, LV_ALIGN_BOTTOM_MID, 0, -28);
    lv_obj_set_flex_flow(grid, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(grid, LV_FLEX_ALIGN_SPACE_EVENLY,
                         LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_bg_opa(grid, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(grid, 0, 0);
    app_make_button(grid, LV_SYMBOL_BATTERY_FULL "\nBattery", open_battery);
    app_make_button(grid, LV_SYMBOL_VIDEO "\nFPV monitor", open_monitor);
    app_make_button(grid, LV_SYMBOL_WIFI "\nSpectrum", open_scanner);
    app_make_button(grid, LV_SYMBOL_GPS "\nFinder", open_finder);
    app_make_button(grid, LV_SYMBOL_UPLOAD "\nStream", open_stream);
    app_make_button(grid, LV_SYMBOL_SETTINGS "\nSettings", open_settings);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_FADE_IN, 120, 0, true);
}
