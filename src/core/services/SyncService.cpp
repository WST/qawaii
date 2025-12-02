#include "SyncService.h"
#include "../storage/StorageDriver.h"

SyncService::SyncService(QObject *parent)
    : QObject(parent)
    , m_driver(nullptr)
{
}

SyncService::~SyncService()
{
}

void SyncService::setStorageDriver(StorageDriver *driver)
{
    m_driver = driver;
}

bool SyncService::sync()
{
    if (!m_driver || !m_driver->isConnected()) {
        emit syncFinished(false);
        return false;
    }

    emit syncStarted();
    bool result = m_driver->sync();
    emit syncFinished(result);
    return result;
}

