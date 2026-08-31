import QtQuick
import QtQuick.Layouts

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.fill: parent; anchors.margins: 38; spacing: 18
        Row { width: parent.width
            Column { width: parent.width - 210
                Text { text: "DIAGNOSTICS"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
                Text { text: "Development-mode hardware self-test"; color: "#71839a"; font.pixelSize: 13 }
            }
            Rectangle { width: 190; height: 36; radius: 11; color: "#282314"; border.color: "#66582a"; Text { anchors.centerIn: parent; text: "SIMULATED BACKENDS"; color: "#ffbf5a"; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1 } }
        }
        GridLayout {
            width: parent.width; columns: 3; columnSpacing: 14; rowSpacing: 14
            Repeater {
                model: [
                    { name: "VIDEO", value: "PAL 50 · ONLINE", state: "PASS", color: "#67e8a5" },
                    { name: "CONTROLLER MCU", value: "Simulator protocol v1", state: "SIM", color: "#6fb4ff" },
                    { name: "BALANCE ADC", value: "6 channels · 12.24 V", state: "SIM", color: "#6fb4ff" },
                    { name: "DVR STORAGE", value: "47.2 GB available", state: "PASS", color: "#67e8a5" },
                    { name: "TELEMETRY", value: "LQ 97 · heartbeat", state: "SIM", color: "#6fb4ff" },
                    { name: "SYSTEM", value: "52°C · watchdog armed", state: "PASS", color: "#67e8a5" }
                ]
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 150; radius: 17; color: "#121d2b"; border.color: "#29394e"
                    Column { anchors.fill: parent; anchors.margins: 18; spacing: 12
                        Row { width: parent.width
                            Text { width: parent.width - 64; text: modelData.name; color: "#71839a"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1.1 }
                            Text { text: modelData.state; color: modelData.color; font.pixelSize: 11; font.bold: true }
                        }
                        Text { text: modelData.value; color: "#f4f8ff"; font.pixelSize: 18; font.weight: Font.DemiBold }
                        Rectangle { width: parent.width; height: 5; radius: 3; color: "#253448"; Rectangle { width: parent.width * 0.88; height: parent.height; radius: 3; color: modelData.color } }
                    }
                }
            }
        }
    }
}
