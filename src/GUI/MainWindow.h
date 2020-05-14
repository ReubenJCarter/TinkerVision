#pragma once

#include <QtWidgets/QMainWindow>

class QMenu;
class QAction; 

namespace TnkrVis
{
namespace GUI
{


namespace NodeEditor
{
class NodeEditorWidget;
}

namespace ProjectHierarchy
{
class ProjectHierarchyWidget; 
}

namespace BuildEditor
{
class BuildEditorWidget; 
}

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		QMenu* fileMenu;
		QAction* saveAction;
		QAction* loadAction;
		QAction* newProjectAction; 
		NodeEditor::NodeEditorWidget* nodeEditorWidegt; 
		ProjectHierarchy::ProjectHierarchyWidget* projectHierarchyWidget; 
		BuildEditor::BuildEditorWidget* buildEditorWidget; 
		
		MainWindow();
		void ExpandToFitScreen();
		
	public slots:
		void Save();
		void Load();
		void NewProject(); 
};

}
}