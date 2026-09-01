import QtQuick
import QtQuick.Layouts
import FPVDeck
import "../components"

Item {
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column { anchors.fill: parent; anchors.margins: Theme.space5; spacing: 20
        TopBar { width: parent.width; title: "RECEIVER"; subtitle: "5.8 GHz analog video" }
        Row { spacing: 12
            StatusChip { text: SystemService.vrxOnline ? "VRX ONLINE" : "VRX OFFLINE"; icon: "⌁"; accent: SystemService.vrxOnline ? Theme.accent : Theme.error; alert: !SystemService.vrxOnline }
            StatusChip { text: VideoService.standard + " · R" + VideoService.channel; accent: Theme.blue }
            StatusChip { text: VideoService.rssi + "% RSSI"; accent: VideoService.rssi < 35 ? Theme.warning : Theme.accent }
        }
        Rectangle { width: parent.width; height: 215; radius: Theme.radiusLarge; color: Theme.surface; border.color: Theme.border
            Column { anchors.fill: parent; anchors.margins: 24; spacing: 16
                Row { width: parent.width
                    Text { width: parent.width - 300; text: "CHANNELS"; color: Theme.text; font.pixelSize: 18; font.bold: true }
                    TouchButton { text: VideoService.scanning ? "Scanning…" : "Scan strongest"; icon: "⌁"; accent: Theme.accent; enabled: !VideoService.scanning; onClicked: VideoService.scanStrongest() }
                }
                Flow { width: parent.width; spacing: 10
                    Repeater { model: 8; TouchButton { text: "R" + (index + 1); checked: VideoService.channel === index + 1; accent: Theme.accent; onClicked: VideoService.setChannel(index + 1) } }
                }
                TouchButton { icon: VideoService.favorite ? "★" : "☆"; text: VideoService.favorite ? "Favorite channel" : "Add favorite"; accent: Theme.warning; checked: VideoService.favorite; onClicked: VideoService.toggleFavorite() }
            }
        }
        Text { text: "Channel control is simulated. The SteadyView X control interface remains a bench characterization task."; color: Theme.textMuted; font.pixelSize: 12 }
    }
}
