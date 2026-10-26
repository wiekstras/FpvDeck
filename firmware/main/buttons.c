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

static void keypad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
    (void)indev;
    static uint32_t last_key = LV_KEY_ENTER;
    data->state = LV_INDEV_STATE_RELEASED;
    if (button_pressed(BUTTON_UP)) {
        last_key = LV_KEY_PREV;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (button_pressed(BUTTON_DOWN)) {
        last_key = LV_KEY_NEXT;
        data->state = LV_INDEV_STATE_PRESSED;
    } else if (button_pressed(BUTTON_OK)) {
        last_key = LV_KEY_ENTER;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    data->key = last_key;
}

lv_indev_t *buttons_attach_lvgl(void)
{
    lv_group_t *group = lv_group_create();
    if (group == NULL) return NULL;
    lv_group_set_default(group);
    lv_indev_t *indev = lv_indev_create();
    if (indev == NULL) {
        lv_group_delete(group);
        return NULL;
    }
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    lv_indev_set_read_cb(indev, keypad_read);
    lv_indev_set_group(indev, group);
    return indev;
}
#endif
