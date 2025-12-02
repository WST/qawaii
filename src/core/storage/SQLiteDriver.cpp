#include "SQLiteDriver.h"
#include "../models/Folder.h"
#include "../models/Note.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QUuid>
#include <QDateTime>

SQLiteDriver::SQLiteDriver(QObject *parent)
    : StorageDriver(parent)
    , m_connectionName("qawaii_sqlite_" + QString::number(reinterpret_cast<quintptr>(this)))
{
}

SQLiteDriver::~SQLiteDriver()
{
    disconnect();
}

bool SQLiteDriver::initialize(const QVariantMap &config)
{
    m_config = config;
    
    // Если путь не указан, используем стандартный путь
    if (!m_config.contains("database_path") || m_config["database_path"].toString().isEmpty()) {
        m_config["database_path"] = getDatabasePath();
    }
    
    return true;
}

QString SQLiteDriver::getDatabasePath() const
{
    // Используем QStandardPaths для получения каноничного пути
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    
    // Создаем директорию, если её нет
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
    
    return QDir(configDir).filePath("notes.db");
}

bool SQLiteDriver::connect()
{
    if (m_connected) {
        return true;
    }

    // Закрываем предыдущее подключение, если есть
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }

    QString dbPath = m_config["database_path"].toString();
    if (dbPath.isEmpty()) {
        dbPath = getDatabasePath();
        m_config["database_path"] = dbPath;
    }

    m_database = QSqlDatabase::addDatabase("QSQLITE", m_connectionName);
    m_database.setDatabaseName(dbPath);

    if (!m_database.open()) {
        qWarning() << "Не удалось открыть базу данных:" << m_database.lastError().text();
        m_connected = false;
        return false;
    }

    // Включаем поддержку внешних ключей в SQLite
    QSqlQuery query(m_database);
    query.exec("PRAGMA foreign_keys = ON");

    // Создаем схему, если её нет
    if (!createSchema()) {
        qWarning() << "Не удалось создать схему базы данных";
        m_database.close();
        m_connected = false;
        return false;
    }

    m_connected = true;
    return true;
}

void SQLiteDriver::disconnect()
{
    if (m_connected && m_database.isOpen()) {
        m_database.close();
    }
    if (QSqlDatabase::contains(m_connectionName)) {
        QSqlDatabase::removeDatabase(m_connectionName);
    }
    m_connected = false;
}

bool SQLiteDriver::isConnected() const
{
    return m_connected && m_database.isOpen();
}

bool SQLiteDriver::createSchema()
{
    QSqlQuery query(m_database);

    // Создаем таблицу folders
    QString createFoldersTable = R"(
        CREATE TABLE IF NOT EXISTS folders (
            id TEXT PRIMARY KEY,
            name TEXT NOT NULL,
            icon_name TEXT DEFAULT 'folder',
            created_at TEXT NOT NULL,
            modified_at TEXT NOT NULL
        )
    )";

    if (!query.exec(createFoldersTable)) {
        qWarning() << "Ошибка создания таблицы folders:" << query.lastError().text();
        return false;
    }

    // Создаем таблицу notes
    QString createNotesTable = R"(
        CREATE TABLE IF NOT EXISTS notes (
            id TEXT PRIMARY KEY,
            folder_id TEXT NOT NULL,
            title TEXT NOT NULL,
            content TEXT DEFAULT '',
            icon_name TEXT DEFAULT 'note',
            created_at TEXT NOT NULL,
            modified_at TEXT NOT NULL,
            version TEXT DEFAULT '1',
            FOREIGN KEY (folder_id) REFERENCES folders(id) ON DELETE CASCADE
        )
    )";

    if (!query.exec(createNotesTable)) {
        qWarning() << "Ошибка создания таблицы notes:" << query.lastError().text();
        return false;
    }

    // Создаем индексы для ускорения запросов
    query.exec("CREATE INDEX IF NOT EXISTS idx_notes_folder_id ON notes(folder_id)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_folders_modified ON folders(modified_at)");
    query.exec("CREATE INDEX IF NOT EXISTS idx_notes_modified ON notes(modified_at)");

    return true;
}

