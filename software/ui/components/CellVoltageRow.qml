import QtQuick
import FPVDeck

Rectangle {
    property int cellNumber: 1
    property real voltage: 0
    property bool weakest: false
    implicitHeight: 62
    radius: Theme.radiusSmall
    color: weakest ? "#2a2119" : Theme.surfaceRaised
    border.color: weakest ? Theme.warning : "transparent"
    Row { anchors.fill: parent; anchors.margins: 14; spacing: 14
        Text { width: 42; text: "C" + cellNumber; color: weakest ? Theme.warning : Theme.textMuted; font.pixelSize: 14; font.bold: true; anchors.verticalCenter: parent.verticalCenter }
        Rectangle { width: parent.width - 200; height: 10; radius: 5; color: "#2a3a50"; anchors.verticalCenter: parent.verticalCenter
            Rectangle { height: parent.height; radius: 5; width: parent.width * Math.max(0, Math.min(1, (voltage - 3.0) / 1.25)); color: voltage < 3.4 || voltage > 4.25 ? Theme.error : weakest ? Theme.warning : Theme.accent }
        }
        Text { text: Number(voltage).toFixed(3) + " V"; color: Theme.text; font.pixelSize: 17; font.bold: true; anchors.verticalCenter: parent.verticalCenter }
    }
}
