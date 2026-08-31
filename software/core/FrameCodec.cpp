#include "FrameCodec.h"

namespace fpvdeck::protocol {
namespace {

void appendLe16(QByteArray& bytes, const quint16 value)
{
    bytes.append(static_cast<char>(value & 0xff));
    bytes.append(static_cast<char>((value >> 8) & 0xff));
}

quint16 readLe16(const QByteArray& bytes, const qsizetype offset)
{
    return static_cast<quint16>(static_cast<quint8>(bytes[offset]))
        | static_cast<quint16>(static_cast<quint8>(bytes[offset + 1]) << 8);
}

QByteArray cobsEncode(const QByteArray& input)
{
    QByteArray output;
    output.reserve(input.size() + input.size() / 254 + 1);
    qsizetype codeIndex = 0;
    output.append('\0');
    quint8 code = 1;

    for (const char raw : input) {
        if (raw == '\0') {
            output[codeIndex] = static_cast<char>(code);
            codeIndex = output.size();
            output.append('\0');
            code = 1;
        } else {
            output.append(raw);
            ++code;
            if (code == 0xff) {
                output[codeIndex] = static_cast<char>(code);
                codeIndex = output.size();
                output.append('\0');
                code = 1;
            }
        }
    }
    output[codeIndex] = static_cast<char>(code);
    return output;
}

bool cobsDecode(const QByteArray& input, QByteArray& output)
{
    output.clear();
    qsizetype index = 0;
    while (index < input.size()) {
        const quint8 code = static_cast<quint8>(input[index++]);
        if (code == 0 || index + code - 1 > input.size()) {
            return false;
        }
        for (quint8 i = 1; i < code; ++i) {
            output.append(input[index++]);
        }
        if (code != 0xff && index < input.size()) {
            output.append('\0');
        }
    }
    return true;
}

} // namespace

quint16 crc16Ccitt(const QByteArray& data)
{
    quint16 crc = 0xffff;
    for (const char raw : data) {
        crc ^= static_cast<quint16>(static_cast<quint8>(raw)) << 8;
        for (int bit = 0; bit < 8; ++bit) {
            crc = (crc & 0x8000) ? static_cast<quint16>((crc << 1) ^ 0x1021)
                                 : static_cast<quint16>(crc << 1);
        }
    }
    return crc;
}

QByteArray encode(const Frame& frame)
{
    if (frame.payload.size() > kMaxPayload) {
        return {};
    }
    QByteArray raw;
    raw.reserve(8 + frame.payload.size());
    raw.append(static_cast<char>(frame.version));
    raw.append(static_cast<char>(frame.type));
    appendLe16(raw, frame.sequence);
    appendLe16(raw, static_cast<quint16>(frame.payload.size()));
    raw.append(frame.payload);
    appendLe16(raw, crc16Ccitt(raw));

    QByteArray framed = cobsEncode(raw);
    framed.append('\0');
    return framed;
}

DecodeResult decode(const QByteArray& delimitedFrame)
{
    DecodeResult result;
    if (delimitedFrame.isEmpty() || delimitedFrame.at(delimitedFrame.size() - 1) != '\0') {
        result.error = DecodeError::MissingDelimiter;
        return result;
    }
    QByteArray raw;
    if (!cobsDecode(delimitedFrame.first(delimitedFrame.size() - 1), raw)) {
        result.error = DecodeError::InvalidCobs;
        return result;
    }
    if (raw.size() < 8) {
        result.error = DecodeError::TooShort;
        return result;
    }
    const quint16 payloadLength = readLe16(raw, 4);
    if (payloadLength > kMaxPayload) {
        result.error = DecodeError::PayloadTooLarge;
        return result;
    }
    if (raw.size() != 8 + payloadLength) {
        result.error = DecodeError::LengthMismatch;
        return result;
    }
    const quint16 expected = readLe16(raw, raw.size() - 2);
    if (crc16Ccitt(raw.first(raw.size() - 2)) != expected) {
        result.error = DecodeError::CrcMismatch;
        return result;
    }
    result.frame.version = static_cast<quint8>(raw[0]);
    result.frame.type = static_cast<quint8>(raw[1]);
    result.frame.sequence = readLe16(raw, 2);
    result.frame.payload = raw.sliced(6, payloadLength);
    return result;
}

} // namespace fpvdeck::protocol
