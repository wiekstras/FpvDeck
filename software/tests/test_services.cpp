#include "services/BatteryService.h"
#include "services/InputService.h"
#include "services/InteractionService.h"
#include "services/MediaService.h"
#include "services/RadioService.h"
#include "services/StorageService.h"
#include "services/SystemService.h"
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
        QCOMPARE(battery.weakestCell(), battery.cellCount());
        QCOMPARE(battery.strongestCell(), 1);
        QCOMPARE(battery.health(), QString("ATTENTION"));
        battery.setScenario("disconnected");
        QVERIFY(!battery.connected());
        QCOMPARE(battery.health(), QString("DISCONNECTED"));
    }

    void inputAllowlistAndLockout()
    {
        InputService input;
        QSignalSpy actions(&input, &InputService::action);
        QSignalSpy rejected(&input, &InputService::rejected);
        input.dispatch("menu");
        QCOMPARE(actions.count(), 1);
        input.dispatch("right");
        input.dispatch("select");
        QCOMPARE(actions.count(), 3);
        input.dispatch("gimbal-left");
        QCOMPARE(rejected.count(), 1);
        input.setNavigationLocked(true);
        input.dispatch("menu");
        QCOMPARE(rejected.count(), 2);
        input.dispatch("dvr");
        QCOMPARE(actions.count(), 4);
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
        QCOMPARE(video.state(), QString("lost"));

        RadioService radio;
        radio.setScenario("lost");
        QCOMPARE(radio.rssi(), 0);
    }

    void touchControlsAutoHideAndFlightLock()
    {
        InteractionService interaction;
        QSignalSpy requested(&interaction, &InteractionService::navigationRequested);
        QSignalSpy rejected(&interaction, &InteractionService::navigationRejected);
        interaction.setAutoHideMs(100);
        interaction.showControls();
        QVERIFY(interaction.controlsVisible());
        QTRY_VERIFY_WITH_TIMEOUT(!interaction.controlsVisible(), 300);

        interaction.setFlightLocked(true);
        QVERIFY(!interaction.requestNavigation("settings"));
        QCOMPARE(rejected.count(), 1);
        QVERIFY(interaction.requestNavigation("fpv"));
        QCOMPARE(requested.count(), 1);
        interaction.noteTouch(320, 180);
        QCOMPARE(interaction.lastTouch(), QPointF(320, 180));
    }

    void removableStorageFaults()
    {
        StorageService storage;
        QVERIFY(storage.sdPresent());
        QVERIFY(!storage.mediaFiles().isEmpty());
        storage.setScenario("removed");
        QCOMPARE(storage.status(), QString("SD REMOVED"));
        storage.setScenario("corrupt");
        QCOMPARE(storage.status(), QString("SD NEEDS CHECK"));
        storage.setScenario("full");
        QCOMPARE(storage.status(), QString("SD ALMOST FULL"));
        storage.setScenario("inserted");
        QCOMPARE(storage.status(), QString("SD READY"));
    }

    void mediaPlaybackState()
    {
        MediaService media(QUrl::fromLocalFile("/tmp/fpvdeck-test.mp4"));
        media.openFile("TEST RUN", 120, "16:9");
        QVERIFY(media.open());
        QVERIFY(media.playing());
        QCOMPARE(media.aspect(), QString("16:9"));
        media.seek(200);
        QCOMPARE(media.positionSeconds(), 120);
        media.skip(-10);
        QCOMPARE(media.positionSeconds(), 110);
        media.togglePlaying();
        QVERIFY(!media.playing());
        media.close();
        QVERIFY(!media.open());
    }

    void systemFaultInjectionAndBrightness()
    {
        SystemService system;
        QVERIFY(system.warning().isEmpty());
        system.setScenario("mcu offline");
        QCOMPARE(system.warning(), QString("CONTROLLER OFFLINE"));
        system.resetFaults();
        system.setScenario("adc error");
        QCOMPARE(system.warning(), QString("BALANCE ADC ERROR"));
        system.resetFaults();
        system.setScenario("t8l offline");
        QCOMPARE(system.warning(), QString("T8L CONTROL OFFLINE"));
        system.resetFaults();
        system.setScenario("elrs not ready");
        QCOMPARE(system.warning(), QString("ELRS TX NOT READY"));
        system.resetFaults();
        system.setScenario("vrx offline");
        QCOMPARE(system.warning(), QString("VIDEO RECEIVER OFFLINE"));
        system.resetFaults();
        system.setScenario("decoder offline");
        QCOMPARE(system.warning(), QString("VIDEO DECODER OFFLINE"));
        system.resetFaults();
        system.setScenario("over temp");
        QCOMPARE(system.warning(), QString("SYSTEM OVER TEMPERATURE"));
        system.setDisplayBrightness(2);
        QCOMPARE(system.displayBrightness(), 10);
        system.setDisplayBrightness(150);
        QCOMPARE(system.displayBrightness(), 100);
    }

    void receiverScanAndFavorites()
    {
        RadioService radio;
        radio.setChannel(3);
        QCOMPARE(radio.channel(), 3);
        radio.toggleFavorite();
        QVERIFY(radio.favorite());
        radio.scanStrongest();
        QVERIFY(radio.scanning());
        QTRY_VERIFY_WITH_TIMEOUT(!radio.scanning(), 1000);
        QCOMPARE(radio.channel(), 6);
        QCOMPARE(radio.rssi(), 93);
    }
};

QTEST_GUILESS_MAIN(ServicesTest)
#include "test_services.moc"
