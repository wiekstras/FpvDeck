import QtQuick
import FPVDeck

Rectangle {
    id: root
    property string message: ""
    property bool error: false
    function show(text, isError) { message = text; error = isError || false; visible = true; timer.restart() }
    visible: false
    implicitWidth: label.implicitWidth + 40
    implicitHeight: 52
    radius: Theme.radiusSmall
    color: error ? "#381a22" : "#183228"
    border.color: error ? Theme.error : Theme.accent
    Text { id: label; anchors.centerIn: parent; text: root.message; color: Theme.text; font.pixelSize: 13; font.bold: true }
    Timer { id: timer; interval: 2400; onTriggered: root.visible = false }
}
