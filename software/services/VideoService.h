#pragma once

#include <QObject>
#include <QTimer>
#include <QUrl>

class VideoService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString standard READ standard NOTIFY changed)
    Q_PROPERTY(QString state READ state NOTIFY changed)
    Q_PROPERTY(int channel READ channel NOTIFY changed)
    Q_PROPERTY(int rssi READ rssi NOTIFY changed)
    Q_PROPERTY(bool rolling READ rolling NOTIFY changed)
    Q_PROPERTY(QUrl source READ source CONSTANT)

public:
    explicit VideoService(QUrl source, QObject* parent = nullptr);
    QString standard() const { return m_standard; }
    QString state() const { return m_state; }
    int channel() const { return m_channel; }
    int rssi() const { return m_rssi; }
    bool rolling() const { return m_state == "rolling"; }
    QUrl source() const { return m_source; }

    Q_INVOKABLE void setScenario(const QString& scenario);
    Q_INVOKABLE void toggleLoss();
    Q_INVOKABLE void setStandard(const QString& standard);
    Q_INVOKABLE void nextChannel();

signals:
    void changed();

private:
    QUrl m_source;
    QTimer m_timer;
    QString m_standard{"PAL"};
    QString m_state{"locked"};
    int m_channel{1};
    int m_rssi{82};
    double m_phase{0.0};
};

