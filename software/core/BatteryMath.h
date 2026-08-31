#pragma once

#include <QVector>

namespace fpvdeck {

struct BatteryReading {
    QVector<double> cells;
    double packVoltage{0.0};
    double delta{0.0};
    bool valid{false};
};

// Converts cumulative balance-tap readings into individual cells. A pack is
// invalid if taps do not rise monotonically or any derived cell is outside the
// broad electrical sanity range. Chemistry-specific warnings belong upstream.
BatteryReading deriveCells(const QVector<double>& cumulativeTaps);

} // namespace fpvdeck

