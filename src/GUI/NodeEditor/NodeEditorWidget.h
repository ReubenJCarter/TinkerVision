#pragma once

#include <QWidget>

class QVBoxLayout; 
class QHBoxLayout; 
class QLabel; 

namespace QtNodes
{
	class FlowScene;
	class FlowView;
}

namespace Viso
{

class SerializedObject; 

namespace GUI
{
namespace NodeEditor
{

class NodeEditorWidget: public QWidget
{
	Q_OBJECT 
	
	private:
		QtNodes::FlowScene* flowScene;
		QtNodes::FlowView* flowView;
		QLabel* nameLabel; 
		QVBoxLayout* layout;
		QHBoxLayout* topBarLayout;
		void SetStyle(bool darkMode);
		
	public:
		NodeEditorWidget(); 
		void Load(QByteArray d, QString name);
		QByteArray Save(); 
		void Clear(); 
		void SerializeToComputeGraph(SerializedObject* so); 
};

}	
}
}
