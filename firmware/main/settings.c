#include "settings.h"

#include <string.h>
#include "nvs.h"

static void defaults(fpvdeck_settings_t *s)
{
    memset(s, 0, sizeof(*s));
    s->band = RX5808_BAND_R;
    s->channel = 0;
    s->brightness_percent = 70;
    s->buzzer_enabled = true;
    s->video_mode = SETTINGS_VIDEO_AUTO;
    for (unsigned i = 0; i < 6; ++i) s->cell_calibration[i] = 1.0f;
    strcpy(s->wifi_password, "change-me-now");
}

esp_err_t settings_load(fpvdeck_settings_t *settings)
{
    if (settings == NULL) return ESP_ERR_INVALID_ARG;
    defaults(settings);
    nvs_handle_t nvs;
    esp_err_t err = nvs_open("fpvdeck", NVS_READONLY, &nvs);
    if (err == ESP_ERR_NVS_NOT_FOUND) return ESP_OK;
    if (err != ESP_OK) return err;
    size_t size = sizeof(*settings);
    err = nvs_get_blob(nvs, "settings_v1", settings, &size);
    nvs_close(nvs);
    if (err == ESP_ERR_NVS_NOT_FOUND || size != sizeof(*settings)) {
        defaults(settings);
        return ESP_OK;
    }
    return err;
}

esp_err_t settings_save(const fpvdeck_settings_t *settings)
{
    if (settings == NULL) return ESP_ERR_INVALID_ARG;
    nvs_handle_t nvs;
    esp_err_t err = nvs_open("fpvdeck", NVS_READWRITE, &nvs);
    if (err != ESP_OK) return err;
    if (err == ESP_OK) err = nvs_set_blob(nvs, "settings_v1", settings, sizeof(*settings));
    if (err == ESP_OK) err = nvs_commit(nvs);
    nvs_close(nvs);
    return err;
}
