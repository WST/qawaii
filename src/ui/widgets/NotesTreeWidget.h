#ifndef NOTESTREEWIDGET_H
#define NOTESTREEWIDGET_H

#include <QTreeWidget>

class NotesTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit NotesTreeWidget(QWidget *parent = nullptr);
    ~NotesTreeWidget();

private:
    void setupUI();
    void retranslateUI();
};

#endif // NOTESTREEWIDGET_H

