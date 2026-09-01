#include "VideoService.h"

VideoService::VideoService(QUrl source, QObject* parent)
    : QObject(parent), m_source(std::move(source))
{
}

void VideoService::useFile(const QUrl& source)
{
    m_backend = "file";
    m_source = source;
    m_devicePath.clear();
    m_deviceDescription.clear();
    m_captureAvailable = true;
    m_state = "locked";
    emit changed();
}

void VideoService::useSimulated(const QUrl& source)
{
    useFile(source);
    m_backend = "simulated";
    emit changed();
}

void VideoService::useV4l2(const QString& devicePath, const QString& description,
    bool available)
{
    m_backend = "v4l2";
    m_source = QUrl();
    m_devicePath = devicePath;
    m_deviceDescription = description;
    m_captureAvailable = available;
    m_state = available ? "locked" : "lost";
    emit changed();
}

void VideoService::reportCaptureAvailability(bool available)
{
    if (m_backend != "v4l2" || m_captureAvailable == available) return;
    m_captureAvailable = available;
    m_state = available ? "locked" : "lost";
    emit changed();
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
