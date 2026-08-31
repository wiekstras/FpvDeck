import QtQuick
import QtQuick.Controls
import "pages"

ApplicationWindow {
    id: window
    width: 1280; height: 720; minimumWidth: 960; minimumHeight: 540
    visible: true; color: "#080d14"; title: "FpvDeck Simulator"
    property string currentApp: "fpv"
    property bool simulatorOpen: false

    function openApp(app) { currentApp = app; simulatorOpen = false }
    function back() { if (simulatorOpen) simulatorOpen = false; else if (currentApp !== "fpv") currentApp = "fpv"; else fpv.menuOpen = false }

    Component.onCompleted: openApp(StartupDemoState)

    HomePage { anchors.fill: parent; visible: window.currentApp === "home" && !window.simulatorOpen; onAppRequested: app => window.openApp(app) }
    FpvPage { id: fpv; anchors.fill: parent; visible: window.currentApp === "fpv" && !window.simulatorOpen; onAppRequested: app => window.openApp(app) }
    BatteryPage { anchors.fill: parent; visible: window.currentApp === "battery" && !window.simulatorOpen }
    MediaPage { anchors.fill: parent; visible: window.currentApp === "media" && !window.simulatorOpen }
    FlightsPage { anchors.fill: parent; visible: window.currentApp === "flights" && !window.simulatorOpen }
    BatteriesPage { anchors.fill: parent; visible: window.currentApp === "batteries" && !window.simulatorOpen }
    DvrPage { anchors.fill: parent; visible: window.currentApp === "dvr" && !window.simulatorOpen }
    AircraftPage { anchors.fill: parent; visible: window.currentApp === "aircraft" && !window.simulatorOpen }
    SettingsPage { anchors.fill: parent; visible: window.currentApp === "settings" && !window.simulatorOpen }
    DiagnosticsPage { anchors.fill: parent; visible: window.currentApp === "diagnostics" && !window.simulatorOpen }
    SimulatorPage { anchors.fill: parent; visible: window.simulatorOpen }

    Rectangle {
        visible: window.currentApp !== "fpv" && !window.simulatorOpen
        anchors { left: parent.left; bottom: parent.bottom; margins: 18 }
        width: backHint.implicitWidth + 20; height: 29; radius: 8; color: "#182334"
        Text { id: backHint; anchors.centerIn: parent; text: "ESC  BACK TO FPV"; color: "#91a2b8"; font.pixelSize: 10; font.letterSpacing: 1 }
    }

    Connections {
        target: InputService
        function onAction(action) {
            if (action === "menu" && window.currentApp === "fpv" && !window.simulatorOpen) fpv.menuOpen = !fpv.menuOpen
            else if (action === "dvr") DvrService.toggleRecording()
            else if (action === "battery" && window.currentApp === "fpv") fpv.batteryExpanded = !fpv.batteryExpanded
            else if (action === "signal") VideoService.toggleLoss()
            else if (action === "simulator") window.simulatorOpen = !window.simulatorOpen
            else if (action === "back") window.back()
        }
    }
    Shortcut { sequence: "M"; onActivated: InputService.dispatch("menu") }
    Shortcut { sequence: "R"; onActivated: InputService.dispatch("dvr") }
    Shortcut { sequence: "B"; onActivated: InputService.dispatch("battery") }
    Shortcut { sequence: "L"; onActivated: InputService.dispatch("signal") }
    Shortcut { sequence: "F10"; onActivated: InputService.dispatch("simulator") }
    Shortcut { sequence: "Escape"; onActivated: InputService.dispatch("back") }
}
