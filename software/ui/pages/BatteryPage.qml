import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    RowLayout {
        anchors.fill: parent; anchors.margins: 34; spacing: 28
        Rectangle {
            Layout.fillHeight: true; Layout.preferredWidth: 350; radius: 20
            color: "#131d2b"; border.color: "#2b3a50"
            Column {
                anchors.fill: parent; anchors.margins: 28; spacing: 14
                Text { text: "BALANCE CHECK"; color: "#71839a"; font.pixelSize: 12; font.letterSpacing: 1.8 }
                Text { text: BatteryService.connected ? BatteryService.packVoltage.toFixed(2) + " V" : "—"; color: "#f4f8ff"; font.pixelSize: 52; font.weight: Font.DemiBold }
                Text { text: BatteryService.cellCount + "S LiPo  •  Δ " + (BatteryService.delta * 1000).toFixed(0) + " mV"; color: "#91a2b8"; font.pixelSize: 15 }
                Rectangle { width: parent.width; height: 1; color: "#2a374b" }
                Text { text: BatteryService.warning.length ? BatteryService.warning : "PACK HEALTHY"; color: BatteryService.warning.length ? "#ffbf5a" : "#67e8a5"; font.pixelSize: 14; font.bold: true; font.letterSpacing: 1 }
                Item { width: 1; height: 18 }
                Text { text: "SIMULATED INPUT"; color: "#53647c"; font.pixelSize: 10; font.letterSpacing: 1.5 }
                Row {
                    spacing: 8
                    Repeater {
                        model: 6
                        Button { text: (index + 1) + "S"; onClicked: BatteryService.configureCells(index + 1) }
                    }
                }
            }
        }
        Rectangle {
            Layout.fillHeight: true; Layout.fillWidth: true; radius: 20
            color: "#101927"; border.color: "#28364a"
            Column {
                anchors.fill: parent; anchors.margins: 28; spacing: 14
                Text { text: "CELL VOLTAGES"; color: "#f4f8ff"; font.pixelSize: 18; font.bold: true }
                Repeater {
                    model: BatteryService.cellVoltages
                    Rectangle {
                        width: parent.width; height: 54; radius: 11; color: "#172334"
                        Row {
                            anchors { fill: parent; margins: 14 }
                            spacing: 14
                            Text { width: 35; text: "C" + (index + 1); color: "#71839a"; font.pixelSize: 14; anchors.verticalCenter: parent.verticalCenter }
                            Rectangle {
                                width: parent.width - 180; height: 10; radius: 5; color: "#2a374b"; anchors.verticalCenter: parent.verticalCenter
                                Rectangle { height: parent.height; radius: 5; width: parent.width * Math.max(0, Math.min(1, (modelData - 3.0) / 1.25)); color: modelData < 3.4 || modelData > 4.25 ? "#ff6f78" : "#67e8a5" }
                            }
                            Text { text: Number(modelData).toFixed(3) + " V"; color: "#f4f8ff"; font.pixelSize: 17; font.bold: true; anchors.verticalCenter: parent.verticalCenter }
                        }
                    }
                }
                Text { text: "DEVELOPMENT FAULTS"; color: "#53647c"; font.pixelSize: 10; font.letterSpacing: 1.5 }
                Flow {
                    width: parent.width; spacing: 8
                    Repeater {
                        model: ["nominal", "imbalanced", "undervoltage", "overvoltage", "disconnected", "invalid taps"]
                        Button { required property string modelData; text: modelData; onClicked: BatteryService.setScenario(modelData) }
                    }
                }
            }
        }
    }
}
