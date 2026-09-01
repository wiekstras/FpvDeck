#include "services/BatteryService.h"
#include "services/DatabaseService.h"
#include "services/DvrService.h"
#include "services/InputService.h"
#include "services/InteractionService.h"
#include "services/MediaService.h"
#include "services/StorageService.h"
#include "services/SystemService.h"
#include "services/TelemetryService.h"
#include "services/VideoService.h"

#include <QCommandLineParser>
#include <QDir>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickWindow>
#include <QStandardPaths>
#include <QTimer>

int main(int argc, char* argv[])
{
    QGuiApplication application(argc, argv);
    QCoreApplication::setOrganizationName("FpvDeck");
    QCoreApplication::setApplicationName("FPVDeck");
    QCoreApplication::setApplicationVersion("0.1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("FPVDeck hardware-independent simulator");
    parser.addHelpOption();
    parser.addVersionOption();
    QCommandLineOption videoOption({"i", "video"}, "Synthetic/test video file", "path");
    QCommandLineOption databaseOption({"d", "database"}, "SQLite database path", "path");
    QCommandLineOption exitOption("exit-after-ms", "Exit automatically (test use)", "milliseconds");
    QCommandLineOption screenshotOption("screenshot", "Save a rendered screenshot", "path");
    QCommandLineOption demoStateOption("demo-state", "Open a deterministic showcase state", "state", "fpv");
    parser.addOption(videoOption);
    parser.addOption(databaseOption);
    parser.addOption(exitOption);
    parser.addOption(screenshotOption);
    parser.addOption(demoStateOption);
    parser.process(application);

    const QString dataDirectory = qEnvironmentVariableIsSet("FPVDECK_DATA_DIR")
        ? qEnvironmentVariable("FPVDECK_DATA_DIR")
        : QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir().mkpath(dataDirectory);
    const QString databasePath = parser.isSet(databaseOption)
        ? parser.value(databaseOption) : dataDirectory + "/fpvdeck.sqlite3";
    const QString videoPath = parser.isSet(videoOption)
        ? parser.value(videoOption) : QDir::currentPath() + "/generated/fpv-test-pal.mp4";

    BatteryService batteryService;
    DvrService dvrService;
    InputService inputService;
    InteractionService interactionService;
    MediaService mediaService(QUrl::fromLocalFile(videoPath));
    StorageService storageService;
    SystemService systemService;
    TelemetryService telemetryService;
    VideoService videoService(QUrl::fromLocalFile(videoPath));
    DatabaseService databaseService(databasePath);
    QString databaseError;
    if (!databaseService.initialize(&databaseError)) {
        qCritical("Database initialization failed: %s", qPrintable(databaseError));
        return 2;
    }
    const QString demoState = parser.value(demoStateOption).trimmed().toLower();
    const QStringList validDemoStates = {
        "home", "fpv", "fpv-controls", "fpv-lock", "battery", "media",
        "media-playback", "receiver", "flights", "diagnostics"
    };
    if (parser.isSet(demoStateOption) && !validDemoStates.contains(demoState)) {
        qCritical("Unknown showcase state: %s", qPrintable(demoState));
        return 7;
    }
    if (parser.isSet(demoStateOption) && !databaseService.seedShowcaseData()) {
        qCritical("Could not seed showcase data");
        return 6;
    }
    if (parser.isSet(demoStateOption) && demoState == "battery") batteryService.setScenario("imbalanced");
    if (parser.isSet(demoStateOption) && demoState.startsWith("fpv")) dvrService.toggleRecording();
    if (parser.isSet(demoStateOption) && demoState == "fpv-controls") interactionService.showControls();
    if (parser.isSet(demoStateOption) && demoState == "fpv-lock") {
        interactionService.setFlightLocked(true);
        interactionService.showControls();
    }
    if (parser.isSet(demoStateOption) && demoState == "media-playback") {
        mediaService.openFile("RIDGE RUN", 381, "4:3");
    }

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("BatteryService", &batteryService);
    engine.rootContext()->setContextProperty("DvrService", &dvrService);
    engine.rootContext()->setContextProperty("InputService", &inputService);
    engine.rootContext()->setContextProperty("InteractionService", &interactionService);
    engine.rootContext()->setContextProperty("MediaService", &mediaService);
    engine.rootContext()->setContextProperty("StorageService", &storageService);
    engine.rootContext()->setContextProperty("SystemService", &systemService);
    engine.rootContext()->setContextProperty("TelemetryService", &telemetryService);
    engine.rootContext()->setContextProperty("VideoService", &videoService);
    engine.rootContext()->setContextProperty("DatabaseService", &databaseService);
    engine.rootContext()->setContextProperty("StartupDemoState", demoState);
    engine.rootContext()->setContextProperty("StartupDemoMode", parser.isSet(demoStateOption));
    engine.load(QUrl(QStringLiteral("qrc:/FPVDeck/ui/Main.qml")));
    if (engine.rootObjects().isEmpty()) return 3;
    if (parser.isSet(screenshotOption)) {
        QTimer::singleShot(1500, &application, [&application, &engine, &parser] {
            auto* window = qobject_cast<QQuickWindow*>(engine.rootObjects().constFirst());
            if (!window || !window->grabWindow().save(parser.value("screenshot"))) {
                application.exit(4);
                return;
            }
            application.quit();
        });
    } else if (parser.isSet(exitOption)) {
        bool valid = false;
        const int milliseconds = parser.value(exitOption).toInt(&valid);
        if (!valid || milliseconds < 1) return 5;
        QTimer::singleShot(milliseconds, &application, &QCoreApplication::quit);
    }
    return application.exec();
}
