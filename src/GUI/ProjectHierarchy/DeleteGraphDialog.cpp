#include "DeleteGraphDialog.h"

#include <QVBoxLayout> 
#include <QHBoxLayout> 
#include <QPushButton>
#include <QCloseEvent>

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
DeleteGraphDialog::DeleteGraphDialog(QWidget *parent): QWidget(parent)
{
    setWindowTitle("Delete");

    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    layoutBase = new QVBoxLayout(); 
    setLayout(layoutBase);

    buttonlayout = new QHBoxLayout(); 
    layoutBase->addLayout(buttonlayout); 
    
    addGraphButton = new QPushButton("Delete"); 
    buttonlayout->addWidget(addGraphButton); 

    cancelButton = new QPushButton("Cancel"); 
    buttonlayout->addWidget(cancelButton); 

    //connections
    connect(addGraphButton, &QPushButton::clicked, [this](bool checked)
	{
		emit GraphDeleted( ); 
        hide(); 
	});
    
    connect(cancelButton, &QPushButton::clicked, [this](bool checked)
	{
		hide(); 
	});
}

void DeleteGraphDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

}	
}
}
