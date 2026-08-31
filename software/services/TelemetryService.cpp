#include "TelemetryService.h"

#include <cmath>

TelemetryService::TelemetryService(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(150);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        if (m_state != "connected") return;
        m_phase += 0.09;
        m_linkQuality = 95 + static_cast<int>(std::sin(m_phase * 0.6) * 3.0);
        m_rssiDbm = -61 + static_cast<int>(std::sin(m_phase) * 5.0);
        m_snrDb = 8 + static_cast<int>(std::sin(m_phase * 1.4) * 2.0);
        m_altitudeMeters = 23.0 + std::sin(m_phase * 0.2) * 6.0;
        m_speedKph = 48.0 + std::sin(m_phase * 1.7) * 13.0;
        emit changed();
    });
    m_timer.start();
}

void TelemetryService::setScenario(const QString& scenario)
{
    if (scenario != "connected" && scenario != "lost" && scenario != "corrupt"
        && scenario != "version mismatch") return;
    m_state = scenario;
    if (scenario != "connected") {
        m_linkQuality = 0;
        m_snrDb = 0;
    }
    emit changed();
}

