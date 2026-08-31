import QtQuick
import QtQuick.Layouts

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.fill: parent; anchors.margins: 38; spacing: 18
        Text { text: "BATTERIES"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
        Text { text: "Pack inventory · showcase records"; color: "#71839a"; font.pixelSize: 13 }
        GridLayout {
            width: parent.width; columns: 3; columnSpacing: 16
            Repeater {
                model: [
                    { name: "Cinewhoop A", spec: "4S · 850 mAh", cycles: "21 cycles", health: "96%" },
                    { name: "Freestyle 01", spec: "6S · 1300 mAh", cycles: "34 cycles", health: "91%" },
                    { name: "Freestyle 02", spec: "6S · 1300 mAh", cycles: "29 cycles", health: "94%" }
                ]
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 185; radius: 18; color: "#121d2b"; border.color: "#29394e"
                    Column { anchors.fill: parent; anchors.margins: 20; spacing: 10
                        Text { text: "▰"; color: "#6fb4ff"; font.pixelSize: 27 }
                        Text { text: modelData.name; color: "#f4f8ff"; font.pixelSize: 18; font.bold: true }
                        Text { text: modelData.spec; color: "#91a2b8"; font.pixelSize: 13 }
                        Text { text: modelData.cycles + "  ·  " + modelData.health; color: "#67e8a5"; font.pixelSize: 12 }
                        Text { text: "SIMULATED DATA"; color: "#53647c"; font.pixelSize: 9; font.letterSpacing: 1.2 }
                    }
                }
            }
        }
    }
}
