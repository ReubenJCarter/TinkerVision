#pragma once

#include <QDockWidget>

class QScrollArea;
class QLabel; 
class QWidget;
class QVBoxLayout; 
class QHBoxLayout; 
class QListView;
class QPushButton; 

namespace Visi
{
namespace GUI
{
    
namespace NodeEditor
{
class NodeEditorWidget; 
}

namespace BuildEditor
{
	
class BuildEditorWidget: public QDockWidget
{
	Q_OBJECT 
	
	private:
    	QScrollArea* scrollAreaBase;
		QWidget* widgetBase; 
		QVBoxLayout* layoutBase;

		QScrollArea* compileJsonTextScrollArea;
		QLabel*	compileJsonText; 
		QHBoxLayout* compileRunButtonLayout; 
		QPushButton* compileButton; 
		QPushButton* runButton; 

		NodeEditor::NodeEditorWidget* nodeEditorWidget; 		

	public:
		BuildEditorWidget(NodeEditor::NodeEditorWidget* ne); 
};

}	
}
}
