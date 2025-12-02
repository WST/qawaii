#include "StorageDriver.h"

StorageDriver::StorageDriver(QObject *parent)
    : QObject(parent)
    , m_connected(false)
{
}

