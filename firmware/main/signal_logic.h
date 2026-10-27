#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "fpvdeck_platform.h"

typedef struct {
    float smoothed;
    float peak;
    float alpha;
    bool initialized;
} rssi_filter_t;

typedef struct {
    uint16_t frequency_mhz;
    float rssi;
} scanner_sample_t;

void rssi_filter_reset(rssi_filter_t *filter, float alpha);
float rssi_filter_update(rssi_filter_t *filter, float sample);
void scanner_strongest(const scanner_sample_t *samples, size_t sample_count,
                       scanner_sample_t *out, size_t out_count);
unsigned finder_beep_interval_ms(float rssi_v);

#ifdef ESP_PLATFORM
#include "ads1115.h"
esp_err_t signal_start_sampling(const ads1115_t *adc, bool ready);
#endif
esp_err_t signal_read_raw(float *rssi_a_v, float *rssi_b_v);
bool signal_latest(float *rssi_a_v, float *rssi_b_v, float *peak_v,
                   bool *active_rx_a);
