#include "services/BatteryService.h"
#include "services/InputService.h"
#include "services/TelemetryService.h"
#include "services/VideoService.h"

#include <QSignalSpy>
#include <QTest>

class ServicesTest final : public QObject {
    Q_OBJECT

private slots:
    void batteryScenarios()
    {
        BatteryService battery;
        for (int cells = 1; cells <= 6; ++cells) {
            battery.configureCells(cells);
            QCOMPARE(battery.cellCount(), cells);
            QVERIFY(battery.connected());
        }
        battery.setScenario("imbalanced");
        QVERIFY(battery.delta() > 0.3);
        QVERIFY(!battery.warning().isEmpty());
        battery.setScenario("disconnected");
        QVERIFY(!battery.connected());
    }

    void inputAllowlistAndLockout()
    {
        InputService input;
        QSignalSpy actions(&input, &InputService::action);
        QSignalSpy rejected(&input, &InputService::rejected);
        input.dispatch("menu");
        QCOMPARE(actions.count(), 1);
        input.dispatch("gimbal-left");
        QCOMPARE(rejected.count(), 1);
        input.setNavigationLocked(true);
        input.dispatch("menu");
        QCOMPARE(rejected.count(), 2);
        input.dispatch("dvr");
        QCOMPARE(actions.count(), 2);
    }

    void telemetryFailureIsExplicit()
    {
        TelemetryService telemetry;
        QVERIFY(telemetry.connected());
        telemetry.setScenario("corrupt");
        QVERIFY(!telemetry.connected());
        QCOMPARE(telemetry.linkQuality(), 0);
        telemetry.setScenario("connected");
        QVERIFY(telemetry.connected());
    }

    void videoStandardValidation()
    {
        VideoService video(QUrl::fromLocalFile("/nonexistent"));
        video.setStandard("NTSC");
        QCOMPARE(video.standard(), QString("NTSC"));
        video.setStandard("SECAM");
        QCOMPARE(video.standard(), QString("NTSC"));
        video.setScenario("lost");
        QCOMPARE(video.rssi(), 0);
    }
};

QTEST_APPLESS_MAIN(ServicesTest)
#include "test_services.moc"
