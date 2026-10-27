#include "apps.h"

void app_stream_show(void)
{
    lv_obj_t *screen = app_screen_create("Spectator stream");
    lv_obj_t *label = lv_label_create(screen);
    lv_label_set_text(label,
        "STREAMING IS A BRING-UP GATE\n\nSoftAP: FPV-Deck\nTarget: MJPEG 640×360 at 15 fps\n\n"
        "Not enabled until camera capture and ESP-Hosted coexistence\n"
        "are measured on the physical HAT.");
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(label, lv_color_hex(0xEAF6F7), 0);
    lv_obj_center(label);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
