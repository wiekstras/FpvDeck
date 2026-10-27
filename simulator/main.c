#include "apps.h"
#include "sim_panel.h"

#include "lvgl.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef void (*screen_show_fn_t)(void);

typedef struct {
    const char *name;
    screen_show_fn_t show;
} screen_entry_t;

static const screen_entry_t s_screens[] = {
    {"launcher", app_launcher_show},
    {"battery", app_battery_show},
    {"monitor", app_monitor_show},
    {"scanner", app_scanner_show},
    {"finder", app_finder_show},
    {"stream", app_stream_show},
    {"settings", app_settings_show},
};

static screen_show_fn_t find_screen(const char *name)
{
    for (size_t i = 0; i < sizeof(s_screens) / sizeof(s_screens[0]); ++i) {
        if (strcmp(name, s_screens[i].name) == 0) return s_screens[i].show;
    }
    return NULL;
}

static void usage(const char *program)
{
    fprintf(stderr, "Usage: %s [--screen NAME] [--smoke-test]\n", program);
    fprintf(stderr, "Screens: launcher, battery, monitor, scanner, finder, "
                    "stream, settings\n");
}

int main(int argc, char **argv)
{
    bool smoke = false;
    const char *screen_name = "launcher";
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--smoke-test") == 0) {
            smoke = true;
        } else if (strcmp(argv[i], "--screen") == 0 && i + 1 < argc) {
            screen_name = argv[++i];
        } else {
            usage(argv[0]);
            return 2;
        }
    }
    screen_show_fn_t show = find_screen(screen_name);
    if (show == NULL) {
        fprintf(stderr, "Unknown simulator screen: %s\n", screen_name);
        usage(argv[0]);
        return 2;
    }

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

    show();
    sim_panel_create();
    const uint32_t started = lv_tick_get();
    while (!smoke || lv_tick_elaps(started) < 500U) {
        const uint32_t delay = lv_timer_handler();
        lv_delay_ms(delay < 2U ? 2U : (delay > 16U ? 16U : delay));
    }
    return 0;
}
