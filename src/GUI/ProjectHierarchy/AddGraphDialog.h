#pragma once

#include <QWidget>

class QVBoxLayout; 
class QLineEdit;
class QHBoxLayout;
class QPushButton; 
class QCloseEvent; 

namespace Visi
{
namespace GUI
{
namespace ProjectHierarchy
{
	
class AddGraphDialog: public QWidget
{
	Q_OBJECT 
	
	private:
		QVBoxLayout* layoutBase;
        QLineEdit* nameEdit; 
        QHBoxLayout* buttonlayout; 
		QPushButton* addGraphButton; 
		QPushButton* cancelButton; 
		
    protected:
        void closeEvent(QCloseEvent *event); 

	public:
		AddGraphDialog(QWidget *parent = nullptr); 

    signals:
		void GraphAdded(QString& str); 
};

}	
}
}
