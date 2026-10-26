#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
