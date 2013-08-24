#include <QApplication>
#include "qawaii.h"

int main(int argc, char **argv) {
	QApplication app(argc, argv);
	app.setApplicationName("qawaii");
	Qawaii win;
	win.show();
	app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
	return app.exec();
}
