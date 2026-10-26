#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef enum {
    RX5808_BAND_A = 0,
    RX5808_BAND_B,
    RX5808_BAND_E,
    RX5808_BAND_F,
    RX5808_BAND_R,
    RX5808_BAND_L,
    RX5808_BAND_COUNT
} rx5808_band_t;

typedef enum {
    RX5808_MODULE_A = 0,
    RX5808_MODULE_B = 1
} rx5808_module_t;

extern const uint16_t rx5808_channels_mhz[RX5808_BAND_COUNT][8];

uint16_t rx5808_frequency_register(uint16_t frequency_mhz);
uint32_t rx5808_write_frame(uint8_t register_address, uint32_t data20);
bool rx5808_channel_frequency(rx5808_band_t band, unsigned channel,
                              uint16_t *frequency_mhz);

#ifdef ESP_PLATFORM
#include "esp_err.h"
esp_err_t rx5808_init(void);
esp_err_t rx5808_set_frequency(rx5808_module_t module, uint16_t frequency_mhz);
esp_err_t rx5808_set_both(uint16_t frequency_mhz);
#endif
