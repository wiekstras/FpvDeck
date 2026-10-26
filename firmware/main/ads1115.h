#pragma once

#include <stdint.h>

#ifdef ESP_PLATFORM
#include "driver/i2c_master.h"
#include "esp_err.h"

typedef enum {
    ADS1115_PGA_4V096 = 1,
    ADS1115_PGA_2V048 = 2,
} ads1115_pga_t;

typedef struct {
    i2c_master_dev_handle_t device;
    uint8_t address;
} ads1115_t;

esp_err_t ads1115_init(ads1115_t *adc, i2c_master_bus_handle_t bus,
                       uint8_t address);
esp_err_t ads1115_read_single(ads1115_t *adc, unsigned channel,
                              ads1115_pga_t pga, float *volts);
#endif
