#pragma once

#include <QObject>
#include <QTimer>
#include <QUrl>

class MediaService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool open READ open NOTIFY changed)
    Q_PROPERTY(bool playing READ playing NOTIFY changed)
    Q_PROPERTY(int positionSeconds READ positionSeconds NOTIFY changed)
    Q_PROPERTY(int durationSeconds READ durationSeconds NOTIFY changed)
    Q_PROPERTY(QString title READ title NOTIFY changed)
    Q_PROPERTY(QString aspect READ aspect NOTIFY changed)
    Q_PROPERTY(QUrl source READ source CONSTANT)

public:
    explicit MediaService(QUrl source, QObject* parent = nullptr);
    bool open() const { return m_open; }
    bool playing() const { return m_playing; }
    int positionSeconds() const { return m_positionSeconds; }
    int durationSeconds() const { return m_durationSeconds; }
    QString title() const { return m_title; }
    QString aspect() const { return m_aspect; }
    QUrl source() const { return m_source; }
    Q_INVOKABLE void openFile(const QString& title, int duration, const QString& aspect);
    Q_INVOKABLE void close();
    Q_INVOKABLE void togglePlaying();
    Q_INVOKABLE void seek(int seconds);
    Q_INVOKABLE void skip(int deltaSeconds);

signals:
    void changed();

private:
    QUrl m_source;
    QTimer m_timer;
    bool m_open{false};
    bool m_playing{false};
    int m_positionSeconds{0};
    int m_durationSeconds{0};
    QString m_title;
    QString m_aspect{"4:3"};
};
