#include "Folder.h"
#include "Note.h"

Folder::Folder()
    : m_iconName("folder")
{
    m_createdAt = QDateTime::currentDateTime();
    m_modifiedAt = m_createdAt;
}

Folder::Folder(const QString &id, const QString &name)
    : m_id(id)
    , m_name(name)
    , m_iconName("folder")
{
    m_createdAt = QDateTime::currentDateTime();
    m_modifiedAt = m_createdAt;
}

Folder::~Folder()
{
    qDeleteAll(m_notes);
}

void Folder::addNote(Note *note)
{
    if (note && !m_notes.contains(note)) {
        m_notes.append(note);
        m_modifiedAt = QDateTime::currentDateTime();
    }
}

void Folder::removeNote(Note *note)
{
    if (m_notes.removeOne(note)) {
        m_modifiedAt = QDateTime::currentDateTime();
    }
}

