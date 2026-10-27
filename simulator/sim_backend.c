#include "sim_backend.h"

#include "battery.h"
#include "buzzer.h"
#include "deck_power.h"
#include "platform.h"
#include "rx5808.h"
#include "signal_logic.h"
#include "tvp5150.h"

#include <math.h>
#include <string.h>
#include <time.h>

static sim_state_t s_state = {
    .pack_connected = true,
    .signal_lost = false,
    .video_noise = false,
    .deck_low = false,
    .cell_imbalance = false,
    .cell_count = 4,
    .tuned_frequency_mhz = 5658,
};
static float s_peak;

const sim_state_t *sim_state(void) { return &s_state; }
void sim_set_pack_connected(bool enabled) { s_state.pack_connected = enabled; }
void sim_set_signal_lost(bool enabled) { s_state.signal_lost = enabled; }
void sim_set_video_noise(bool enabled) { s_state.video_noise = enabled; }
void sim_set_deck_low(bool enabled) { s_state.deck_low = enabled; }
void sim_set_cell_imbalance(bool enabled) { s_state.cell_imbalance = enabled; }
void sim_set_cell_count(unsigned count)
{
    if (count >= 2 && count <= BATTERY_MAX_CELLS) s_state.cell_count = count;
}

esp_err_t rx5808_init(void) { return ESP_OK; }
esp_err_t rx5808_set_frequency(rx5808_module_t module, uint16_t frequency_mhz)
{
    (void)module;
    s_state.tuned_frequency_mhz = frequency_mhz;
    return ESP_OK;
}
esp_err_t rx5808_set_both(uint16_t frequency_mhz)
{
    s_state.tuned_frequency_mhz = frequency_mhz;
    return ESP_OK;
}

static float receiver_level(uint16_t frequency, unsigned receiver)
{
    if (s_state.signal_lost) return 0.05f;
    static const uint16_t transmitters[] = {5658, 5800, 5880};
    static const float strengths[] = {1.72f, 1.12f, 0.76f};
    float result = 0.12f;
    for (unsigned i = 0; i < 3; ++i) {
        const float delta = fabsf((float)frequency - transmitters[i]);
        const float candidate = strengths[i] * expf(-delta / 22.0f);
        if (candidate > result) result = candidate;
    }
    const float wobble = 0.025f * sinf((float)clock() / 90000.0f + receiver);
    return result + wobble + (receiver == 0 ? 0.04f : 0.0f);
}

esp_err_t signal_read_raw(float *rssi_a_v, float *rssi_b_v)
{
    if (rssi_a_v == NULL || rssi_b_v == NULL) return ESP_ERR_INVALID_ARG;
    *rssi_a_v = receiver_level(s_state.tuned_frequency_mhz, 0);
    *rssi_b_v = receiver_level(s_state.tuned_frequency_mhz, 1);
    return ESP_OK;
}

bool signal_latest(float *a, float *b, float *peak, bool *active_a)
{
    if (a == NULL || b == NULL || peak == NULL || active_a == NULL) return false;
    (void)signal_read_raw(a, b);
    const float current = *a > *b ? *a : *b;
    if (current > s_peak) s_peak = current;
    *peak = s_peak;
    *active_a = *a >= *b;
    return true;
}

bool battery_latest(battery_pack_t *pack)
{
    if (pack == NULL || !s_state.pack_connected) return false;
    memset(pack, 0, sizeof(*pack));
    pack->cell_count = s_state.cell_count;
    pack->minimum_v = 99.0f;
    for (unsigned i = 0; i < pack->cell_count; ++i) {
        const float cell = 3.84f + 0.008f * (float)i -
                           ((s_state.cell_imbalance && i == 1) ? 0.31f : 0.0f);
        pack->cells_v[i] = cell;
        pack->total_v += cell;
        pack->taps_v[i] = pack->total_v;
        if (cell < pack->minimum_v) pack->minimum_v = cell;
        if (cell > pack->maximum_v) pack->maximum_v = cell;
    }
    pack->delta_v = pack->maximum_v - pack->minimum_v;
    pack->valid = true;
    return true;
}

bool deck_power_latest(float *voltage)
{
    if (voltage == NULL) return false;
    *voltage = s_state.deck_low ? 3.42f : 3.91f;
    return true;
}

esp_err_t buzzer_init(void) { return ESP_OK; }
void buzzer_tone(unsigned frequency_hz, unsigned duty_percent)
{
    (void)frequency_hz;
    (void)duty_percent;
}
void buzzer_off(void) {}
esp_err_t tvp5150_set_standard(tvp5150_standard_t standard)
{
    (void)standard;
    return ESP_OK;
}
esp_err_t platform_display_set_brightness(uint8_t percent)
{
    (void)percent;
    return ESP_OK;
}

