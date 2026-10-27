#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "fpvdeck_platform.h"
#include "rx5808.h"

typedef enum {
    SETTINGS_VIDEO_AUTO = 0,
    SETTINGS_VIDEO_PAL,
    SETTINGS_VIDEO_NTSC,
} settings_video_mode_t;

typedef struct {
    rx5808_band_t band;
    uint8_t channel;
    uint8_t brightness_percent;
    bool buzzer_enabled;
    settings_video_mode_t video_mode;
    float cell_calibration[6];
    char wifi_password[64];
} fpvdeck_settings_t;

esp_err_t settings_load(fpvdeck_settings_t *settings);
esp_err_t settings_save(const fpvdeck_settings_t *settings);
