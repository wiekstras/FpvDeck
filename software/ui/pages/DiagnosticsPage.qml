import QtQuick
import QtQuick.Layouts
import FPVDeck
import "../components"

Item {
    Rectangle { anchors.fill: parent; color: Theme.background }
    Column {
        anchors.fill: parent; anchors.margins: Theme.space5; spacing: 16
        TopBar { width: parent.width; title: "DIAGNOSTICS"; subtitle: "Boot, bench and field self-test" }
        GridLayout {
            width: parent.width; columns: 4; columnSpacing: 12; rowSpacing: 12
            Repeater {
                model: [
                    { name: "SOFTWARE", value: "0.1.0-dev", state: "PASS", tint: Theme.accent },
                    { name: "MCU", value: SystemService.mcuOnline ? "sim-protocol v1" : "No heartbeat", state: SystemService.mcuOnline ? "SIM" : "FAIL", tint: SystemService.mcuOnline ? Theme.blue : Theme.error },
                    { name: "BALANCE ADC", value: SystemService.adcHealthy ? BatteryService.cellCount + " channels" : "Read error", state: SystemService.adcHealthy ? "SIM" : "FAIL", tint: SystemService.adcHealthy ? Theme.blue : Theme.error },
                    { name: "TOUCHSCREEN", value: SystemService.touchDebug ? "Touch trace on" : "Input online", state: "PASS", tint: Theme.accent },
                    { name: "T8L CONTROL", value: SystemService.t8lConnected ? "Donor controls connected" : "Disconnected", state: SystemService.t8lConnected ? "SIM" : "FAIL", tint: SystemService.t8lConnected ? Theme.blue : Theme.error },
                    { name: "ELRS TX 2.4 GHz", value: SystemService.elrsReady ? "Control transmitter ready" : "Not ready", state: SystemService.elrsReady ? "SIM" : "FAIL", tint: SystemService.elrsReady ? Theme.blue : Theme.error },
                    { name: "5.8 GHz VIDEO VRX", value: SystemService.vrxOnline ? "R" + RadioService.channel + " · " + RadioService.rssi + "%" : "Offline", state: SystemService.vrxOnline ? "SIM" : "FAIL", tint: SystemService.vrxOnline ? Theme.blue : Theme.error },
                    { name: "CVBS CAPTURE", value: VideoService.liveCapture ? VideoService.deviceDescription : (SystemService.decoderOnline ? "Simulated decoder" : "Offline"), state: VideoService.liveCapture ? (VideoService.captureAvailable ? "PASS" : "FAIL") : (SystemService.decoderOnline ? "SIM" : "FAIL"), tint: VideoService.liveCapture ? (VideoService.captureAvailable ? Theme.accent : Theme.error) : (SystemService.decoderOnline ? Theme.blue : Theme.error) },
                    { name: "DIGITAL VIDEO", value: VideoService.standard + " · " + VideoService.backend.toUpperCase() + " · " + VideoService.state, state: VideoService.state === "locked" ? "PASS" : "WARN", tint: VideoService.state === "locked" ? Theme.accent : Theme.warning },
                    { name: "STORAGE", value: StorageService.status + " · " + StorageService.freeGigabytes.toFixed(1) + "GB", state: StorageService.sdPresent && !StorageService.sdCorrupt ? "PASS" : "WARN", tint: StorageService.sdPresent && !StorageService.sdCorrupt ? Theme.accent : Theme.warning },
                    { name: "SYSTEM", value: SystemService.temperatureC.toFixed(0) + "°C · " + SystemService.deckBatteryPercent + "%", state: SystemService.warning.length ? "WARN" : "PASS", tint: SystemService.warning.length ? Theme.warning : Theme.accent },
                    { name: "DISPLAY", value: Screen.width + "×" + Screen.height + " · " + SystemService.displayBrightness + "%", state: VideoService.liveCapture ? "PASS" : "SIM", tint: VideoService.liveCapture ? Theme.accent : Theme.blue },
                    { name: "TELEMETRY", value: TelemetryService.connected ? "LQ " + TelemetryService.linkQuality : "Disconnected", state: TelemetryService.connected ? "SIM" : "WARN", tint: TelemetryService.connected ? Theme.blue : Theme.warning },
                    { name: "UPTIME", value: Math.floor(SystemService.uptimeSeconds / 60) + "m " + (SystemService.uptimeSeconds % 60) + "s", state: "PASS", tint: Theme.accent },
                    { name: "PROCESS", value: (SystemService.cpuPercent < 0 ? "CPU —" : "CPU " + SystemService.cpuPercent.toFixed(0) + "%") + " · " + (SystemService.memoryMegabytes < 0 ? "MEM —" : SystemService.memoryMegabytes.toFixed(0) + " MB"), state: "PASS", tint: Theme.accent },
                    { name: "NETWORK", value: "Not required for FPV", state: "ISOLATED", tint: Theme.textMuted }
                ]
                Rectangle {
                    required property var modelData
                    Layout.fillWidth: true; Layout.preferredHeight: 118; radius: Theme.radius; color: Theme.surface; border.color: Theme.border
                    Column { anchors.fill: parent; anchors.margins: 15; spacing: 9
                        Row { width: parent.width; Text { width: parent.width - 58; text: modelData.name; color: Theme.textDim; font.pixelSize: 10; font.bold: true; font.letterSpacing: 1 } Text { text: modelData.state; color: modelData.tint; font.pixelSize: 10; font.bold: true } }
                        Text { text: modelData.value; color: Theme.text; font.pixelSize: 15; font.bold: true; elide: Text.ElideRight; width: parent.width }
                        Rectangle { width: parent.width; height: 5; radius: 3; color: Theme.border; Rectangle { width: parent.width * 0.88; height: parent.height; radius: 3; color: modelData.tint } }
                    }
                }
            }
        }
    }
}
