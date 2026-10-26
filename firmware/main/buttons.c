#include "buttons.h"

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "driver/gpio.h"

static const int s_pins[BUTTON_COUNT] = {
    PIN_BUTTON_UP, PIN_BUTTON_OK, PIN_BUTTON_DOWN
};

esp_err_t buttons_init(void)
{
    const gpio_config_t cfg = {
        .pin_bit_mask = (1ULL << PIN_BUTTON_UP) | (1ULL << PIN_BUTTON_OK) |
                        (1ULL << PIN_BUTTON_DOWN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&cfg);
}

bool button_pressed(button_id_t button)
{
    return button < BUTTON_COUNT && gpio_get_level(s_pins[button]) == 0;
}
#endif
