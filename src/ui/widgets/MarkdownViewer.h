#ifndef MARKDOWNVIEWER_H
#define MARKDOWNVIEWER_H

#include <QWidget>

class MarkdownViewer : public QWidget
{
    Q_OBJECT

public:
    explicit MarkdownViewer(QWidget *parent = nullptr);
    ~MarkdownViewer();
};

#endif // MARKDOWNVIEWER_H

