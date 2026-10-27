#include "tvp5150.h"

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TVP_TIMEOUT_MS 50
static i2c_master_dev_handle_t s_tvp;

static esp_err_t write_reg(uint8_t reg, uint8_t value)
{
    const uint8_t data[] = {reg, value};
    ESP_RETURN_ON_ERROR(i2c_master_transmit(s_tvp, data, sizeof(data),
                                             TVP_TIMEOUT_MS),
                        "tvp5150", "write register");
    /* SLES209E section 3.15.3: internal processor may need up to 64 us. */
    if (reg <= 0x8FU) esp_rom_delay_us(64);
    return ESP_OK;
}

static esp_err_t read_reg(uint8_t reg, uint8_t *value)
{
    return i2c_master_transmit_receive(s_tvp, &reg, 1, value, 1,
                                       TVP_TIMEOUT_MS);
}

esp_err_t tvp5150_read_id(uint8_t *msb, uint8_t *lsb)
{
    if (s_tvp == NULL || msb == NULL || lsb == NULL) return ESP_ERR_INVALID_ARG;
    ESP_RETURN_ON_ERROR(read_reg(0x80, msb), "tvp5150", "ID MSB");
    return read_reg(0x81, lsb);
}

esp_err_t tvp5150_init(i2c_master_bus_handle_t bus)
{
    if (bus == NULL) return ESP_ERR_INVALID_ARG;
    const gpio_config_t reset_cfg = {
        .pin_bit_mask = 1ULL << PIN_TVP_RESETB,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&reset_cfg), "tvp5150", "reset gpio");

    /* SLES209E: supplies stable >=20 ms, RESETB low >=500 ns, then >=200 us. */
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(PIN_TVP_RESETB, 0);
    esp_rom_delay_us(2);
    gpio_set_level(PIN_TVP_RESETB, 1);
    esp_rom_delay_us(200);

    const i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = TVP5150_I2C_ADDRESS,
        .scl_speed_hz = 100000,
        .scl_wait_us = 1000,
    };
    ESP_RETURN_ON_ERROR(i2c_master_bus_add_device(bus, &dev_cfg, &s_tvp),
                        "tvp5150", "add I2C device");

    uint8_t id_msb = 0, id_lsb = 0;
    ESP_RETURN_ON_ERROR(tvp5150_read_id(&id_msb, &id_lsb),
                        "tvp5150", "read device ID");
    if (id_msb != 0x51 || id_lsb != 0x50) return ESP_ERR_NOT_FOUND;

    ESP_RETURN_ON_ERROR(write_reg(0x28, 0x00), "tvp5150", "autoswitch");
    ESP_RETURN_ON_ERROR(write_reg(0x0D, 0x00), "tvp5150", "discrete YCbCr");
    ESP_RETURN_ON_ERROR(write_reg(0x0F, 0x00), "tvp5150", "VSYNC pin");
    return write_reg(0x03, 0x0D); /* YCbCr + discrete syncs + SCLK on */
}

esp_err_t tvp5150_read_status(tvp5150_status_t *status)
{
    if (s_tvp == NULL || status == NULL) return ESP_ERR_INVALID_ARG;
    ESP_RETURN_ON_ERROR(read_reg(0x88, &status->raw_status_88),
                        "tvp5150", "status 1");
    ESP_RETURN_ON_ERROR(read_reg(0x8C, &status->raw_status_8c),
                        "tvp5150", "status 5");
    status->field_rate_50_hz = (status->raw_status_88 & (1U << 5U)) != 0;
    status->color_lock = (status->raw_status_88 & (1U << 3U)) != 0;
    status->vertical_lock = (status->raw_status_88 & (1U << 2U)) != 0;
    status->horizontal_lock = (status->raw_status_88 & (1U << 1U)) != 0;
    status->standard_code = status->raw_status_8c & 0x0FU;
    return ESP_OK;
}

esp_err_t tvp5150_set_standard(tvp5150_standard_t standard)
{
    if (s_tvp == NULL) return ESP_ERR_INVALID_STATE;
    uint8_t value;
    switch (standard) {
    case TVP5150_STANDARD_AUTO:
        value = 0x00;
        break;
    case TVP5150_STANDARD_NTSC:
        value = 0x02; /* SLES209E 3.21.32: NTSC M/J BT.601. */
        break;
    case TVP5150_STANDARD_PAL:
        value = 0x04; /* SLES209E 3.21.32: PAL B/G/H/I/N BT.601. */
        break;
    default:
        return ESP_ERR_INVALID_ARG;
    }
    return write_reg(0x28, value);
}
#endif
