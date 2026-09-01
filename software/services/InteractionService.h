#pragma once

#include <QObject>
#include <QPointF>
#include <QTimer>

class InteractionService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool controlsVisible READ controlsVisible NOTIFY changed)
    Q_PROPERTY(bool flightLocked READ flightLocked WRITE setFlightLocked NOTIFY changed)
    Q_PROPERTY(int autoHideMs READ autoHideMs WRITE setAutoHideMs NOTIFY changed)
    Q_PROPERTY(QPointF lastTouch READ lastTouch NOTIFY touchNoted)

public:
    explicit InteractionService(QObject* parent = nullptr);
    bool controlsVisible() const { return m_controlsVisible; }
    bool flightLocked() const { return m_flightLocked; }
    int autoHideMs() const { return m_autoHideMs; }
    QPointF lastTouch() const { return m_lastTouch; }

    void setFlightLocked(bool locked);
    void setAutoHideMs(int milliseconds);
    Q_INVOKABLE void showControls();
    Q_INVOKABLE void hideControls();
    Q_INVOKABLE void toggleControls();
    Q_INVOKABLE bool requestNavigation(const QString& app);
    Q_INVOKABLE void noteTouch(double x, double y);

signals:
    void changed();
    void navigationRequested(const QString& app);
    void navigationRejected(const QString& app);
    void touchNoted();

private:
    void restartAutoHide();
    QTimer m_autoHideTimer;
    bool m_controlsVisible{false};
    bool m_flightLocked{false};
    int m_autoHideMs{4500};
    QPointF m_lastTouch{-100.0, -100.0};
};
