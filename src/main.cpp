#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Настройка локализации
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qawaii_" + QLocale(locale).name();
        if (translator.load(":/translations/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // В Qt6 UTF-8 используется по умолчанию
    // В Qt5 можно использовать QTextCodec, но для совместимости оставляем без изменений

    MainWindow window;
    window.show();

    return app.exec();
}

