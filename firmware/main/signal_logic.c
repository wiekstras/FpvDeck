#include "signal_logic.h"

#include <stdbool.h>

void rssi_filter_reset(rssi_filter_t *filter, float alpha)
{
    filter->smoothed = 0.0f;
    filter->peak = 0.0f;
    filter->alpha = alpha > 0.0f && alpha <= 1.0f ? alpha : 0.25f;
    filter->initialized = false;
}

float rssi_filter_update(rssi_filter_t *filter, float sample)
{
    if (!filter->initialized) {
        filter->smoothed = sample;
        filter->peak = sample;
        filter->initialized = true;
    } else {
        filter->smoothed += filter->alpha * (sample - filter->smoothed);
        if (filter->smoothed > filter->peak) filter->peak = filter->smoothed;
    }
    return filter->smoothed;
}

void scanner_strongest(const scanner_sample_t *samples, size_t sample_count,
                       scanner_sample_t *out, size_t out_count)
{
    if (samples == NULL || out == NULL || out_count == 0) return;
    for (size_t i = 0; i < out_count; ++i) out[i] = (scanner_sample_t){0, -1.0f};
    for (size_t i = 0; i < sample_count; ++i) {
        for (size_t rank = 0; rank < out_count; ++rank) {
            if (samples[i].rssi <= out[rank].rssi) continue;
            for (size_t move = out_count - 1; move > rank; --move) out[move] = out[move - 1];
            out[rank] = samples[i];
            break;
        }
    }
}

unsigned finder_beep_interval_ms(float rssi_v)
{
    if (rssi_v < 0.0f) rssi_v = 0.0f;
    if (rssi_v > 2.048f) rssi_v = 2.048f;
    const float normalized = rssi_v / 2.048f;
    return (unsigned)(900.0f - 750.0f * normalized + 0.5f);
}

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

static ads1115_t s_rssi_adc;
static bool s_rssi_ready;
static float s_rssi_a_v;
static float s_rssi_b_v;
static float s_peak_v;
static bool s_active_rx_a;
static bool s_available;
static SemaphoreHandle_t s_adc_mutex;
static portMUX_TYPE s_signal_lock = portMUX_INITIALIZER_UNLOCKED;

esp_err_t signal_read_raw(float *rssi_a_v, float *rssi_b_v)
{
    if (rssi_a_v == NULL || rssi_b_v == NULL) return ESP_ERR_INVALID_ARG;
    if (!s_rssi_ready || s_adc_mutex == NULL) return ESP_ERR_INVALID_STATE;
    if (xSemaphoreTake(s_adc_mutex, pdMS_TO_TICKS(100)) != pdTRUE) {
        return ESP_ERR_TIMEOUT;
    }
    esp_err_t result = ads1115_read_single(&s_rssi_adc, 0,
                                            ADS1115_PGA_2V048, rssi_a_v);
    if (result == ESP_OK) {
        result = ads1115_read_single(&s_rssi_adc, 1,
                                     ADS1115_PGA_2V048, rssi_b_v);
    }
    xSemaphoreGive(s_adc_mutex);
    return result;
}

static void signal_task(void *argument)
{
    (void)argument;
    rssi_filter_t a;
    rssi_filter_t b;
    rssi_filter_reset(&a, 0.25f);
    rssi_filter_reset(&b, 0.25f);
    while (true) {
        float raw_a = 0.0f;
        float raw_b = 0.0f;
        const bool ok = signal_read_raw(&raw_a, &raw_b) == ESP_OK;
        if (ok) {
            const float filtered_a = rssi_filter_update(&a, raw_a);
            const float filtered_b = rssi_filter_update(&b, raw_b);
            taskENTER_CRITICAL(&s_signal_lock);
            s_rssi_a_v = filtered_a;
            s_rssi_b_v = filtered_b;
            s_peak_v = a.peak > b.peak ? a.peak : b.peak;
            s_active_rx_a = gpio_get_level(PIN_ACTIVE_RX) != 0;
            s_available = true;
            taskEXIT_CRITICAL(&s_signal_lock);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

esp_err_t signal_start_sampling(const ads1115_t *adc, bool ready)
{
    if (adc == NULL) return ESP_ERR_INVALID_ARG;
    s_rssi_adc = *adc;
    s_rssi_ready = ready;
    s_adc_mutex = xSemaphoreCreateMutex();
    if (s_adc_mutex == NULL) return ESP_ERR_NO_MEM;
    const gpio_config_t cfg = {
        .pin_bit_mask = 1ULL << PIN_ACTIVE_RX,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    if (gpio_config(&cfg) != ESP_OK) return ESP_FAIL;
    if (xTaskCreate(signal_task, "rssi_sample", 4096, NULL, 3, NULL) != pdPASS) {
        vSemaphoreDelete(s_adc_mutex);
        s_adc_mutex = NULL;
        return ESP_ERR_NO_MEM;
    }
    return ESP_OK;
}

bool signal_latest(float *rssi_a_v, float *rssi_b_v, float *peak_v,
                   bool *active_rx_a)
{
    if (rssi_a_v == NULL || rssi_b_v == NULL || peak_v == NULL ||
        active_rx_a == NULL) return false;
    taskENTER_CRITICAL(&s_signal_lock);
    *rssi_a_v = s_rssi_a_v;
    *rssi_b_v = s_rssi_b_v;
    *peak_v = s_peak_v;
    *active_rx_a = s_active_rx_a;
    const bool available = s_available;
    taskEXIT_CRITICAL(&s_signal_lock);
    return available;
}
#endif
