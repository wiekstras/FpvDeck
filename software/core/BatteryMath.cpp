#include "BatteryMath.h"

#include <algorithm>
#include <cmath>

namespace fpvdeck {

BatteryReading deriveCells(const QVector<double>& cumulativeTaps)
{
    BatteryReading result;
    if (cumulativeTaps.isEmpty() || cumulativeTaps.size() > 8) {
        return result;
    }

    double previous = 0.0;
    result.cells.reserve(cumulativeTaps.size());
    for (const double tap : cumulativeTaps) {
        if (!std::isfinite(tap) || tap <= previous) {
            return BatteryReading{};
        }
        const double cell = tap - previous;
        // Wide fault-detection bounds, deliberately not a charge recommendation.
        if (cell < 0.5 || cell > 5.0) {
            return BatteryReading{};
        }
        result.cells.push_back(cell);
        previous = tap;
    }

    const auto [minimum, maximum] = std::minmax_element(result.cells.cbegin(), result.cells.cend());
    result.packVoltage = cumulativeTaps.constLast();
    result.delta = *maximum - *minimum;
    result.valid = true;
    return result;
}

} // namespace fpvdeck

