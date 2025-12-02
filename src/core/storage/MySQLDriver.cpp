#include "MySQLDriver.h"

MySQLDriver::MySQLDriver(QObject *parent)
    : StorageDriver(parent)
{
    // TODO: Реализация
}

MySQLDriver::~MySQLDriver()
{
    disconnect();
}

bool MySQLDriver::initialize(const QVariantMap &config)
{
    m_config = config;
    return true;
}

bool MySQLDriver::connect()
{
    // TODO: Реализация подключения к MySQL
    m_connected = false;
    return false;
}

void MySQLDriver::disconnect()
{
    m_connected = false;
}

bool MySQLDriver::isConnected() const
{
    return m_connected;
}

QList<Folder*> MySQLDriver::getFolders()
{
    return QList<Folder*>();
}

Folder* MySQLDriver::createFolder(const QString &name)
{
    Q_UNUSED(name);
    return nullptr;
}

bool MySQLDriver::updateFolder(Folder *folder)
{
    Q_UNUSED(folder);
    return false;
}

bool MySQLDriver::deleteFolder(const QString &folderId)
{
    Q_UNUSED(folderId);
    return false;
}

QList<Note*> MySQLDriver::getNotes(const QString &folderId)
{
    Q_UNUSED(folderId);
    return QList<Note*>();
}

Note* MySQLDriver::createNote(const QString &folderId, const QString &title)
{
    Q_UNUSED(folderId);
    Q_UNUSED(title);
    return nullptr;
}

bool MySQLDriver::updateNote(Note *note)
{
    Q_UNUSED(note);
    return false;
}

bool MySQLDriver::deleteNote(const QString &noteId)
{
    Q_UNUSED(noteId);
    return false;
}

Note* MySQLDriver::getNote(const QString &noteId)
{
    Q_UNUSED(noteId);
    return nullptr;
}

bool MySQLDriver::sync()
{
    return true;
}

QString MySQLDriver::driverName() const
{
    return "MySQL";
}

QString MySQLDriver::driverType() const
{
    return "network";
}

QVariantMap MySQLDriver::getDefaultConfig() const
{
    QVariantMap config;
    config["host"] = "localhost";
    config["port"] = 3306;
    config["username"] = "";
    config["password"] = "";
    config["database_name"] = "qawaii";
    return config;
}

QStringList MySQLDriver::getConfigKeys() const
{
    return QStringList() << "host" << "port" << "username" << "password" << "database_name";
}

bool MySQLDriver::beginTransaction()
{
    return false;
}

bool MySQLDriver::commitTransaction()
{
    return false;
}

bool MySQLDriver::rollbackTransaction()
{
    return false;
}

