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

static void sanitize(fpvdeck_settings_t *s)
{
    if (s->band >= RX5808_BAND_COUNT) s->band = RX5808_BAND_R;
    if (s->channel >= 8) s->channel = 0;
    if (s->brightness_percent < 10 || s->brightness_percent > 100) {
        s->brightness_percent = 70;
    }
    if (s->video_mode > SETTINGS_VIDEO_NTSC) {
        s->video_mode = SETTINGS_VIDEO_AUTO;
    }
    for (unsigned i = 0; i < 6; ++i) {
        if (s->cell_calibration[i] < 0.8f || s->cell_calibration[i] > 1.2f) {
            s->cell_calibration[i] = 1.0f;
        }
    }
    s->wifi_password[sizeof(s->wifi_password) - 1U] = '\0';
    const size_t password_length = strlen(s->wifi_password);
    if (password_length < 8 || password_length > 63) {
        strcpy(s->wifi_password, "change-me-now");
    }
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
    if (err == ESP_OK) sanitize(settings);
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
