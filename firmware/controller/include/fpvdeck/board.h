#ifndef FPVDECK_BOARD_H
#define FPVDECK_BOARD_H

#include "fpvdeck/protocol.h"

#include <stdbool.h>
#include <stdint.h>

#define FPVDECK_MSG_BOARD_REQUEST 0x40u
#define FPVDECK_MSG_BOARD_REPLY 0x41u

typedef enum {
    FPVDECK_ADC_BNEG = 0u,
    FPVDECK_ADC_DECK = 1u,
    FPVDECK_ADC_TAP1 = 2u,
    FPVDECK_ADC_TAP2 = 3u,
    FPVDECK_ADC_TAP3 = 4u,
    FPVDECK_ADC_TAP4 = 5u,
    FPVDECK_ADC_TAP5 = 6u,
    FPVDECK_ADC_TAP6 = 7u
} fpvdeck_adc_channel_t;

typedef enum {
    FPVDECK_BOARD_IDENTIFY = 0x01,
    FPVDECK_BOARD_SELF_TEST = 0x02,
    FPVDECK_BOARD_ADC_RAW = 0x10,
    FPVDECK_BOARD_ADC_MILLIVOLTS = 0x11,
    FPVDECK_BOARD_TAP_DUMP = 0x12,
    FPVDECK_BOARD_TEMPERATURES = 0x20,
    FPVDECK_BOARD_LED_SET = 0x30,
    FPVDECK_BOARD_BUZZER_SET = 0x31,
    FPVDECK_BOARD_FAN_SET = 0x32,
    FPVDECK_BOARD_BUTTONS = 0x40,
    FPVDECK_BOARD_SD_DETECT = 0x41,
    FPVDECK_BOARD_RAILS = 0x42
} fpvdeck_board_command_t;

typedef enum {
    FPVDECK_BOARD_STATUS_OK = 0,
    FPVDECK_BOARD_STATUS_BAD_COMMAND = 1,
    FPVDECK_BOARD_STATUS_BAD_ARGUMENT = 2,
    FPVDECK_BOARD_STATUS_NOT_READY = 3,
    FPVDECK_BOARD_STATUS_HARDWARE_FAULT = 4
} fpvdeck_board_status_t;

enum {
    FPVDECK_SELFTEST_MCU = 1u << 0,
    FPVDECK_SELFTEST_ADC = 1u << 1,
    FPVDECK_SELFTEST_BALANCE = 1u << 2,
    FPVDECK_SELFTEST_SD_DETECT = 1u << 3,
    FPVDECK_SELFTEST_TEMPERATURE = 1u << 4,
    FPVDECK_SELFTEST_VRX_LINK = 1u << 5,
    FPVDECK_SELFTEST_BUTTONS = 1u << 6,
    FPVDECK_SELFTEST_POWER = 1u << 7
};

typedef struct {
    void *context;
    bool (*read_adc)(void *context, uint8_t channel, uint32_t *raw, int32_t *millivolts);
    bool (*read_temperatures)(void *context, int16_t *mcu_centi_c, int16_t *external_centi_c);
    bool (*read_rails)(void *context, uint16_t *rail_5v_mv, uint16_t *rail_3v3_mv, uint16_t *deck_mv);
    uint32_t (*read_buttons)(void *context);
    bool (*read_sd_detect)(void *context);
    bool (*set_output)(void *context, fpvdeck_board_command_t command, uint8_t value);
    void (*self_test)(void *context, uint32_t *pass_mask, uint32_t *interactive_mask, uint32_t *fail_mask);
} fpvdeck_board_hal_t;

fpvdeck_board_status_t fpvdeck_board_handle(
    const fpvdeck_board_hal_t *hal,
    const fpvdeck_frame_t *request,
    fpvdeck_frame_t *reply);

#endif
