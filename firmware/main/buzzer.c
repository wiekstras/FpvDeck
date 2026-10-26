#include "buzzer.h"

#ifdef ESP_PLATFORM
#include "board_pins.h"
#include "driver/ledc.h"

#define BUZZER_MODE LEDC_LOW_SPEED_MODE
#define BUZZER_TIMER LEDC_TIMER_0
#define BUZZER_CHANNEL LEDC_CHANNEL_0

esp_err_t buzzer_init(void)
{
    const ledc_timer_config_t timer = {
        .speed_mode = BUZZER_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = BUZZER_TIMER,
        .freq_hz = 2000,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    esp_err_t err = ledc_timer_config(&timer);
    if (err != ESP_OK) return err;
    const ledc_channel_config_t channel = {
        .gpio_num = PIN_BUZZER,
        .speed_mode = BUZZER_MODE,
        .channel = BUZZER_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = BUZZER_TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    return ledc_channel_config(&channel);
}

void buzzer_tone(unsigned frequency_hz, unsigned duty_percent)
{
    if (frequency_hz < 100) frequency_hz = 100;
    if (duty_percent > 50) duty_percent = 50;
    ledc_set_freq(BUZZER_MODE, BUZZER_TIMER, frequency_hz);
    ledc_set_duty(BUZZER_MODE, BUZZER_CHANNEL, (1023U * duty_percent) / 100U);
    ledc_update_duty(BUZZER_MODE, BUZZER_CHANNEL);
}

void buzzer_off(void)
{
    ledc_stop(BUZZER_MODE, BUZZER_CHANNEL, 0);
}
#endif
