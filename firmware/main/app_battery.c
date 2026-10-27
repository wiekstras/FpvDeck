#include "apps.h"
#include "battery.h"

#include <string.h>

typedef struct {
    lv_obj_t *summary;
    lv_obj_t *rows[BATTERY_MAX_CELLS];
    lv_timer_t *timer;
} battery_ui_t;

static battery_ui_t s_ui;

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    battery_pack_t pack;
    if (!battery_latest(&pack)) {
        lv_label_set_text(s_ui.summary,
            "NO PACK\n\n— V\n\nDELTA  — mV\nLOWEST — V\nHIGHEST — V\n\n"
            "Connect a 2S–6S balance lead");
        for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
            lv_label_set_text_fmt(s_ui.rows[i], "CELL %u     — V", i + 1U);
            lv_obj_set_style_text_opa(s_ui.rows[i], LV_OPA_40, 0);
        }
        return;
    }

    const char *status = pack.valid ? "READY" : "CHECK BALANCE WIRING";
    bool storage = pack.valid;
    for (unsigned i = 0; i < pack.cell_count; ++i) {
        if (pack.cells_v[i] < 3.50f || pack.cells_v[i] > 4.25f) status = "VOLTAGE WARNING";
        if (!battery_is_storage_voltage(pack.cells_v[i])) storage = false;
    }
    if (storage) status = "STORAGE VOLTAGE";
    lv_label_set_text_fmt(s_ui.summary,
        "%uS PACK\n\n%.2f V\n\nDELTA  %.0f mV\nLOWEST %.3f V\n"
        "HIGHEST %.3f V\n\n%s",
        pack.cell_count, pack.total_v, pack.delta_v * 1000.0f,
        pack.minimum_v, pack.maximum_v, status);

    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        if (i < pack.cell_count) {
            lv_label_set_text_fmt(s_ui.rows[i], "CELL %u     %.3f V",
                                  i + 1U, pack.cells_v[i]);
            lv_obj_set_style_text_opa(s_ui.rows[i], LV_OPA_COVER, 0);
            const uint32_t color = pack.cells_v[i] > 4.25f ? 0xFF5D5D :
                                   pack.cells_v[i] < 3.50f ? 0xFFB24A : 0xEAF6F7;
            lv_obj_set_style_text_color(s_ui.rows[i], lv_color_hex(color), 0);
        } else {
            lv_label_set_text_fmt(s_ui.rows[i], "CELL %u     — V", i + 1U);
            lv_obj_set_style_text_opa(s_ui.rows[i], LV_OPA_40, 0);
        }
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    if (s_ui.timer != NULL) lv_timer_delete(s_ui.timer);
    memset(&s_ui, 0, sizeof(s_ui));
}

void app_battery_show(void)
{
    lv_obj_t *screen = app_screen_create("Battery checker");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);
    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_flex_flow(content, LV_FLEX_FLOW_ROW);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 28, 0);

    s_ui.summary = lv_label_create(content);
    lv_obj_set_width(s_ui.summary, 410);
    lv_obj_set_style_text_font(s_ui.summary, &lv_font_montserrat_24, 0);
    lv_obj_set_style_text_color(s_ui.summary, lv_color_hex(0xEAF6F7), 0);

    lv_obj_t *cells = lv_obj_create(content);
    lv_obj_set_flex_grow(cells, 1);
    lv_obj_set_height(cells, lv_pct(100));
    lv_obj_set_flex_flow(cells, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cells, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER);
    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        s_ui.rows[i] = lv_label_create(cells);
        lv_obj_set_style_text_font(s_ui.rows[i], &lv_font_montserrat_22, 0);
    }
    s_ui.timer = lv_timer_create(refresh, 500, NULL);
    refresh(s_ui.timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 130, 0, true);
}
