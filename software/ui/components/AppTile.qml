import QtQuick
import FPVDeck

Rectangle {
    id: root
    property string title: ""
    property string detail: ""
    property string icon: ""
    property color accent: Theme.blue
    property bool primary: false
    signal clicked()
    activeFocusOnTab: enabled && visible
    implicitHeight: 156
    radius: Theme.radius
    color: tap.pressed ? "#203049" : primary ? "#142a25" : Theme.surface
    border.color: activeFocus ? Theme.cyan : primary ? Qt.rgba(accent.r, accent.g, accent.b, 0.7) : Theme.border
    border.width: activeFocus || primary ? 2 : 1
    Column { anchors.fill: parent; anchors.margins: 20; spacing: 9
        Text { text: root.icon; color: root.accent; font.pixelSize: root.primary ? 34 : 27 }
        Text { text: root.title; color: Theme.text; font.pixelSize: root.primary ? 21 : 18; font.bold: true }
        Text { text: root.detail; color: Theme.textMuted; font.pixelSize: 12 }
    }
    TapHandler { id: tap; onTapped: root.clicked() }
    Behavior on color { ColorAnimation { duration: Theme.animationFast } }
}
