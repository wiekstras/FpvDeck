#include "core/FrameCodec.h"

#include <QTest>

using namespace fpvdeck::protocol;

class FrameCodecTest final : public QObject {
    Q_OBJECT

private slots:
    void crcKnownVector()
    {
        QCOMPARE(crc16Ccitt("123456789"), static_cast<quint16>(0x29b1));
    }

    void roundTrip()
    {
        Frame frame{1, 0x21, 0xbeef, QByteArray::fromHex("00010200ff55")};
        const QByteArray encoded = encode(frame);
        QVERIFY(!encoded.isEmpty());
        QCOMPARE(encoded.at(encoded.size() - 1), '\0');
        const auto decoded = decode(encoded);
        QVERIFY(decoded.ok());
        QCOMPARE(decoded.frame, frame);
    }

    void catchesCorruption()
    {
        QByteArray encoded = encode(Frame{1, 2, 3, "heartbeat"});
        encoded[3] = static_cast<char>(encoded[3] ^ 0x20);
        QVERIFY(!decode(encoded).ok());
    }

    void rejectsMissingDelimiter()
    {
        QByteArray encoded = encode(Frame{1, 2, 3, "ok"});
        encoded.chop(1);
        QCOMPARE(decode(encoded).error, DecodeError::MissingDelimiter);
    }

    void refusesOversizedPayload()
    {
        Frame frame;
        frame.payload.fill('x', kMaxPayload + 1);
        QVERIFY(encode(frame).isEmpty());
    }
};

QTEST_APPLESS_MAIN(FrameCodecTest)
#include "test_frame_codec.moc"
