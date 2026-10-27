#pragma once

#include <stdbool.h>
#include "fpvdeck_platform.h"

typedef enum {
    DECK_POWER_NO_BATTERY,
    DECK_POWER_SHUTDOWN,
    DECK_POWER_LOW,
    DECK_POWER_NORMAL,
} deck_power_state_t;

deck_power_state_t deck_power_classify(float voltage);
#ifdef ESP_PLATFORM
esp_err_t deck_power_init(void);
esp_err_t deck_power_read_voltage(float *voltage);
#endif
bool deck_power_latest(float *voltage);
