import QtQuick
import FPVDeck
import "../components"

Item {
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column { anchors.fill: parent; anchors.margins: Theme.space5; spacing: 14
        TopBar { width: parent.width; title: "FAULT INJECTION"; subtitle: "Development only · all states are simulated" }
        Text { text: "VIDEO / VRX"; color: Theme.textDim; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1.4 }
        Flow { width: parent.width; spacing: 8
            Repeater { model: ["locked", "weak", "rolling", "lost", "black"]; TouchButton { required property string modelData; text: modelData; checked: VideoService.state === modelData; onClicked: VideoService.setScenario(modelData) } }
            TouchButton { text: "VRX OFFLINE"; destructive: true; onClicked: SystemService.setScenario("vrx offline") }
        }
        Text { text: "EXTERNAL BATTERY / CONTROLLER"; color: Theme.textDim; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1.4 }
        Flow { width: parent.width; spacing: 8
            Repeater { model: ["nominal", "imbalanced", "undervoltage", "overvoltage", "disconnected", "invalid taps"]; TouchButton { required property string modelData; text: modelData; onClicked: BatteryService.setScenario(modelData) } }
            TouchButton { text: "ADC ERROR"; destructive: true; onClicked: SystemService.setScenario("adc error") }
            TouchButton { text: "MCU OFFLINE"; destructive: true; onClicked: SystemService.setScenario("mcu offline") }
        }
        Text { text: "STORAGE / POWER / THERMAL"; color: Theme.textDim; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1.4 }
        Flow { width: parent.width; spacing: 8
            Repeater { model: ["inserted", "removed", "corrupt", "full"]; TouchButton { required property string modelData; text: "SD " + modelData; onClicked: StorageService.setScenario(modelData) } }
            TouchButton { text: "LOW DECK BATTERY"; destructive: true; onClicked: SystemService.setScenario("low deck battery") }
            TouchButton { text: "OVER TEMP"; destructive: true; onClicked: SystemService.setScenario("over temp") }
            TouchButton { text: SystemService.deckCharging ? "STOP CHARGING" : "START CHARGING"; checked: SystemService.deckCharging; onClicked: SystemService.setCharging(!SystemService.deckCharging) }
        }
        Flow { width: parent.width; spacing: 8
            TouchButton { text: SystemService.touchDebug ? "TOUCH TRACE ON" : "TOUCH TRACE OFF"; checked: SystemService.touchDebug; accent: Theme.cyan; onClicked: SystemService.touchDebug = !SystemService.touchDebug }
            TouchButton { text: "RESET ALL"; icon: "↻"; accent: Theme.accent; onClicked: { SystemService.resetFaults(); StorageService.setScenario("inserted"); BatteryService.setScenario("nominal"); VideoService.setScenario("locked") } }
        }
    }
}