QList<Folder*> SQLiteDriver::getFolders()
{
    QList<Folder*> folders;

    if (!isConnected()) {
        return folders;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT id, name, icon_name, created_at, modified_at FROM folders ORDER BY name");
    
    if (!query.exec()) {
        qWarning() << "Ошибка получения папок:" << query.lastError().text();
        return folders;
    }

    while (query.next()) {
        Folder *folder = new Folder();
        folder->setId(query.value(0).toString());
        folder->setName(query.value(1).toString());
        folder->setIconName(query.value(2).toString());
        folder->setCreatedAt(QDateTime::fromString(query.value(3).toString(), Qt::ISODate));
        folder->setModifiedAt(QDateTime::fromString(query.value(4).toString(), Qt::ISODate));
        folders.append(folder);
    }

    return folders;
}

Folder* SQLiteDriver::createFolder(const QString &name)
{
    if (!isConnected() || name.isEmpty()) {
        return nullptr;
    }

    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QDateTime now = QDateTime::currentDateTime();

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO folders (id, name, icon_name, created_at, modified_at) "
                  "VALUES (:id, :name, :icon_name, :created_at, :modified_at)");
    query.bindValue(":id", id);
    query.bindValue(":name", name);
    query.bindValue(":icon_name", "folder");
    query.bindValue(":created_at", now.toString(Qt::ISODate));
    query.bindValue(":modified_at", now.toString(Qt::ISODate));

    if (!query.exec()) {
        qWarning() << "Ошибка создания папки:" << query.lastError().text();
        return nullptr;
    }

    Folder *folder = new Folder(id, name);
    folder->setCreatedAt(now);
    folder->setModifiedAt(now);
    return folder;
}

