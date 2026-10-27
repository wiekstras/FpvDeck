#include "apps.h"
#include "sim_panel.h"

#include "lvgl.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    const bool smoke = argc > 1 && strcmp(argv[1], "--smoke-test") == 0;
    lv_init();
    lv_display_t *display = lv_sdl_window_create(1280, 720);
    if (display == NULL) {
        fprintf(stderr, "Unable to create SDL display\n");
        return 1;
    }
    (void)lv_sdl_mouse_create();
    (void)lv_sdl_mousewheel_create();
    lv_indev_t *keyboard = lv_sdl_keyboard_create();
    lv_group_t *group = lv_group_create();
    lv_group_set_default(group);
    lv_indev_set_group(keyboard, group);

    app_launcher_show();
    sim_panel_create();
    const uint32_t started = lv_tick_get();
    while (!smoke || lv_tick_elaps(started) < 500U) {
        const uint32_t delay = lv_timer_handler();
        lv_delay_ms(delay < 2U ? 2U : (delay > 16U ? 16U : delay));
    }
    return 0;
}

