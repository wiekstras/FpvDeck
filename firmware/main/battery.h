#pragma once

#include <stdbool.h>
#include <stddef.h>
#include "fpvdeck_platform.h"

#define BATTERY_MAX_CELLS 6
#define BATTERY_DIVIDER_RATIO ((100.0f + 12.0f) / 12.0f)

typedef struct {
    float taps_v[BATTERY_MAX_CELLS];
    float cells_v[BATTERY_MAX_CELLS];
    unsigned cell_count;
    float total_v;
    float minimum_v;
    float maximum_v;
    float delta_v;
    bool valid;
} battery_pack_t;

unsigned battery_detect_cell_count(const float taps_v[BATTERY_MAX_CELLS],
                                   float threshold_v);
void battery_compute(const float adc_v[BATTERY_MAX_CELLS],
                     const float calibration[BATTERY_MAX_CELLS],
                     battery_pack_t *pack);
const char *battery_cell_state(float voltage);
bool battery_is_storage_voltage(float voltage);

#ifdef ESP_PLATFORM
#include "ads1115.h"
esp_err_t battery_start_sampling(const ads1115_t adc[2],
                                 const bool ready[2],
                                 const float calibration[BATTERY_MAX_CELLS]);
#endif
bool battery_latest(battery_pack_t *pack);
