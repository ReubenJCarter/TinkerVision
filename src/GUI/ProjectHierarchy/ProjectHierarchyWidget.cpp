#include "ProjectHierarchyWidget.h"
#include "../NodeEditor/NodeEditorWidget.h"

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
	
ProjectHierarchyWidget::ProjectHierarchyWidget(NodeEditor::NodeEditorWidget* ne)
{
	nodeEditorWidget = ne;

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

	hierarchyList->setSelectionMode(QAbstractItemView::ExtendedSelection); //SingleSelection
	//hierarchyList->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//hierarchyList->setDefaultDropAction(Qt::TargetMoveAction);

	hierarchyList->setDragEnabled(true);
	hierarchyList->setAcceptDrops(true);
	hierarchyList->setDropIndicatorShown(true);
	//hierarchyList->setMovement(QListView::Free);

	hierarchyList->setEditTriggers(QAbstractItemView::EditKeyPressed); 

	hierarchyListModel = new HierarchyListModel(); 
	hierarchyList->setModel(hierarchyListModel);
	layoutBase->addWidget(hierarchyList); 
	
	//Add dialog
	addGraphDialog = new AddGraphDialog(this); 
	
	//delete dialog
	deleteGraphDialog = new DeleteGraphDialog(this);
	
	//Connection add
	connect(addGraphButton, &QPushButton::clicked, [this](bool checked)
	{
		addGraphDialog->show(); 
		addGraphDialog->move( QCursor::pos() );
	});
	connect(addGraphDialog, &AddGraphDialog::GraphAdded, [this](QString& name)
	{
		if(hierarchyListModel->GetInxFromName(name) < 0)
		{
			hierarchyListModel->AddNew(name); 
		}
	});

	//Connection delete
	connect(deleteGraphButton, &QPushButton::clicked, [this](bool checked)
	{
		QModelIndexList selectedList = hierarchyList->selectionModel()->selectedIndexes(); 
		if(selectedList.size() > 0)
		{
			deleteGraphDialog->show(); 
			deleteGraphDialog->move( QCursor::pos() );
		}
	});
	connect(deleteGraphDialog, &DeleteGraphDialog::GraphDeleted, [this]()
	{	
		QModelIndexList selectedList = hierarchyList->selectionModel()->selectedIndexes(); 	
		std::vector<int> selectedInxs; 
		bool deletedGraphOpen = false; 
		for(int i = 0; i < selectedList.size(); i++)
		{
			int inx = selectedList[i].row(); 
			selectedInxs.push_back(inx); 
			QString name = hierarchyListModel->GetName(inx); 

			//test the name of the graph to delete agaisnt the open graph
			if(name == openGraphName)
				deletedGraphOpen = true; 
		}
		hierarchyListModel->Remove(selectedInxs); 

		//we have no graphs left so make a new default one so as too have at least one graph present
		if(hierarchyListModel->GetCount() <= 0)
		{
			int newInx = hierarchyListModel->AddNew("New Graph"); 
			nodeEditorWidget->Load( hierarchyListModel->GetData(newInx), hierarchyListModel->GetName(newInx)  ); 
			openGraphName = hierarchyListModel->GetName(newInx);
		}
		//else if we have deleted the currently open graph, open the first graph  in the list
		else if(deletedGraphOpen)
		{
			int newInx = 0; 
			nodeEditorWidget->Load( hierarchyListModel->GetData(newInx), hierarchyListModel->GetName(newInx)  ); 
			openGraphName = hierarchyListModel->GetName(newInx);
		}
	});

	//Connection open
	connect(hierarchyList, &QAbstractItemView::doubleClicked, [this](const QModelIndex &index)
	{
		int inx = index.row(); 
		QByteArray data = hierarchyListModel->GetData(inx);
		QString name = hierarchyListModel->GetName(inx); 
		nodeEditorWidget->Load(data, name); 
		openGraphName = name; 
	}); 

	//Add default graph
	int newInx = hierarchyListModel->AddNew("New Graph"); 
	nodeEditorWidget->Load( hierarchyListModel->GetData(newInx), hierarchyListModel->GetName(newInx)  ); 
	openGraphName = hierarchyListModel->GetName(newInx);
}

}	
}
}