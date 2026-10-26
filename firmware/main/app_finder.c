#include "apps.h"

void app_finder_show(void)
{
    lv_obj_t *screen = app_screen_create("Lost-quad finder");
    lv_obj_t *rssi = lv_label_create(screen);
    lv_label_set_text(rssi, "RSSI\n\n72%\n\nPEAK 81%\nR1 · 5658 MHz");
    lv_obj_set_style_text_align(rssi, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(rssi, &lv_font_montserrat_26, 0);
    lv_obj_center(rssi);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
