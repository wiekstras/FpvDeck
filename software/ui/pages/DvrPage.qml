import QtQuick
import QtQuick.Controls

Item {
    Rectangle { anchors.fill: parent; color: "#0c121c" }
    Column {
        anchors.centerIn: parent; spacing: 18
        Rectangle {
            anchors.horizontalCenter: parent.horizontalCenter; width: 90; height: 90; radius: 45
            color: DvrService.recording ? "#ff6f78" : "#223047"
            Text { anchors.centerIn: parent; text: "●"; color: "white"; font.pixelSize: 38 }
        }
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: DvrService.recording ? "RECORDING" : "DVR READY"; color: "#f4f8ff"; font.pixelSize: 28; font.bold: true }
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: DvrService.freeGigabytes.toFixed(1) + " GB AVAILABLE  •  PRE-OVERLAY STREAM"; color: "#71839a"; font.pixelSize: 13 }
        Button { anchors.horizontalCenter: parent.horizontalCenter; text: DvrService.recording ? "STOP RECORDING" : "START RECORDING"; enabled: !DvrService.error.length; onClicked: DvrService.toggleRecording() }
        Row {
            anchors.horizontalCenter: parent.horizontalCenter; spacing: 10
            Button { text: "SIMULATE STORAGE FULL"; onClicked: DvrService.simulateStorageFull() }
            Button { visible: DvrService.error.length; text: "RESET FAULT"; onClicked: DvrService.clearError() }
        }
        Text { anchors.horizontalCenter: parent.horizontalCenter; visible: DvrService.error.length; text: DvrService.error; color: "#ff6f78"; font.bold: true }
    }
}
