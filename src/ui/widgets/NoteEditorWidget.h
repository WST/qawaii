#ifndef NOTEEDITORWIDGET_H
#define NOTEEDITORWIDGET_H

#include <QWidget>
#include <QSplitter>
#include <QTextEdit>
#include <QLabel>

class NoteEditorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NoteEditorWidget(QWidget *parent = nullptr);
    ~NoteEditorWidget();

private:
    void setupUI();
    void retranslateUI();

    QSplitter *m_splitter;
    QTextEdit *m_editor;
    QLabel *m_preview;
};

#endif // NOTEEDITORWIDGET_H

