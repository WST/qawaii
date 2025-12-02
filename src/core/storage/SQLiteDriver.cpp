#include "SQLiteDriver.h"

SQLiteDriver::SQLiteDriver(QObject *parent)
    : StorageDriver(parent)
{
    // TODO: Реализация
}

SQLiteDriver::~SQLiteDriver()
{
    disconnect();
}

bool SQLiteDriver::initialize(const QVariantMap &config)
{
    m_config = config;
    return true;
}

bool SQLiteDriver::connect()
{
    // TODO: Реализация подключения к SQLite
    m_connected = false;
    return false;
}

void SQLiteDriver::disconnect()
{
    m_connected = false;
}

bool SQLiteDriver::isConnected() const
{
    return m_connected;
}

QList<Folder*> SQLiteDriver::getFolders()
{
    return QList<Folder*>();
}

Folder* SQLiteDriver::createFolder(const QString &name)
{
    Q_UNUSED(name);
    return nullptr;
}

bool SQLiteDriver::updateFolder(Folder *folder)
{
    Q_UNUSED(folder);
    return false;
}

bool SQLiteDriver::deleteFolder(const QString &folderId)
{
    Q_UNUSED(folderId);
    return false;
}

QList<Note*> SQLiteDriver::getNotes(const QString &folderId)
{
    Q_UNUSED(folderId);
    return QList<Note*>();
}

Note* SQLiteDriver::createNote(const QString &folderId, const QString &title)
{
    Q_UNUSED(folderId);
    Q_UNUSED(title);
    return nullptr;
}

bool SQLiteDriver::updateNote(Note *note)
{
    Q_UNUSED(note);
    return false;
}

bool SQLiteDriver::deleteNote(const QString &noteId)
{
    Q_UNUSED(noteId);
    return false;
}

Note* SQLiteDriver::getNote(const QString &noteId)
{
    Q_UNUSED(noteId);
    return nullptr;
}

bool SQLiteDriver::sync()
{
    return true;
}

QString SQLiteDriver::driverName() const
{
    return "SQLite";
}

QString SQLiteDriver::driverType() const
{
    return "local";
}

QVariantMap SQLiteDriver::getDefaultConfig() const
{
    QVariantMap config;
    config["database_path"] = "qawaii.db";
    return config;
}

QStringList SQLiteDriver::getConfigKeys() const
{
    return QStringList() << "database_path";
}

bool SQLiteDriver::beginTransaction()
{
    return false;
}

bool SQLiteDriver::commitTransaction()
{
    return false;
}

bool SQLiteDriver::rollbackTransaction()
{
    return false;
}

