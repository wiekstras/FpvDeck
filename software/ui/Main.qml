import QtQuick
import QtQuick.Controls
import FPVDeck
import "pages"
import "components"

ApplicationWindow {
    id: window
    width: 1280; height: 720; minimumWidth: 960; minimumHeight: 540
    visible: true; color: Theme.background; title: "FpvDeck Simulator"
    property string currentApp: "fpv"
    property bool simulatorOpen: false

    function moveFocus(forward) {
        const start = window.activeFocusItem ? window.activeFocusItem : window.contentItem
        const next = start.nextItemInFocusChain(forward)
        if (next) next.forceActiveFocus(Qt.TabFocusReason)
    }

    function activateFocus() {
        const item = window.activeFocusItem
        if (!item) { moveFocus(true); return }
        if (item.clicked) item.clicked()
    }

    function performOpen(app) { currentApp = app; simulatorOpen = false; MediaService.close() }
    function openApp(app) { InteractionService.requestNavigation(app) }
    function back() {
        if (simulatorOpen) { simulatorOpen = false; return }
        if (MediaService.open) { MediaService.close(); return }
        if (currentApp !== "fpv") { performOpen("fpv"); return }
        if (InteractionService.controlsVisible) InteractionService.hideControls()
    }

    Component.onCompleted: {
        if (StartupDemoState.indexOf("fpv") === 0) performOpen("fpv")
        else if (StartupDemoState === "media-playback") { currentApp = "media"; simulatorOpen = false }
        else if (StartupDemoState === "media") performOpen("media")
        else performOpen(StartupDemoState)
    }

    HomePage { anchors.fill: parent; visible: window.currentApp === "home" && !window.simulatorOpen; onAppRequested: app => window.openApp(app) }
    FpvPage { id: fpv; anchors.fill: parent; visible: window.currentApp === "fpv" && !window.simulatorOpen; onAppRequested: app => window.openApp(app) }
    BatteryPage { anchors.fill: parent; visible: window.currentApp === "battery" && !window.simulatorOpen }
    MediaPage { anchors.fill: parent; visible: window.currentApp === "media" && !window.simulatorOpen }
    FlightsPage { anchors.fill: parent; visible: window.currentApp === "flights" && !window.simulatorOpen }
    BatteriesPage { anchors.fill: parent; visible: window.currentApp === "batteries" && !window.simulatorOpen }
    ReceiverPage { anchors.fill: parent; visible: window.currentApp === "receiver" && !window.simulatorOpen }
    DvrPage { anchors.fill: parent; visible: window.currentApp === "dvr" && !window.simulatorOpen }
    AircraftPage { anchors.fill: parent; visible: window.currentApp === "aircraft" && !window.simulatorOpen }
    SettingsPage { anchors.fill: parent; visible: window.currentApp === "settings" && !window.simulatorOpen }
    DiagnosticsPage { anchors.fill: parent; visible: window.currentApp === "diagnostics" && !window.simulatorOpen }
    SimulatorPage { anchors.fill: parent; visible: window.simulatorOpen }

    TouchButton {
        visible: window.currentApp !== "fpv" && !window.simulatorOpen && !MediaService.open
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.margins: 18
        icon: "◉"; text: "RETURN TO FLY"; accent: Theme.accent
        onClicked: window.openApp("fpv")
        z: 30
    }

    Toast {
        id: toast
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 28
        z: 200
    }

    Rectangle {
        id: touchMarker
        width: 34; height: 34; radius: 17
        color: "transparent"; border.color: Theme.cyan; border.width: 3
        x: InteractionService.lastTouch.x - width / 2
        y: InteractionService.lastTouch.y - height / 2
        visible: SystemService.touchDebug
        z: 190
        SequentialAnimation on scale { running: SystemService.touchDebug; loops: 1; NumberAnimation { from: 1.4; to: 1.0; duration: 140 } }
    }
    TapHandler {
        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchScreen
        gesturePolicy: TapHandler.WithinBounds
        onTapped: function(point) { InteractionService.noteTouch(point.position.x, point.position.y) }
    }

    Connections {
        target: InteractionService
        function onNavigationRequested(app) { window.performOpen(app) }
        function onNavigationRejected(app) { toast.show("FLIGHT LOCK — " + app.toUpperCase() + " BLOCKED", true) }
        function onChanged() { InputService.navigationLocked = InteractionService.flightLocked }
    }
    Connections {
        target: InputService
        function onAction(action) {
            if (action === "menu" && window.currentApp === "fpv" && !window.simulatorOpen) InteractionService.showControls()
            else if (action === "dvr") DvrService.toggleRecording()
            else if (action === "battery" && window.currentApp === "fpv") InteractionService.showControls()
            else if (action === "signal") { VideoService.toggleLoss(); RadioService.toggleLoss() }
            else if (action === "simulator") window.simulatorOpen = !window.simulatorOpen
            else if (action === "right" || action === "down") window.moveFocus(true)
            else if (action === "left" || action === "up") window.moveFocus(false)
            else if (action === "select") window.activateFocus()
            else if (action === "back") window.back()
        }
        function onRejected(action) { toast.show("INPUT LOCKED: " + action.toUpperCase(), true) }
    }

    BootSplash { anchors.fill: parent; visible: !StartupDemoMode; z: 300; onComplete: visible = false }

    Shortcut { sequence: "M"; onActivated: InputService.dispatch("menu") }
    Shortcut { sequence: "R"; onActivated: InputService.dispatch("dvr") }
    Shortcut { sequence: "B"; onActivated: InputService.dispatch("battery") }
    Shortcut { sequence: "L"; onActivated: InputService.dispatch("signal") }
    Shortcut { sequence: "F10"; onActivated: InputService.dispatch("simulator") }
    Shortcut { sequence: "Right"; onActivated: InputService.dispatch("right") }
    Shortcut { sequence: "Down"; onActivated: InputService.dispatch("down") }
    Shortcut { sequence: "Left"; onActivated: InputService.dispatch("left") }
    Shortcut { sequence: "Up"; onActivated: InputService.dispatch("up") }
    Shortcut { sequence: "Return"; onActivated: InputService.dispatch("select") }
    Shortcut { sequence: "Enter"; onActivated: InputService.dispatch("select") }
    Shortcut { sequence: "Escape"; onActivated: InputService.dispatch("back") }
}
