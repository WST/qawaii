#ifndef NOTESMANAGER_H
#define NOTESMANAGER_H

#include <QObject>
#include <QList>

class Folder;
class Note;
class StorageDriver;

class NotesManager : public QObject
{
    Q_OBJECT

public:
    explicit NotesManager(QObject *parent = nullptr);
    ~NotesManager();

    void setStorageDriver(StorageDriver *driver);
    StorageDriver* storageDriver() const { return m_driver; }

    QList<Folder*> folders() const { return m_folders; }
    QList<Note*> notes() const { return m_notes; }

public slots:
    void loadAll();
    Folder* createFolder(const QString &name);
    Note* createNote(const QString &folderId, const QString &title);
    bool updateFolder(Folder *folder);
    bool updateNote(Note *note);
    bool deleteFolder(const QString &folderId);
    bool deleteNote(const QString &noteId);

signals:
    void folderCreated(Folder *folder);
    void folderUpdated(Folder *folder);
    void folderDeleted(const QString &folderId);
    void noteCreated(Note *note);
    void noteUpdated(Note *note);
    void noteDeleted(const QString &noteId);

private:
    StorageDriver *m_driver;
    QList<Folder*> m_folders;
    QList<Note*> m_notes;
};

#endif // NOTESMANAGER_H

