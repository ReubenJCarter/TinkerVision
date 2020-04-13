#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include <QScrollArea>
#include <QLabel>

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{

class TextDisplay: public BaseNode
{
	private:
		QScrollArea* scrollArea;
		QLabel* label;
	 
	public:
		TextDisplay()
        {
            BaseNode::PortInfo inPort = {"in", true, StringData().type()};
            std::vector<BaseNode::PortInfo> inputPorts( {inPort} );
            std::vector<BaseNode::PortInfo> outputPorts;
            
            Init("TextDisplay", inputPorts, outputPorts, true, "Text Display", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
            
            scrollArea = new QScrollArea();
            label = new QLabel();
            
            scrollArea->setWidget(label); 
        }

		virtual ~TextDisplay(){}
		
        void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex)
        {
            auto d = std::dynamic_pointer_cast<StringData>(portData);
            if(d)
            {
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                label->setText( QString(d->ToString().c_str()) );
                label->adjustSize();
            }
            else
            {
                SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
                label->clear();
                label->adjustSize();
            }
        }

        QWidget* embeddedWidget()  
        { 
            return scrollArea; 
        }
};

}
}
}
}