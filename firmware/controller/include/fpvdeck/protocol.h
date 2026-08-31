#ifndef FPVDECK_PROTOCOL_H
#define FPVDECK_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FPVDECK_PROTOCOL_VERSION 1u
#define FPVDECK_PROTOCOL_MAX_PAYLOAD 1024u
#define FPVDECK_PROTOCOL_RAW_OVERHEAD 8u
#define FPVDECK_PROTOCOL_MAX_ENCODED 1038u

typedef enum {
    FPVDECK_PROTOCOL_OK = 0,
    FPVDECK_PROTOCOL_BAD_ARGUMENT,
    FPVDECK_PROTOCOL_OUTPUT_TOO_SMALL,
    FPVDECK_PROTOCOL_MISSING_DELIMITER,
    FPVDECK_PROTOCOL_INVALID_COBS,
    FPVDECK_PROTOCOL_TOO_SHORT,
    FPVDECK_PROTOCOL_LENGTH_MISMATCH,
    FPVDECK_PROTOCOL_CRC_MISMATCH
} fpvdeck_protocol_result_t;

typedef struct {
    uint8_t version;
    uint8_t type;
    uint16_t sequence;
    uint16_t payload_length;
    uint8_t payload[FPVDECK_PROTOCOL_MAX_PAYLOAD];
} fpvdeck_frame_t;

uint16_t fpvdeck_crc16_ccitt(const uint8_t *data, size_t length);

fpvdeck_protocol_result_t fpvdeck_frame_encode(
    const fpvdeck_frame_t *frame,
    uint8_t *output,
    size_t output_capacity,
    size_t *output_length);

fpvdeck_protocol_result_t fpvdeck_frame_decode(
    const uint8_t *input,
    size_t input_length,
    fpvdeck_frame_t *frame);

#endif

