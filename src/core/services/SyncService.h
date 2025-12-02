#ifndef SYNCSERVICE_H
#define SYNCSERVICE_H

#include <QObject>

class StorageDriver;

class SyncService : public QObject
{
    Q_OBJECT

public:
    explicit SyncService(QObject *parent = nullptr);
    ~SyncService();

    void setStorageDriver(StorageDriver *driver);

public slots:
    bool sync();

signals:
    void syncStarted();
    void syncFinished(bool success);
    void syncProgress(int percent);

private:
    StorageDriver *m_driver;
};

#endif // SYNCSERVICE_H

