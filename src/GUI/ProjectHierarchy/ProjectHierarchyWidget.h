#pragma once

#include <QDockWidget>


class QScrollArea;
class QWidget;
class QVBoxLayout; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
class ProjectHierarchyWidget: public QDockWidget
{
	Q_OBJECT 
	
	private:
		QScrollArea* scrollAreaBase;
		QWidget* widgetBase; 
		QVBoxLayout* layoutBase;
		
	public:
		ProjectHierarchyWidget(); 
};

}	
}
}
