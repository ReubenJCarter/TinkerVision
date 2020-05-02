#include "MainWindow.h"

#include "NodeEditor/NodeEditorWidget.h"
#include "ProjectHierarchy/ProjectHierarchyWidget.h"
#include "Core/SerializedObject.h"

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QMenuBar>
#include <QFileDialog>

#include <iostream>
#include <fstream>

namespace Visi
{
namespace GUI
{
	
MainWindow::MainWindow()
{
	setWindowTitle("Visi GUI");
	
	//Set the Top bar 
	fileMenu = menuBar()->addMenu("File");
	saveAction = fileMenu->addAction("Save Project");
	loadAction = fileMenu->addAction("Load Project");
	newProjectAction = fileMenu->addAction("New Project");
	connect(saveAction, &QAction::triggered, this, &MainWindow::Save);
	connect(loadAction, &QAction::triggered, this, &MainWindow::Load);
	connect(newProjectAction, &QAction::triggered, this, &MainWindow::NewProject);
	
	//central
	nodeEditorWidegt = new NodeEditor::NodeEditorWidget(); 
	setCentralWidget(nodeEditorWidegt);

	//dockables
	projectHierarchyWidget = new ProjectHierarchy::ProjectHierarchyWidget(nodeEditorWidegt); 
	addDockWidget(Qt::LeftDockWidgetArea, projectHierarchyWidget);

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

	SerializedObject serializedObject; 
	projectHierarchyWidget->Serialize(&serializedObject); 
	
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Project"), "", tr("vis (*.vis)"));
	if(fileName.length() > 0)
	{
		std::ofstream fs(fileName.toStdString().c_str());
		if(fs.is_open())
		{
			fs << serializedObject.ToString(); 
			fs.close();
		}
	}
}

void MainWindow::Load()
{
	std::cout << "Load" << std::endl;
	
	SerializedObject serializedObject; 
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("vis (*.vis)"));
	if(fileName.length() > 0)
	{
		std::ifstream fs(fileName.toStdString().c_str());
		if(fs.is_open())
		{
			//get length of file
			fs.seekg(0, std::ios::end);
			size_t length = fs.tellg();
			fs.seekg(0, std::ios::beg);
			char* buffer = new char[length];
			fs.read(&buffer[0], length);
			fs.close();

			std::string fStr(buffer, length); 

			serializedObject.FromString(fStr); 
			projectHierarchyWidget->Deserialize(&serializedObject); 
			
			delete[] buffer; 
		}
	}


	
}

void MainWindow::NewProject()
{
	projectHierarchyWidget->StartNewProject(); 
}

}
}