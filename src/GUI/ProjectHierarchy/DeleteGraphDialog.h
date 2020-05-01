#pragma once

#include <QWidget>

class QVBoxLayout; 
class QHBoxLayout;
class QPushButton; 
class QCloseEvent; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
class DeleteGraphDialog: public QWidget
{
	Q_OBJECT 
	
	private:
		QVBoxLayout* layoutBase;
        QHBoxLayout* buttonlayout; 
		QPushButton* addGraphButton; 
		QPushButton* cancelButton; 
		
    protected:
        void closeEvent(QCloseEvent *event); 

	public:
		DeleteGraphDialog(QWidget *parent = nullptr); 

    signals:
		void GraphDeleted(); 
};

}	
}
}
