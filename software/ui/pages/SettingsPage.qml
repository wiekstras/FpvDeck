import QtQuick
import FPVDeck
import "../components"

Item {
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column { anchors.fill: parent; anchors.margins: Theme.space5; spacing: 16
        TopBar { width: parent.width; title: "SETTINGS"; subtitle: "Touch and flight behavior" }
        Rectangle { width: parent.width; height: 92; radius: Theme.radius; color: Theme.surface; border.color: Theme.border
            Row { anchors.fill: parent; anchors.margins: 18; spacing: 20
                Column { width: parent.width - 250; anchors.verticalCenter: parent.verticalCenter; Text { text: "Touch debug trace"; color: Theme.text; font.pixelSize: 17; font.bold: true } Text { text: "Show contact locations in development mode"; color: Theme.textMuted; font.pixelSize: 12 } }
                TouchButton { anchors.verticalCenter: parent.verticalCenter; text: SystemService.touchDebug ? "Enabled" : "Disabled"; checked: SystemService.touchDebug; accent: Theme.cyan; onClicked: SystemService.touchDebug = !SystemService.touchDebug }
            }
        }
        Rectangle { width: parent.width; height: 92; radius: Theme.radius; color: Theme.surface; border.color: Theme.border
            Row { anchors.fill: parent; anchors.margins: 18; spacing: 20
                Column { width: parent.width - 360; anchors.verticalCenter: parent.verticalCenter; Text { text: "Display brightness"; color: Theme.text; font.pixelSize: 17; font.bold: true } Text { text: "Hardware backlight binding pending"; color: Theme.textMuted; font.pixelSize: 12 } }
                TouchButton { text: "−"; onClicked: SystemService.setDisplayBrightness(SystemService.displayBrightness - 10) }
                StatusChip { text: SystemService.displayBrightness + "%"; icon: "☀"; accent: Theme.warning; anchors.verticalCenter: parent.verticalCenter }
                TouchButton { text: "+"; onClicked: SystemService.setDisplayBrightness(SystemService.displayBrightness + 10) }
            }
        }
        Rectangle { width: parent.width; height: 92; radius: Theme.radius; color: Theme.surface; border.color: Theme.border
            Row { anchors.fill: parent; anchors.margins: 18; spacing: 20
                Column { width: parent.width - 250; anchors.verticalCenter: parent.verticalCenter; Text { text: "Flight lock"; color: Theme.text; font.pixelSize: 17; font.bold: true } Text { text: "Blocks app navigation while keeping DVR and video controls available"; color: Theme.textMuted; font.pixelSize: 12 } }
                TouchButton { text: InteractionService.flightLocked ? "Locked" : "Unlocked"; checked: InteractionService.flightLocked; accent: Theme.warning; onClicked: InteractionService.flightLocked = !InteractionService.flightLocked }
            }
        }
    }
}
