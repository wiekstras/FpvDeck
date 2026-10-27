#include "ads1115.h"
#include "apps.h"
#include "battery.h"
#include "buttons.h"
#include "buzzer.h"
#include "deck_power.h"
#include "rx5808.h"
#include "settings.h"
#include "signal_logic.h"
#include "tvp5150.h"

#include "bsp/display.h"
#include "bsp/esp-bsp.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"

static const char *TAG = "fpvdeck";
static ads1115_t s_adc[3];
static bool s_adc_ready[3];

static void deck_power_task(void *argument)
{
    (void)argument;
    bool shutdown_screen = false;
    while (true) {
        float voltage = 0.0f;
        if (deck_power_read_voltage(&voltage) == ESP_OK) {
            const deck_power_state_t state = deck_power_classify(voltage);
            if (state == DECK_POWER_LOW) {
                ESP_LOGW(TAG, "deck battery low: %.2f V", voltage);
            } else if (state == DECK_POWER_SHUTDOWN) {
                ESP_LOGE(TAG, "deck battery critical: %.2f V", voltage);
            }
            bsp_display_lock(-1);
            if (state == DECK_POWER_SHUTDOWN && !shutdown_screen) {
                app_shutdown_show(voltage);
                shutdown_screen = true;
            } else if (!shutdown_screen) {
                app_set_deck_battery_warning(voltage);
            }
            bsp_display_unlock();
        }
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void app_main(void)
{
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    bsp_display_cfg_t display_cfg = {
        .lv_adapter_cfg = ESP_LV_ADAPTER_DEFAULT_CONFIG(),
        .rotation = ESP_LV_ADAPTER_ROTATE_90,
        .tear_avoid_mode = ESP_LV_ADAPTER_TEAR_AVOID_MODE_TRIPLE_PARTIAL,
        .touch_flags = {.swap_xy = 1, .mirror_x = 0, .mirror_y = 1},
    };
    bsp_display_start_with_config(&display_cfg);
    bsp_display_backlight_on();

    ESP_ERROR_CHECK(buttons_init());
    bsp_display_lock(-1);
    ESP_ERROR_CHECK(buttons_attach_lvgl() != NULL ? ESP_OK : ESP_ERR_NO_MEM);
    bsp_display_unlock();
    ESP_ERROR_CHECK(buzzer_init());
    ESP_ERROR_CHECK(rx5808_init());
    err = deck_power_init();
    if (err == ESP_OK) {
        xTaskCreate(deck_power_task, "deck_power", 4096, NULL, 4, NULL);
    } else {
        ESP_LOGW(TAG, "onboard BAT_ADC unavailable: %s", esp_err_to_name(err));
    }

    fpvdeck_settings_t settings;
    ESP_ERROR_CHECK(settings_load(&settings));
    ESP_ERROR_CHECK(bsp_display_brightness_set(settings.brightness_percent));
    uint16_t frequency = 0;
    if (rx5808_channel_frequency(settings.band, settings.channel, &frequency)) {
        ESP_ERROR_CHECK(rx5808_set_both(frequency));
    }

    i2c_master_bus_handle_t bus = bsp_i2c_get_handle();
    for (unsigned i = 0; i < 3; ++i) {
        err = ads1115_init(&s_adc[i], bus, (uint8_t)(0x48U + i));
        if (err != ESP_OK) {
            ESP_LOGW(TAG, "ADS1115 0x%02x unavailable: %s", 0x48U + i,
                     esp_err_to_name(err));
        } else {
            s_adc_ready[i] = true;
        }
    }
    ESP_ERROR_CHECK(battery_start_sampling(s_adc, s_adc_ready,
                                            settings.cell_calibration));
    ESP_ERROR_CHECK(signal_start_sampling(&s_adc[2], s_adc_ready[2]));
    err = tvp5150_init(bus);
    if (err != ESP_OK) ESP_LOGW(TAG, "TVP5150 HAT not detected: %s", esp_err_to_name(err));

    bsp_display_lock(-1);
    app_launcher_show();
    bsp_display_unlock();
    ESP_LOGI(TAG, "launcher ready; physical HAT status is reported independently");
}
