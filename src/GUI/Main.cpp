#include <QApplication>
#include <QFile>
#include <QTextStream>

#include <iostream> 

#include "MainWindow.h"

int main(int argc, char* argv[])
{
	//create app
	QApplication app(argc, argv);
	
	//Set Style sheet
	QFile f(":qdarkstyle/style.qss");

	if (!f.exists())   
	{
		std::cout << "Unable to set stylesheet, file not found\n";
	}
	else   
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		qApp->setStyleSheet(ts.readAll());
	}

	//create main window
	Viso::GUI::MainWindow mainWindow;
	
	
    return app.exec();
}