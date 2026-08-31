#pragma once

#include <QObject>
#include <QSqlDatabase>
#include <QVariantList>

class DatabaseService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList recentFlights READ recentFlights NOTIFY recentFlightsChanged)
    Q_PROPERTY(QString path READ path CONSTANT)

public:
    explicit DatabaseService(QString path, QObject* parent = nullptr);
    ~DatabaseService() override;
    bool initialize(QString* error = nullptr);
    QVariantList recentFlights() const { return m_recentFlights; }
    QString path() const { return m_path; }
    Q_INVOKABLE bool addDemoFlight();

signals:
    void recentFlightsChanged();

private:
    bool migrate(QString* error);
    bool applyMigration2(QString* error);
    void reload();
    QString m_path;
    QString m_connectionName;
    QSqlDatabase m_database;
    QVariantList m_recentFlights;
};
