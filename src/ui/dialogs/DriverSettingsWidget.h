#ifndef DRIVERSETTINGSWIDGET_H
#define DRIVERSETTINGSWIDGET_H

#include <QWidget>

class DriverSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DriverSettingsWidget(QWidget *parent = nullptr);
    ~DriverSettingsWidget();
};

#endif // DRIVERSETTINGSWIDGET_H

