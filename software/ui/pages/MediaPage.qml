import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import FPVDeck
import "../components"

Item {
    id: root
    property bool playbackControls: true
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column {
        anchors.fill: parent; anchors.margins: Theme.space5; spacing: 16; visible: !MediaService.open
        TopBar { width: parent.width; title: "MEDIA"; subtitle: "Internal DVR and removable SD" }
        Row { width: parent.width; spacing: 10
            StatusChip { text: StorageService.status; icon: "SD"; accent: StorageService.sdPresent && !StorageService.sdCorrupt ? Theme.cyan : Theme.warning; alert: !StorageService.sdPresent || StorageService.sdCorrupt }
            StatusChip { visible: StorageService.sdPresent; text: StorageService.freeGigabytes.toFixed(1) + " GB FREE"; accent: StorageService.freeGigabytes < 1 ? Theme.error : Theme.textMuted; alert: StorageService.freeGigabytes < 1 }
            TouchButton { text: "List"; icon: "☷"; checked: true }
        }
        Rectangle {
            width: parent.width; height: parent.height - 126; radius: Theme.radiusLarge; color: Theme.surface; border.color: Theme.border
            Column { anchors.fill: parent; anchors.margins: 20; spacing: 10
                Text { visible: !StorageService.sdPresent; text: "INSERT A REMOVABLE SD CARD TO BROWSE RECORDINGS"; color: Theme.warning; font.pixelSize: 16; font.bold: true }
                Text { visible: StorageService.sdCorrupt; text: "CARD REQUIRES A FILESYSTEM CHECK BEFORE PLAYBACK"; color: Theme.error; font.pixelSize: 16; font.bold: true }
                Repeater {
                    model: StorageService.sdPresent && !StorageService.sdCorrupt ? StorageService.mediaFiles : []
                    MediaTile { required property var modelData; width: parent.width; media: modelData; onClicked: { MediaService.openFile(modelData.title, modelData.duration, modelData.aspect); root.playbackControls = true } }
                }
            }
        }
    }

    Item {
        anchors.fill: parent; visible: MediaService.open
        MediaPlayer { id: playback; source: MediaService.source; videoOutput: playbackOutput; loops: MediaPlayer.Infinite; onPlaybackStateChanged: if (MediaService.open && playbackState === MediaPlayer.StoppedState) play() }
        VideoOutput { id: playbackOutput; anchors.fill: parent; fillMode: VideoOutput.PreserveAspectFit; TapHandler { onTapped: { root.playbackControls = !root.playbackControls; hideTimer.restart() } } }
        Rectangle { anchors.fill: parent; visible: root.playbackControls; color: "#45000000" }
        TouchButton {
            visible: root.playbackControls
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.margins: 20
            icon: "‹"
            text: "Media"
            onClicked: { playback.stop(); MediaService.close() }
        }
        StatusChip {
            visible: root.playbackControls
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 24
            text: MediaService.title + " · " + MediaService.aspect
            accent: Theme.cyan
        }
        Rectangle {
            visible: root.playbackControls
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.margins: 20
            height: 112; radius: Theme.radiusLarge; color: "#ed0b111a"; border.color: Theme.border
            Column { anchors.fill: parent; anchors.margins: 14; spacing: 8
                Slider { width: parent.width; from: 0; to: Math.max(1, MediaService.durationSeconds); value: MediaService.positionSeconds; onMoved: MediaService.seek(value) }
                Row { anchors.horizontalCenter: parent.horizontalCenter; spacing: 10
                    TouchButton { icon: "↶"; text: "10s"; onClicked: MediaService.skip(-10) }
                    TouchButton { icon: MediaService.playing ? "Ⅱ" : "▶"; text: MediaService.playing ? "Pause" : "Play"; accent: Theme.cyan; checked: MediaService.playing; onClicked: { MediaService.togglePlaying(); if (MediaService.playing) playback.play(); else playback.pause(); hideTimer.restart() } }
                    TouchButton { icon: "↷"; text: "10s"; onClicked: MediaService.skip(10) }
                }
            }
        }
        Timer { id: hideTimer; interval: 3500; running: root.playbackControls; onTriggered: root.playbackControls = false }
        Component.onCompleted: if (MediaService.open) playback.play()
    }
}
