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

	//button
	pushButton = new QPushButton("Add Graph");
	layoutBase->addWidget(pushButton); 

    //List
	hierarchyList = new QListView(); 

	hierarchyList->setSelectionMode(QAbstractItemView::SingleSelection); //SingleSelection
	//hierarchyList->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//hierarchyList->setDefaultDropAction(Qt::TargetMoveAction);


	hierarchyList->setDragEnabled(true);
	hierarchyList->setAcceptDrops(true);
	hierarchyList->setDropIndicatorShown(true);
	//hierarchyList->setMovement(QListView::Free);

	hierarchyListModel = new HierarchyListModel(); 
	hierarchyList->setModel(hierarchyListModel);
	layoutBase->addWidget(hierarchyList); 
	
	//Connections
	connect(pushButton, &QPushButton::clicked, [this](bool checked)
	{
		hierarchyListModel->AddNew("new graph"); 
	});
}

}	
}
}