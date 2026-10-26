#pragma once

#include <stddef.h>
#include <stdint.h>
#include "esp_err.h"

typedef enum {
    VIDEO_STANDARD_NTSC = 0,
    VIDEO_STANDARD_PAL,
} video_standard_t;

typedef struct {
    void *pixels;
    size_t size_bytes;
    uint16_t width;
    uint16_t height;
    video_standard_t standard;
} video_frame_t;

/* Captures one analog field. RGB565 output is deliberately field-based: the
 * latency budget forbids waiting for a full deinterlaced frame. */
esp_err_t video_init(video_standard_t standard);
esp_err_t video_capture_field(video_frame_t *frame, uint32_t timeout_ms);