bool SQLiteDriver::updateFolder(Folder *folder)
{
    if (!isConnected() || !folder || folder->id().isEmpty()) {
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    folder->setModifiedAt(now);

    QSqlQuery query(m_database);
    query.prepare("UPDATE folders SET name = :name, icon_name = :icon_name, modified_at = :modified_at "
                  "WHERE id = :id");
    query.bindValue(":name", folder->name());
    query.bindValue(":icon_name", folder->iconName());
    query.bindValue(":modified_at", folder->modifiedAt().toString(Qt::ISODate));
    query.bindValue(":id", folder->id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления папки:" << query.lastError().text();
        return false;
    }

    return true;
}

bool SQLiteDriver::deleteFolder(const QString &folderId)
{
    if (!isConnected() || folderId.isEmpty()) {
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM folders WHERE id = :id");
    query.bindValue(":id", folderId);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления папки:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

QList<Note*> SQLiteDriver::getNotes(const QString &folderId)
{
    QList<Note*> notes;

    if (!isConnected() || folderId.isEmpty()) {
        return notes;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT id, folder_id, title, content, icon_name, created_at, modified_at, version "
                  "FROM notes WHERE folder_id = :folder_id ORDER BY modified_at DESC");
    query.bindValue(":folder_id", folderId);

    if (!query.exec()) {
        qWarning() << "Ошибка получения заметок:" << query.lastError().text();
        return notes;
    }

    while (query.next()) {
        Note *note = new Note();
        note->setId(query.value(0).toString());
        note->setFolderId(query.value(1).toString());
        note->setTitle(query.value(2).toString());
        note->setContent(query.value(3).toString());
        note->setIconName(query.value(4).toString());
        note->setCreatedAt(QDateTime::fromString(query.value(5).toString(), Qt::ISODate));
        note->setModifiedAt(QDateTime::fromString(query.value(6).toString(), Qt::ISODate));
        note->setVersion(query.value(7).toString());
        notes.append(note);
    }

    return notes;
}

Note* SQLiteDriver::createNote(const QString &folderId, const QString &title)
{
    if (!isConnected() || folderId.isEmpty() || title.isEmpty()) {
        return nullptr;
    }

    QString id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    QDateTime now = QDateTime::currentDateTime();

    QSqlQuery query(m_database);
    query.prepare("INSERT INTO notes (id, folder_id, title, content, icon_name, created_at, modified_at, version) "
                  "VALUES (:id, :folder_id, :title, :content, :icon_name, :created_at, :modified_at, :version)");
    query.bindValue(":id", id);
    query.bindValue(":folder_id", folderId);
    query.bindValue(":title", title);
    query.bindValue(":content", "");
    query.bindValue(":icon_name", "note");
    query.bindValue(":created_at", now.toString(Qt::ISODate));
    query.bindValue(":modified_at", now.toString(Qt::ISODate));
    query.bindValue(":version", "1");

    if (!query.exec()) {
        qWarning() << "Ошибка создания заметки:" << query.lastError().text();
        return nullptr;
    }

    Note *note = new Note(id, folderId, title);
    note->setCreatedAt(now);
    note->setModifiedAt(now);
    return note;
}

bool SQLiteDriver::updateNote(Note *note)
{
    if (!isConnected() || !note || note->id().isEmpty()) {
        return false;
    }

    QDateTime now = QDateTime::currentDateTime();
    note->setModifiedAt(now);

    // Увеличиваем версию при обновлении
    QString newVersion = QString::number(note->version().toInt() + 1);
    note->setVersion(newVersion);

    QSqlQuery query(m_database);
    query.prepare("UPDATE notes SET folder_id = :folder_id, title = :title, content = :content, "
                  "icon_name = :icon_name, modified_at = :modified_at, version = :version "
                  "WHERE id = :id");
    query.bindValue(":folder_id", note->folderId());
    query.bindValue(":title", note->title());
    query.bindValue(":content", note->content());
    query.bindValue(":icon_name", note->iconName());
    query.bindValue(":modified_at", note->modifiedAt().toString(Qt::ISODate));
    query.bindValue(":version", note->version());
    query.bindValue(":id", note->id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления заметки:" << query.lastError().text();
        return false;
    }

    return true;
}

bool SQLiteDriver::deleteNote(const QString &noteId)
{
    if (!isConnected() || noteId.isEmpty()) {
        return false;
    }

    QSqlQuery query(m_database);
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", noteId);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления заметки:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

Note* SQLiteDriver::getNote(const QString &noteId)
{
    if (!isConnected() || noteId.isEmpty()) {
        return nullptr;
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT id, folder_id, title, content, icon_name, created_at, modified_at, version "
                  "FROM notes WHERE id = :id");
    query.bindValue(":id", noteId);

    if (!query.exec() || !query.next()) {
        return nullptr;
    }

    Note *note = new Note();
    note->setId(query.value(0).toString());
    note->setFolderId(query.value(1).toString());
    note->setTitle(query.value(2).toString());
    note->setContent(query.value(3).toString());
    note->setIconName(query.value(4).toString());
    note->setCreatedAt(QDateTime::fromString(query.value(5).toString(), Qt::ISODate));
    note->setModifiedAt(QDateTime::fromString(query.value(6).toString(), Qt::ISODate));
    note->setVersion(query.value(7).toString());

    return note;
}

bool SQLiteDriver::sync()
{
    // Для локального SQLite синхронизация не требуется
    // Все изменения уже сохранены в БД
    return isConnected();
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
    // Используем стандартный путь по умолчанию
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
    QDir dir;
    if (!dir.exists(configDir)) {
        dir.mkpath(configDir);
    }
    config["database_path"] = QDir(configDir).filePath("notes.db");
    return config;
}

QStringList SQLiteDriver::getConfigKeys() const
{
    return QStringList() << "database_path";
}

bool SQLiteDriver::beginTransaction()
{
    if (!isConnected()) {
        return false;
    }
    return m_database.transaction();
}

bool SQLiteDriver::commitTransaction()
{
    if (!isConnected()) {
        return false;
    }
    return m_database.commit();
}

bool SQLiteDriver::rollbackTransaction()
{
    if (!isConnected()) {
        return false;
    }
    return m_database.rollback();
}
