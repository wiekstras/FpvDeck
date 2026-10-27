#include "apps.h"
#include "ui_theme.h"

void app_stream_show(void)
{
    lv_obj_t *screen = app_screen_create("Spectator stream");
    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_remove_flag(content, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_t *card = lv_obj_create(content);
    lv_obj_set_size(card, 920, 430);
    ui_surface_style(card);
    lv_obj_set_style_border_color(card, lv_color_hex(UI_COLOR_BLUE), 0);
    lv_obj_set_style_pad_all(card, 36, 0);
    lv_obj_t *state = ui_chip_create(card, "BRING-UP GATE", UI_COLOR_WARNING);
    lv_obj_align(state, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_t *title = lv_label_create(card);
    lv_label_set_text(title, "SPECTATOR STREAM");
    ui_label_title(title);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 0, 70);
    lv_obj_t *target = lv_label_create(card);
    lv_label_set_text(target,
        "SOFT AP     FPV-Deck\nFORMAT      MJPEG / 640 x 360\nTARGET      15 FPS OR BETTER");
    lv_obj_set_style_text_font(target, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_line_space(target, 16, 0);
    lv_obj_align(target, LV_ALIGN_TOP_LEFT, 0, 130);
    lv_obj_t *note = lv_label_create(card);
    lv_label_set_text(note,
        "Activation waits for measured camera capture and ESP-Hosted\n"
        "coexistence on physical hardware.");
    ui_label_muted(note);
    lv_obj_align(note, LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_center(card);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
}
