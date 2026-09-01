#include "MediaService.h"

#include <algorithm>

MediaService::MediaService(QUrl source, QObject* parent)
    : QObject(parent), m_source(std::move(source))
{
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        if (!m_playing || !m_open) return;
        if (++m_positionSeconds >= m_durationSeconds) {
            m_positionSeconds = m_durationSeconds;
            m_playing = false;
            m_timer.stop();
        }
        emit changed();
    });
}

void MediaService::openFile(const QString& title, const int duration, const QString& aspect)
{
    m_title = title;
    m_durationSeconds = std::max(1, duration);
    m_aspect = aspect == "16:9" ? "16:9" : "4:3";
    m_positionSeconds = 0;
    m_open = true;
    m_playing = true;
    m_timer.start();
    emit changed();
}

void MediaService::close()
{
    m_timer.stop();
    m_open = false;
    m_playing = false;
    emit changed();
}

void MediaService::togglePlaying()
{
    if (!m_open) return;
    m_playing = !m_playing;
    if (m_playing) m_timer.start(); else m_timer.stop();
    emit changed();
}

void MediaService::seek(const int seconds)
{
    m_positionSeconds = std::clamp(seconds, 0, m_durationSeconds);
    emit changed();
}

void MediaService::skip(const int deltaSeconds)
{
    seek(m_positionSeconds + deltaSeconds);
}
