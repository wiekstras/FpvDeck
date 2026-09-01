import QtQuick
import QtQuick.Layouts
import FPVDeck
import "../components"

Item {
    Rectangle { anchors.fill: parent; color: Theme.background }
    RowLayout {
        anchors.fill: parent; anchors.margins: Theme.space5; spacing: Theme.space4
        Rectangle {
            Layout.fillHeight: true; Layout.preferredWidth: 370; radius: Theme.radiusLarge; color: Theme.surface; border.color: Theme.border
            Column { anchors.fill: parent; anchors.margins: 26; spacing: 13
                Text { text: "EXTERNAL BALANCE PACK"; color: Theme.blue; font.pixelSize: 12; font.bold: true; font.letterSpacing: 1.5 }
                Text { text: BatteryService.connected ? BatteryService.packVoltage.toFixed(2) + " V" : "NOT CONNECTED"; color: Theme.text; font.pixelSize: BatteryService.connected ? 48 : 27; font.bold: true }
                Text { visible: BatteryService.connected; text: BatteryService.cellCount + "S " + BatteryService.chemistry + " · Δ " + (BatteryService.delta * 1000).toFixed(0) + " mV"; color: Theme.textMuted; font.pixelSize: 15 }
                Rectangle { width: parent.width; height: 1; color: Theme.border }
                Text { text: BatteryService.health; color: BatteryService.warning.length ? Theme.warning : Theme.accent; font.pixelSize: 15; font.bold: true; font.letterSpacing: 1 }
                GridLayout { visible: BatteryService.connected; width: parent.width; columns: 2; columnSpacing: 10; rowSpacing: 10
                    Rectangle { Layout.fillWidth: true; height: 72; radius: 12; color: Theme.surfaceRaised; Column { anchors.centerIn: parent; Text { text: "WEAKEST"; color: Theme.textDim; font.pixelSize: 9; font.bold: true } Text { text: "C" + BatteryService.weakestCell + " · " + BatteryService.weakestVoltage.toFixed(3) + "V"; color: Theme.warning; font.pixelSize: 16; font.bold: true } } }
                    Rectangle { Layout.fillWidth: true; height: 72; radius: 12; color: Theme.surfaceRaised; Column { anchors.centerIn: parent; Text { text: "STRONGEST"; color: Theme.textDim; font.pixelSize: 9; font.bold: true } Text { text: "C" + BatteryService.strongestCell + " · " + BatteryService.strongestVoltage.toFixed(3) + "V"; color: Theme.accent; font.pixelSize: 16; font.bold: true } } }
                }
                Text { text: "This port measures the drone pack only. It never powers FpvDeck."; width: parent.width; wrapMode: Text.WordWrap; color: Theme.textMuted; font.pixelSize: 12; lineHeight: 1.35 }
                Text { text: "INTERNAL DECK BATTERY"; color: Theme.textDim; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1.3 }
                StatusChip { text: SystemService.deckBatteryPercent + "%" + (SystemService.deckCharging ? " · CHARGING" : ""); icon: "▰"; accent: Theme.accent }
            }
        }
        Rectangle {
            Layout.fillWidth: true; Layout.fillHeight: true; radius: Theme.radiusLarge; color: Theme.surface; border.color: Theme.border
            Column { anchors.fill: parent; anchors.margins: 26; spacing: 12
                Row { width: parent.width
                    Text { width: parent.width - 170; text: "CELL VOLTAGES"; color: Theme.text; font.pixelSize: 19; font.bold: true }
                    StatusChip { text: "SIMULATED ADC"; accent: Theme.warning }
                }
                Repeater { model: BatteryService.cellVoltages; CellVoltageRow { width: parent.width; cellNumber: index + 1; voltage: modelData; weakest: index + 1 === BatteryService.weakestCell } }
                Flow { width: parent.width; spacing: 8
                    Repeater { model: [1, 2, 3, 4, 5, 6]; TouchButton { required property int modelData; text: modelData + "S"; checked: BatteryService.cellCount === modelData; onClicked: BatteryService.configureCells(modelData) } }
                }
            }
        }
    }
}
