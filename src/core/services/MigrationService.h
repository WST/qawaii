#ifndef MIGRATIONSERVICE_H
#define MIGRATIONSERVICE_H

#include <QObject>

class StorageDriver;

class MigrationService : public QObject
{
    Q_OBJECT

public:
    explicit MigrationService(QObject *parent = nullptr);
    ~MigrationService();

    bool migrate(StorageDriver *fromDriver, StorageDriver *toDriver);

signals:
    void migrationStarted();
    void migrationProgress(int percent);
    void migrationFinished(bool success);
};

#endif // MIGRATIONSERVICE_H

