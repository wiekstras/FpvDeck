import QtQuick
import FPVDeck

Rectangle {
    id: root
    property string text: ""
    property string icon: ""
    property color accent: Theme.blue
    property bool checked: false
    property bool destructive: false
    property string accessibleName: text
    signal clicked()
    activeFocusOnTab: enabled && visible
    implicitWidth: Math.max(Theme.touchTarget, content.implicitWidth + 32)
    implicitHeight: Theme.touchTarget
    radius: Theme.radiusSmall
    color: !enabled ? "#111925" : checked ? Qt.rgba(accent.r, accent.g, accent.b, 0.24) : tap.pressed ? "#24344b" : Theme.surfaceRaised
    border.color: activeFocus ? Theme.cyan : checked ? accent : Theme.border
    border.width: activeFocus || checked ? 2 : 1
    opacity: enabled ? 1 : 0.45

    Row {
        id: content
        anchors.centerIn: parent
        spacing: root.icon.length && root.text.length ? 9 : 0
        Text { visible: root.icon.length; text: root.icon; color: root.destructive ? Theme.error : root.accent; font.pixelSize: 19; anchors.verticalCenter: parent.verticalCenter }
        Text { visible: root.text.length; text: root.text; color: root.destructive ? Theme.error : Theme.text; font.pixelSize: 13; font.weight: Font.DemiBold; anchors.verticalCenter: parent.verticalCenter }
    }
    TapHandler { id: tap; enabled: root.enabled; onTapped: root.clicked() }
    Behavior on color { ColorAnimation { duration: Theme.animationFast } }
}
