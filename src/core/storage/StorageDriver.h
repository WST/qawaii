#ifndef STORAGEDRIVER_H
#define STORAGEDRIVER_H

#include <QString>
#include <QList>
#include <QVariantMap>
#include <QObject>

class Folder;
class Note;

class StorageDriver : public QObject
{
    Q_OBJECT

public:
    explicit StorageDriver(QObject *parent = nullptr);
    virtual ~StorageDriver() = default;

    // Инициализация и подключение
    virtual bool initialize(const QVariantMap &config) = 0;
    virtual bool connect() = 0;
    virtual void disconnect() = 0;
    virtual bool isConnected() const = 0;

    // Работа с папками
    virtual QList<Folder*> getFolders() = 0;
    virtual Folder* createFolder(const QString &name) = 0;
    virtual bool updateFolder(Folder *folder) = 0;
    virtual bool deleteFolder(const QString &folderId) = 0;

    // Работа с заметками
    virtual QList<Note*> getNotes(const QString &folderId) = 0;
    virtual Note* createNote(const QString &folderId, const QString &title) = 0;
    virtual bool updateNote(Note *note) = 0;
    virtual bool deleteNote(const QString &noteId) = 0;
    virtual Note* getNote(const QString &noteId) = 0;

    // Синхронизация
    virtual bool sync() = 0;

    // Метаданные драйвера
    virtual QString driverName() const = 0;
    virtual QString driverType() const = 0;
    virtual QVariantMap getDefaultConfig() const = 0;
    virtual QStringList getConfigKeys() const = 0;

    // Транзакции
    virtual bool beginTransaction() = 0;
    virtual bool commitTransaction() = 0;
    virtual bool rollbackTransaction() = 0;

protected:
    QVariantMap m_config;
    bool m_connected;
};

#endif // STORAGEDRIVER_H

