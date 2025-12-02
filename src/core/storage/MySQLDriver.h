#ifndef MYSQLDRIVER_H
#define MYSQLDRIVER_H

#include "StorageDriver.h"

class MySQLDriver : public StorageDriver
{
    Q_OBJECT

public:
    explicit MySQLDriver(QObject *parent = nullptr);
    ~MySQLDriver() override;

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
    class Private;
    Private *d;
};

#endif // MYSQLDRIVER_H

