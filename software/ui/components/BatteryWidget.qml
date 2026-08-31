import QtQuick

Rectangle {
    id: root
    property bool expanded: false
    width: expanded ? 330 : 186
    height: expanded ? 172 : 62
    radius: 14
    color: "#d90b111a"
    border.color: BatteryService.warning.length ? "#ffbf5a" : "#35465f"

    Behavior on width { NumberAnimation { duration: 120 } }
    Behavior on height { NumberAnimation { duration: 120 } }

    Row {
        x: 16; y: 12; spacing: 12
        Rectangle {
            width: 34; height: 20; radius: 4
            anchors.verticalCenter: parent.verticalCenter
            color: "transparent"; border.width: 2
            border.color: BatteryService.warning.length ? "#ffbf5a" : "#67e8a5"
            Rectangle {
                width: 3; height: 9; radius: 1
                anchors.left: parent.right; anchors.leftMargin: 2
                anchors.verticalCenter: parent.verticalCenter
                color: parent.border.color
            }
            Rectangle {
                anchors { left: parent.left; right: parent.right; bottom: parent.bottom; margins: 4 }
                height: Math.max(3, parent.height * 0.55)
                radius: 2; color: parent.border.color
            }
        }
        Column {
            Text {
                text: BatteryService.connected
                      ? BatteryService.cellCount + "S  " + BatteryService.packVoltage.toFixed(2) + " V"
                      : "NO PACK"
                color: "#f4f8ff"; font.pixelSize: 18; font.weight: Font.DemiBold
            }
            Text {
                text: BatteryService.warning.length ? BatteryService.warning
                      : "Δ " + (BatteryService.delta * 1000).toFixed(0) + " mV"
                color: BatteryService.warning.length ? "#ffbf5a" : "#91a2b8"
                font.pixelSize: 11; font.weight: Font.Medium
            }
        }
    }

    Column {
        visible: root.expanded
        x: 16; y: 78; spacing: 6
        Repeater {
            model: BatteryService.cellVoltages
            Row {
                spacing: 9
                Text { width: 25; text: "C" + (index + 1); color: "#71839a"; font.pixelSize: 11 }
                Rectangle {
                    width: 174; height: 8; radius: 4; color: "#253044"
                    Rectangle {
                        height: parent.height; radius: 4
                        width: parent.width * Math.max(0, Math.min(1, (modelData - 3.0) / 1.25))
                        color: modelData < 3.4 ? "#ff6f78" : "#67e8a5"
                    }
                }
                Text { text: Number(modelData).toFixed(3) + " V"; color: "#dce6f4"; font.pixelSize: 11 }
            }
        }
    }

    MouseArea { anchors.fill: parent; onClicked: root.expanded = !root.expanded }
}

