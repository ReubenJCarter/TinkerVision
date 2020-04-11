#include "MainWindow.h"

#include "NodeEditor/NodeEditorWidget.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QMenuBar>

#include <iostream>

namespace Visi
{
namespace GUI
{
	
MainWindow::MainWindow()
{
	setWindowTitle("Visi GUI");
	
	//Set the Top bar 
	fileMenu = menuBar()->addMenu("File");
	saveAction = fileMenu->addAction("Save");
	loadAction = fileMenu->addAction("Load");
	connect(saveAction, &QAction::triggered, this, &MainWindow::Save);
	connect(loadAction, &QAction::triggered, this, &MainWindow::Load);
	
	//central
	nodeEditor = new NodeEditor(); 
	setCentralWidget(nodeEditor);

	//dockables

	
	//Show
	showNormal();
	ExpandToFitScreen();
}

void MainWindow::ExpandToFitScreen()
{
	//Expand the window to fill the avalible screen space
	QRect rec = QApplication::desktop()->availableGeometry(this);
	int differenceH = frameGeometry().height() - geometry().height();
	int differenceW = frameGeometry().width() - geometry().width();
	int height = rec.height() - differenceH;
	int width = rec.width() - differenceW;
	resize(width *  0.75, height * 0.75);
	move(rec.topLeft().x(), rec.topLeft().y());	
}

void MainWindow::Save()
{
	std::cout << "Save" << std::endl;
}

void MainWindow::Load()
{
	std::cout << "Load" << std::endl;
}

}
}