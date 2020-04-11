#pragma once

#include <QtWidgets/QMainWindow>

class QMenu;
class QAction; 

namespace Visi
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

class MainWindow: public QMainWindow
{
	Q_OBJECT
	public:
		QMenu* fileMenu;
		QAction* saveAction;
		QAction* loadAction;
		NodeEditor::NodeEditorWidget* nodeEditorWidegt; 
		ProjectHierarchy::ProjectHierarchyWidget* projectHierarchyWidget; 
		
		MainWindow();
		void ExpandToFitScreen();
		
	public slots:
		void Save();
		void Load();
};

}
}