#ifndef FOLDER_H
#define FOLDER_H

#include <QString>
#include <QDateTime>
#include <QList>

class Note;

class Folder
{
public:
    Folder();
    Folder(const QString &id, const QString &name);
    ~Folder();

    QString id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString &name) { m_name = name; }

    QString iconName() const { return m_iconName; }
    void setIconName(const QString &iconName) { m_iconName = iconName; }

    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime &dateTime) { m_createdAt = dateTime; }

    QDateTime modifiedAt() const { return m_modifiedAt; }
    void setModifiedAt(const QDateTime &dateTime) { m_modifiedAt = dateTime; }

    QList<Note*> notes() const { return m_notes; }
    void addNote(Note *note);
    void removeNote(Note *note);

private:
    QString m_id;
    QString m_name;
    QString m_iconName;
    QDateTime m_createdAt;
    QDateTime m_modifiedAt;
    QList<Note*> m_notes;
};

#endif // FOLDER_H

