#include "RadioService.h"

#include <cmath>

RadioService::RadioService(QObject* parent)
    : QObject(parent)
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

void RadioService::setScenario(const QString& scenario)
{
    m_state = scenario;
    if (scenario == "lost" || scenario == "black") m_rssi = 0;
    else if (scenario == "weak" || scenario == "rolling") m_rssi = 28;
    else m_rssi = 82;
    emit changed();
}

void RadioService::toggleLoss()
{
    setScenario(m_state == "lost" ? "locked" : "lost");
}

void RadioService::nextChannel()
{
    setChannel(m_channel % 8 + 1);
}

void RadioService::setChannel(const int channel)
{
    if (channel < 1 || channel > 8) return;
    m_channel = channel;
    m_state = "locked";
    m_rssi = 74 + channel * 2;
    emit changed();
}

void RadioService::toggleFavorite()
{
    if (m_favorites.contains(m_channel)) m_favorites.remove(m_channel);
    else m_favorites.insert(m_channel);
    emit changed();
}

void RadioService::scanStrongest()
{
    if (m_scanning) return;
    m_scanning = true;
    emit changed();
    QTimer::singleShot(650, this, [this] {
        m_channel = 6;
        m_rssi = 93;
        m_state = "locked";
        m_scanning = false;
        emit changed();
    });
}
