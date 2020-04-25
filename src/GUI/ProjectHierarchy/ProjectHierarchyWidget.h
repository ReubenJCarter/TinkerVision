#pragma once

#include <QDockWidget>
#include <QListView>
#include <QPushButton>


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

		QPushButton* pushButton; 
		QListView* hierarchyList; 
		HierarchyListModel* hierarchyListModel; 
		
	public:
		ProjectHierarchyWidget(); 
};

}	
}
}
