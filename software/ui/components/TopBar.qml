import QtQuick
import FPVDeck

Item {
    id: root
    property string title: "FPVDECK"
    property string subtitle: ""
    property bool showBack: false
    property string currentTime: Qt.formatTime(new Date(), "HH:mm")
    signal backRequested()
    implicitHeight: 62
    Row {
        anchors.fill: parent; spacing: 14
        TouchButton { visible: root.showBack; icon: "‹"; accessibleName: "Back"; onClicked: root.backRequested() }
        Column { width: parent.width - status.width - (root.showBack ? 76 : 0); anchors.verticalCenter: parent.verticalCenter; spacing: 2
            Text { text: root.title; color: Theme.text; font.pixelSize: 25; font.bold: true; font.letterSpacing: 1.2 }
            Text { visible: root.subtitle.length; text: root.subtitle; color: Theme.textMuted; font.pixelSize: 12 }
        }
        Row { id: status; anchors.verticalCenter: parent.verticalCenter; spacing: 8
            StatusChip { text: SystemService.deckBatteryPercent + "%"; icon: SystemService.deckCharging ? "⚡" : "▰"; accent: SystemService.deckBatteryPercent <= 15 ? Theme.error : Theme.accent; alert: SystemService.deckBatteryPercent <= 15 }
            StatusChip { text: StorageService.status; icon: "SD"; accent: StorageService.sdPresent && !StorageService.sdCorrupt ? Theme.cyan : Theme.warning; alert: !StorageService.sdPresent || StorageService.sdCorrupt }
            Text { text: root.currentTime; color: Theme.text; font.pixelSize: 16; font.weight: Font.DemiBold; anchors.verticalCenter: parent.verticalCenter }
        }
    }
    Timer { interval: 1000; running: true; repeat: true; onTriggered: root.currentTime = Qt.formatTime(new Date(), "HH:mm") }
}
