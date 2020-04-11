#pragma once

#include <QWidget>

class QGridLayout; 

namespace QtNodes
{
	class FlowScene;
	class FlowView;
}

namespace Visi
{
namespace GUI
{
class NodeEditorWidget: public QWidget
{
	Q_OBJECT 
	
	private:
		QtNodes::FlowScene* flowScene;
		QtNodes::FlowView* flowView;
		QGridLayout* layout;
		void SetStyle();
		
	public:
		NodeEditorWidget(); 
};
	
}
}
