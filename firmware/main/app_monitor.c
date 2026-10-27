#include "apps.h"
#include "deck_power.h"
#include "signal_logic.h"

static lv_obj_t *s_overlay;
static lv_timer_t *s_timer;

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    float a = 0.0f, b = 0.0f, peak = 0.0f, deck = 0.0f;
    bool active_a = false;
    const bool rssi_ok = signal_latest(&a, &b, &peak, &active_a);
    const bool deck_ok = deck_power_latest(&deck);
    (void)peak;
    if (rssi_ok && deck_ok) {
        lv_label_set_text_fmt(s_overlay,
            "R1  5658 MHz     RSSI A %.3f V  B %.3f V     RX %c     DECK %.2f V",
            a, b, active_a ? 'A' : 'B', deck);
    } else if (rssi_ok) {
        lv_label_set_text_fmt(s_overlay,
            "R1  5658 MHz     RSSI A %.3f V  B %.3f V     RX %c     DECK —",
            a, b, active_a ? 'A' : 'B');
    } else {
        lv_label_set_text(s_overlay,
            "R1  5658 MHz     RSSI A —  B —     RX —     DECK —");
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    if (s_timer != NULL) lv_timer_delete(s_timer);
    s_timer = NULL;
    s_overlay = NULL;
}

void app_monitor_show(void)
{
    lv_obj_t *screen = app_screen_create("FPV monitor");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    lv_obj_t *video = lv_obj_create(screen);
    lv_obj_set_width(video, lv_pct(100));
    lv_obj_set_flex_grow(video, 1);
    lv_obj_set_style_bg_color(video, lv_color_hex(0x101A1D), 0);
    lv_obj_set_style_border_width(video, 0, 0);
    lv_obj_t *hint = lv_label_create(video);
    lv_label_set_text(hint, "VIDEO INPUT\nwaiting for TVP5150 lock");
    lv_obj_set_style_text_align(hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_center(hint);
    s_overlay = lv_label_create(video);
    lv_obj_set_style_bg_color(s_overlay, lv_color_hex(0x071016), 0);
    lv_obj_set_style_bg_opa(s_overlay, LV_OPA_80, 0);
    lv_obj_set_style_pad_all(s_overlay, 14, 0);
    lv_obj_align(s_overlay, LV_ALIGN_BOTTOM_MID, 0, -20);
    s_timer = lv_timer_create(refresh, 250, NULL);
    refresh(s_timer);
    lv_screen_load(screen);
}
