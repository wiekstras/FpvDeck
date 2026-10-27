#include "ui_theme.h"

static void reset_box(lv_obj_t *object)
{
    lv_obj_set_style_border_width(object, 0, LV_PART_MAIN);
    lv_obj_set_style_outline_width(object, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(object, 0, LV_PART_MAIN);
}

void ui_screen_style(lv_obj_t *screen)
{
    lv_obj_set_style_bg_color(screen, lv_color_hex(UI_COLOR_BG), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(screen, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(screen, lv_color_hex(UI_COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_pad_all(screen, 0, LV_PART_MAIN);
    reset_box(screen);
    lv_obj_remove_flag(screen, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(screen, LV_SCROLLBAR_MODE_OFF);
}

void ui_surface_style(lv_obj_t *object)
{
    lv_obj_set_style_bg_color(object, lv_color_hex(UI_COLOR_SURFACE), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(object, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(object, lv_color_hex(UI_COLOR_TEXT), LV_PART_MAIN);
    lv_obj_set_style_border_color(object, lv_color_hex(UI_COLOR_BORDER), LV_PART_MAIN);
    lv_obj_set_style_border_width(object, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(object, 18, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(object, 0, LV_PART_MAIN);
    lv_obj_remove_flag(object, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(object, LV_SCROLLBAR_MODE_OFF);
}

void ui_button_style(lv_obj_t *button, bool primary)
{
    const uint32_t background = primary ? UI_COLOR_ACCENT : UI_COLOR_SURFACE_HI;
    const uint32_t foreground = primary ? UI_COLOR_BG : UI_COLOR_TEXT;
    lv_obj_set_style_bg_color(button, lv_color_hex(background), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(button, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_text_color(button, lv_color_hex(foreground), LV_PART_MAIN);
    lv_obj_set_style_border_color(button,
        lv_color_hex(primary ? UI_COLOR_ACCENT : UI_COLOR_BORDER), LV_PART_MAIN);
    lv_obj_set_style_border_width(button, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(button, 15, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(button, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(button,
        lv_color_hex(primary ? 0x35C6AB : 0x18313A),
        LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(button, lv_color_hex(UI_COLOR_ACCENT),
        LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_width(button, 2,
        LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x142127),
        LV_PART_MAIN | LV_STATE_DISABLED);
    lv_obj_set_style_text_opa(button, LV_OPA_40,
        LV_PART_MAIN | LV_STATE_DISABLED);
}

void ui_label_title(lv_obj_t *label)
{
    lv_obj_set_style_text_font(label, &lv_font_montserrat_26, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_TEXT), LV_PART_MAIN);
}

void ui_label_overline(lv_obj_t *label)
{
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_MUTED), LV_PART_MAIN);
    lv_obj_set_style_text_letter_space(label, 2, LV_PART_MAIN);
}

void ui_label_muted(lv_obj_t *label)
{
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(UI_COLOR_MUTED), LV_PART_MAIN);
}

void ui_bar_style(lv_obj_t *bar, uint32_t indicator_color)
{
    lv_obj_set_style_bg_color(bar, lv_color_hex(0x173039), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(bar, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_radius(bar, 5, LV_PART_MAIN);
    lv_obj_set_style_bg_color(bar, lv_color_hex(indicator_color), LV_PART_INDICATOR);
    lv_obj_set_style_radius(bar, 5, LV_PART_INDICATOR);
}

lv_obj_t *ui_chip_create(lv_obj_t *parent, const char *text,
                         uint32_t accent_color)
{
    lv_obj_t *chip = lv_obj_create(parent);
    lv_obj_set_size(chip, LV_SIZE_CONTENT, 36);
    lv_obj_set_style_bg_color(chip, lv_color_hex(UI_COLOR_SURFACE_HI), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chip, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_color(chip, lv_color_hex(accent_color), LV_PART_MAIN);
    lv_obj_set_style_border_width(chip, 1, LV_PART_MAIN);
    lv_obj_set_style_radius(chip, 18, LV_PART_MAIN);
    lv_obj_set_style_pad_hor(chip, 14, LV_PART_MAIN);
    lv_obj_set_style_pad_ver(chip, 7, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(chip, 0, LV_PART_MAIN);
    lv_obj_t *label = lv_label_create(chip);
    lv_label_set_text(label, text);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_PART_MAIN);
    lv_obj_set_style_text_color(label, lv_color_hex(accent_color), LV_PART_MAIN);
    lv_obj_center(label);
    return chip;
}

lv_obj_t *ui_metric_create(lv_obj_t *parent, const char *caption,
                           lv_obj_t **value_label)
{
    lv_obj_t *metric = lv_obj_create(parent);
    ui_surface_style(metric);
    lv_obj_set_style_pad_all(metric, 18, LV_PART_MAIN);
    lv_obj_t *caption_label = lv_label_create(metric);
    lv_label_set_text(caption_label, caption);
    ui_label_overline(caption_label);
    lv_obj_align(caption_label, LV_ALIGN_TOP_LEFT, 0, 0);
    *value_label = lv_label_create(metric);
    lv_label_set_text(*value_label, "--");
    lv_obj_set_style_text_font(*value_label, &lv_font_montserrat_24, LV_PART_MAIN);
    lv_obj_set_style_text_color(*value_label, lv_color_hex(UI_COLOR_TEXT), LV_PART_MAIN);
    lv_obj_align(*value_label, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    return metric;
}
