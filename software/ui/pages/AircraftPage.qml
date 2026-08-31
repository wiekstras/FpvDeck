import QtQuick

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.centerIn: parent; spacing: 12
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: "✦"; color: "#ffbf5a"; font.pixelSize: 52 }
        Text { text: "AIRCRAFT"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
        Text { text: "Aircraft profiles will bind defaults, channels, batteries and flight history."; color: "#71839a"; font.pixelSize: 14 }
    }
}
