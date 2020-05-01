#include "ProjectHierarchyWidget.h"

#include "HierarchyListModel.h"
#include "AddGraphDialog.h"
#include "DeleteGraphDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QListView>
#include <QPushButton>

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

	//buttons
	buttonLayout = new QHBoxLayout(); 
	layoutBase->addLayout(buttonLayout); 
	addGraphButton = new QPushButton("Add");
	buttonLayout->addWidget(addGraphButton); 
	deleteGraphButton = new QPushButton("Delete"); 
	buttonLayout->addWidget(deleteGraphButton); 

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
	
	//Add dialog
	addGraphDialog = new AddGraphDialog(this); 
	
	//delete dialog
	deleteGraphDialog = new DeleteGraphDialog(this);
	
	//Connections
	connect(addGraphButton, &QPushButton::clicked, [this](bool checked)
	{
		addGraphDialog->show(); 
		addGraphDialog->move( QCursor::pos() );
	});
	connect(addGraphDialog, &AddGraphDialog::GraphAdded, [this](QString& name)
	{
		hierarchyListModel->AddNew(name); 
	});


	connect(deleteGraphButton, &QPushButton::clicked, [this](bool checked)
	{
		deleteGraphDialog->show(); 
		deleteGraphDialog->move( QCursor::pos() );
	});
	connect(deleteGraphDialog, &DeleteGraphDialog::GraphDeleted, [this]()
	{

	});
}

}	
}
}