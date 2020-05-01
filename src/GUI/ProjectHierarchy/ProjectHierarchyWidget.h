#pragma once

#include <QDockWidget>

class QScrollArea;
class QWidget;
class QVBoxLayout; 
class QHBoxLayout; 
class QListView;
class QPushButton; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{

class HierarchyListModel; 
class AddGraphDialog; 
class DeleteGraphDialog;
	
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
		
	public:
		ProjectHierarchyWidget(); 
};

}	
}
}
