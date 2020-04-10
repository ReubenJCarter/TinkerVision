#include <QtWidgets/QApplication>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	
	Visi::GUI::MainWindow mainWindow;
    return app.exec();
}