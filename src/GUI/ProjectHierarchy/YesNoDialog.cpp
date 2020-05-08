#include "YesNoDialog.h"

#include <QVBoxLayout> 
#include <QHBoxLayout> 
#include <QPushButton>
#include <QCloseEvent>
#include <QLabel>

namespace Viso
{
namespace GUI
{
namespace ProjectHierarchy
{
	
YesNoDialog::YesNoDialog(QWidget *parent): QWidget(parent)
{
    setWindowTitle("Yes Or No ?");

    setWindowFlags(Qt::Dialog);
    setWindowModality(Qt::WindowModal);

    layoutBase = new QVBoxLayout(); 
    setLayout(layoutBase);

    message = new QLabel("Are you sure you want to do this?"); 
    layoutBase->addWidget(message); 

    buttonlayout = new QHBoxLayout(); 
    layoutBase->addLayout(buttonlayout); 
    
    yesButton = new QPushButton("Yes"); 
    buttonlayout->addWidget(yesButton); 

    noButton = new QPushButton("No"); 
    buttonlayout->addWidget(noButton); 

    //connections
    connect(yesButton, &QPushButton::clicked, [this](bool checked)
	{
		emit Yes( ); 
        hide(); 
	});
    
    connect(noButton, &QPushButton::clicked, [this](bool checked)
	{
        emit No( ); 
		hide(); 
	});
}

void YesNoDialog::closeEvent(QCloseEvent *event)
{
    event->accept();
}

}	
}
}
