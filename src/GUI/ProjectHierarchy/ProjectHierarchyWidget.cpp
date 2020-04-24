#include "ProjectHierarchyWidget.h"

#include "HierarchyListModel.h"

#include <QVBoxLayout>
#include <QWidget>
#include <QScrollArea>

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
ProjectHierarchyWidget::ProjectHierarchyWidget()
{
    //Set up panel
    setWindowTitle("Project Hierarchy"); 
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

    //
	hierarchyList = new QListView(); 
	hierarchyListModel = new HierarchyListModel(); 
	hierarchyList->setModel(hierarchyListModel);
	layoutBase->addWidget(hierarchyList); 
	

}

}	
}
}