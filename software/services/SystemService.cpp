#include "SystemService.h"

SystemService::SystemService(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, [this] {
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
    if (!m_vrxOnline) return "RECEIVER OFFLINE";
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
    else if (scenario == "vrx offline") m_vrxOnline = false;
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
    m_vrxOnline = true;
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
