#pragma once

#include <QObject>
#include <QSet>
#include <QTimer>

/*
 * Dedicated analog-video receiver state. This service represents FpvDeck's
 * 5.8 GHz RF-to-CVBS stage; it never represents the T8L 2.4 GHz ELRS control TX
 * or the downstream CVBS decoder/capture path.
 */
class RadioService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(int channel READ channel NOTIFY changed)
    Q_PROPERTY(int rssi READ rssi NOTIFY changed)
    Q_PROPERTY(bool favorite READ favorite NOTIFY changed)
    Q_PROPERTY(bool scanning READ scanning NOTIFY changed)

public:
    explicit RadioService(QObject* parent = nullptr);
    int channel() const { return m_channel; }
    int rssi() const { return m_rssi; }
    bool favorite() const { return m_favorites.contains(m_channel); }
    bool scanning() const { return m_scanning; }

    Q_INVOKABLE void setScenario(const QString& scenario);
    Q_INVOKABLE void toggleLoss();
    Q_INVOKABLE void nextChannel();
    Q_INVOKABLE void setChannel(int channel);
    Q_INVOKABLE void toggleFavorite();
    Q_INVOKABLE void scanStrongest();

signals:
    void changed();

private:
    QTimer m_timer;
    QString m_state{"locked"};
    int m_channel{1};
    int m_rssi{82};
    double m_phase{0.0};
    QSet<int> m_favorites{1, 4};
    bool m_scanning{false};
};
