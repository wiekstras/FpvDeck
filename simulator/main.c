#include "apps.h"
#include "sim_panel.h"

#include "lvgl.h"
#include <SDL.h>

#include <stdbool.h>
#include <stdlib.h>
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
    fprintf(stderr, "Usage: %s [--screen NAME] [--smoke-test] "
                    "[--screenshot FILE.bmp] [--duration-ms N]\n", program);
    fprintf(stderr, "Screens: launcher, battery, monitor, scanner, finder, "
                    "stream, settings\n");
}

static bool save_screenshot(lv_display_t *display, const char *path)
{
    SDL_Renderer *renderer = lv_sdl_window_get_renderer(display);
    int width = 0;
    int height = 0;
    if (renderer == NULL || SDL_GetRendererOutputSize(renderer, &width, &height) != 0) {
        fprintf(stderr, "Unable to query SDL renderer: %s\n", SDL_GetError());
        return false;
    }
    SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(
        0, width, height, 24, SDL_PIXELFORMAT_RGB24);
    if (surface == NULL) {
        fprintf(stderr, "Unable to allocate screenshot: %s\n", SDL_GetError());
        return false;
    }
    const int read_result = SDL_RenderReadPixels(
        renderer, NULL, surface->format->format, surface->pixels, surface->pitch);
    const int save_result = read_result == 0 ? SDL_SaveBMP(surface, path) : -1;
    if (read_result != 0 || save_result != 0) {
        fprintf(stderr, "Unable to save screenshot %s: %s\n", path, SDL_GetError());
    }
    SDL_FreeSurface(surface);
    return read_result == 0 && save_result == 0;
}

int main(int argc, char **argv)
{
    bool smoke = false;
    const char *screen_name = "launcher";
    const char *screenshot_path = NULL;
    uint32_t duration_ms = 500U;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--smoke-test") == 0) {
            smoke = true;
        } else if (strcmp(argv[i], "--screen") == 0 && i + 1 < argc) {
            screen_name = argv[++i];
        } else if (strcmp(argv[i], "--screenshot") == 0 && i + 1 < argc) {
            screenshot_path = argv[++i];
        } else if (strcmp(argv[i], "--duration-ms") == 0 && i + 1 < argc) {
            char *end = NULL;
            const unsigned long value = strtoul(argv[++i], &end, 10);
            if (end == NULL || *end != '\0' || value < 100UL || value > 60000UL) {
                fprintf(stderr, "Invalid duration (100..60000 ms required)\n");
                return 2;
            }
            duration_ms = (uint32_t)value;
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
    while ((!smoke && screenshot_path == NULL) ||
           lv_tick_elaps(started) < duration_ms) {
        const uint32_t delay = lv_timer_handler();
        lv_delay_ms(delay < 2U ? 2U : (delay > 16U ? 16U : delay));
    }
    if (screenshot_path != NULL && !save_screenshot(display, screenshot_path)) return 1;
    return 0;
}
