#include "platform.h"

#include "bsp/display.h"

esp_err_t platform_display_set_brightness(uint8_t percent)
{
    return bsp_display_brightness_set(percent);
}
