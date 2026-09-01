#include "SystemService.h"

#include <QFile>

#ifdef Q_OS_UNIX
#include <sys/resource.h>
#endif

namespace {
qint64 processCpuUsec()
{
#ifdef Q_OS_UNIX
    rusage usage{};
    if (getrusage(RUSAGE_SELF, &usage) != 0) return -1;
    return (usage.ru_utime.tv_sec + usage.ru_stime.tv_sec) * 1000000LL
        + usage.ru_utime.tv_usec + usage.ru_stime.tv_usec;
#else
    return -1;
#endif
}

double currentRssMegabytes()
{
    QFile status(QStringLiteral("/proc/self/status"));
    if (!status.open(QIODevice::ReadOnly | QIODevice::Text)) return -1.0;
    for (const QByteArray& line : status.readAll().split('\n')) {
        if (!line.startsWith("VmRSS:")) continue;
        const auto fields = line.simplified().split(' ');
        return fields.size() >= 2 ? fields.at(1).toDouble() / 1024.0 : -1.0;
    }
    return -1.0;
}
}

SystemService::SystemService(QObject* parent)
    : QObject(parent)
{
    m_lastCpuUsec = processCpuUsec();
    m_cpuTimer.start();
    m_memoryMegabytes = currentRssMegabytes();
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        const qint64 elapsedUsec = m_cpuTimer.restart() * 1000;
        const qint64 cpuUsec = processCpuUsec();
        if (elapsedUsec > 0 && cpuUsec >= 0 && m_lastCpuUsec >= 0) {
            m_cpuPercent = 100.0 * static_cast<double>(cpuUsec - m_lastCpuUsec)
                / static_cast<double>(elapsedUsec);
        }
        m_lastCpuUsec = cpuUsec;
        m_memoryMegabytes = currentRssMegabytes();
        ++m_uptimeSeconds;
        if (m_deckCharging && m_deckBatteryPercent < 100 && m_uptimeSeconds % 5 == 0) {
            ++m_deckBatteryPercent;
        } else if (!m_deckCharging && m_deckBatteryPercent > 0 && m_uptimeSeconds % 30 == 0) {
            --m_deckBatteryPercent;
        }
        emit changed();
    });
    m_timer.start();
}

QString SystemService::warning() const
{
    if (!m_mcuOnline) return "CONTROLLER OFFLINE";
    if (!m_adcHealthy) return "BALANCE ADC ERROR";
    if (!m_t8lConnected) return "T8L CONTROL OFFLINE";
    if (!m_elrsReady) return "ELRS TX NOT READY";
    if (!m_vrxOnline) return "VIDEO RECEIVER OFFLINE";
    if (!m_decoderOnline) return "VIDEO DECODER OFFLINE";
    if (m_temperatureC >= 75.0) return "SYSTEM OVER TEMPERATURE";
    if (m_deckBatteryPercent <= 15) return "LOW DECK BATTERY";
    return {};
}

void SystemService::setTouchDebug(const bool enabled)
{
    if (m_touchDebug == enabled) return;
    m_touchDebug = enabled;
    emit changed();
}

void SystemService::setScenario(const QString& scenario)
{
    if (scenario == "mcu offline") m_mcuOnline = false;
    else if (scenario == "adc error") m_adcHealthy = false;
    else if (scenario == "t8l offline") m_t8lConnected = false;
    else if (scenario == "elrs not ready") m_elrsReady = false;
    else if (scenario == "vrx offline") m_vrxOnline = false;
    else if (scenario == "decoder offline") m_decoderOnline = false;
    else if (scenario == "over temp") m_temperatureC = 82.0;
    else if (scenario == "low deck battery") m_deckBatteryPercent = 9;
    else if (scenario == "nominal") resetFaults();
    emit changed();
}

void SystemService::setCharging(const bool charging)
{
    if (m_deckCharging == charging) return;
    m_deckCharging = charging;
    emit changed();
}

void SystemService::resetFaults()
{
    m_mcuOnline = true;
    m_adcHealthy = true;
    m_t8lConnected = true;
    m_elrsReady = true;
    m_vrxOnline = true;
    m_decoderOnline = true;
    m_temperatureC = 48.0;
    if (m_deckBatteryPercent <= 15) m_deckBatteryPercent = 78;
    emit changed();
}

void SystemService::setDisplayBrightness(const int percent)
{
    const int bounded = qBound(10, percent, 100);
    if (m_displayBrightness == bounded) return;
    m_displayBrightness = bounded;
    emit changed();
}
