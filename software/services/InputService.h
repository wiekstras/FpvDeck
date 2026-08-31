#pragma once

#include <QObject>
#include <QString>

class InputService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool navigationLocked READ navigationLocked WRITE setNavigationLocked NOTIFY navigationLockedChanged)

public:
    explicit InputService(QObject* parent = nullptr) : QObject(parent) {}
    bool navigationLocked() const { return m_navigationLocked; }
    void setNavigationLocked(bool locked);
    Q_INVOKABLE void dispatch(const QString& action);

signals:
    void action(const QString& action);
    void rejected(const QString& action);
    void navigationLockedChanged();

private:
    bool m_navigationLocked{false};
};

