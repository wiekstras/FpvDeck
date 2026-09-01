#include "fpvdeck/board.h"

#include <string.h>

static void put_u16(uint8_t *target, uint16_t value)
{
    target[0] = (uint8_t)value;
    target[1] = (uint8_t)(value >> 8u);
}

static void put_u32(uint8_t *target, uint32_t value)
{
    target[0] = (uint8_t)value;
    target[1] = (uint8_t)(value >> 8u);
    target[2] = (uint8_t)(value >> 16u);
    target[3] = (uint8_t)(value >> 24u);
}

static void begin_reply(const fpvdeck_frame_t *request, fpvdeck_frame_t *reply, uint8_t command)
{
    memset(reply, 0, sizeof(*reply));
    reply->version = FPVDECK_PROTOCOL_VERSION;
    reply->type = FPVDECK_MSG_BOARD_REPLY;
    reply->sequence = request->sequence;
    reply->payload[0] = command;
    reply->payload[1] = FPVDECK_BOARD_STATUS_OK;
    reply->payload_length = 2u;
}

static fpvdeck_board_status_t finish(fpvdeck_frame_t *reply, fpvdeck_board_status_t status)
{
    reply->payload[1] = (uint8_t)status;
    return status;
}

fpvdeck_board_status_t fpvdeck_board_handle(
    const fpvdeck_board_hal_t *hal,
    const fpvdeck_frame_t *request,
    fpvdeck_frame_t *reply)
{
    if (hal == NULL || request == NULL || reply == NULL) return FPVDECK_BOARD_STATUS_BAD_ARGUMENT;
    if (request->version != FPVDECK_PROTOCOL_VERSION || request->type != FPVDECK_MSG_BOARD_REQUEST
        || request->payload_length < 1u)
        return FPVDECK_BOARD_STATUS_BAD_ARGUMENT;

    const uint8_t command = request->payload[0];
    const uint8_t argument = request->payload_length > 1u ? request->payload[1] : 0u;
    begin_reply(request, reply, command);

    switch ((fpvdeck_board_command_t)command) {
    case FPVDECK_BOARD_IDENTIFY:
        reply->payload[2] = 1u; /* Test PCB hardware major. */
        reply->payload[3] = 0u; /* Hardware minor / Rev A. */
        reply->payload[4] = 0u; /* Firmware major. */
        reply->payload[5] = 1u;
        reply->payload[6] = 0u;
        put_u32(&reply->payload[7], 0x000001ffu);
        reply->payload_length = 11u;
        return FPVDECK_BOARD_STATUS_OK;

    case FPVDECK_BOARD_SELF_TEST: {
        uint32_t pass = 0u, interactive = 0u, fail = 0u;
        if (hal->self_test == NULL) return finish(reply, FPVDECK_BOARD_STATUS_NOT_READY);
        hal->self_test(hal->context, &pass, &interactive, &fail);
        put_u32(&reply->payload[2], pass);
        put_u32(&reply->payload[6], interactive);
        put_u32(&reply->payload[10], fail);
        reply->payload_length = 14u;
        return finish(reply, fail == 0u ? FPVDECK_BOARD_STATUS_OK : FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
    }

    case FPVDECK_BOARD_ADC_RAW:
    case FPVDECK_BOARD_ADC_MILLIVOLTS: {
        uint32_t raw = 0u;
        int32_t millivolts = 0;
        if (argument > 7u) return finish(reply, FPVDECK_BOARD_STATUS_BAD_ARGUMENT);
        if (hal->read_adc == NULL || !hal->read_adc(hal->context, argument, &raw, &millivolts))
            return finish(reply, FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
        reply->payload[2] = argument;
        put_u32(&reply->payload[3], command == FPVDECK_BOARD_ADC_RAW ? raw : (uint32_t)millivolts);
        reply->payload_length = 7u;
        return FPVDECK_BOARD_STATUS_OK;
    }

    case FPVDECK_BOARD_TAP_DUMP:
        if (hal->read_adc == NULL) return finish(reply, FPVDECK_BOARD_STATUS_NOT_READY);
        reply->payload[2] = 6u;
        for (uint8_t channel = 0u; channel < 6u; ++channel) {
            uint32_t raw = 0u;
            int32_t millivolts = 0;
            if (!hal->read_adc(hal->context, channel, &raw, &millivolts))
                return finish(reply, FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
            put_u32(&reply->payload[3u + (uint16_t)channel * 4u], (uint32_t)millivolts);
        }
        reply->payload_length = 27u;
        return FPVDECK_BOARD_STATUS_OK;

    case FPVDECK_BOARD_TEMPERATURES: {
        int16_t mcu = 0, external = 0;
        if (hal->read_temperatures == NULL || !hal->read_temperatures(hal->context, &mcu, &external))
            return finish(reply, FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
        put_u16(&reply->payload[2], (uint16_t)mcu);
        put_u16(&reply->payload[4], (uint16_t)external);
        reply->payload_length = 6u;
        return FPVDECK_BOARD_STATUS_OK;
    }

    case FPVDECK_BOARD_LED_SET:
    case FPVDECK_BOARD_BUZZER_SET:
    case FPVDECK_BOARD_FAN_SET:
        if (request->payload_length < 2u) return finish(reply, FPVDECK_BOARD_STATUS_BAD_ARGUMENT);
        if (hal->set_output == NULL || !hal->set_output(hal->context, (fpvdeck_board_command_t)command, argument))
            return finish(reply, FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
        reply->payload[2] = argument;
        reply->payload_length = 3u;
        return FPVDECK_BOARD_STATUS_OK;

    case FPVDECK_BOARD_BUTTONS:
        if (hal->read_buttons == NULL) return finish(reply, FPVDECK_BOARD_STATUS_NOT_READY);
        put_u32(&reply->payload[2], hal->read_buttons(hal->context));
        reply->payload_length = 6u;
        return FPVDECK_BOARD_STATUS_OK;

    case FPVDECK_BOARD_SD_DETECT:
        if (hal->read_sd_detect == NULL) return finish(reply, FPVDECK_BOARD_STATUS_NOT_READY);
        reply->payload[2] = hal->read_sd_detect(hal->context) ? 1u : 0u;
        reply->payload_length = 3u;
        return FPVDECK_BOARD_STATUS_OK;

    case FPVDECK_BOARD_RAILS: {
        uint16_t rail_5v = 0u, rail_3v3 = 0u, deck = 0u;
        if (hal->read_rails == NULL || !hal->read_rails(hal->context, &rail_5v, &rail_3v3, &deck))
            return finish(reply, FPVDECK_BOARD_STATUS_HARDWARE_FAULT);
        put_u16(&reply->payload[2], rail_5v);
        put_u16(&reply->payload[4], rail_3v3);
        put_u16(&reply->payload[6], deck);
        reply->payload_length = 8u;
        return FPVDECK_BOARD_STATUS_OK;
    }
    }
    return finish(reply, FPVDECK_BOARD_STATUS_BAD_COMMAND);
}
