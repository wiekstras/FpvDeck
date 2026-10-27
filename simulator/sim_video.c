#include "sim_video.h"
#include "sim_backend.h"

#include <stdlib.h>

typedef struct {
    lv_obj_t *horizon;
    lv_obj_t *noise;
    lv_timer_t *timer;
} sim_video_t;

static void animate(lv_timer_t *timer)
{
    sim_video_t *video = lv_timer_get_user_data(timer);
    const int32_t y = (int32_t)(lv_tick_get() / 45U % 18U) - 9;
    lv_obj_align(video->horizon, LV_ALIGN_CENTER, 0, y);
    lv_obj_set_style_opa(video->noise,
        sim_state()->video_noise || sim_state()->signal_lost ? LV_OPA_70 : LV_OPA_10, 0);
    lv_label_set_text(video->noise, sim_state()->signal_lost ?
        "NO SIGNAL  ·  RECEIVER SEARCHING" : "·  ·    ·       ·  ·");
}

static void deleted(lv_event_t *event)
{
    sim_video_t *video = lv_event_get_user_data(event);
    if (video->timer != NULL) lv_timer_delete(video->timer);
    free(video);
}

void sim_video_populate(lv_obj_t *parent)
{
    sim_video_t *video = calloc(1, sizeof(*video));
    if (video == NULL) return;
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x395E7B), 0);
    video->horizon = lv_obj_create(parent);
    lv_obj_set_size(video->horizon, lv_pct(115), 15);
    lv_obj_align(video->horizon, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_color(video->horizon, lv_color_hex(0xE7D09A), 0);
    lv_obj_set_style_border_width(video->horizon, 0, 0);

    lv_obj_t *osd = lv_label_create(parent);
    lv_label_set_text(osd,
        "AIR65                       00:42\n"
        "14.8V        +              LQ 99\n\n\n"
        "             0.0\n"
        "DISARMED                 0 km/h");
    lv_obj_set_style_text_font(osd, &lv_font_montserrat_20, 0);
    lv_obj_set_style_text_color(osd, lv_color_white(), 0);
    lv_obj_set_size(osd, lv_pct(94), lv_pct(82));
    lv_obj_center(osd);
    video->noise = lv_label_create(parent);
    lv_obj_set_width(video->noise, lv_pct(100));
    lv_obj_set_style_text_align(video->noise, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(video->noise, LV_ALIGN_TOP_MID, 0, 85);
    lv_obj_add_event_cb(parent, deleted, LV_EVENT_DELETE, video);
    video->timer = lv_timer_create(animate, 60, video);
    animate(video->timer);
}

