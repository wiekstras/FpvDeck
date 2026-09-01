#include "InteractionService.h"

#include <algorithm>

InteractionService::InteractionService(QObject* parent)
    : QObject(parent)
{
    m_autoHideTimer.setSingleShot(true);
    connect(&m_autoHideTimer, &QTimer::timeout, this, [this] {
        if (!m_controlsVisible) return;
        m_controlsVisible = false;
        emit changed();
    });
}

void InteractionService::setFlightLocked(const bool locked)
{
    if (m_flightLocked == locked) return;
    m_flightLocked = locked;
    emit changed();
}

void InteractionService::setAutoHideMs(const int milliseconds)
{
    const int bounded = std::clamp(milliseconds, 100, 30000);
    if (m_autoHideMs == bounded) return;
    m_autoHideMs = bounded;
    if (m_controlsVisible) restartAutoHide();
    emit changed();
}

void InteractionService::showControls()
{
    if (!m_controlsVisible) {
        m_controlsVisible = true;
        emit changed();
    }
    restartAutoHide();
}

void InteractionService::hideControls()
{
    m_autoHideTimer.stop();
    if (!m_controlsVisible) return;
    m_controlsVisible = false;
    emit changed();
}

void InteractionService::toggleControls()
{
    if (m_controlsVisible) hideControls(); else showControls();
}

bool InteractionService::requestNavigation(const QString& app)
{
    if (m_flightLocked && app != "fpv") {
        emit navigationRejected(app);
        return false;
    }
    emit navigationRequested(app);
    return true;
}

void InteractionService::noteTouch(const double x, const double y)
{
    m_lastTouch = {x, y};
    emit touchNoted();
}

void InteractionService::restartAutoHide()
{
    m_autoHideTimer.start(m_autoHideMs);
}
