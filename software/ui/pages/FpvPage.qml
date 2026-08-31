import QtQuick
import QtMultimedia
import "../components"

Item {
    id: root
    property bool batteryExpanded: false
    property bool menuOpen: false
    signal appRequested(string app)

    MediaPlayer {
        id: player
        source: VideoService.source
        videoOutput: output
        loops: MediaPlayer.Infinite
        Component.onCompleted: play()
    }

    Rectangle { anchors.fill: parent; color: "#090d14" }
    VideoOutput {
        id: output
        anchors.fill: parent
        fillMode: VideoOutput.PreserveAspectCrop
        visible: VideoService.state !== "black" && VideoService.state !== "lost"
        y: VideoService.rolling ? rollAnimation.value : 0
    }

    // Weak-signal interference is deliberately an overlay in the simulator. The
    // real backend will report state while the decoder-provided pixels remain direct.
    Item {
        anchors.fill: parent
        visible: VideoService.state === "weak" || VideoService.state === "rolling"
        opacity: 0.34
        Repeater {
            model: 28
            Rectangle {
                x: 0; width: root.width; height: index % 4 === 0 ? 2 : 1
                y: (index * 71 + noiseOffset.value) % root.height
                color: index % 3 === 0 ? "#ffffff" : "#7ca8ff"
            }
        }
    }
    NumberAnimation { id: noiseOffset; target: noiseOffset; property: "value"; from: 0; to: 450; duration: 210; loops: Animation.Infinite; running: true; property real value: 0 }
    SequentialAnimation { id: rollAnimation; running: VideoService.rolling; loops: Animation.Infinite; property real value: 0; NumberAnimation { target: rollAnimation; property: "value"; from: -50; to: 50; duration: 800 } }

    Rectangle {
        anchors.fill: parent; color: "#10141b"
        visible: VideoService.state === "lost" || VideoService.state === "black"
        Column {
            anchors.centerIn: parent; spacing: 10
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: VideoService.state === "lost" ? "NO VIDEO" : "BLACK FRAME"; color: "#f4f8ff"; font.pixelSize: 26; font.bold: true; font.letterSpacing: 3 }
            Text { anchors.horizontalCenter: parent.horizontalCenter; text: "VRX " + VideoService.standard + "  •  CH R" + VideoService.channel; color: "#71839a"; font.pixelSize: 13 }
        }
    }

    // Layer 2: FpvDeck widgets. The Betaflight OSD, if present, is already in video.
    Row {
        anchors { left: parent.left; top: parent.top; margins: 24 }
        spacing: 9
        StatusPill { label: VideoService.standard; accent: "#6fb4ff" }
        StatusPill { label: "R" + VideoService.channel + "  " + VideoService.rssi + "%"; accent: VideoService.rssi < 35 ? "#ffbf5a" : "#67e8a5" }
        StatusPill { label: TelemetryService.connected ? "LQ " + TelemetryService.linkQuality : "TLM LOST"; accent: TelemetryService.connected ? "#c38cff" : "#ffbf5a" }
    }
    Row {
        anchors { right: parent.right; top: parent.top; margins: 24 }
        spacing: 9
        StatusPill { visible: DvrService.recording; label: "●  REC  " + Math.floor(DvrService.elapsedSeconds / 60) + ":" + (DvrService.elapsedSeconds % 60).toString().padStart(2, "0"); accent: "#ff6f78" }
        StatusPill { visible: DvrService.error.length; label: DvrService.error; accent: "#ff6f78" }
        StatusPill { label: "SYS  52°C"; accent: "#91a2b8" }
    }
    BatteryWidget {
        id: batteryWidget
        anchors { right: parent.right; bottom: parent.bottom; margins: 24 }
        expanded: root.batteryExpanded
        onExpandedChanged: root.batteryExpanded = expanded
    }
    Rectangle {
        anchors { left: parent.left; bottom: parent.bottom; margins: 24 }
        width: hint.implicitWidth + 24; height: 32; radius: 9; color: "#b50b111a"
        Text { id: hint; anchors.centerIn: parent; text: "M  MENU     R  DVR     B  BATTERY     L  SIGNAL"; color: "#9bacbf"; font.pixelSize: 11; font.letterSpacing: 0.6 }
    }

    AppMenu {
        id: menu
        width: 650; height: 410
        anchors.centerIn: parent
        visible: root.menuOpen
        opacity: visible ? 1 : 0
        scale: visible ? 1 : 0.96
        Behavior on opacity { NumberAnimation { duration: 100 } }
        Behavior on scale { NumberAnimation { duration: 100 } }
        onAppSelected: function(app) { root.menuOpen = false; root.appRequested(app) }
    }
}
