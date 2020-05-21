#include "BuildEditorWidget.h"
#include "../NodeEditor/NodeEditorWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QProcess>
#include <QTextEdit>

#include <iostream>
#include <fstream>

namespace TnkrVis
{
namespace GUI
{
namespace BuildEditor
{
	
BuildEditorWidget::BuildEditorWidget(NodeEditor::NodeEditorWidget* ne, ProjectHierarchy::ProjectHierarchyWidget* phw)
{
	nodeEditorWidget = ne;
	projectHierarchyWidget = phw; 

    //Set up panel
    setWindowTitle("Build And Run"); 
    setMinimumWidth(250);
    setFeatures(QDockWidget::DockWidgetMovable); 
    scrollAreaBase = new QScrollArea();
	widgetBase = new QWidget();
	layoutBase = new QVBoxLayout();
	widgetBase->setLayout(layoutBase);
	scrollAreaBase->setWidget(widgetBase);
	scrollAreaBase->setWidgetResizable(true);
	setWidget(scrollAreaBase);
	layoutBase->setAlignment(Qt::AlignTop);

	//Add compile text box and button
	compileButton = new QPushButton("Compile"); 
	runButton = new QPushButton("Run"); 
	compileRunButtonLayout = new QHBoxLayout; 
	compileRunButtonLayout->addWidget(compileButton); 
	compileRunButtonLayout->addWidget(runButton); 
	layoutBase->addLayout(compileRunButtonLayout); 

	compileJsonTextScrollArea = new QScrollArea();
	compileJsonTextScrollArea->setFixedHeight(80); 
	compileJsonText = new QLabel(); 
	compileJsonText->setSizePolicy(QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );
	compileJsonText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	compileJsonTextScrollArea->setWidget(compileJsonText);
	layoutBase->addWidget(compileJsonTextScrollArea); 

	//stdOutputTextScrollArea = new QScrollArea();
	stdOutputText = new QTextEdit(); 
	stdOutputText->setSizePolicy(QSizePolicy::MinimumExpanding , QSizePolicy::MinimumExpanding );
	stdOutputText->setTextInteractionFlags(Qt::TextSelectableByMouse);
	stdOutputText->setReadOnly(true);
	//stdOutputTextScrollArea->setWidget(stdOutputText);
	layoutBase->addWidget(stdOutputText); 
	
	tinkerVisionProcess = NULL; 

	//Connections
	connect(compileButton, &QPushButton::clicked, [this](){

		nodeEditorWidget->SerializeToComputeGraph(&serializedComputeGraph);
		std::string str = serializedComputeGraph.ToString(); 
		compileJsonText->setText( QString::fromStdString(str) ); 
		compileJsonText->adjustSize(); 
	});

	connect(runButton, &QPushButton::clicked, [this](){
		
		if(tinkerVisionProcess!=NULL)
		{
			//Stop the process
			runButton->setText("Run"); 
			delete tinkerVisionProcess;
			tinkerVisionProcess = NULL; 
		}
		else
		{
			//compile the graph
			nodeEditorWidget->SerializeToComputeGraph(&serializedComputeGraph);
			std::string str = serializedComputeGraph.ToString(); 
			compileJsonText->setText( QString::fromStdString(str) ); 
			compileJsonText->adjustSize(); 

			//write the jsonb into a temp file
			std::string tempJsonFile = std::tmpnam(nullptr);
			std::ofstream fs;
			fs.open(tempJsonFile.c_str(), std::ofstream::binary);
			if(fs.is_open())
			{
				//Write json into file
				fs.write(str.c_str(), sizeof(char)*str.size()); 

				//Close the file 
				fs.close(); 

				//Setup the process
				runButton->setText("Kill"); 
				QStringList args; 
				args << QString(tempJsonFile.c_str()); 
				tinkerVisionProcess = new QProcess(this);
				
				//Connect up std output
				stdOutputText->clear(); 
				connect(tinkerVisionProcess, &QProcess::readyReadStandardOutput, [this](){
					stdOutputText->append(QString(tinkerVisionProcess->readAllStandardOutput())); 
				}); 
				connect(tinkerVisionProcess, &QProcess::readyReadStandardError, [this](){
					stdOutputText->append(QString(tinkerVisionProcess->readAllStandardOutput())); 
				}); 
				connect(tinkerVisionProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [this](int exitCode, QProcess::ExitStatus exitStatus){
					
					//kill the process
					runButton->setText("Run"); 
					if(tinkerVisionProcess != NULL)
					delete tinkerVisionProcess;
					tinkerVisionProcess = NULL; 

					//delete the temp file
					
				});

				//Run
				std::cout << "Starting vision process \n" << "tempfile:  " << tempJsonFile  << "\n"; 
				tinkerVisionProcess->start("TinkerVision_CMD.exe", args); 
			}
		}
	});
}

}	
}
}