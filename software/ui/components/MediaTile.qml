import QtQuick
import FPVDeck

Rectangle {
    id: root
    property var media
    signal clicked()
    activeFocusOnTab: enabled && visible
    implicitHeight: 96
    radius: Theme.radius
    color: tap.pressed ? "#203049" : Theme.surfaceRaised
    border.color: activeFocus ? Theme.cyan : "transparent"
    border.width: activeFocus ? 2 : 0
    Row { anchors.fill: parent; anchors.margins: 14; spacing: 16
        Rectangle { width: 118; height: 68; radius: 10; color: "#26384f"; anchors.verticalCenter: parent.verticalCenter
            Text { anchors.centerIn: parent; text: "▶"; color: Theme.cyan; font.pixelSize: 24 }
            Text {
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 6
                text: root.media.aspect; color: Theme.text; font.pixelSize: 9
            }
        }
        Column { width: parent.width - 250; anchors.verticalCenter: parent.verticalCenter; spacing: 7
            Text { text: root.media.title; color: Theme.text; font.pixelSize: 16; font.bold: true }
            Text { text: root.media.date + "  ·  " + root.media.channel; color: Theme.textMuted; font.pixelSize: 11 }
        }
        Column { anchors.verticalCenter: parent.verticalCenter; spacing: 6
            Text { text: Math.floor(root.media.duration / 60) + ":" + (root.media.duration % 60).toString().padStart(2, "0"); color: Theme.text; font.pixelSize: 13 }
            Text { text: root.media.size; color: Theme.textMuted; font.pixelSize: 11 }
        }
    }
    TapHandler { id: tap; onTapped: root.clicked() }
}
