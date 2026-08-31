#include "VideoService.h"

#include <algorithm>
#include <cmath>

VideoService::VideoService(QUrl source, QObject* parent)
    : QObject(parent), m_source(std::move(source))
{
    m_timer.setInterval(250);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        m_phase += 0.23;
        if (m_state == "locked") m_rssi = 82 + static_cast<int>(std::sin(m_phase) * 4.0);
        if (m_state == "weak") m_rssi = 25 + static_cast<int>(std::sin(m_phase * 2.0) * 12.0);
        emit changed();
    });
    m_timer.start();
}

void VideoService::setScenario(const QString& scenario)
{
    m_state = scenario;
    if (scenario == "lost" || scenario == "black") m_rssi = 0;
    else if (scenario == "weak" || scenario == "rolling") m_rssi = 28;
    else m_rssi = 82;
    emit changed();
}

void VideoService::toggleLoss()
{
    setScenario(m_state == "lost" ? "locked" : "lost");
}

void VideoService::setStandard(const QString& standard)
{
    if (standard != "PAL" && standard != "NTSC") return;
    m_standard = standard;
    emit changed();
}

void VideoService::nextChannel()
{
    m_channel = m_channel % 8 + 1;
    m_state = "locked";
    emit changed();
}

