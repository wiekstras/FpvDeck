#include "apps.h"

void app_settings_show(void)
{
    lv_obj_t *screen = app_screen_create("Settings");
    lv_obj_t *list = lv_obj_create(screen);
    lv_obj_set_width(list, lv_pct(100));
    lv_obj_set_flex_grow(list, 1);
    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_style_pad_all(list, 28, 0);
    static const char *items[] = {
        "Brightness                                      70%",
        "Buzzer                                          On",
        "Video standard                                  Auto",
        "Wi-Fi password                           Configured",
        "Cell calibration                         Not run",
        "Hardware self-test                     Open report",
    };
    for (unsigned i = 0; i < sizeof(items) / sizeof(items[0]); ++i) {
        lv_obj_t *row = lv_label_create(list);
        lv_label_set_text(row, items[i]);
        lv_obj_set_style_text_font(row, &lv_font_montserrat_20, 0);
        lv_obj_set_style_pad_all(row, 18, 0);
    }
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
