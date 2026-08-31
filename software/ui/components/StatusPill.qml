import QtQuick

Rectangle {
    id: root
    property string label: ""
    property color accent: "#67e8a5"
    implicitWidth: text.implicitWidth + 22
    implicitHeight: 28
    radius: 8
    color: Qt.rgba(accent.r, accent.g, accent.b, 0.12)
    border.color: Qt.rgba(accent.r, accent.g, accent.b, 0.4)

    Text {
        id: text
        anchors.centerIn: parent
        text: root.label
        color: root.accent
        font.pixelSize: 12
        font.weight: Font.DemiBold
        font.letterSpacing: 0.6
    }
}

