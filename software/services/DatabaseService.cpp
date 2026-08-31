#include "DatabaseService.h"

#include <QDateTime>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>

DatabaseService::DatabaseService(QString path, QObject* parent)
    : QObject(parent), m_path(std::move(path)),
      m_connectionName("fpvdeck-" + QUuid::createUuid().toString(QUuid::WithoutBraces))
{
}

DatabaseService::~DatabaseService()
{
    if (m_database.isValid()) m_database.close();
    m_database = {};
    QSqlDatabase::removeDatabase(m_connectionName);
}

bool DatabaseService::initialize(QString* error)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_database.setDatabaseName(m_path);
    if (!m_database.open()) {
        if (error) *error = m_database.lastError().text();
        return false;
    }
    QSqlQuery pragmas(m_database);
    pragmas.exec("PRAGMA foreign_keys=ON");
    pragmas.exec("PRAGMA journal_mode=WAL");
    pragmas.exec("PRAGMA synchronous=FULL");
    if (!migrate(error)) return false;
    reload();
    return true;
}

bool DatabaseService::migrate(QString* error)
{
    QSqlQuery query(m_database);
    const QStringList statements = {
        "CREATE TABLE IF NOT EXISTS schema_migrations(version INTEGER PRIMARY KEY, applied_at TEXT NOT NULL)",
        "CREATE TABLE IF NOT EXISTS aircraft(id INTEGER PRIMARY KEY, name TEXT NOT NULL UNIQUE, notes TEXT NOT NULL DEFAULT '')",
        "CREATE TABLE IF NOT EXISTS batteries(id INTEGER PRIMARY KEY, battery_uid TEXT UNIQUE, name TEXT NOT NULL, chemistry TEXT NOT NULL, cell_count INTEGER NOT NULL CHECK(cell_count BETWEEN 1 AND 12), capacity_mah INTEGER, cycle_count INTEGER NOT NULL DEFAULT 0, notes TEXT NOT NULL DEFAULT '')",
        "CREATE TABLE IF NOT EXISTS flights(id INTEGER PRIMARY KEY, started_at TEXT NOT NULL, ended_at TEXT, aircraft_id INTEGER REFERENCES aircraft(id), battery_id INTEGER REFERENCES batteries(id), receiver_channel TEXT, min_rssi REAL, min_lq REAL, dvr_path TEXT, notes TEXT NOT NULL DEFAULT '', software_version TEXT NOT NULL)",
        "CREATE TABLE IF NOT EXISTS telemetry_samples(flight_id INTEGER NOT NULL REFERENCES flights(id) ON DELETE CASCADE, monotonic_ms INTEGER NOT NULL, pack_voltage REAL, rssi REAL, lq REAL, latitude REAL, longitude REAL, altitude_m REAL, PRIMARY KEY(flight_id, monotonic_ms))",
        "INSERT OR IGNORE INTO schema_migrations(version, applied_at) VALUES(1, datetime('now'))",
    };
    if (!m_database.transaction()) return false;
    for (const QString& statement : statements) {
        if (!query.exec(statement)) {
            m_database.rollback();
            if (error) *error = query.lastError().text();
            return false;
        }
    }
    if (!m_database.commit()) return false;
    return applyMigration2(error);
}

