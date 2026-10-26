#include "ads1115.h"

#ifdef ESP_PLATFORM
#include "esp_check.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ADS1115_REG_CONVERSION 0x00U
#define ADS1115_REG_CONFIG     0x01U
#define ADS1115_TIMEOUT_MS     50

esp_err_t ads1115_init(ads1115_t *adc, i2c_master_bus_handle_t bus,
                       uint8_t address)
{
    if (adc == NULL || bus == NULL || address < 0x48 || address > 0x4B) {
        return ESP_ERR_INVALID_ARG;
    }
    const i2c_device_config_t cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = address,
        .scl_speed_hz = 100000,
    };
    adc->address = address;
    return i2c_master_bus_add_device(bus, &cfg, &adc->device);
}

static esp_err_t read_register(ads1115_t *adc, uint8_t reg, uint16_t *value)
{
    uint8_t data[2];
    ESP_RETURN_ON_ERROR(i2c_master_transmit_receive(adc->device, &reg, 1,
                                                    data, sizeof(data),
                                                    ADS1115_TIMEOUT_MS),
                        "ads1115", "read register");
    *value = (uint16_t)((uint16_t)data[0] << 8U) | data[1];
    return ESP_OK;
}

esp_err_t ads1115_read_single(ads1115_t *adc, unsigned channel,
                              ads1115_pga_t pga, float *volts)
{
    if (adc == NULL || adc->device == NULL || channel > 3 || volts == NULL ||
        (pga != ADS1115_PGA_4V096 && pga != ADS1115_PGA_2V048)) {
        return ESP_ERR_INVALID_ARG;
    }
    /* TI ADS1115 SBAS444: OS=1, MUX=AINx-GND, single-shot, 128 SPS,
       traditional comparator disabled. */
    const uint16_t config = (1U << 15U) |
                            (uint16_t)((4U + channel) << 12U) |
                            (uint16_t)((unsigned)pga << 9U) |
                            (1U << 8U) | (4U << 5U) | 3U;
    const uint8_t command[3] = {
        ADS1115_REG_CONFIG, (uint8_t)(config >> 8U), (uint8_t)config
    };
    ESP_RETURN_ON_ERROR(i2c_master_transmit(adc->device, command,
                                             sizeof(command), ADS1115_TIMEOUT_MS),
                        "ads1115", "start conversion");
    vTaskDelay(pdMS_TO_TICKS(10));
    uint16_t raw = 0;
    ESP_RETURN_ON_ERROR(read_register(adc, ADS1115_REG_CONVERSION, &raw),
                        "ads1115", "conversion");
    const float full_scale = pga == ADS1115_PGA_4V096 ? 4.096f : 2.048f;
    *volts = (float)(int16_t)raw * full_scale / 32768.0f;
    return ESP_OK;
}
#endif
