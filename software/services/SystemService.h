#pragma once

#include <QObject>
#include <QTimer>

class SystemService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(int deckBatteryPercent READ deckBatteryPercent NOTIFY changed)
    Q_PROPERTY(bool deckCharging READ deckCharging NOTIFY changed)
    Q_PROPERTY(double temperatureC READ temperatureC NOTIFY changed)
    Q_PROPERTY(bool mcuOnline READ mcuOnline NOTIFY changed)
    Q_PROPERTY(bool adcHealthy READ adcHealthy NOTIFY changed)
    Q_PROPERTY(bool t8lConnected READ t8lConnected NOTIFY changed)
    Q_PROPERTY(bool elrsReady READ elrsReady NOTIFY changed)
    Q_PROPERTY(bool vrxOnline READ vrxOnline NOTIFY changed)
    Q_PROPERTY(bool decoderOnline READ decoderOnline NOTIFY changed)
    Q_PROPERTY(bool touchDebug READ touchDebug WRITE setTouchDebug NOTIFY changed)
    Q_PROPERTY(qint64 uptimeSeconds READ uptimeSeconds NOTIFY changed)
    Q_PROPERTY(QString warning READ warning NOTIFY changed)
    Q_PROPERTY(int displayBrightness READ displayBrightness NOTIFY changed)

public:
    explicit SystemService(QObject* parent = nullptr);
    int deckBatteryPercent() const { return m_deckBatteryPercent; }
    bool deckCharging() const { return m_deckCharging; }
    double temperatureC() const { return m_temperatureC; }
    bool mcuOnline() const { return m_mcuOnline; }
    bool adcHealthy() const { return m_adcHealthy; }
    bool t8lConnected() const { return m_t8lConnected; }
    bool elrsReady() const { return m_elrsReady; }
    bool vrxOnline() const { return m_vrxOnline; }
    bool decoderOnline() const { return m_decoderOnline; }
    bool touchDebug() const { return m_touchDebug; }
    qint64 uptimeSeconds() const { return m_uptimeSeconds; }
    QString warning() const;
    int displayBrightness() const { return m_displayBrightness; }

    void setTouchDebug(bool enabled);
    Q_INVOKABLE void setScenario(const QString& scenario);
    Q_INVOKABLE void setCharging(bool charging);
    Q_INVOKABLE void resetFaults();
    Q_INVOKABLE void setDisplayBrightness(int percent);

signals:
    void changed();

private:
    QTimer m_timer;
    int m_deckBatteryPercent{78};
    bool m_deckCharging{false};
    double m_temperatureC{48.0};
    bool m_mcuOnline{true};
    bool m_adcHealthy{true};
    bool m_t8lConnected{true};
    bool m_elrsReady{true};
    bool m_vrxOnline{true};
    bool m_decoderOnline{true};
    bool m_touchDebug{false};
    qint64 m_uptimeSeconds{0};
    int m_displayBrightness{78};
};
