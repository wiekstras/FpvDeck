#pragma once

#include <QObject>
#include <QTimer>
#include <QVariantList>

class BatteryService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool connected READ connected NOTIFY changed)
    Q_PROPERTY(int cellCount READ cellCount NOTIFY changed)
    Q_PROPERTY(QVariantList cellVoltages READ cellVoltages NOTIFY changed)
    Q_PROPERTY(double packVoltage READ packVoltage NOTIFY changed)
    Q_PROPERTY(double delta READ delta NOTIFY changed)
    Q_PROPERTY(QString warning READ warning NOTIFY changed)
    Q_PROPERTY(QString scenario READ scenario NOTIFY changed)

public:
    explicit BatteryService(QObject* parent = nullptr);
    bool connected() const { return m_connected; }
    int cellCount() const { return m_cells.size(); }
    QVariantList cellVoltages() const;
    double packVoltage() const;
    double delta() const;
    QString warning() const { return m_warning; }
    QString scenario() const { return m_scenario; }

    Q_INVOKABLE void configureCells(int count);
    Q_INVOKABLE void setScenario(const QString& scenario);

signals:
    void changed();

private:
    void update();
    QVector<double> m_cells;
    QTimer m_timer;
    bool m_connected{true};
    QString m_warning;
    QString m_scenario{"nominal"};
    double m_phase{0.0};
};

