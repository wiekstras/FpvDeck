import QtQuick
import QtQuick.Layouts

Item {
    id: root
    signal appRequested(string app)
    Rectangle { anchors.fill: parent; color: "#0a1018" }
    Column {
        anchors.fill: parent; anchors.margins: 38; spacing: 22
        Row {
            width: parent.width
            Column {
                width: parent.width - 230
                Text { text: "FPVDECK"; color: "#f4f8ff"; font.pixelSize: 30; font.bold: true; font.letterSpacing: 2 }
                Text { text: "Radio · video · flight computer"; color: "#71839a"; font.pixelSize: 13 }
            }
            Rectangle {
                width: 190; height: 38; radius: 12; color: "#14241f"; border.color: "#285945"
                Text { anchors.centerIn: parent; text: "●  SIMULATOR ONLINE"; color: "#67e8a5"; font.pixelSize: 11; font.bold: true; font.letterSpacing: 0.8 }
            }
        }
        GridLayout {
            width: parent.width; columns: 4; columnSpacing: 14; rowSpacing: 14
            Repeater {
                model: [
                    { id: "fpv", title: "FPV", detail: "Live view + overlays", glyph: "◉", tint: "#67e8a5" },
                    { id: "battery", title: "Balance", detail: "6S simulated input", glyph: "▰", tint: "#6fb4ff" },
                    { id: "media", title: "Media", detail: "Removable DVR card", glyph: "▶", tint: "#4ed7d1" },
                    { id: "flights", title: "Flights", detail: "Local history", glyph: "↗", tint: "#c38cff" },
                    { id: "batteries", title: "Batteries", detail: "Pack inventory", glyph: "▥", tint: "#7cc6ff" },
                    { id: "aircraft", title: "Aircraft", detail: "Model profiles", glyph: "✦", tint: "#ffbf5a" },
                    { id: "diagnostics", title: "Diagnostics", detail: "System self-test", glyph: "◇", tint: "#8bd5ca" },
                    { id: "settings", title: "Settings", detail: "Device preferences", glyph: "⚙", tint: "#91a2b8" }
                ]
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 170; radius: 18
                    color: hit.containsMouse ? "#192638" : "#111a27"; border.color: hit.containsMouse ? modelData.tint : "#253448"
                    Column {
                        anchors.fill: parent; anchors.margins: 20; spacing: 10
                        Text { text: modelData.glyph; color: modelData.tint; font.pixelSize: 30 }
                        Text { text: modelData.title; color: "#f4f8ff"; font.pixelSize: 19; font.bold: true }
                        Text { text: modelData.detail; color: "#71839a"; font.pixelSize: 12 }
                    }
                    MouseArea { id: hit; anchors.fill: parent; hoverEnabled: true; onClicked: root.appRequested(modelData.id) }
                }
            }
        }
    }
}
