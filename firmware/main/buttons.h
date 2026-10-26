#pragma once

#include <stdbool.h>

typedef enum {
    BUTTON_UP = 0,
    BUTTON_OK,
    BUTTON_DOWN,
    BUTTON_COUNT
} button_id_t;

#ifdef ESP_PLATFORM
#include "esp_err.h"
esp_err_t buttons_init(void);
bool button_pressed(button_id_t button);
#endif
