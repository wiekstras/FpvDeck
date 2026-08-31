#include "services/DatabaseService.h"

#include <QSqlQuery>
#include <QTemporaryDir>
#include <QTest>

class DatabaseTest final : public QObject {
    Q_OBJECT

private slots:
    void migratesAndPersistsFlight()
    {
        QTemporaryDir directory;
        QVERIFY(directory.isValid());
        const QString path = directory.path() + "/test.sqlite3";
        {
            DatabaseService database(path);
            QString error;
            QVERIFY2(database.initialize(&error), qPrintable(error));
            QCOMPARE(database.recentFlights().size(), 0);
            QVERIFY(database.addDemoFlight());
            QCOMPARE(database.recentFlights().size(), 1);
            const QVariantMap flight = database.recentFlights().first().toMap();
            QCOMPARE(flight.value("channel").toString(), QString("R1"));
            QVERIFY(flight.value("durationSeconds").toInt() >= 246);
        }
        {
            DatabaseService reopened(path);
            QString error;
            QVERIFY2(reopened.initialize(&error), qPrintable(error));
            QCOMPARE(reopened.recentFlights().size(), 1);
        }
    }

    void seedsDeterministicShowcaseHistory()
    {
        DatabaseService database(":memory:");
        QString error;
        QVERIFY2(database.initialize(&error), qPrintable(error));
        QVERIFY(database.seedShowcaseData());
        QCOMPARE(database.recentFlights().size(), 3);
        const QVariantMap newest = database.recentFlights().first().toMap();
        QCOMPARE(newest.value("startedAt").toString(), QString("2026-08-30T17:42:10Z"));
        QVERIFY(newest.value("durationSeconds").toInt() >= 380);
        QVERIFY(newest.value("durationSeconds").toInt() <= 381);
        QCOMPARE(newest.value("channel").toString(), QString("R1"));
        QVERIFY(database.seedShowcaseData());
        QCOMPARE(database.recentFlights().size(), 3);
    }
};

QTEST_APPLESS_MAIN(DatabaseTest)
#include "test_database.moc"
