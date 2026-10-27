#include "apps.h"
#include "battery.h"
#include "ui_theme.h"

#include <string.h>

typedef struct {
    lv_obj_t *pack_type;
    lv_obj_t *total;
    lv_obj_t *status;
    lv_obj_t *cell_count;
    lv_obj_t *delta;
    lv_obj_t *voltage_range;
    lv_obj_t *cell_values[BATTERY_MAX_CELLS];
    lv_obj_t *cell_bars[BATTERY_MAX_CELLS];
    lv_timer_t *timer;
} battery_ui_t;

static battery_ui_t s_ui;

static uint32_t cell_color(float voltage)
{
    if (voltage > 4.25f) return UI_COLOR_DANGER;
    if (voltage < 3.50f) return UI_COLOR_WARNING;
    return UI_COLOR_SUCCESS;
}

static void set_no_pack(void)
{
    lv_label_set_text(s_ui.pack_type, "BALANCE INPUT");
    lv_label_set_text(s_ui.total, "--.-- V");
    lv_label_set_text(s_ui.status, "CONNECT 2S-6S PACK");
    lv_obj_set_style_text_color(s_ui.status, lv_color_hex(UI_COLOR_MUTED), 0);
    lv_label_set_text(s_ui.cell_count, "--");
    lv_label_set_text(s_ui.delta, "-- mV");
    lv_label_set_text(s_ui.voltage_range, "-- / -- V");
    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        lv_label_set_text(s_ui.cell_values[i], "--.--- V");
        lv_obj_set_style_text_color(s_ui.cell_values[i],
                                    lv_color_hex(UI_COLOR_MUTED), 0);
        lv_bar_set_value(s_ui.cell_bars[i], 3000, LV_ANIM_OFF);
        lv_obj_set_style_bg_color(s_ui.cell_bars[i],
                                  lv_color_hex(UI_COLOR_MUTED),
                                  LV_PART_INDICATOR);
    }
}

