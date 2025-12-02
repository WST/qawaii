#include "Note.h"

Note::Note()
    : m_iconName("note")
{
    m_createdAt = QDateTime::currentDateTime();
    m_modifiedAt = m_createdAt;
    m_version = "1";
}

Note::Note(const QString &id, const QString &folderId, const QString &title)
    : m_id(id)
    , m_folderId(folderId)
    , m_title(title)
    , m_iconName("note")
{
    m_createdAt = QDateTime::currentDateTime();
    m_modifiedAt = m_createdAt;
    m_version = "1";
}

Note::~Note()
{
}

