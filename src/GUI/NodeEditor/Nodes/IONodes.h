#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include "../../../ComputeGraph/Nodes/IONodes.h"

namespace TnkrVis
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{

class ImageFileRead: public BaseNode
{
	 
	public:
		ImageFileRead()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts( { {"im", true, BaseImageData().type(), true}, 
                                                               {"src", true, StringData().type(), true} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, BaseImageData().type()} }); 
            
            Init("ImageFileRead", inputPorts, outputPorts, true, "Image File Read", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ImageFileRead(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ImageFileRead; }

        void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex) override
        {
            //Set the node data on te input
            _inputPorts[portIndex].data = portData;

            //first test the src and dst for likeness and valid type
            auto _dstDataLk = _inputPorts[0].data.lock();
            bool typeValid = true;
            if(_dstDataLk) //not null
            {
                if(_dstDataLk->type().id != _inputPorts[0].type.id )//invalidness on the types
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
            for(int i = 1; i < _inputPorts.size(); i++)
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

class ImageFileWrite: public BaseProcess1In1Out
{
	 
	public:
		ImageFileWrite()
        {
            inputPorts.push_back( {"src", true, StringData().type(), true} );
            
		    Init("ImageFileWrite", inputPorts, outputPorts, true, "Image File Write", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ImageFileWrite(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ImageFileWrite; }
};

}
}
}
}