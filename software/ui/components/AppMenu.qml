import QtQuick
import QtQuick.Layouts
import FPVDeck

Rectangle {
    id: root
    signal appSelected(string app)
    color: "#f50b111b"
    radius: Theme.radiusLarge
    border.color: Theme.border
    Column {
        anchors.fill: parent; anchors.margins: Theme.space4; spacing: 16
        Text { text: "APPLICATIONS"; color: Theme.text; font.pixelSize: 18; font.bold: true; font.letterSpacing: 1.8 }
        GridLayout {
            width: parent.width; columns: 3; columnSpacing: 10; rowSpacing: 10
            Repeater {
                model: [
                    { id: "home", label: "Home", icon: "⌂", tint: Theme.text },
                    { id: "battery", label: "Battery", icon: "▰", tint: Theme.blue },
                    { id: "media", label: "Media", icon: "▶", tint: Theme.cyan },
                    { id: "flights", label: "Flights", icon: "↗", tint: Theme.purple },
                    { id: "batteries", label: "Packs", icon: "▥", tint: Theme.blue },
                    { id: "aircraft", label: "Aircraft", icon: "✦", tint: Theme.warning },
                    { id: "receiver", label: "Receiver", icon: "⌁", tint: Theme.accent },
                    { id: "diagnostics", label: "Diagnostics", icon: "◇", tint: Theme.cyan },
                    { id: "settings", label: "Settings", icon: "⚙", tint: Theme.textMuted }
                ]
                TouchButton {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 64
                    icon: modelData.icon; text: modelData.label; accent: modelData.tint
                    onClicked: root.appSelected(modelData.id)
                }
            }
        }
    }
}
