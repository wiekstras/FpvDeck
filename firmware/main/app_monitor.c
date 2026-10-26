#include "apps.h"

void app_monitor_show(void)
{
    lv_obj_t *screen = app_screen_create("FPV monitor");
    lv_obj_t *video = lv_obj_create(screen);
    lv_obj_set_width(video, lv_pct(100));
    lv_obj_set_flex_grow(video, 1);
    lv_obj_set_style_bg_color(video, lv_color_hex(0x101A1D), 0);
    lv_obj_set_style_border_width(video, 0, 0);
    lv_obj_t *hint = lv_label_create(video);
    lv_label_set_text(hint, "VIDEO INPUT\nwaiting for TVP5150 lock");
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(hint);
    lv_obj_t *overlay = lv_label_create(video);
    lv_label_set_text(overlay, "R1  5658 MHz     A 72%  B 65%     RX A     DECK 82%");
    lv_obj_set_style_bg_color(overlay, lv_color_hex(0x071016), 0);
    lv_obj_set_style_bg_opa(overlay, LV_OPA_80, 0);
    lv_obj_set_style_pad_all(overlay, 14, 0);
    lv_obj_align(overlay, LV_ALIGN_BOTTOM_MID, 0, -20);
    lv_screen_load(screen);
}
