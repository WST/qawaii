#include "NotesManager.h"
#include "../storage/StorageDriver.h"
#include "../models/Folder.h"
#include "../models/Note.h"

NotesManager::NotesManager(QObject *parent)
    : QObject(parent)
    , m_driver(nullptr)
{
}

NotesManager::~NotesManager()
{
    qDeleteAll(m_folders);
    qDeleteAll(m_notes);
}

void NotesManager::setStorageDriver(StorageDriver *driver)
{
    m_driver = driver;
}

void NotesManager::loadAll()
{
    if (!m_driver || !m_driver->isConnected()) {
        return;
    }

    qDeleteAll(m_folders);
    qDeleteAll(m_notes);
    m_folders.clear();
    m_notes.clear();

    m_folders = m_driver->getFolders();
    for (Folder *folder : m_folders) {
        QList<Note*> notes = m_driver->getNotes(folder->id());
        m_notes.append(notes);
        for (Note *note : notes) {
            folder->addNote(note);
        }
    }
}

Folder* NotesManager::createFolder(const QString &name)
{
    if (!m_driver || !m_driver->isConnected()) {
        return nullptr;
    }

    Folder *folder = m_driver->createFolder(name);
    if (folder) {
        m_folders.append(folder);
        emit folderCreated(folder);
    }
    return folder;
}

Note* NotesManager::createNote(const QString &folderId, const QString &title)
{
    if (!m_driver || !m_driver->isConnected()) {
        return nullptr;
    }

    Note *note = m_driver->createNote(folderId, title);
    if (note) {
        m_notes.append(note);
        Folder *folder = nullptr;
        for (Folder *f : m_folders) {
            if (f->id() == folderId) {
                folder = f;
                break;
            }
        }
        if (folder) {
            folder->addNote(note);
        }
        emit noteCreated(note);
    }
    return note;
}

bool NotesManager::updateFolder(Folder *folder)
{
    if (!m_driver || !m_driver->isConnected() || !folder) {
        return false;
    }

    bool result = m_driver->updateFolder(folder);
    if (result) {
        emit folderUpdated(folder);
    }
    return result;
}

bool NotesManager::updateNote(Note *note)
{
    if (!m_driver || !m_driver->isConnected() || !note) {
        return false;
    }

    bool result = m_driver->updateNote(note);
    if (result) {
        emit noteUpdated(note);
    }
    return result;
}

bool NotesManager::deleteFolder(const QString &folderId)
{
    if (!m_driver || !m_driver->isConnected()) {
        return false;
    }

    Folder *folder = nullptr;
    for (Folder *f : m_folders) {
        if (f->id() == folderId) {
            folder = f;
            break;
        }
    }

    if (!folder) {
        return false;
    }

    // Удаляем все заметки в папке
    for (Note *note : folder->notes()) {
        m_driver->deleteNote(note->id());
        m_notes.removeOne(note);
        delete note;
    }

    bool result = m_driver->deleteFolder(folderId);
    if (result) {
        m_folders.removeOne(folder);
        emit folderDeleted(folderId);
        delete folder;
    }
    return result;
}

bool NotesManager::deleteNote(const QString &noteId)
{
    if (!m_driver || !m_driver->isConnected()) {
        return false;
    }

    Note *note = nullptr;
    for (Note *n : m_notes) {
        if (n->id() == noteId) {
            note = n;
            break;
        }
    }

    if (!note) {
        return false;
    }

    QString folderId = note->folderId();
    bool result = m_driver->deleteNote(noteId);
    if (result) {
        m_notes.removeOne(note);
        Folder *folder = nullptr;
        for (Folder *f : m_folders) {
            if (f->id() == folderId) {
                folder = f;
                break;
            }
        }
        if (folder) {
            folder->removeNote(note);
        }
        emit noteDeleted(noteId);
        delete note;
    }
    return result;
}

