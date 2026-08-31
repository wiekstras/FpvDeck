#include "BatteryService.h"

#include <algorithm>
#include <cmath>
#include <numeric>

BatteryService::BatteryService(QObject* parent)
    : QObject(parent)
{
    configureCells(4);
    m_timer.setInterval(180);
    connect(&m_timer, &QTimer::timeout, this, &BatteryService::update);
    m_timer.start();
}

QVariantList BatteryService::cellVoltages() const
{
    QVariantList list;
    for (const double cell : m_cells) {
        list.append(cell);
    }
    return list;
}

double BatteryService::packVoltage() const
{
    return std::accumulate(m_cells.cbegin(), m_cells.cend(), 0.0);
}

double BatteryService::delta() const
{
    if (m_cells.isEmpty()) {
        return 0.0;
    }
    const auto [minimum, maximum] = std::minmax_element(m_cells.cbegin(), m_cells.cend());
    return *maximum - *minimum;
}

void BatteryService::configureCells(const int count)
{
    const int bounded = std::clamp(count, 1, 6);
    m_cells.fill(4.08, bounded);
    m_connected = true;
    m_scenario = "nominal";
    m_warning.clear();
    emit changed();
}

void BatteryService::setScenario(const QString& scenario)
{
    m_scenario = scenario;
    m_connected = scenario != "disconnected";
    m_warning.clear();
    if (scenario == "imbalanced" && !m_cells.isEmpty()) {
        m_cells.last() = 3.72;
        m_warning = "CELL IMBALANCE";
    } else if (scenario == "undervoltage") {
        std::fill(m_cells.begin(), m_cells.end(), 3.18);
        m_warning = "LOW CELL VOLTAGE";
    } else if (scenario == "overvoltage") {
        std::fill(m_cells.begin(), m_cells.end(), 4.32);
        m_warning = "OVER VOLTAGE";
    } else if (scenario == "invalid taps") {
        m_warning = "INVALID TAP ORDER";
    } else if (scenario == "nominal") {
        std::fill(m_cells.begin(), m_cells.end(), 4.08);
    }
    emit changed();
}

void BatteryService::update()
{
    if (!m_connected || m_scenario == "invalid taps") {
        return;
    }
    m_phase += 0.17;
    for (int i = 0; i < m_cells.size(); ++i) {
        const double noise = std::sin(m_phase + i * 1.7) * 0.003;
        double center = 4.08 - i * 0.002;
        if (m_scenario == "imbalanced" && i == m_cells.size() - 1) center = 3.72;
        if (m_scenario == "undervoltage") center = 3.18;
        if (m_scenario == "overvoltage") center = 4.32;
        m_cells[i] = center + noise;
    }
    emit changed();
}

