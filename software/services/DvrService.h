#pragma once

#include <QObject>
#include <QTimer>
#include <QUrl>

class DvrService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool recording READ recording NOTIFY changed)
    Q_PROPERTY(int elapsedSeconds READ elapsedSeconds NOTIFY changed)
    Q_PROPERTY(double freeGigabytes READ freeGigabytes NOTIFY changed)
    Q_PROPERTY(QString error READ error NOTIFY changed)
    Q_PROPERTY(QUrl outputLocation READ outputLocation NOTIFY changed)

public:
    explicit DvrService(QObject* parent = nullptr);
    bool recording() const { return m_recording; }
    int elapsedSeconds() const { return m_elapsedSeconds; }
    double freeGigabytes() const { return m_freeGigabytes; }
    QString error() const { return m_error; }
    QUrl outputLocation() const { return m_outputLocation; }
    void setOutputDirectory(const QString& directory);
    Q_INVOKABLE void toggleRecording();
    Q_INVOKABLE void simulateStorageFull();
    Q_INVOKABLE void clearError();
    Q_INVOKABLE void reportRecorderError(const QString& message);

signals:
    void changed();

private:
    QTimer m_timer;
    bool m_recording{false};
    int m_elapsedSeconds{0};
    double m_freeGigabytes{47.2};
    QString m_error;
    QString m_outputDirectory;
    QUrl m_outputLocation;
};
