#include "settings.h"

#include <string.h>

static fpvdeck_settings_t s_settings;
static bool s_initialized;

static void initialize(void)
{
    if (s_initialized) return;
    memset(&s_settings, 0, sizeof(s_settings));
    s_settings.band = RX5808_BAND_R;
    s_settings.channel = 0;
    s_settings.brightness_percent = 72;
    s_settings.buzzer_enabled = true;
    s_settings.video_mode = SETTINGS_VIDEO_AUTO;
    for (unsigned i = 0; i < 6; ++i) s_settings.cell_calibration[i] = 1.0f;
    strcpy(s_settings.wifi_password, "fpv-deck-demo");
    s_initialized = true;
}

esp_err_t settings_load(fpvdeck_settings_t *settings)
{
    if (settings == NULL) return ESP_ERR_INVALID_ARG;
    initialize();
    *settings = s_settings;
    return ESP_OK;
}

esp_err_t settings_save(const fpvdeck_settings_t *settings)
{
    if (settings == NULL) return ESP_ERR_INVALID_ARG;
    s_settings = *settings;
    s_initialized = true;
    return ESP_OK;
}

