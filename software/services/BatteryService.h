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
    Q_PROPERTY(QString chemistry READ chemistry CONSTANT)
    Q_PROPERTY(double weakestVoltage READ weakestVoltage NOTIFY changed)
    Q_PROPERTY(double strongestVoltage READ strongestVoltage NOTIFY changed)
    Q_PROPERTY(int weakestCell READ weakestCell NOTIFY changed)
    Q_PROPERTY(int strongestCell READ strongestCell NOTIFY changed)
    Q_PROPERTY(QString health READ health NOTIFY changed)

public:
    explicit BatteryService(QObject* parent = nullptr);
    bool connected() const { return m_connected; }
    int cellCount() const { return m_cells.size(); }
    QVariantList cellVoltages() const;
    double packVoltage() const;
    double delta() const;
    QString warning() const { return m_warning; }
    QString scenario() const { return m_scenario; }
    QString chemistry() const { return "LiPo"; }
    double weakestVoltage() const;
    double strongestVoltage() const;
    int weakestCell() const;
    int strongestCell() const;
    QString health() const;

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
