#pragma once

#include <QByteArray>
#include <QtGlobal>

namespace fpvdeck::protocol {

constexpr quint8 kProtocolVersion = 1;
constexpr qsizetype kMaxPayload = 1024;

struct Frame {
    quint8 version{kProtocolVersion};
    quint8 type{0};
    quint16 sequence{0};
    QByteArray payload;

    bool operator==(const Frame&) const = default;
};

enum class DecodeError {
    None,
    MissingDelimiter,
    InvalidCobs,
    TooShort,
    LengthMismatch,
    PayloadTooLarge,
    CrcMismatch,
};

struct DecodeResult {
    Frame frame;
    DecodeError error{DecodeError::None};
    [[nodiscard]] bool ok() const { return error == DecodeError::None; }
};

quint16 crc16Ccitt(const QByteArray& data);
QByteArray encode(const Frame& frame);
DecodeResult decode(const QByteArray& delimitedFrame);

} // namespace fpvdeck::protocol

