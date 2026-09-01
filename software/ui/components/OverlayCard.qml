import QtQuick
import FPVDeck

Rectangle {
    id: root
    property string title: ""
    property string value: ""
    property string detail: ""
    property color accent: Theme.accent
    signal clicked()
    implicitWidth: 230
    implicitHeight: 88
    radius: Theme.radius
    color: "#df0b111a"
    border.color: Qt.rgba(accent.r, accent.g, accent.b, 0.65)
    Column { anchors.fill: parent; anchors.margins: 14; spacing: 3
        Text { text: root.title; color: root.accent; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1.2 }
        Text { text: root.value; color: Theme.text; font.pixelSize: 20; font.bold: true }
        Text { text: root.detail; color: Theme.textMuted; font.pixelSize: 11 }
    }
    TapHandler { onTapped: root.clicked() }
}
