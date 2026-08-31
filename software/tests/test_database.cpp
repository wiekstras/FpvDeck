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
};

QTEST_APPLESS_MAIN(DatabaseTest)
#include "test_database.moc"
