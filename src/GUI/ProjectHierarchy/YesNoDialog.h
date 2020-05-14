#pragma once

#include <QWidget>

class QVBoxLayout; 
class QHBoxLayout;
class QPushButton; 
class QCloseEvent; 
class QLabel; 

namespace TnkrVis
{
namespace GUI
{
namespace ProjectHierarchy
{
	
class YesNoDialog: public QWidget
{
	Q_OBJECT 
	
	private:
		QVBoxLayout* layoutBase;
        QHBoxLayout* buttonlayout; 
		QLabel* message; 
		QPushButton* yesButton; 
		QPushButton* noButton; 
		
    protected:
        void closeEvent(QCloseEvent *event); 

	public:
		YesNoDialog(QWidget *parent = nullptr); 

    signals:
		void Yes(); 
		void No(); 
};

}	
}
}
