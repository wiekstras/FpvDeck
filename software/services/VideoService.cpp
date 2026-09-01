#include "VideoService.h"

VideoService::VideoService(QUrl source, QObject* parent)
    : QObject(parent), m_source(std::move(source))
{
}

void VideoService::setScenario(const QString& scenario)
{
    m_state = scenario;
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
