#ifndef ICONSELECTIONDIALOG_H
#define ICONSELECTIONDIALOG_H

#include <QDialog>

class IconSelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit IconSelectionDialog(QWidget *parent = nullptr);
    ~IconSelectionDialog();
};

#endif // ICONSELECTIONDIALOG_H

