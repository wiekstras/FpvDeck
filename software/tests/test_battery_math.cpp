#include "core/BatteryMath.h"

#include <QTest>

class BatteryMathTest final : public QObject {
    Q_OBJECT

private slots:
    void validPacks_data()
    {
        QTest::addColumn<QVector<double>>("cells");
        for (int count = 1; count <= 6; ++count) {
            QVector<double> cells(count, 4.1);
            QTest::newRow(qPrintable(QString::number(count) + "S")) << cells;
        }
    }

    void validPacks()
    {
        QFETCH(QVector<double>, cells);
        QVector<double> taps;
        double cumulative = 0.0;
        for (const double cell : cells) {
            cumulative += cell;
            taps.push_back(cumulative);
        }
        const auto reading = fpvdeck::deriveCells(taps);
        QVERIFY(reading.valid);
        QCOMPARE(reading.cells.size(), cells.size());
        QVERIFY(qAbs(reading.packVoltage - cumulative) < 1e-9);
    }

    void imbalanceAndNoise()
    {
        const auto reading = fpvdeck::deriveCells({4.101, 8.198, 12.300, 16.020});
        QVERIFY(reading.valid);
        QCOMPARE(reading.cells.size(), 4);
        QVERIFY(qAbs(reading.cells.last() - 3.720) < 1e-9);
        QVERIFY(reading.delta > 0.38);
    }

    void rejectsFaults_data()
    {
        QTest::addColumn<QVector<double>>("taps");
        QTest::newRow("no connector") << QVector<double>{};
        QTest::newRow("missing/reversed cell") << QVector<double>{4.0, 8.0, 7.9};
        QTest::newRow("undetectably low impossible cell") << QVector<double>{4.0, 4.2};
        QTest::newRow("overvoltage") << QVector<double>{5.1};
        QTest::newRow("nan") << QVector<double>{qQNaN()};
        QTest::newRow("too many") << QVector<double>{1, 2, 3, 4, 5, 6, 7, 8, 9};
    }

    void rejectsFaults()
    {
        QFETCH(QVector<double>, taps);
        QVERIFY(!fpvdeck::deriveCells(taps).valid);
    }
};

QTEST_APPLESS_MAIN(BatteryMathTest)
#include "test_battery_math.moc"

