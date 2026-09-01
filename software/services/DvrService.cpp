#include "DvrService.h"

#include <QDateTime>
#include <QDir>

DvrService::DvrService(QObject* parent)
    : QObject(parent)
{
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, [this] {
        ++m_elapsedSeconds;
        m_freeGigabytes = qMax(0.0, m_freeGigabytes - 0.002);
        emit changed();
    });
}

void DvrService::toggleRecording()
{
    if (!m_error.isEmpty()) return;
    m_recording = !m_recording;
    if (m_recording) {
        m_elapsedSeconds = 0;
        if (!m_outputDirectory.isEmpty()) {
            const QString stamp = QDateTime::currentDateTimeUtc().toString("yyyyMMdd-HHmmss-zzz");
            m_outputLocation = QUrl::fromLocalFile(
                QDir(m_outputDirectory).filePath("fpvdeck-" + stamp + ".mp4"));
        }
        m_timer.start();
    } else {
        m_timer.stop();
    }
    emit changed();
}

void DvrService::setOutputDirectory(const QString& directory)
{
    m_outputDirectory = directory;
    if (!m_outputDirectory.isEmpty()) QDir().mkpath(m_outputDirectory);
}

void DvrService::simulateStorageFull()
{
    m_recording = false;
    m_timer.stop();
    m_freeGigabytes = 0.0;
    m_error = "STORAGE FULL";
    emit changed();
}

void DvrService::clearError()
{
    m_error.clear();
    m_freeGigabytes = 47.2;
    emit changed();
}

void DvrService::reportRecorderError(const QString& message)
{
    m_recording = false;
    m_timer.stop();
    m_error = message.trimmed().isEmpty() ? "RECORDER ERROR" : message.trimmed();
    emit changed();
}
