#include "apps.h"
#include "signal_logic.h"

static lv_obj_t *s_rssi;
static lv_timer_t *s_timer;

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    float a = 0.0f, b = 0.0f, peak = 0.0f;
    bool active_a = false;
    if (signal_latest(&a, &b, &peak, &active_a)) {
        const float active = active_a ? a : b;
        lv_label_set_text_fmt(s_rssi,
            "RSSI\n\n%.3f V\n\nPEAK %.3f V\nR1 · 5658 MHz\n\nACTIVE RX %c",
            active, peak, active_a ? 'A' : 'B');
    } else {
        lv_label_set_text(s_rssi,
            "RSSI\n\n—\n\nPEAK —\nR1 · 5658 MHz\n\nWaiting for receiver hardware");
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_rssi = NULL;
}

void app_finder_show(void)
{
    lv_obj_t *screen = app_screen_create("Lost-quad finder");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    s_rssi = lv_label_create(screen);
    lv_obj_set_style_text_align(s_rssi, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(s_rssi, &lv_font_montserrat_26, 0);
    lv_obj_center(s_rssi);
    s_timer = lv_timer_create(refresh, 250, NULL);
    refresh(s_timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
