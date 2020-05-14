#pragma once

#include <QDockWidget>

class QScrollArea;
class QLabel; 
class QWidget;
class QVBoxLayout; 
class QHBoxLayout; 
class QListView;
class QPushButton; 

namespace TnkrVis
{
	
class SerializedObject; 

namespace GUI
{

namespace NodeEditor
{
class NodeEditorWidget; 
}

namespace ProjectHierarchy
{

class HierarchyListModel; 
class AddGraphDialog; 
class DeleteGraphDialog;
class YesNoDialog; 
	
class ProjectHierarchyWidget: public QDockWidget
{
	Q_OBJECT 
	
	private:
		QScrollArea* scrollAreaBase;
		QWidget* widgetBase; 
		QVBoxLayout* layoutBase;

		QPushButton* addGraphButton; 
		QPushButton* deleteGraphButton; 
		QPushButton* renameGraphButton; 
		QListView* hierarchyList; 
		HierarchyListModel* hierarchyListModel; 

		QHBoxLayout* buttonLayout; 
		AddGraphDialog* addGraphDialog;
		DeleteGraphDialog* deleteGraphDialog;
		YesNoDialog* startNewProjectDialog; 

		NodeEditor::NodeEditorWidget* nodeEditorWidget; 

		QString openGraphName; 
		
		void SaveCurrentNodeEditor(); 
		void LoadNodeEditorFromIndex(int inx); 

	public:
		ProjectHierarchyWidget(NodeEditor::NodeEditorWidget* ne); 

		void Serialize(SerializedObject* so); 
		void Deserialize(SerializedObject* so); 
		void StartNewProject(); 
};

}	
}
}
