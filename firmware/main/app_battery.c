#include "apps.h"

void app_battery_show(void)
{
    lv_obj_t *screen = app_screen_create("Battery checker");
    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 28, 0);

    lv_obj_t *summary = lv_label_create(content);
    lv_label_set_text(summary,
        "6S LiPo\n\n25.17 V\n\nDELTA  18 mV\nLOWEST 4.187 V\nHIGHEST 4.205 V\n\nReady to fly");
    lv_obj_set_width(summary, 410);
    lv_obj_set_style_text_font(summary, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(summary, lv_color_hex(0xEAF6F7), 0);

    lv_obj_t *cells = lv_obj_create(content);
    lv_obj_set_flex_grow(cells, 1);
    lv_obj_set_height(cells, lv_pct(100));
    lv_obj_set_flex_flow(cells, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cells, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    static const char *rows[] = {
        "CELL 1     4.198 V", "CELL 2     4.193 V", "CELL 3     4.205 V",
        "CELL 4     4.187 V", "CELL 5     4.196 V", "CELL 6     4.191 V"};
    for (unsigned i = 0; i < 6; ++i) {
        lv_obj_t *row = lv_label_create(cells);
        lv_label_set_text(row, rows[i]);
        lv_obj_set_style_text_font(row, &lv_font_montserrat_22, 0);
    }
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
