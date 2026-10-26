#include "battery.h"

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

unsigned battery_detect_cell_count(const float taps_v[BATTERY_MAX_CELLS],
                                   float threshold_v)
{
    unsigned highest = 0;
    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        if (taps_v[i] > threshold_v) highest = i + 1;
    }
    return highest;
}

void battery_compute(const float adc_v[BATTERY_MAX_CELLS],
                     const float calibration[BATTERY_MAX_CELLS],
                     battery_pack_t *pack)
{
    memset(pack, 0, sizeof(*pack));
    for (unsigned i = 0; i < BATTERY_MAX_CELLS; ++i) {
        const float cal = calibration[i] > 0.0f ? calibration[i] : 1.0f;
        pack->taps_v[i] = adc_v[i] * BATTERY_DIVIDER_RATIO * cal;
    }
    pack->cell_count = battery_detect_cell_count(pack->taps_v, 1.0f);
    pack->valid = pack->cell_count > 0;
    pack->minimum_v = FLT_MAX;
    for (unsigned i = 0; i < pack->cell_count; ++i) {
        pack->cells_v[i] = i == 0 ? pack->taps_v[0]
                                  : pack->taps_v[i] - pack->taps_v[i - 1];
        if (!isfinite(pack->cells_v[i]) || pack->cells_v[i] < 1.0f ||
            pack->cells_v[i] > 5.0f) {
            pack->valid = false;
        }
        if (pack->cells_v[i] < pack->minimum_v) pack->minimum_v = pack->cells_v[i];
        if (pack->cells_v[i] > pack->maximum_v) pack->maximum_v = pack->cells_v[i];
    }
    if (pack->cell_count == 0) {
        pack->minimum_v = 0.0f;
        pack->valid = false;
        return;
    }
    pack->total_v = pack->taps_v[pack->cell_count - 1];
    pack->delta_v = pack->maximum_v - pack->minimum_v;
}

const char *battery_cell_state(float voltage)
{
    if (voltage > 4.25f) return "ERROR_HIGH";
    if (voltage < 3.50f) return "WARNING_LOW";
    return "OK";
}

bool battery_is_storage_voltage(float voltage)
{
    return voltage >= 3.80f && voltage <= 3.85f;
}
