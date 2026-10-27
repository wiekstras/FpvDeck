#pragma once

#include "fpvdeck_platform.h"

esp_err_t buzzer_init(void);
void buzzer_tone(unsigned frequency_hz, unsigned duty_percent);
void buzzer_off(void);
