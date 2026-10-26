#include "rx5808.h"

const uint16_t rx5808_channels_mhz[RX5808_BAND_COUNT][8] = {
    {5865, 5845, 5825, 5805, 5785, 5765, 5745, 5725},
    {5733, 5752, 5771, 5790, 5809, 5828, 5847, 5866},
    {5705, 5685, 5665, 5645, 5885, 5905, 5925, 5945},
    {5740, 5760, 5780, 5800, 5820, 5840, 5860, 5880},
    {5658, 5695, 5732, 5769, 5806, 5843, 5880, 5917},
    {5362, 5399, 5436, 5473, 5510, 5547, 5584, 5621},
};

uint16_t rx5808_frequency_register(uint16_t frequency_mhz)
{
    if (frequency_mhz < 479) return 0;
    const uint16_t n = (uint16_t)((frequency_mhz - 479U) / 2U);
    return (uint16_t)(((n / 32U) << 7U) | (n % 32U));
}

uint32_t rx5808_write_frame(uint8_t register_address, uint32_t data20)
{
    return ((uint32_t)register_address & 0x0FU) |
           (1UL << 4U) |
           ((data20 & 0xFFFFFUL) << 5U);
}

bool rx5808_channel_frequency(rx5808_band_t band, unsigned channel,
                              uint16_t *frequency_mhz)
{
    if (band >= RX5808_BAND_COUNT || channel >= 8 || frequency_mhz == 0) {
        return false;
    }
    *frequency_mhz = rx5808_channels_mhz[band][channel];
    return true;
}

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "driver/gpio.h"
#include "esp_check.h"
#include "esp_rom_sys.h"

#define RX5808_REG_FREQUENCY 0x01U
#define RX5808_EDGE_US       3U

static int latch_pin(rx5808_module_t module)
{
    return module == RX5808_MODULE_A ? PIN_RX_LE_A : PIN_RX_LE_B;
}

static void send_frame(rx5808_module_t module, uint32_t frame)
{
    const int le = latch_pin(module);
    gpio_set_level(le, 0);
    gpio_set_level(PIN_RX_CLK, 0);
    esp_rom_delay_us(RX5808_EDGE_US);
    for (unsigned bit = 0; bit < 25; ++bit) {
        gpio_set_level(PIN_RX_DATA, (frame >> bit) & 1U);
        esp_rom_delay_us(RX5808_EDGE_US);
        gpio_set_level(PIN_RX_CLK, 1);
        esp_rom_delay_us(RX5808_EDGE_US);
        gpio_set_level(PIN_RX_CLK, 0);
    }
    esp_rom_delay_us(RX5808_EDGE_US);
    gpio_set_level(le, 1);
    esp_rom_delay_us(RX5808_EDGE_US);
    gpio_set_level(le, 0);
}

esp_err_t rx5808_init(void)
{
    const uint64_t mask = (1ULL << PIN_RX_DATA) | (1ULL << PIN_RX_CLK) |
                          (1ULL << PIN_RX_LE_A) | (1ULL << PIN_RX_LE_B);
    const gpio_config_t cfg = {
        .pin_bit_mask = mask,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    ESP_RETURN_ON_ERROR(gpio_config(&cfg), "rx5808", "gpio config");
    gpio_set_level(PIN_RX_DATA, 0);
    gpio_set_level(PIN_RX_CLK, 0);
    gpio_set_level(PIN_RX_LE_A, 0);
    gpio_set_level(PIN_RX_LE_B, 0);
    return ESP_OK;
}

esp_err_t rx5808_set_frequency(rx5808_module_t module, uint16_t frequency_mhz)
{
    if (module > RX5808_MODULE_B || frequency_mhz < 4900 || frequency_mhz > 6100) {
        return ESP_ERR_INVALID_ARG;
    }
    send_frame(module, rx5808_write_frame(
        RX5808_REG_FREQUENCY, rx5808_frequency_register(frequency_mhz)));
    return ESP_OK;
}

esp_err_t rx5808_set_both(uint16_t frequency_mhz)
{
    ESP_RETURN_ON_ERROR(rx5808_set_frequency(RX5808_MODULE_A, frequency_mhz),
                        "rx5808", "RX A tune");
    return rx5808_set_frequency(RX5808_MODULE_B, frequency_mhz);
}

#endif
