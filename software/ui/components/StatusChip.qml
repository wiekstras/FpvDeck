import QtQuick
import FPVDeck

Rectangle {
    property string text: ""
    property string icon: ""
    property color accent: Theme.textMuted
    property bool alert: false
    implicitWidth: label.implicitWidth + (icon.length ? glyph.implicitWidth + 28 : 22)
    implicitHeight: 38
    radius: 12
    color: Qt.rgba((alert ? Theme.error : accent).r, (alert ? Theme.error : accent).g, (alert ? Theme.error : accent).b, 0.13)
    border.color: Qt.rgba((alert ? Theme.error : accent).r, (alert ? Theme.error : accent).g, (alert ? Theme.error : accent).b, 0.42)
    Row { anchors.centerIn: parent; spacing: 7
        Text { id: glyph; visible: icon.length; text: icon; color: alert ? Theme.error : accent; font.pixelSize: 13 }
        Text { id: label; text: parent.parent.text; color: Theme.text; font.pixelSize: 11; font.bold: true; font.letterSpacing: 0.7 }
    }
}
