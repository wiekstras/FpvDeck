#include "StorageService.h"

StorageService::StorageService(QObject* parent)
    : QObject(parent)
{
    m_mediaFiles = {
        QVariantMap{{"title", "RIDGE RUN"}, {"date", "30 AUG 2026"}, {"duration", 381}, {"size", "2.8 GB"}, {"channel", "R1"}, {"aspect", "4:3"}},
        QVariantMap{{"title", "PARK LINE"}, {"date", "28 AUG 2026"}, {"duration", 314}, {"size", "2.3 GB"}, {"channel", "R3"}, {"aspect", "4:3"}},
        QVariantMap{{"title", "SUNSET TEST"}, {"date", "24 AUG 2026"}, {"duration", 249}, {"size", "1.9 GB"}, {"channel", "F4"}, {"aspect", "16:9"}},
    };
}

QString StorageService::status() const
{
    if (!m_sdPresent) return "SD REMOVED";
    if (m_sdCorrupt) return "SD NEEDS CHECK";
    if (m_freeGigabytes < 1.0) return "SD ALMOST FULL";
    return "SD READY";
}

void StorageService::setScenario(const QString& scenario)
{
    if (scenario == "removed") {
        m_sdPresent = false;
        m_sdCorrupt = false;
    } else if (scenario == "corrupt") {
        m_sdPresent = true;
        m_sdCorrupt = true;
    } else if (scenario == "full") {
        m_sdPresent = true;
        m_sdCorrupt = false;
        m_freeGigabytes = 0.3;
    } else if (scenario == "inserted") {
        m_sdPresent = true;
        m_sdCorrupt = false;
        m_freeGigabytes = 47.2;
    }
    emit changed();
}
