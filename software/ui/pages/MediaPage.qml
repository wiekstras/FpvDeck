import QtQuick
import QtQuick.Layouts

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    RowLayout {
        anchors.fill: parent; anchors.margins: 38; spacing: 22
        Column {
            Layout.preferredWidth: 310; Layout.fillHeight: true; spacing: 14
            Text { text: "MEDIA"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
            Text { text: "DVR browser"; color: "#71839a"; font.pixelSize: 13 }
            Rectangle {
                width: parent.width; height: 118; radius: 16; color: "#11221f"; border.color: "#24544b"
                Column { anchors.fill: parent; anchors.margins: 18; spacing: 9
                    Text { text: "REMOVABLE CARD"; color: "#4ed7d1"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1.2 }
                    Text { text: "47.2 GB free"; color: "#f4f8ff"; font.pixelSize: 24; font.bold: true }
                    Text { text: "SIMULATED MOUNT"; color: "#71839a"; font.pixelSize: 10; font.letterSpacing: 1.2 }
                }
            }
            Text { text: "Recording and card mounting are not yet connected to physical storage."; width: parent.width; wrapMode: Text.WordWrap; color: "#71839a"; font.pixelSize: 12; lineHeight: 1.35 }
        }
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true; radius: 18; color: "#101927"; border.color: "#28364a"
            Column { anchors.fill: parent; anchors.margins: 22; spacing: 12
                Text { text: "RECENT RECORDINGS"; color: "#f4f8ff"; font.pixelSize: 17; font.bold: true }
                Repeater {
                    model: [
                        { name: "RIDGE RUN", date: "30 AUG · 06:21", size: "2.8 GB", channel: "R1" },
                        { name: "PARK LINE", date: "28 AUG · 05:14", size: "2.3 GB", channel: "R3" },
                        { name: "SUNSET TEST", date: "24 AUG · 04:09", size: "1.9 GB", channel: "F4" }
                    ]
                    Rectangle {
                        required property var modelData
                        width: parent.width; height: 92; radius: 13; color: "#162233"
                        Row { anchors.fill: parent; anchors.margins: 14; spacing: 18
                            Rectangle { width: 112; height: 64; radius: 9; color: "#26364b"; Text { anchors.centerIn: parent; text: "▶"; color: "#4ed7d1"; font.pixelSize: 22 } }
                            Column { width: parent.width - 265; anchors.verticalCenter: parent.verticalCenter; spacing: 7
                                Text { text: modelData.name; color: "#f4f8ff"; font.pixelSize: 15; font.bold: true }
                                Text { text: modelData.date + "  ·  " + modelData.channel; color: "#71839a"; font.pixelSize: 11 }
                            }
                            Text { text: modelData.size; color: "#91a2b8"; font.pixelSize: 12; anchors.verticalCenter: parent.verticalCenter }
                        }
                    }
                }
            }
        }
    }
}
