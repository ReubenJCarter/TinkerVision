#pragma once

#include <nodes/NodeDataModel>
#include <nodes/NodeData>

#include "DataTypes.h"

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{

class BaseNode: public QtNodes::NodeDataModel
{
	Q_OBJECT
	
	protected:
		struct InputPortInfo
		{
			std::string caption; 
			bool captionVisible; 
			QtNodes::NodeDataType type; 
			bool required; 
			std::weak_ptr<QtNodes::NodeData> data; 
		};
		struct OutputPortInfo
		{
			std::string caption; 
			bool captionVisible; 
			QtNodes::NodeDataType type; 
			std::shared_ptr<QtNodes::NodeData> data; 
		};
		
		void SetValidationState(QtNodes::NodeValidationState s, std::string mes); 
	
		std::string _name;
		std::string _caption; 
		bool _captionVisible; 
		
		
		std::vector<InputPortInfo> _inputPorts;
		std::vector<OutputPortInfo> _outputPorts; 
		bool _resizeable; 
		
		QtNodes::NodeValidationState _validationState;
		std::string _validationMessage; 
	
	public:
		BaseNode();
		virtual ~BaseNode();
		
		void Init(std::string nm, std::vector<InputPortInfo>& inPorts, std::vector<OutputPortInfo>& outPorts, bool capVis=false, std::string cap="", bool resz=false);
		
		QString caption() const override;
		bool captionVisible() const override;
		QString name() const override;
		QString portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		unsigned int nPorts(QtNodes::PortType portType) const override;
		QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
		bool resizable() const override; 
		
		virtual std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) override;
		virtual void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex) override;
		virtual QWidget* embeddedWidget() override;
		
		QtNodes::NodeValidationState validationState() const override;
		QString validationMessage() const override;
};

class BaseProcess1In1Out: public BaseNode
{
    protected:
        std::vector<BaseNode::InputPortInfo> inputPorts; 
        std::vector<BaseNode::OutputPortInfo> outputPorts;
		bool cpuOnly; 

    public:
        BaseProcess1In1Out()
        {
            inputPorts.push_back( {"dst", true, BaseImageData().type(), true} ); 
            inputPorts.push_back( {"src", true, BaseImageData().type(), true} ); 
            outputPorts.push_back( {"im", true, BaseImageData().type() }); 
			cpuOnly = false;
        }
       
		void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex) override
        {
            //Set the node data on te input
            _inputPorts[portIndex].data = portData;

            //first test the src and dst for likeness and valid type
            auto _dstDataLk = _inputPorts[0].data.lock();
            auto _srcDataLk = _inputPorts[1].data.lock(); 
            bool typeValid = true;
            if(_dstDataLk && _srcDataLk) //not null
            {
                if(_dstDataLk->type().id != _inputPorts[0].type.id ||
                   _srcDataLk->type().id != _inputPorts[1].type.id ||
                   _dstDataLk->type().name != _srcDataLk->type().name ||
				   cpuOnly && (_dstDataLk->type().name != "ImageData") )//invalidness on the types
                {
                    typeValid = false; 
                } 
                else 
                {
                    //set the correct output type 
                    _outputPorts[0].type = _dstDataLk->type(); 
                }           
            }
            else
            {
                typeValid = false;
            }

            //handle any other inputs
            for(int i = 2; i < _inputPorts.size(); i++)
            {
                auto _inputPortDataL = _inputPorts[i].data.lock();
                if(_inputPortDataL)
                {
                    if(_inputPortDataL->type().id != _inputPorts[i].type.id)
                        typeValid = false; 
                }
                else if(_inputPorts[i].required)
                {
                    typeValid = false; 
                }
            }

            //next create the correct data type for the output
            if(typeValid)
            {
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                for(int i = 0; i < _outputPorts.size(); i++)
                {
                    _outputPorts[i].data = std::make_shared<BaseNodeData>(_outputPorts[i].type);
                }
            }
            else
            {
                SetValidationState(QtNodes::NodeValidationState::Error, "input error");
                for(int i = 0; i < _outputPorts.size(); i++)
                {
                    _outputPorts[i].data.reset(); 
                }
            }

			for(int i = 0; i < _outputPorts.size(); i++)
			{
				Q_EMIT dataUpdated(i);
			}
        }
};

}
}
}