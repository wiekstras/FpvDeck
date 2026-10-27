#include "apps.h"
#include "settings.h"
#include "tvp5150.h"
#include "platform.h"
#include "ui_theme.h"

#include <string.h>

static fpvdeck_settings_t s_settings;
static lv_obj_t *s_brightness_value;
static lv_obj_t *s_password_status;
static lv_obj_t *s_password_modal;

static lv_obj_t *make_row(lv_obj_t *parent, const char *title)
{
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_size(row, lv_pct(100), 84);
    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_hor(row, 24, 0);
    ui_surface_style(row);
    lv_obj_t *label = lv_label_create(row);
    lv_label_set_text(label, title);
    lv_obj_set_width(label, 330);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_MUTED), 0);
    return row;
}

static void brightness_changed(lv_event_t *event)
{
    lv_obj_t *slider = lv_event_get_target_obj(event);
    s_settings.brightness_percent = (uint8_t)lv_slider_get_value(slider);
    lv_label_set_text_fmt(s_brightness_value, "%u%%",
                          s_settings.brightness_percent);
    (void)platform_display_set_brightness(s_settings.brightness_percent);
}

static void brightness_saved(lv_event_t *event)
{
    (void)event;
    (void)settings_save(&s_settings);
}

static void buzzer_changed(lv_event_t *event)
{
    s_settings.buzzer_enabled =
        lv_obj_has_state(lv_event_get_target_obj(event), LV_STATE_CHECKED);
    (void)settings_save(&s_settings);
}

static void video_mode_changed(lv_event_t *event)
{
    const uint32_t selected = lv_dropdown_get_selected(
        lv_event_get_target_obj(event));
    if (selected <= SETTINGS_VIDEO_NTSC) {
        s_settings.video_mode = (settings_video_mode_t)selected;
        (void)settings_save(&s_settings);
        const tvp5150_standard_t standard =
            selected == SETTINGS_VIDEO_PAL ? TVP5150_STANDARD_PAL :
            (selected == SETTINGS_VIDEO_NTSC ? TVP5150_STANDARD_NTSC :
                                               TVP5150_STANDARD_AUTO);
        (void)tvp5150_set_standard(standard);
    }
}

static void password_modal_deleted(lv_event_t *event)
{
    (void)event;
    s_password_modal = NULL;
}

static void keyboard_event(lv_event_t *event)
{
    const lv_event_code_t code = lv_event_get_code(event);
    if (code == LV_EVENT_CANCEL) {
        lv_obj_delete_async(s_password_modal);
        return;
    }
    if (code != LV_EVENT_READY) return;
    lv_obj_t *textarea = lv_keyboard_get_textarea(lv_event_get_target_obj(event));
    const char *password = lv_textarea_get_text(textarea);
    const size_t length = strlen(password);
    if (length < 8 || length > 63) {
        lv_textarea_set_placeholder_text(textarea,
            "WPA2 password must be 8-63 characters");
        lv_textarea_set_text(textarea, "");
        return;
    }
    memcpy(s_settings.wifi_password, password, length + 1U);
    (void)settings_save(&s_settings);
    lv_label_set_text_fmt(s_password_status, "Configured | %u characters",
                          (unsigned)length);
    lv_obj_delete_async(s_password_modal);
}

static void edit_password(lv_event_t *event)
{
    (void)event;
    if (s_password_modal != NULL) return;
    s_password_modal = lv_obj_create(lv_screen_active());
    lv_obj_add_event_cb(s_password_modal, password_modal_deleted,
                        LV_EVENT_DELETE, NULL);
    lv_obj_set_size(s_password_modal, 1160, 620);
    lv_obj_center(s_password_modal);
    ui_surface_style(s_password_modal);
    lv_obj_set_style_border_color(s_password_modal,
                                  lv_color_hex(UI_COLOR_ACCENT), 0);

    lv_obj_t *title = lv_label_create(s_password_modal);
    lv_label_set_text(title, "Spectator Wi-Fi password");
    ui_label_title(title);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_t *textarea = lv_textarea_create(s_password_modal);
    lv_obj_set_size(textarea, 1040, 68);
    lv_obj_align(textarea, LV_ALIGN_TOP_MID, 0, 60);
    lv_textarea_set_one_line(textarea, true);
    lv_textarea_set_password_mode(textarea, true);
    lv_textarea_set_max_length(textarea, 63);
    lv_textarea_set_text(textarea, s_settings.wifi_password);

    lv_obj_t *keyboard = lv_keyboard_create(s_password_modal);
    lv_obj_set_size(keyboard, 1080, 430);
    lv_obj_align(keyboard, LV_ALIGN_BOTTOM_MID, 0, -8);
    lv_keyboard_set_textarea(keyboard, textarea);
    lv_obj_add_event_cb(keyboard, keyboard_event, LV_EVENT_ALL, NULL);
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    s_brightness_value = NULL;
    s_password_status = NULL;
    s_password_modal = NULL;
}

