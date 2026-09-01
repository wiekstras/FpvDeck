#pragma once

#include <QObject>
#include <QUrl>

class VideoService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString backend READ backend NOTIFY changed)
    Q_PROPERTY(QString standard READ standard NOTIFY changed)
    Q_PROPERTY(QString state READ state NOTIFY changed)
    Q_PROPERTY(bool rolling READ rolling NOTIFY changed)
    Q_PROPERTY(bool liveCapture READ liveCapture NOTIFY changed)
    Q_PROPERTY(bool captureAvailable READ captureAvailable NOTIFY changed)
    Q_PROPERTY(QString devicePath READ devicePath NOTIFY changed)
    Q_PROPERTY(QString deviceDescription READ deviceDescription NOTIFY changed)
    Q_PROPERTY(QUrl source READ source NOTIFY changed)

public:
    explicit VideoService(QUrl source, QObject* parent = nullptr);
    QString backend() const { return m_backend; }
    QString standard() const { return m_standard; }
    QString state() const { return m_state; }
    bool rolling() const { return m_state == "rolling"; }
    bool liveCapture() const { return m_backend == "v4l2"; }
    bool captureAvailable() const { return m_captureAvailable; }
    QString devicePath() const { return m_devicePath; }
    QString deviceDescription() const { return m_deviceDescription; }
    QUrl source() const { return m_source; }

    Q_INVOKABLE void useFile(const QUrl& source);
    Q_INVOKABLE void useSimulated(const QUrl& source);
    Q_INVOKABLE void useV4l2(const QString& devicePath, const QString& description,
        bool available = true);
    Q_INVOKABLE void reportCaptureAvailability(bool available);
    Q_INVOKABLE void setScenario(const QString& scenario);
    Q_INVOKABLE void toggleLoss();
    Q_INVOKABLE void setStandard(const QString& standard);

signals:
    void changed();

private:
    QString m_backend{"file"};
    QUrl m_source;
    QString m_standard{"PAL"};
    QString m_state{"locked"};
    bool m_captureAvailable{true};
    QString m_devicePath;
    QString m_deviceDescription;
};
