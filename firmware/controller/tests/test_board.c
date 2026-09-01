#include "fpvdeck/board.h"

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

static bool read_adc(void *context, uint8_t channel, uint32_t *raw, int32_t *millivolts)
{
    (void)context;
    *raw = 1000u + channel;
    *millivolts = 4200 * (int32_t)(channel + 1u);
    return true;
}

static void self_test(void *context, uint32_t *pass, uint32_t *interactive, uint32_t *fail)
{
    (void)context;
    *pass = FPVDECK_SELFTEST_MCU | FPVDECK_SELFTEST_ADC | FPVDECK_SELFTEST_TEMPERATURE;
    *interactive = FPVDECK_SELFTEST_BUTTONS;
    *fail = 0u;
}

static uint32_t read_u32(const uint8_t *source)
{
    return (uint32_t)source[0] | (uint32_t)source[1] << 8u
        | (uint32_t)source[2] << 16u | (uint32_t)source[3] << 24u;
}

int main(void)
{
    fpvdeck_board_hal_t hal = {0};
    hal.read_adc = read_adc;
    hal.self_test = self_test;
    fpvdeck_frame_t request = {0};
    fpvdeck_frame_t reply = {0};
    request.version = FPVDECK_PROTOCOL_VERSION;
    request.type = FPVDECK_MSG_BOARD_REQUEST;
    request.sequence = 42u;
    request.payload_length = 2u;

    request.payload[0] = FPVDECK_BOARD_ADC_MILLIVOLTS;
    request.payload[1] = 3u;
    CHECK(fpvdeck_board_handle(&hal, &request, &reply) == FPVDECK_BOARD_STATUS_OK);
    CHECK(reply.sequence == 42u);
    CHECK(reply.type == FPVDECK_MSG_BOARD_REPLY);
    CHECK(reply.payload[2] == 3u);
    CHECK(read_u32(&reply.payload[3]) == 16800u);

    request.payload[0] = FPVDECK_BOARD_TAP_DUMP;
    request.payload_length = 1u;
    CHECK(fpvdeck_board_handle(&hal, &request, &reply) == FPVDECK_BOARD_STATUS_OK);
    CHECK(reply.payload_length == 27u);
    CHECK(read_u32(&reply.payload[3]) == 4200u);
    CHECK(read_u32(&reply.payload[23]) == 25200u);

    request.payload[0] = FPVDECK_BOARD_SELF_TEST;
    CHECK(fpvdeck_board_handle(&hal, &request, &reply) == FPVDECK_BOARD_STATUS_OK);
    CHECK((read_u32(&reply.payload[2]) & FPVDECK_SELFTEST_ADC) != 0u);
    CHECK(read_u32(&reply.payload[10]) == 0u);

    request.payload[0] = 0xffu;
    CHECK(fpvdeck_board_handle(&hal, &request, &reply) == FPVDECK_BOARD_STATUS_BAD_COMMAND);
    return 0;
}
