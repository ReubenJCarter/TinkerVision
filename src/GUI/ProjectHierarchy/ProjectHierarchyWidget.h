#pragma once

#include <QDockWidget>
#include <QListView>


class QScrollArea;
class QWidget;
class QVBoxLayout; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{

class HierarchyListModel; 
	
class ProjectHierarchyWidget: public QDockWidget
{
	Q_OBJECT 
	
	private:
		QScrollArea* scrollAreaBase;
		QWidget* widgetBase; 
		QVBoxLayout* layoutBase;
		QListView* hierarchyList; 
		HierarchyListModel* hierarchyListModel; 
		
	public:
		ProjectHierarchyWidget(); 
};

}	
}
}
