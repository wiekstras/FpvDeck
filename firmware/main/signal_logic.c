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
