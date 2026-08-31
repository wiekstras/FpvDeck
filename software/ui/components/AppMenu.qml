import QtQuick
import QtQuick.Layouts

Rectangle {
    id: root
    signal appSelected(string app)
    color: "#f50b111b"
    radius: 20
    border.color: "#324158"

    Column {
        anchors.fill: parent; anchors.margins: 24; spacing: 18
        Row {
            width: parent.width
            Text { text: "FPVDECK"; color: "#f5f8fc"; font.pixelSize: 18; font.bold: true; font.letterSpacing: 2 }
            Text { text: "  APPLICATIONS"; color: "#71839a"; font.pixelSize: 12; anchors.baseline: parent.children[0].baseline }
        }
        GridLayout {
            width: parent.width; columns: 3; columnSpacing: 12; rowSpacing: 12
            Repeater {
                model: [
                    { id: "fpv", label: "FPV", glyph: "◉", tint: "#67e8a5" },
                    { id: "battery", label: "Battery", glyph: "▰", tint: "#6fb4ff" },
                    { id: "flights", label: "Flights", glyph: "↗", tint: "#c38cff" },
                    { id: "dvr", label: "DVR", glyph: "●", tint: "#ff6f78" },
                    { id: "aircraft", label: "Aircraft", glyph: "✦", tint: "#ffbf5a" },
                    { id: "settings", label: "Settings", glyph: "⚙", tint: "#91a2b8" }
                ]
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 92
                    radius: 14; color: mouse.containsMouse ? "#253248" : "#172131"
                    border.color: mouse.containsMouse ? modelData.tint : "#2a374b"
                    Column {
                        anchors.centerIn: parent; spacing: 7
                        Text { anchors.horizontalCenter: parent.horizontalCenter; text: modelData.glyph; color: modelData.tint; font.pixelSize: 23 }
                        Text { anchors.horizontalCenter: parent.horizontalCenter; text: modelData.label; color: "#ecf3fb"; font.pixelSize: 13; font.weight: Font.DemiBold }
                    }
                    MouseArea { id: mouse; anchors.fill: parent; hoverEnabled: true; onClicked: root.appSelected(modelData.id) }
                }
            }
        }
        Text { text: "M / ESC  CLOSE"; color: "#53647c"; font.pixelSize: 10; font.letterSpacing: 1 }
    }
}

