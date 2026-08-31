#pragma once

#include <QObject>
#include <QTimer>

class TelemetryService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY changed)
    Q_PROPERTY(int linkQuality READ linkQuality NOTIFY changed)
    Q_PROPERTY(int rssiDbm READ rssiDbm NOTIFY changed)
    Q_PROPERTY(int snrDb READ snrDb NOTIFY changed)
    Q_PROPERTY(double receiverVoltage READ receiverVoltage NOTIFY changed)
    Q_PROPERTY(double altitudeMeters READ altitudeMeters NOTIFY changed)
    Q_PROPERTY(double speedKph READ speedKph NOTIFY changed)
    Q_PROPERTY(QString flightMode READ flightMode NOTIFY changed)
    Q_PROPERTY(bool armed READ armed NOTIFY changed)
    Q_PROPERTY(QString state READ state NOTIFY changed)

public:
    explicit TelemetryService(QObject* parent = nullptr);
    bool connected() const { return m_state == "connected"; }
    int linkQuality() const { return m_linkQuality; }
    int rssiDbm() const { return m_rssiDbm; }
    int snrDb() const { return m_snrDb; }
    double receiverVoltage() const { return m_receiverVoltage; }
    double altitudeMeters() const { return m_altitudeMeters; }
    double speedKph() const { return m_speedKph; }
    QString flightMode() const { return m_flightMode; }
    bool armed() const { return m_armed; }
    QString state() const { return m_state; }
    Q_INVOKABLE void setScenario(const QString& scenario);

signals:
    void changed();

private:
    QTimer m_timer;
    QString m_state{"connected"};
    QString m_flightMode{"ACRO"};
    int m_linkQuality{96};
    int m_rssiDbm{-61};
    int m_snrDb{8};
    double m_receiverVoltage{5.02};
    double m_altitudeMeters{23.0};
    double m_speedKph{48.0};
    bool m_armed{true};
    double m_phase{0.0};
};