void app_settings_show(void)
{
    (void)settings_load(&s_settings);
    lv_obj_t *screen = app_screen_create("Settings");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    lv_obj_t *list = lv_obj_create(screen);
    lv_obj_set_width(list, lv_pct(100));
    lv_obj_set_flex_grow(list, 1);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_hor(list, 24, 0);
    lv_obj_set_style_pad_ver(list, 16, 0);
    lv_obj_set_style_pad_row(list, 8, 0);
    lv_obj_set_style_bg_opa(list, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(list, 0, 0);

    lv_obj_t *row = make_row(list, "Brightness");
    lv_obj_t *slider = lv_slider_create(row);
    lv_obj_set_size(slider, 560, 42);
    lv_slider_set_range(slider, 10, 100);
    lv_slider_set_value(slider, s_settings.brightness_percent, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x173039), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, lv_color_hex(UI_COLOR_ACCENT),
                              LV_PART_INDICATOR);
    lv_obj_set_style_bg_color(slider, lv_color_hex(UI_COLOR_TEXT), LV_PART_KNOB);
    lv_obj_add_event_cb(slider, brightness_changed, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(slider, brightness_saved, LV_EVENT_RELEASED, NULL);
    s_brightness_value = lv_label_create(row);
    lv_obj_set_width(s_brightness_value, 100);
    lv_obj_set_style_text_align(s_brightness_value, LV_TEXT_ALIGN_RIGHT, 0);
    lv_label_set_text_fmt(s_brightness_value, "%u%%",
                          s_settings.brightness_percent);

    row = make_row(list, "Buzzer");
    lv_obj_t *toggle = lv_switch_create(row);
    lv_obj_set_size(toggle, 92, 52);
    lv_obj_set_style_bg_color(toggle, lv_color_hex(UI_COLOR_ACCENT),
                              LV_PART_INDICATOR | LV_STATE_CHECKED);
    if (s_settings.buzzer_enabled) lv_obj_add_state(toggle, LV_STATE_CHECKED);
    lv_obj_add_event_cb(toggle, buzzer_changed, LV_EVENT_VALUE_CHANGED, NULL);

    row = make_row(list, "Video standard");
    lv_obj_t *dropdown = lv_dropdown_create(row);
    lv_obj_set_size(dropdown, 300, 58);
    lv_dropdown_set_options(dropdown, "Auto\nPAL\nNTSC");
    lv_obj_set_style_bg_color(dropdown, lv_color_hex(UI_COLOR_SURFACE_HI), 0);
    lv_obj_set_style_border_color(dropdown, lv_color_hex(UI_COLOR_BORDER), 0);
    lv_obj_set_style_text_color(dropdown, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_dropdown_set_selected(dropdown, (uint32_t)s_settings.video_mode);
    lv_obj_add_event_cb(dropdown, video_mode_changed, LV_EVENT_VALUE_CHANGED,
                        NULL);

    row = make_row(list, "Spectator Wi-Fi");
    s_password_status = lv_label_create(row);
    lv_label_set_text_fmt(s_password_status, "Configured | %u characters",
                          (unsigned)strlen(s_settings.wifi_password));
    lv_obj_set_flex_grow(s_password_status, 1);
    lv_obj_t *edit = app_make_button(row, "Edit password", edit_password);
    lv_obj_set_size(edit, 240, 62);

    row = make_row(list, "Cell calibration");
    lv_obj_t *pending = lv_label_create(row);
    lv_label_set_text(pending,
        "Requires connected HAT, precision source and DMM | not run");
    lv_obj_set_style_text_color(pending, lv_color_hex(UI_COLOR_WARNING), 0);

    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
}
