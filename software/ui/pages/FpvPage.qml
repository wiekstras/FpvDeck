import QtQuick
import QtMultimedia
import FPVDeck
import "../components"

Item {
    id: root
    signal appRequested(string app)
    property bool menuOpen: false
    property int videoFrameCount: 0
    property int lastVideoFrameCount: 0
    property int videoFps: 0

    MediaPlayer {
        id: player
        source: VideoService.source
        videoOutput: VideoService.liveCapture ? null : output
        loops: MediaPlayer.Infinite
        Component.onCompleted: if (!VideoService.liveCapture) play()
    }
    CaptureSession {
        id: liveSession
        camera: VideoService.captureAvailable ? PrototypeCamera : null
        videoOutput: VideoService.liveCapture && VideoService.captureAvailable ? output : null
        recorder: VideoService.liveCapture && VideoService.captureAvailable ? liveRecorder : null
    }
    MediaRecorder {
        id: liveRecorder
        outputLocation: DvrService.outputLocation
        quality: MediaRecorder.NormalQuality
        onErrorOccurred: function(error, errorString) {
            if (error !== MediaRecorder.NoError) DvrService.reportRecorderError(errorString)
        }
    }
    Connections {
        target: DvrService
        function onChanged() {
            if (!VideoService.liveCapture) return
            if (DvrService.recording && liveRecorder.recorderState === MediaRecorder.StoppedState)
                liveRecorder.record()
            else if (!DvrService.recording && liveRecorder.recorderState === MediaRecorder.RecordingState)
                liveRecorder.stop()
        }
    }
    Rectangle { anchors.fill: parent; color: "#090d14" }
    VideoOutput {
        id: output; anchors.fill: parent; fillMode: VideoOutput.PreserveAspectCrop
        visible: VideoService.state !== "black" && VideoService.state !== "lost"
        y: VideoService.rolling ? rollAnimation.value : 0
        TapHandler { onTapped: { root.menuOpen = false; InteractionService.toggleControls() } }
    }
    Connections {
        target: output.videoSink
        function onVideoFrameChanged(frame) { root.videoFrameCount += 1 }
    }
    Timer {
        interval: 1000; repeat: true; running: true
        onTriggered: {
            root.videoFps = root.videoFrameCount - root.lastVideoFrameCount
            root.lastVideoFrameCount = root.videoFrameCount
        }
    }
    Item {
        anchors.fill: parent; visible: VideoService.state === "weak" || VideoService.state === "rolling"; opacity: 0.32
        Repeater { model: 28; Rectangle { x: 0; width: root.width; height: index % 4 === 0 ? 2 : 1; y: (index * 71 + noiseOffset.value) % root.height; color: index % 3 === 0 ? "#ffffff" : Theme.blue } }
    }
    NumberAnimation { id: noiseOffset; target: noiseOffset; property: "value"; from: 0; to: 450; duration: 210; loops: Animation.Infinite; running: true; property real value: 0 }
    SequentialAnimation { id: rollAnimation; running: VideoService.rolling; loops: Animation.Infinite; property real value: 0; NumberAnimation { target: rollAnimation; property: "value"; from: -50; to: 50; duration: 800 } }

    Rectangle {
        anchors.fill: parent; color: "#10141b"; visible: VideoService.state === "lost" || VideoService.state === "black"
        TapHandler { onTapped: InteractionService.toggleControls() }
        Column { anchors.centerIn: parent; spacing: 10
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: VideoService.state === "lost" ? "NO VIDEO" : "BLACK FRAME"; color: Theme.text; font.pixelSize: 28; font.bold: true; font.letterSpacing: 3 }
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: "VIDEO " + VideoService.standard + " · " + VideoService.backend.toUpperCase() + " · VRX R" + RadioService.channel; color: Theme.textMuted; font.pixelSize: 14 }
        }
    }

    Row {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 20
        spacing: 8
        StatusChip { text: VideoService.standard; accent: Theme.blue }
        StatusChip { text: "R" + RadioService.channel + "  " + RadioService.rssi + "%"; icon: RadioService.favorite ? "★" : "⌁"; accent: RadioService.rssi < 35 ? Theme.warning : Theme.accent; alert: RadioService.rssi < 20 }
        StatusChip { text: TelemetryService.connected ? "LQ " + TelemetryService.linkQuality : "TLM LOST"; accent: TelemetryService.connected ? Theme.purple : Theme.warning; alert: !TelemetryService.connected }
        StatusChip { visible: InteractionService.flightLocked; text: "FLIGHT LOCK"; icon: "▣"; accent: Theme.warning }
    }
    Row {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 20
        spacing: 8
        StatusChip { visible: DvrService.recording; text: "REC " + Math.floor(DvrService.elapsedSeconds / 60) + ":" + (DvrService.elapsedSeconds % 60).toString().padStart(2, "0"); icon: "●"; accent: Theme.error }
        StatusChip { text: SystemService.deckBatteryPercent + "%"; icon: "▰"; accent: SystemService.deckBatteryPercent <= 15 ? Theme.error : Theme.textMuted; alert: SystemService.deckBatteryPercent <= 15 }
    }

    OverlayCard {
        anchors.right: parent.right
        anchors.bottom: controls.visible ? controls.top : parent.bottom
        anchors.margins: 20
        visible: BatteryService.connected
        title: "EXTERNAL BALANCE PACK"; value: BatteryService.cellCount + "S  " + BatteryService.packVoltage.toFixed(2) + " V"
        detail: "Δ " + (BatteryService.delta * 1000).toFixed(0) + " mV · tap for detail"
        accent: BatteryService.warning.length ? Theme.warning : Theme.blue
        onClicked: root.appRequested("battery")
    }

    Rectangle {
        id: controls
        visible: InteractionService.controlsVisible
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: 112; color: "#ee0b111a"; border.color: Theme.border
        Row { anchors.centerIn: parent; spacing: 10
            TouchButton { icon: DvrService.recording ? "■" : "●"; text: DvrService.recording ? "Stop DVR" : "Record"; accent: Theme.error; checked: DvrService.recording; onClicked: { DvrService.toggleRecording(); InteractionService.showControls() } }
            TouchButton { icon: RadioService.favorite ? "★" : "☆"; text: "R" + RadioService.channel; accent: Theme.accent; onClicked: { RadioService.nextChannel(); InteractionService.showControls() } }
            TouchButton { icon: "☀"; text: SystemService.displayBrightness + "%"; accent: Theme.warning; onClicked: { SystemService.setDisplayBrightness(SystemService.displayBrightness >= 100 ? 55 : SystemService.displayBrightness + 15); InteractionService.showControls() } }
            TouchButton { icon: "▰"; text: "Battery"; accent: Theme.blue; onClicked: root.appRequested("battery") }
            TouchButton { icon: "▶"; text: "Media"; accent: Theme.cyan; enabled: !InteractionService.flightLocked; onClicked: root.appRequested("media") }
            TouchButton { icon: "⌂"; text: "Apps"; enabled: !InteractionService.flightLocked; onClicked: { root.menuOpen = true; InteractionService.showControls() } }
            TouchButton { icon: InteractionService.flightLocked ? "▣" : "□"; text: InteractionService.flightLocked ? "Unlock" : "Flight lock"; accent: Theme.warning; checked: InteractionService.flightLocked; onClicked: { InteractionService.flightLocked = !InteractionService.flightLocked; root.menuOpen = false; InteractionService.showControls() } }
        }
    }

    AppMenu {
        width: 650; height: 370; anchors.centerIn: parent; visible: root.menuOpen && !InteractionService.flightLocked; z: 20
        onAppSelected: function(app) { root.menuOpen = false; root.appRequested(app) }
    }

    Rectangle {
        visible: SystemService.touchDebug
        anchors.left: parent.left; anchors.bottom: controls.visible ? controls.top : parent.bottom
        anchors.margins: 20; width: 270; height: 80; radius: 10
        color: "#e80a1019"; border.color: Theme.cyan; z: 25
        Column {
            anchors.fill: parent; anchors.margins: 10; spacing: 4
            Text { text: "VIDEO " + root.videoFps + " FPS  ·  DROP —  ·  LAT —"; color: Theme.cyan; font.pixelSize: 11; font.bold: true }
            Text { text: "BACKEND " + VideoService.backend.toUpperCase() + "  ·  " + VideoService.standard; color: Theme.text; font.pixelSize: 11 }
            Text { text: "CPU " + (SystemService.cpuPercent < 0 ? "—" : SystemService.cpuPercent.toFixed(0) + "%") + "  ·  MEM " + (SystemService.memoryMegabytes < 0 ? "—" : SystemService.memoryMegabytes.toFixed(0) + " MB") + "  ·  " + SystemService.temperatureC.toFixed(0) + "°C"; color: Theme.textMuted; font.pixelSize: 11 }
        }
    }
}
