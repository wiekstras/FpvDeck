#pragma once

#include <QObject>
#include <QVariantList>

class StorageService final : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool sdPresent READ sdPresent NOTIFY changed)
    Q_PROPERTY(bool sdCorrupt READ sdCorrupt NOTIFY changed)
    Q_PROPERTY(double freeGigabytes READ freeGigabytes NOTIFY changed)
    Q_PROPERTY(QVariantList mediaFiles READ mediaFiles NOTIFY changed)
    Q_PROPERTY(QString status READ status NOTIFY changed)

public:
    explicit StorageService(QObject* parent = nullptr);
    bool sdPresent() const { return m_sdPresent; }
    bool sdCorrupt() const { return m_sdCorrupt; }
    double freeGigabytes() const { return m_freeGigabytes; }
    QVariantList mediaFiles() const { return m_mediaFiles; }
    QString status() const;
    Q_INVOKABLE void setScenario(const QString& scenario);

signals:
    void changed();

private:
    bool m_sdPresent{true};
    bool m_sdCorrupt{false};
    double m_freeGigabytes{47.2};
    QVariantList m_mediaFiles;
};