bool DatabaseService::applyMigration2(QString* error)
{
    QSqlQuery query(m_database);
    if (!query.exec("SELECT 1 FROM schema_migrations WHERE version=2")) {
        if (error) *error = query.lastError().text();
        return false;
    }
    if (query.next()) return true;

    const QStringList statements = {
        "ALTER TABLE aircraft ADD COLUMN aircraft_uid TEXT",
        "ALTER TABLE aircraft ADD COLUMN model TEXT NOT NULL DEFAULT ''",
        "ALTER TABLE aircraft ADD COLUMN created_at TEXT NOT NULL DEFAULT ''",
        "CREATE UNIQUE INDEX IF NOT EXISTS aircraft_uid_unique ON aircraft(aircraft_uid) WHERE aircraft_uid IS NOT NULL",
        "ALTER TABLE batteries ADD COLUMN brand TEXT NOT NULL DEFAULT ''",
        "ALTER TABLE batteries ADD COLUMN purchase_date TEXT",
        "ALTER TABLE batteries ADD COLUMN estimated_health REAL",
        "ALTER TABLE batteries ADD COLUMN created_at TEXT NOT NULL DEFAULT ''",
        "ALTER TABLE flights ADD COLUMN max_distance_m REAL",
        "ALTER TABLE flights ADD COLUMN max_altitude_m REAL",
        "ALTER TABLE flights ADD COLUMN minimum_pack_voltage REAL",
        "ALTER TABLE flights ADD COLUMN maximum_cell_delta REAL",
        "ALTER TABLE flights ADD COLUMN aircraft_firmware TEXT",
        "CREATE TABLE recordings(id INTEGER PRIMARY KEY, flight_id INTEGER REFERENCES flights(id) ON DELETE SET NULL, path TEXT NOT NULL UNIQUE, started_at TEXT NOT NULL, ended_at TEXT, stream_kind TEXT NOT NULL CHECK(stream_kind IN ('pre-overlay','composited')), container TEXT NOT NULL, recovered INTEGER NOT NULL DEFAULT 0, size_bytes INTEGER)",
        "CREATE TABLE battery_observations(battery_id INTEGER NOT NULL REFERENCES batteries(id) ON DELETE CASCADE, observed_at TEXT NOT NULL, pack_voltage REAL NOT NULL, cell_voltages_json TEXT NOT NULL, delta REAL NOT NULL, source TEXT NOT NULL, PRIMARY KEY(battery_id, observed_at))",
        "CREATE INDEX IF NOT EXISTS flights_battery_started ON flights(battery_id, started_at)",
        "CREATE INDEX IF NOT EXISTS flights_aircraft_started ON flights(aircraft_id, started_at)",
        "INSERT INTO schema_migrations(version, applied_at) VALUES(2, datetime('now'))",
    };
    if (!m_database.transaction()) return false;
    for (const QString& statement : statements) {
        if (!query.exec(statement)) {
            m_database.rollback();
            if (error) *error = query.lastError().text();
            return false;
        }
    }
    return m_database.commit();
}

bool DatabaseService::addDemoFlight()
{
    QSqlQuery query(m_database);
    query.prepare("INSERT INTO flights(started_at, ended_at, receiver_channel, min_rssi, notes, software_version) VALUES(?, ?, ?, ?, ?, ?)");
    const QDateTime end = QDateTime::currentDateTimeUtc();
    query.addBindValue(end.addSecs(-247).toString(Qt::ISODate));
    query.addBindValue(end.toString(Qt::ISODate));
    query.addBindValue("R1");
    query.addBindValue(41.0);
    query.addBindValue("Simulator flight");
    query.addBindValue("0.1.0-dev");
    const bool result = query.exec();
    if (result) reload();
    return result;
}

void DatabaseService::reload()
{
    m_recentFlights.clear();
    QSqlQuery query(m_database);
    query.exec("SELECT id, started_at, CAST((julianday(COALESCE(ended_at, datetime('now'))) - julianday(started_at)) * 86400 AS INTEGER), COALESCE(receiver_channel, '—'), COALESCE(min_rssi, 0) FROM flights ORDER BY started_at DESC LIMIT 20");
    while (query.next()) {
        QVariantMap flight;
        flight["id"] = query.value(0);
        flight["startedAt"] = query.value(1);
        flight["durationSeconds"] = query.value(2);
        flight["channel"] = query.value(3);
        flight["minRssi"] = query.value(4);
        m_recentFlights.append(flight);
    }
    emit recentFlightsChanged();
}
