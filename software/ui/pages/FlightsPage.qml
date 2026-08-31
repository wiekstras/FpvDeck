import QtQuick
import QtQuick.Controls

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.fill: parent; anchors.margins: 38; spacing: 18
        Row {
            width: parent.width
            Column {
                width: parent.width - 180
                Text { text: "FLIGHT HISTORY"; color: "#f4f8ff"; font.pixelSize: 27; font.bold: true }
                Text { text: "Local SQLite flight log"; color: "#71839a"; font.pixelSize: 13 }
            }
            Button { text: "ADD SIM FLIGHT"; onClicked: DatabaseService.addDemoFlight() }
        }
        Rectangle { width: parent.width; height: 1; color: "#2a374b" }
        ListView {
            width: parent.width; height: parent.height - 100; spacing: 9; clip: true
            model: DatabaseService.recentFlights
            delegate: Rectangle {
                required property var modelData
                width: ListView.view.width; height: 66; radius: 12; color: "#131d2b"; border.color: "#27364a"
                Row {
                    anchors { fill: parent; margins: 16 }
                    spacing: 24
                    Text { width: 260; text: modelData.startedAt; color: "#e8eff8"; font.pixelSize: 14; anchors.verticalCenter: parent.verticalCenter }
                    Text { width: 120; text: Math.floor(modelData.durationSeconds / 60) + "m " + (modelData.durationSeconds % 60) + "s"; color: "#91a2b8"; anchors.verticalCenter: parent.verticalCenter }
                    Text { width: 80; text: modelData.channel; color: "#6fb4ff"; anchors.verticalCenter: parent.verticalCenter }
                    Text { text: "MIN RSSI  " + modelData.minRssi; color: "#71839a"; anchors.verticalCenter: parent.verticalCenter }
                }
            }
            Text { anchors.centerIn: parent; visible: parent.count === 0; text: "No flights recorded yet"; color: "#53647c"; font.pixelSize: 16 }
        }
    }
}
