#include "sim_panel.h"

#include "lvgl.h"
#include "sim_backend.h"

static lv_obj_t *s_panel;
static lv_obj_t *s_summary;

static void refresh(void)
{
    const sim_state_t *state = sim_state();
    lv_label_set_text_fmt(s_summary,
        "PACK %s | %uS%s\nVIDEO %s | DECK %s",
        state->pack_connected ? "CONNECTED" : "DISCONNECTED",
        state->cell_count, state->cell_imbalance ? " | IMBALANCED" : "",
        state->signal_lost ? "LOST" : (state->video_noise ? "NOISY" : "LOCKED"),
        state->deck_low ? "LOW" : "NORMAL");
}

static void close_panel(lv_event_t *event)
{
    (void)event;
    lv_obj_delete(s_panel);
    s_panel = NULL;
    s_summary = NULL;
}

static void toggle_pack(lv_event_t *event)
{
    (void)event;
    sim_set_pack_connected(!sim_state()->pack_connected);
    refresh();
}
static void toggle_imbalance(lv_event_t *event)
{
    (void)event;
    sim_set_cell_imbalance(!sim_state()->cell_imbalance);
    refresh();
}
static void toggle_signal(lv_event_t *event)
{
    (void)event;
    sim_set_signal_lost(!sim_state()->signal_lost);
    refresh();
}
static void toggle_noise(lv_event_t *event)
{
    (void)event;
    sim_set_video_noise(!sim_state()->video_noise);
    refresh();
}
static void toggle_deck(lv_event_t *event)
{
    (void)event;
    sim_set_deck_low(!sim_state()->deck_low);
    refresh();
}
static void next_cells(lv_event_t *event)
{
    (void)event;
    sim_set_cell_count(sim_state()->cell_count == 6 ? 2 : sim_state()->cell_count + 1);
    refresh();
}

static lv_obj_t *control(lv_obj_t *parent, const char *text, lv_event_cb_t cb)
{
    lv_obj_t *button = lv_button_create(parent);
    lv_obj_set_size(button, 260, 68);
    lv_obj_add_event_cb(button, cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, text);
    lv_obj_center(label);
    return button;
}

static void open_panel(lv_event_t *event)
{
    (void)event;
    if (s_panel != NULL) return;
    s_panel = lv_obj_create(lv_layer_top());
    lv_obj_set_size(s_panel, 1040, 610);
    lv_obj_center(s_panel);
    lv_obj_set_style_bg_color(s_panel, lv_color_hex(0x071016), 0);
    lv_obj_set_style_border_color(s_panel, lv_color_hex(0x51D9D0), 0);
    lv_obj_set_style_border_width(s_panel, 2, 0);
    lv_obj_set_style_radius(s_panel, 22, 0);
    lv_obj_set_flex_flow(s_panel, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(s_panel, LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER);
    lv_obj_t *title = lv_label_create(s_panel);
    lv_obj_set_width(title, 850);
    lv_label_set_text(title, "SIMULATOR CONTROL DECK");
    lv_obj_set_style_text_font(title, &lv_font_montserrat_24, 0);
    s_summary = lv_label_create(s_panel);
    lv_obj_set_width(s_summary, 850);
    control(s_panel, "Connect / disconnect pack", toggle_pack);
    control(s_panel, "Toggle cell imbalance", toggle_imbalance);
    control(s_panel, "Cycle 2S-6S", next_cells);
    control(s_panel, "Toggle video signal", toggle_signal);
    control(s_panel, "Toggle analog noise", toggle_noise);
    control(s_panel, "Toggle low deck battery", toggle_deck);
    control(s_panel, "Close", close_panel);
    refresh();
}

void sim_panel_create(void)
{
    lv_obj_t *button = lv_button_create(lv_layer_top());
    lv_obj_set_size(button, 94, 48);
    lv_obj_align(button, LV_ALIGN_TOP_RIGHT, -18, 14);
    lv_obj_set_style_bg_color(button, lv_color_hex(0x76520D), 0);
    lv_obj_add_event_cb(button, open_panel, LV_EVENT_CLICKED, NULL);
    lv_obj_t *label = lv_label_create(button);
    lv_label_set_text(label, "SIM");
    lv_obj_center(label);
}
