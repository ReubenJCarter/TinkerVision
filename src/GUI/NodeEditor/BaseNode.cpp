#include "BaseNode.h"

#include "DataTypes.h"

#include "Core/SerializedObject.h"

#include <iostream>

namespace TnkrVis
{
namespace GUI
{
namespace NodeEditor
{

void BaseNode::SetValidationState(QtNodes::NodeValidationState s, std::string mes)
{
	_validationState = s;
	_validationMessage = mes;
}
	
BaseNode::BaseNode()
{
	_captionVisible = true;
	_resizeable = false;
	_validationState = QtNodes::NodeValidationState::Valid;
	_validationMessage = ""; 
}

BaseNode::~BaseNode()
{
	
}

void BaseNode::Init(std::string nm, std::vector<InputPortInfo>& inPorts, std::vector<OutputPortInfo>& outPorts, bool capVis, std::string cap, bool resz)
{
	_name = nm;
	_caption = cap; 
	_captionVisible = capVis;
	_resizeable = resz;
	_inputPorts = inPorts;
	_outputPorts = outPorts; 
}

QString BaseNode::caption() const 
{ 
	return QString(_caption.c_str());
}

bool BaseNode::captionVisible() const 
{ 
	return _captionVisible;
}

QString BaseNode::name() const 
{ 
	return QString(_name.c_str()); 
}

QString BaseNode::portCaption(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	QString result = "";

	if(portType == QtNodes::PortType::In && portIndex < _inputPorts.size())
	{
		result = QString(_inputPorts[portIndex].caption.c_str());
	}
	else if(portType == QtNodes::PortType::Out && portIndex < _outputPorts.size())
	{
		result = QString(_outputPorts[portIndex].caption.c_str());
	}
	
	return result;
}
bool BaseNode::portCaptionVisible(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	bool result = false;

	if(portType == QtNodes::PortType::In && portIndex < _inputPorts.size())
	{
		result = _inputPorts[portIndex].captionVisible;
	}
	else if(portType == QtNodes::PortType::Out && portIndex < _outputPorts.size())
	{
		result = _outputPorts[portIndex].captionVisible;
	}
	
	return result;
}

unsigned int BaseNode::nPorts(QtNodes::PortType portType) const
{
	unsigned int result = 0;

	if(portType == QtNodes::PortType::In)
	{
		result = _inputPorts.size();
	}
	else if(portType == QtNodes::PortType::Out)
	{
		result = _outputPorts.size();
	}
	
	return result;
}

QtNodes::NodeDataType BaseNode::dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const
{
	
	QtNodes::NodeDataType result; 
	
	if(portType == QtNodes::PortType::In && portIndex < _inputPorts.size())
	{
		result = _inputPorts[portIndex].type;
	}
	else if(portType == QtNodes::PortType::Out && portIndex < _outputPorts.size())
	{
		result = _outputPorts[portIndex].type;
	}
	
	return result;
}

bool BaseNode::resizable() const
{
	return _resizeable; 
}

std::shared_ptr<QtNodes::NodeData> BaseNode::outData(QtNodes::PortIndex portIndex)
{
	return std::static_pointer_cast<QtNodes::NodeData>(_outputPorts[portIndex].data); 
}

void BaseNode::setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex)
{
	_inputPorts[portIndex].data = portData;

	bool typeValid = true; 
	for(int i = 0; i < _inputPorts.size(); i++)
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

QWidget* BaseNode::embeddedWidget()  
{ 
	return nullptr;
}

QtNodes::NodeValidationState BaseNode::validationState() const
{
	return _validationState; 
}

QString BaseNode::validationMessage() const
{
	return QString(_validationMessage.c_str()); 
}

}
}
}