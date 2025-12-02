#include "NotesTreeWidget.h"

NotesTreeWidget::NotesTreeWidget(QWidget *parent)
    : QTreeWidget(parent)
{
    setupUI();
    retranslateUI();
}

NotesTreeWidget::~NotesTreeWidget()
{
}

void NotesTreeWidget::setupUI()
{
    setHeaderLabel(tr("Заметки"));
    setRootIsDecorated(true);
    setAlternatingRowColors(true);
}

void NotesTreeWidget::retranslateUI()
{
    setHeaderLabel(tr("Заметки"));
}

