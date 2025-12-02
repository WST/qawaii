#ifndef SQLITEDRIVER_H
#define SQLITEDRIVER_H

#include "StorageDriver.h"
#include <QSqlDatabase>

class SQLiteDriver : public StorageDriver
{
    Q_OBJECT

public:
    explicit SQLiteDriver(QObject *parent = nullptr);
    ~SQLiteDriver() override;

    bool initialize(const QVariantMap &config) override;
    bool connect() override;
    void disconnect() override;
    bool isConnected() const override;

    QList<Folder*> getFolders() override;
    Folder* createFolder(const QString &name) override;
    bool updateFolder(Folder *folder) override;
    bool deleteFolder(const QString &folderId) override;

    QList<Note*> getNotes(const QString &folderId) override;
    Note* createNote(const QString &folderId, const QString &title) override;
    bool updateNote(Note *note) override;
    bool deleteNote(const QString &noteId) override;
    Note* getNote(const QString &noteId) override;

    bool sync() override;

    QString driverName() const override;
    QString driverType() const override;
    QVariantMap getDefaultConfig() const override;
    QStringList getConfigKeys() const override;

    bool beginTransaction() override;
    bool commitTransaction() override;
    bool rollbackTransaction() override;

private:
    bool createSchema();
    QString getDatabasePath() const;
    QSqlDatabase m_database;
    QString m_connectionName;
};

#endif // SQLITEDRIVER_H

