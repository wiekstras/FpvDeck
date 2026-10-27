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

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static ads1115_t s_adc[2];
static bool s_ready[2];
static float s_calibration[BATTERY_MAX_CELLS];
static battery_pack_t s_latest;
static bool s_available;
static portMUX_TYPE s_lock = portMUX_INITIALIZER_UNLOCKED;

static void sampling_task(void *argument)
{
    (void)argument;
    while (true) {
        float adc_v[BATTERY_MAX_CELLS] = {0};
        bool read_ok = s_ready[0] && s_ready[1];
        for (unsigned tap = 0; tap < BATTERY_MAX_CELLS && read_ok; ++tap) {
            const unsigned device = tap < 4 ? 0 : 1;
            const unsigned channel = tap < 4 ? tap : tap - 4;
            read_ok = ads1115_read_single(&s_adc[device], channel,
                                           ADS1115_PGA_4V096,
                                           &adc_v[tap]) == ESP_OK;
        }
        battery_pack_t pack = {0};
        if (read_ok) battery_compute(adc_v, s_calibration, &pack);
        taskENTER_CRITICAL(&s_lock);
        s_latest = pack;
        s_available = read_ok && pack.cell_count > 0;
        taskEXIT_CRITICAL(&s_lock);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

esp_err_t battery_start_sampling(const ads1115_t adc[2],
                                 const bool ready[2],
                                 const float calibration[BATTERY_MAX_CELLS])
{
    if (adc == NULL || ready == NULL || calibration == NULL) {
        return ESP_ERR_INVALID_ARG;
    }
    memcpy(s_adc, adc, sizeof(s_adc));
    memcpy(s_ready, ready, sizeof(s_ready));
    memcpy(s_calibration, calibration, sizeof(s_calibration));
    return xTaskCreate(sampling_task, "battery_sample", 4096, NULL, 3, NULL) == pdPASS
               ? ESP_OK : ESP_ERR_NO_MEM;
}

bool battery_latest(battery_pack_t *pack)
{
    if (pack == NULL) return false;
    taskENTER_CRITICAL(&s_lock);
    *pack = s_latest;
    const bool available = s_available;
    taskEXIT_CRITICAL(&s_lock);
    return available;
}
#endif
