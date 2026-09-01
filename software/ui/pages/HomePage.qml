import QtQuick
import QtQuick.Layouts
import FPVDeck
import "../components"

Item {
    id: root
    signal appRequested(string app)
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column {
        anchors.fill: parent; anchors.margins: Theme.space5; spacing: 20
        TopBar { width: parent.width; title: "FPVDECK"; subtitle: "Ready to fly" }
        Row {
            width: parent.width; spacing: 10
            StatusChip { text: "VRX R" + RadioService.channel + " · " + RadioService.rssi + "%"; icon: "⌁"; accent: Theme.accent }
            StatusChip { text: DvrService.recording ? "DVR RECORDING" : "DVR READY"; icon: "●"; accent: DvrService.recording ? Theme.error : Theme.textMuted }
            StatusChip { visible: BatteryService.connected; text: BatteryService.cellCount + "S EXT · " + BatteryService.packVoltage.toFixed(2) + "V"; icon: "▰"; accent: BatteryService.warning.length ? Theme.warning : Theme.blue; alert: BatteryService.warning.length }
            StatusChip { visible: SystemService.warning.length; text: SystemService.warning; icon: "!"; accent: Theme.error; alert: true }
        }
        GridLayout {
            width: parent.width; columns: 4; columnSpacing: 14; rowSpacing: 14
            AppTile { Layout.columnSpan: 2; Layout.fillWidth: true; title: "FLY"; detail: "Live analog video + FpvDeck overlay"; icon: "◉"; accent: Theme.accent; primary: true; onClicked: root.appRequested("fpv") }
            AppTile { Layout.fillWidth: true; title: "Battery"; detail: BatteryService.connected ? BatteryService.cellCount + "S connected" : "Balance checker"; icon: "▰"; accent: Theme.blue; onClicked: root.appRequested("battery") }
            AppTile { Layout.fillWidth: true; title: "Media"; detail: StorageService.status; icon: "▶"; accent: Theme.cyan; onClicked: root.appRequested("media") }
            AppTile { Layout.fillWidth: true; title: "Flights"; detail: "Local history"; icon: "↗"; accent: Theme.purple; onClicked: root.appRequested("flights") }
            AppTile { Layout.fillWidth: true; title: "Aircraft"; detail: "Profiles"; icon: "✦"; accent: Theme.warning; onClicked: root.appRequested("aircraft") }
            AppTile { Layout.fillWidth: true; title: "Receiver"; detail: "Channel + scan"; icon: "⌁"; accent: Theme.accent; onClicked: root.appRequested("receiver") }
            AppTile { Layout.fillWidth: true; title: "Diagnostics"; detail: SystemService.mcuOnline ? "Systems nominal" : "Attention required"; icon: "◇"; accent: SystemService.warning.length ? Theme.error : Theme.cyan; onClicked: root.appRequested("diagnostics") }
        }
    }
}
