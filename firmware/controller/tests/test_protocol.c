#include "fpvdeck/protocol.h"

#include <string.h>

#define CHECK(condition) do { if (!(condition)) return __LINE__; } while (0)

int main(void)
{
    static const uint8_t known[] = "123456789";
    CHECK(fpvdeck_crc16_ccitt(known, 9u) == 0x29b1u);

    fpvdeck_frame_t source = {0};
    source.version = FPVDECK_PROTOCOL_VERSION;
    source.type = 0x11u;
    source.sequence = 0xbeefu;
    source.payload_length = 6u;
    const uint8_t payload[6] = {0u, 1u, 2u, 0u, 0xffu, 0x55u};
    memcpy(source.payload, payload, sizeof(payload));

    uint8_t encoded[FPVDECK_PROTOCOL_MAX_ENCODED];
    size_t encoded_length = 0u;
    CHECK(fpvdeck_frame_encode(&source, encoded, sizeof(encoded), &encoded_length)
        == FPVDECK_PROTOCOL_OK);
    CHECK(encoded[encoded_length - 1u] == 0u);

    fpvdeck_frame_t decoded = {0};
    CHECK(fpvdeck_frame_decode(encoded, encoded_length, &decoded)
        == FPVDECK_PROTOCOL_OK);
    CHECK(decoded.version == source.version);
    CHECK(decoded.type == source.type);
    CHECK(decoded.sequence == source.sequence);
    CHECK(decoded.payload_length == source.payload_length);
    CHECK(memcmp(decoded.payload, source.payload, source.payload_length) == 0);

    encoded[3] ^= 0x20u;
    CHECK(fpvdeck_frame_decode(encoded, encoded_length, &decoded)
        != FPVDECK_PROTOCOL_OK);
    return 0;
}
