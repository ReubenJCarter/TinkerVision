#include "ProjectHierarchyWidget.h"
#include "../NodeEditor/NodeEditorWidget.h"
#include "Core/SerializedObject.h"

#include "HierarchyListModel.h"
#include "AddGraphDialog.h"
#include "DeleteGraphDialog.h"
#include "YesNoDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QListView>
#include <QPushButton>


#include <QStringListModel>
#include <QStringList>

namespace Viso
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

	//hierarchyList->setSelectionBehavior(QAbstractItemView::SelectRows); 
	//hierarchyList->setDefaultDropAction(Qt::IgnoreAction);
	//hierarchyList->setMovement(QListView::Free);
	hierarchyList->setSelectionMode(QAbstractItemView::ExtendedSelection);
	hierarchyList->setDragDropMode(QAbstractItemView::InternalMove);
	hierarchyList->setEditTriggers(QAbstractItemView::EditKeyPressed); 
	hierarchyList->setDragEnabled(true);
	hierarchyList->setAcceptDrops(true);
	hierarchyList->setDropIndicatorShown(true);

	hierarchyListModel = new HierarchyListModel(); 
	hierarchyList->setModel(hierarchyListModel);
	layoutBase->addWidget(hierarchyList); 

	
	
	//Add dialog
	addGraphDialog = new AddGraphDialog(this); 
	
	//delete dialog
	deleteGraphDialog = new DeleteGraphDialog(this);

	//new project dialog
	startNewProjectDialog = new YesNoDialog(this); 
	
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
			LoadNodeEditorFromIndex(newInx); 
		}
		//else if we have deleted the currently open graph, open the first graph  in the list
		else if(deletedGraphOpen)
		{
			int newInx = 0; 
			LoadNodeEditorFromIndex(newInx); 
		}
	});

	//connect startNewProjectDialog
	connect(startNewProjectDialog, &YesNoDialog::Yes, [this]()
	{	
		openGraphName = ""; 
		hierarchyListModel->Clear(); 
		LoadNodeEditorFromIndex( hierarchyListModel->AddNew("New Graph") ); 
	});

	//Connection open
	connect(hierarchyList, &QAbstractItemView::doubleClicked, [this](const QModelIndex &index)
	{
		SaveCurrentNodeEditor(); 
		LoadNodeEditorFromIndex(index.row()); 
	}); 

	//Add default graph
	openGraphName = ""; 
	LoadNodeEditorFromIndex( hierarchyListModel->AddNew("New Graph") ); 
}

void ProjectHierarchyWidget::SaveCurrentNodeEditor()
{
	if(openGraphName.length() > 0)
	{
		//save the current node editor scene to the correct thingy in the other thing 
		int inx = hierarchyListModel->GetInxFromName(openGraphName); 
		if(inx >= 0)
			hierarchyListModel->ModifyData( inx, nodeEditorWidget->Save() ); 
	}
}

void ProjectHierarchyWidget::LoadNodeEditorFromIndex(int inx)
{
	//Load new data into the node editor
	nodeEditorWidget->Load( hierarchyListModel->GetData(inx), hierarchyListModel->GetName(inx)  ); 
	openGraphName = hierarchyListModel->GetName(inx);
}

void ProjectHierarchyWidget::Serialize(SerializedObject* so)
{
	hierarchyListModel->Serialize(so); 
}

void ProjectHierarchyWidget::Deserialize(SerializedObject* so)
{
	hierarchyListModel->Deserialize(so); 
	LoadNodeEditorFromIndex( 0 ); 
}

void ProjectHierarchyWidget::StartNewProject()
{
	startNewProjectDialog->show(); 
	startNewProjectDialog->move( QCursor::pos() );
}

}	
}
}