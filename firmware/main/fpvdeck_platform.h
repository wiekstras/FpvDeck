#pragma once

/* Keep application-facing result types portable. ESP-IDF supplies the real
 * definitions; the desktop simulator uses the deliberately small subset below. */
#ifdef ESP_PLATFORM
#include "esp_err.h"
#else
typedef int esp_err_t;
#define ESP_OK 0
#define ESP_FAIL (-1)
#define ESP_ERR_INVALID_ARG 0x102
#define ESP_ERR_NOT_SUPPORTED 0x106
#endif

