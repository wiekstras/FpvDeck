#include "fpvdeck/protocol.h"

#include <string.h>

static void write_le16(uint8_t *target, uint16_t value)
{
    target[0] = (uint8_t)(value & 0xffu);
    target[1] = (uint8_t)(value >> 8u);
}

static uint16_t read_le16(const uint8_t *source)
{
    return (uint16_t)source[0] | (uint16_t)((uint16_t)source[1] << 8u);
}

uint16_t fpvdeck_crc16_ccitt(const uint8_t *data, size_t length)
{
    uint16_t crc = 0xffffu;
    if (data == NULL && length != 0u) return 0u;
    for (size_t i = 0; i < length; ++i) {
        crc ^= (uint16_t)((uint16_t)data[i] << 8u);
        for (uint8_t bit = 0; bit < 8u; ++bit) {
            crc = (crc & 0x8000u) != 0u
                ? (uint16_t)((uint16_t)(crc << 1u) ^ 0x1021u)
                : (uint16_t)(crc << 1u);
        }
    }
    return crc;
}

static fpvdeck_protocol_result_t cobs_encode(
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t capacity, size_t *written)
{
    if (capacity == 0u) return FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL;
    size_t read_index = 0u;
    size_t write_index = 1u;
    size_t code_index = 0u;
    uint8_t code = 1u;
    while (read_index < input_length) {
        if (input[read_index] == 0u) {
            if (code_index >= capacity) return FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL;
            output[code_index] = code;
            code_index = write_index++;
            code = 1u;
            ++read_index;
        } else {
            if (write_index >= capacity) return FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL;
            output[write_index++] = input[read_index++];
            ++code;
            if (code == 0xffu) {
                output[code_index] = code;
                code_index = write_index++;
                code = 1u;
            }
        }
    }
    if (code_index >= capacity) return FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL;
    output[code_index] = code;
    *written = write_index;
    return FPVDECK_PROTOCOL_OK;
}

static fpvdeck_protocol_result_t cobs_decode(
    const uint8_t *input, size_t input_length,
    uint8_t *output, size_t capacity, size_t *written)
{
    size_t read_index = 0u;
    size_t write_index = 0u;
    while (read_index < input_length) {
        const uint8_t code = input[read_index++];
        if (code == 0u || read_index + (size_t)code - 1u > input_length)
            return FPVDECK_PROTOCOL_INVALID_COBS;
        for (uint8_t i = 1u; i < code; ++i) {
            if (write_index >= capacity) return FPVDECK_PROTOCOL_LENGTH_MISMATCH;
            output[write_index++] = input[read_index++];
        }
        if (code != 0xffu && read_index < input_length) {
            if (write_index >= capacity) return FPVDECK_PROTOCOL_LENGTH_MISMATCH;
            output[write_index++] = 0u;
        }
    }
    *written = write_index;
    return FPVDECK_PROTOCOL_OK;
}

fpvdeck_protocol_result_t fpvdeck_frame_encode(
    const fpvdeck_frame_t *frame,
    uint8_t *output,
    size_t output_capacity,
    size_t *output_length)
{
    if (frame == NULL || output == NULL || output_length == NULL
        || frame->payload_length > FPVDECK_PROTOCOL_MAX_PAYLOAD)
        return FPVDECK_PROTOCOL_BAD_ARGUMENT;

    uint8_t raw[FPVDECK_PROTOCOL_MAX_PAYLOAD + FPVDECK_PROTOCOL_RAW_OVERHEAD];
    const size_t raw_length = (size_t)frame->payload_length + FPVDECK_PROTOCOL_RAW_OVERHEAD;
    raw[0] = frame->version;
    raw[1] = frame->type;
    write_le16(&raw[2], frame->sequence);
    write_le16(&raw[4], frame->payload_length);
    memcpy(&raw[6], frame->payload, frame->payload_length);
    write_le16(&raw[raw_length - 2u], fpvdeck_crc16_ccitt(raw, raw_length - 2u));

    size_t encoded_length = 0u;
    const fpvdeck_protocol_result_t result = cobs_encode(
        raw, raw_length, output, output_capacity, &encoded_length);
    if (result != FPVDECK_PROTOCOL_OK) return result;
    if (encoded_length >= output_capacity) return FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL;
    output[encoded_length++] = 0u;
    *output_length = encoded_length;
    return FPVDECK_PROTOCOL_OK;
}

fpvdeck_protocol_result_t fpvdeck_frame_decode(
    const uint8_t *input,
    size_t input_length,
    fpvdeck_frame_t *frame)
{
    if (input == NULL || frame == NULL) return FPVDECK_PROTOCOL_BAD_ARGUMENT;
    if (input_length == 0u || input[input_length - 1u] != 0u)
        return FPVDECK_PROTOCOL_MISSING_DELIMITER;

    uint8_t raw[FPVDECK_PROTOCOL_MAX_PAYLOAD + FPVDECK_PROTOCOL_RAW_OVERHEAD];
    size_t raw_length = 0u;
    fpvdeck_protocol_result_t result = cobs_decode(
        input, input_length - 1u, raw, sizeof(raw), &raw_length);
    if (result != FPVDECK_PROTOCOL_OK) return result;
    if (raw_length < FPVDECK_PROTOCOL_RAW_OVERHEAD) return FPVDECK_PROTOCOL_TOO_SHORT;

    const uint16_t payload_length = read_le16(&raw[4]);
    if (payload_length > FPVDECK_PROTOCOL_MAX_PAYLOAD
        || raw_length != (size_t)payload_length + FPVDECK_PROTOCOL_RAW_OVERHEAD)
        return FPVDECK_PROTOCOL_LENGTH_MISMATCH;
    if (read_le16(&raw[raw_length - 2u])
        != fpvdeck_crc16_ccitt(raw, raw_length - 2u))
        return FPVDECK_PROTOCOL_CRC_MISMATCH;

    frame->version = raw[0];
    frame->type = raw[1];
    frame->sequence = read_le16(&raw[2]);
    frame->payload_length = payload_length;
    memcpy(frame->payload, &raw[6], payload_length);
    return FPVDECK_PROTOCOL_OK;
}

