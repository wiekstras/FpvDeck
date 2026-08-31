#include "InputService.h"

#include <QSet>

void InputService::setNavigationLocked(const bool locked)
{
    if (m_navigationLocked == locked) return;
    m_navigationLocked = locked;
    emit navigationLockedChanged();
}

void InputService::dispatch(const QString& action)
{
    static const QSet<QString> allowed = {
        "back", "battery", "dvr", "menu", "signal", "simulator"
    };
    if (!allowed.contains(action) || (m_navigationLocked && action == "menu")) {
        emit rejected(action);
        return;
    }
    emit this->action(action);
}
