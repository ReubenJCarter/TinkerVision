#include "AddGraphDialog.h"

#include <QVBoxLayout> 
#include <QLineEdit>
#include <QHBoxLayout> 
#include <QPushButton>
#include <QCloseEvent>

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
AddGraphDialog::AddGraphDialog(QWidget *parent): QWidget(parent)
{
    setWindowTitle("Add");

    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    layoutBase = new QVBoxLayout(); 
    setLayout(layoutBase);

    nameEdit = new  QLineEdit(); 
    nameEdit->setPlaceholderText("Enter a name"); 
    layoutBase->addWidget(nameEdit); 

    buttonlayout = new QHBoxLayout(); 
    layoutBase->addLayout(buttonlayout); 
    
    addGraphButton = new QPushButton("Add"); 
    buttonlayout->addWidget(addGraphButton); 

    cancelButton = new QPushButton("Cancel"); 
    buttonlayout->addWidget(cancelButton); 

    //connections
    connect(addGraphButton, &QPushButton::clicked, [this](bool checked)
	{
        if(nameEdit->text().length() > 0)
        {
		    emit GraphAdded( nameEdit->text() ); 
        }
        nameEdit->setText(""); 
        hide(); 
	});
    
    connect(cancelButton, &QPushButton::clicked, [this](bool checked)
	{
        nameEdit->setText(""); 
		hide(); 
	});
}

void AddGraphDialog::closeEvent(QCloseEvent *event)
{
    nameEdit->setText(""); 
    event->accept();
}

}	
}
}
