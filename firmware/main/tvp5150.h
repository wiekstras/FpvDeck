#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "fpvdeck_platform.h"

#define TVP5150_I2C_ADDRESS 0x5CU

typedef struct {
    bool horizontal_lock;
    bool vertical_lock;
    bool color_lock;
    bool field_rate_50_hz;
    uint8_t standard_code;
    uint8_t raw_status_88;
    uint8_t raw_status_8c;
} tvp5150_status_t;

typedef enum {
    TVP5150_STANDARD_AUTO = 0,
    TVP5150_STANDARD_NTSC,
    TVP5150_STANDARD_PAL,
} tvp5150_standard_t;

#ifdef ESP_PLATFORM
#include "driver/i2c_master.h"

esp_err_t tvp5150_init(i2c_master_bus_handle_t bus);
esp_err_t tvp5150_read_id(uint8_t *msb, uint8_t *lsb);
esp_err_t tvp5150_read_status(tvp5150_status_t *status);
#endif
esp_err_t tvp5150_set_standard(tvp5150_standard_t standard);
