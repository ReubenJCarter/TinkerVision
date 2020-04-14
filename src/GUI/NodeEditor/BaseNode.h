#pragma once

#include <nodes/NodeDataModel>
#include <nodes/NodeData>

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
		
	private:
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

}
}
}