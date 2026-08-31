import QtQuick
import QtQuick.Controls

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.fill: parent; anchors.margins: 38; spacing: 15
        Text { text: "SIMULATOR CONTROL"; color: "#f4f8ff"; font.pixelSize: 27; font.bold: true }
        Text { text: "Development only — inject failures without physical hardware"; color: "#71839a"; font.pixelSize: 13 }
        Text { text: "VIDEO"; color: "#53647c"; font.pixelSize: 11; font.letterSpacing: 1.4 }
        Flow {
            width: parent.width; spacing: 8
            Repeater { model: ["locked", "weak", "rolling", "lost", "black"]; Button { required property string modelData; text: modelData; onClicked: VideoService.setScenario(modelData) } }
        }
        Row {
            spacing: 8
            Button { text: "PAL"; onClicked: VideoService.setStandard("PAL") }
            Button { text: "NTSC"; onClicked: VideoService.setStandard("NTSC") }
            Button { text: "NEXT CHANNEL"; onClicked: VideoService.nextChannel() }
        }
        Rectangle { width: parent.width; height: 1; color: "#2a374b" }
        Text { text: "TELEMETRY"; color: "#53647c"; font.pixelSize: 11; font.letterSpacing: 1.4 }
        Flow {
            width: parent.width; spacing: 8
            Repeater { model: ["connected", "lost", "corrupt", "version mismatch"]; Button { required property string modelData; text: modelData; onClicked: TelemetryService.setScenario(modelData) } }
        }
        Text { text: "BALANCE BATTERY"; color: "#53647c"; font.pixelSize: 11; font.letterSpacing: 1.4 }
        Flow {
            width: parent.width; spacing: 8
            Repeater { model: ["nominal", "imbalanced", "undervoltage", "overvoltage", "disconnected", "invalid taps"]; Button { required property string modelData; text: modelData; onClicked: BatteryService.setScenario(modelData) } }
        }
        Text { text: "F10 / ESC  CLOSE"; color: "#53647c"; font.pixelSize: 11; font.letterSpacing: 1.2 }
    }
}
