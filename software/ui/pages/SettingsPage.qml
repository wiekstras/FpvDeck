import QtQuick

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.fill: parent; anchors.margins: 40; spacing: 16
        Text { text: "SETTINGS"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
        Repeater {
            model: [
                ["VIDEO PRIORITY", "Realtime / animations reduced under load"],
                ["NETWORK", "Optional • no dependency for FPV operation"],
                ["DATABASE", DatabaseService.path],
                ["INPUT LOCKOUT", "Flight controls never routed to application menus"],
                ["BUILD", "0.1.0-dev • simulated hardware"]
            ]
            Rectangle {
                required property var modelData
                width: parent.width; height: 64; radius: 12; color: "#131d2b"
                Row {
                    anchors { fill: parent; margins: 16 }
                    Text { width: 230; text: modelData[0]; color: "#71839a"; font.pixelSize: 12; font.letterSpacing: 1; anchors.verticalCenter: parent.verticalCenter }
                    Text { text: modelData[1]; color: "#e8eff8"; font.pixelSize: 14; anchors.verticalCenter: parent.verticalCenter }
                }
            }
        }
    }
}
