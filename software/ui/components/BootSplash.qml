import QtQuick
import FPVDeck

Rectangle {
    id: root
    signal complete()
    color: Theme.background
    Column { anchors.centerIn: parent; spacing: 18
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: "FPVDECK"; color: Theme.text; font.pixelSize: 42; font.bold: true; font.letterSpacing: 4 }
        Text { anchors.horizontalCenter: parent.horizontalCenter; text: phase === 0 ? "STARTING VIDEO CORE" : phase === 1 ? "CHECKING SERVICES" : "READY"; color: phase === 2 ? Theme.accent : Theme.textMuted; font.pixelSize: 11; font.bold: true; font.letterSpacing: 1.6 }
        Rectangle { width: 280; height: 5; radius: 3; color: Theme.border
            Rectangle { height: parent.height; radius: 3; width: parent.width * ((phase + 1) / 3); color: Theme.accent; Behavior on width { NumberAnimation { duration: 180 } } }
        }
    }
    property int phase: 0
    Timer { interval: 420; running: true; repeat: true; onTriggered: { if (root.phase < 2) root.phase++; else { stop(); root.complete() } } }
}
