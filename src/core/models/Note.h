#ifndef NOTE_H
#define NOTE_H

#include <QString>
#include <QDateTime>

class Note
{
public:
    Note();
    Note(const QString &id, const QString &folderId, const QString &title);
    ~Note();

    QString id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }

    QString folderId() const { return m_folderId; }
    void setFolderId(const QString &folderId) { m_folderId = folderId; }

    QString title() const { return m_title; }
    void setTitle(const QString &title) { m_title = title; }

    QString content() const { return m_content; }
    void setContent(const QString &content) { m_content = content; }

    QString iconName() const { return m_iconName; }
    void setIconName(const QString &iconName) { m_iconName = iconName; }

    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime &dateTime) { m_createdAt = dateTime; }

    QDateTime modifiedAt() const { return m_modifiedAt; }
    void setModifiedAt(const QDateTime &dateTime) { m_modifiedAt = dateTime; }

    QString version() const { return m_version; }
    void setVersion(const QString &version) { m_version = version; }

private:
    QString m_id;
    QString m_folderId;
    QString m_title;
    QString m_content;
    QString m_iconName;
    QDateTime m_createdAt;
    QDateTime m_modifiedAt;
    QString m_version;
};

#endif // NOTE_H