static void refresh(lv_timer_t *timer)
{
    (void)timer;
    battery_pack_t pack;
    if (!battery_latest(&pack)) {
        set_no_pack();
        return;
    }

    const char *status = pack.valid ? "PACK HEALTHY" : "CHECK BALANCE WIRING";
    uint32_t status_color = pack.valid ? UI_COLOR_SUCCESS : UI_COLOR_DANGER;
    bool storage = pack.valid;
    for (unsigned i = 0; i < pack.cell_count; ++i) {
        if (pack.cells_v[i] < 3.50f || pack.cells_v[i] > 4.25f) {
            status = "VOLTAGE WARNING";
            status_color = UI_COLOR_WARNING;
        }
        if (!battery_is_storage_voltage(pack.cells_v[i])) storage = false;
    }
    if (storage) {
        status = "STORAGE READY";
        status_color = UI_COLOR_BLUE;
    }

    lv_label_set_text_fmt(s_ui.pack_type, "%uS LIPO / BALANCE INPUT",
                          pack.cell_count);
    lv_label_set_text_fmt(s_ui.total, "%.2f V", pack.total_v);
    lv_label_set_text(s_ui.status, status);
    lv_obj_set_style_text_color(s_ui.status, lv_color_hex(status_color), 0);
    lv_label_set_text_fmt(s_ui.cell_count, "%u CELLS", pack.cell_count);
    lv_label_set_text_fmt(s_ui.delta, "%.0f mV", pack.delta_v * 1000.0f);
    lv_label_set_text_fmt(s_ui.voltage_range, "%.3f / %.3f V",
                          pack.minimum_v, pack.maximum_v);

    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        if (i < pack.cell_count) {
            const uint32_t color = cell_color(pack.cells_v[i]);
            lv_label_set_text_fmt(s_ui.cell_values[i], "%.3f V",
                                  pack.cells_v[i]);
            lv_obj_set_style_text_color(s_ui.cell_values[i],
                                        lv_color_hex(color), 0);
            int32_t millivolts = (int32_t)(pack.cells_v[i] * 1000.0f);
            if (millivolts < 3000) millivolts = 3000;
            if (millivolts > 4300) millivolts = 4300;
            lv_bar_set_value(s_ui.cell_bars[i], millivolts, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(s_ui.cell_bars[i], lv_color_hex(color),
                                      LV_PART_INDICATOR);
        } else {
            lv_label_set_text(s_ui.cell_values[i], "NOT PRESENT");
            lv_obj_set_style_text_color(s_ui.cell_values[i],
                                        lv_color_hex(UI_COLOR_MUTED), 0);
            lv_bar_set_value(s_ui.cell_bars[i], 3000, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(s_ui.cell_bars[i],
                                      lv_color_hex(UI_COLOR_MUTED),
                                      LV_PART_INDICATOR);
        }
    }
}

static void screen_deleted(lv_event_t *event)
{
    (void)event;
    if (s_ui.timer != NULL) lv_timer_delete(s_ui.timer);
    memset(&s_ui, 0, sizeof(s_ui));
}

static lv_obj_t *make_cell_card(lv_obj_t *parent, unsigned index,
                                lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_pos(card, x, y);
    lv_obj_set_size(card, 378, 166);
    ui_surface_style(card);
    lv_obj_set_style_pad_all(card, 20, 0);

    char caption[16];
    lv_snprintf(caption, sizeof(caption), "CELL %u", index + 1U);
    lv_obj_t *name = lv_label_create(card);
    lv_label_set_text(name, caption);
    ui_label_overline(name);
    lv_obj_align(name, LV_ALIGN_TOP_LEFT, 0, 0);

    s_ui.cell_values[index] = lv_label_create(card);
    lv_label_set_text(s_ui.cell_values[index], "--.--- V");
    ui_label_title(s_ui.cell_values[index]);
    lv_obj_align(s_ui.cell_values[index], LV_ALIGN_LEFT_MID, 0, -2);

    s_ui.cell_bars[index] = lv_bar_create(card);
    lv_obj_set_size(s_ui.cell_bars[index], 338, 10);
    lv_bar_set_range(s_ui.cell_bars[index], 3000, 4300);
    ui_bar_style(s_ui.cell_bars[index], UI_COLOR_SUCCESS);
    lv_obj_align(s_ui.cell_bars[index], LV_ALIGN_BOTTOM_LEFT, 0, 0);
    return card;
}

void app_battery_show(void)
{
    lv_obj_t *screen = app_screen_create("Battery checker");
    lv_obj_add_event_cb(screen, screen_deleted, LV_EVENT_DELETE, NULL);

    lv_obj_t *content = lv_obj_create(screen);
    lv_obj_set_width(content, lv_pct(100));
    lv_obj_set_flex_grow(content, 1);
    lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(content, 0, 0);
    lv_obj_set_style_pad_all(content, 0, 0);

    lv_obj_t *summary = lv_obj_create(content);
    lv_obj_set_pos(summary, 24, 16);
    lv_obj_set_size(summary, 390, 616);
    ui_surface_style(summary);
    lv_obj_set_style_pad_all(summary, 26, 0);
    lv_obj_set_style_border_color(summary, lv_color_hex(UI_COLOR_ACCENT), 0);

    s_ui.pack_type = lv_label_create(summary);
    lv_label_set_text(s_ui.pack_type, "BALANCE INPUT");
    ui_label_overline(s_ui.pack_type);
    lv_obj_align(s_ui.pack_type, LV_ALIGN_TOP_LEFT, 0, 0);
    s_ui.total = lv_label_create(summary);
    lv_label_set_text(s_ui.total, "--.-- V");
    lv_obj_set_style_text_font(s_ui.total, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_color(s_ui.total, lv_color_hex(UI_COLOR_TEXT), 0);
    lv_obj_align(s_ui.total, LV_ALIGN_TOP_LEFT, 0, 58);
    s_ui.status = lv_label_create(summary);
    lv_label_set_text(s_ui.status, "CONNECT 2S-6S PACK");
    lv_obj_set_style_text_font(s_ui.status, &lv_font_montserrat_16, 0);
    lv_obj_align(s_ui.status, LV_ALIGN_TOP_LEFT, 0, 104);

    lv_obj_t *divider = lv_obj_create(summary);
    lv_obj_set_pos(divider, 0, 160);
    lv_obj_set_size(divider, 338, 1);
    lv_obj_set_style_bg_color(divider, lv_color_hex(UI_COLOR_BORDER), 0);
    lv_obj_set_style_border_width(divider, 0, 0);

    lv_obj_t *metric = ui_metric_create(summary, "PACK SIZE", &s_ui.cell_count);
    lv_obj_set_pos(metric, 0, 190);
    lv_obj_set_size(metric, 338, 104);
    metric = ui_metric_create(summary, "CELL DELTA", &s_ui.delta);
    lv_obj_set_pos(metric, 0, 312);
    lv_obj_set_size(metric, 338, 104);
    metric = ui_metric_create(summary, "LOW / HIGH", &s_ui.voltage_range);
    lv_obj_set_pos(metric, 0, 434);
    lv_obj_set_size(metric, 338, 104);
    lv_obj_t *hint = lv_label_create(summary);
    lv_label_set_text(hint, "Storage target  3.80-3.85 V / cell");
    ui_label_muted(hint);
    lv_obj_align(hint, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    lv_obj_t *cells = lv_obj_create(content);
    lv_obj_set_pos(cells, 434, 16);
    lv_obj_set_size(cells, 822, 616);
    lv_obj_set_style_bg_opa(cells, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cells, 0, 0);
    lv_obj_set_style_pad_all(cells, 0, 0);
    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        const lv_coord_t x = (i % 2U) * 396;
        const lv_coord_t y = (i / 2U) * 184;
        (void)make_cell_card(cells, i, x, y);
    }

    s_ui.timer = lv_timer_create(refresh, 500, NULL);
    refresh(s_ui.timer);
    lv_screen_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_LEFT, 100, 0, true);
}
