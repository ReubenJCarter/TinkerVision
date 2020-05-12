#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include <QSpinBox>


namespace Viso
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{

class GraphInputNode: public BaseNode
{
	private:
        int graphInputInx; 
		std::shared_ptr<BaseNodeData> data;

        QWidget* baseWidget;
        QHBoxLayout* baseLayout; 
		QSpinBox* lineEdit;
        QComboBox* comboEdit;
	 
	public:
		GraphInputNode()
        {
            std::vector<BaseNode::OutputPortInfo> outputPorts( { {"out", true, BaseNodeData("intData", "IntData").type()} } );
            std::vector<BaseNode::InputPortInfo> inputPorts;
            Init("GraphInput", inputPorts, outputPorts, true, "Graph Input", false); 

            lineEdit = new QSpinBox(); 
            lineEdit->setMaximumSize( QSize(30, 50) );
            lineEdit->setStyleSheet("padding: 0px");
            lineEdit->setValue(0);
            connect(lineEdit, QOverload<int>::of(&QSpinBox::valueChanged),[this ](int i)
            {
                graphInputInx = i; 
                Q_EMIT dataUpdated(0);
            });

            comboEdit = new QComboBox(); 
            comboEdit->setMaximumSize( QSize(100, 50) );
            comboEdit->setStyleSheet("padding: 0px");

            comboEdit->addItem("int");
            comboEdit->addItem("float");
            comboEdit->addItem("bool");
            comboEdit->addItem("string");
            comboEdit->addItem("image"); 
            comboEdit->addItem("imageGPU");
            comboEdit->addItem("imageType"); 

            comboEdit->setCurrentText("intData"); 

            connect(comboEdit, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int inx)
            {
                Q_UNUSED(inx);
                std::string dataType = comboEdit->currentText().toStdString(); 
                if(dataType == "int")
                {
                    data = std::make_shared<BaseNodeData>( IntData().type() );
                }
                else if(dataType == "float")
                {
                    data = std::make_shared<BaseNodeData>( FloatData().type() );
                }
                else if(dataType == "bool")
                {
                    data = std::make_shared<BaseNodeData>( BoolData().type() );
                }
                else if(dataType == "string")
                {
                    data = std::make_shared<BaseNodeData>( StringData().type() );
                }
                else if(dataType == "image")
                {
                    data = std::make_shared<BaseNodeData>( ImageData().type() );
                }
                else if(dataType == "imageGPU")
                {
                    data = std::make_shared<BaseNodeData>( ImageGPUData().type() );
                }
                else if(dataType == "imageType")
                {
                    data = std::make_shared<BaseNodeData>( ImageTypeData().type() );
                }
                else
                {
                    data = std::make_shared<BaseNodeData>("NULL", "NULL");
                }
                _outputPorts[0].type = data->type();
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                Q_EMIT dataUpdated(0);
            });
            
            data = std::make_shared<BaseNodeData>(IntData().type());

            baseWidget = new QWidget();
            baseLayout = new QHBoxLayout(); 
            baseLayout->setMargin(0); 
            baseLayout->setSpacing(0);
            baseLayout->setContentsMargins(QMargins(0,0,0,0));
            baseLayout->setSizeConstraint(QLayout::SetFixedSize); 
            baseWidget->setLayout(baseLayout); 
            baseLayout->addWidget(lineEdit); 
            baseLayout->addWidget(comboEdit); 
        }

		virtual ~GraphInputNode(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) 
            {
                modelJson["graphInputInx"] = QString::number(graphInputInx);
                modelJson["combo"] = comboEdit->currentText(); 
            }

            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["graphInputInx"];
            if (!v.isUndefined())
            {
                QString strNum = v.toString();
                bool ok;
                int d = strNum.toInt(&ok);
                if (ok)
                {
                    graphInputInx = d; 
                    lineEdit->setValue(d);
                    
                    QJsonValue vCombo = p["combo"];
                    if (!vCombo.isUndefined())
                    {
                        QString str = vCombo.toString();
                        comboEdit->setCurrentText(str); 
                    }
                }
            }
        }

		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) 
        { 
            return std::static_pointer_cast<QtNodes::NodeData>(data);
        }
		QWidget* embeddedWidget() { return baseWidget; };
        inline int GetGraphInputInx(){ return graphInputInx; }
};

class GraphOutputNode: public BaseNode
{
    private:
        int graphOutputInx; 
		std::shared_ptr<BaseNodeData> data;

        QWidget* baseWidget;
        QHBoxLayout* baseLayout; 
		QSpinBox* lineEdit;
        QComboBox* comboEdit;

	public:
		GraphOutputNode()
        {
            std::vector<BaseNode::OutputPortInfo> outputPorts;
            std::vector<BaseNode::InputPortInfo> inputPorts( { {"", true, IntData().type(), true} } );
            Init("GraphOutput", inputPorts, outputPorts, true, "Graph Output", false); 

            lineEdit = new QSpinBox (); 
            //lineEdit->setValidator(new QIntValidator());
            lineEdit->setMaximumSize( QSize(30, 50) );
            lineEdit->setStyleSheet("padding: 0px");
            lineEdit->setValue(0);
            connect(lineEdit, QOverload<int>::of(&QSpinBox::valueChanged),[this ](int i)
            {
                graphOutputInx = i; 
            });

            comboEdit = new QComboBox(); 
            comboEdit->setMaximumSize( QSize(100, 50) );
            comboEdit->setStyleSheet("padding: 0px");

            comboEdit->addItem("int");
            comboEdit->addItem("float");
            comboEdit->addItem("bool");
            comboEdit->addItem("string");
            comboEdit->addItem("image"); 
            comboEdit->addItem("imageGPU");
            comboEdit->addItem("imageType"); 

            comboEdit->setCurrentText("intData"); 

            connect(comboEdit, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int inx)
            {
                Q_UNUSED(inx);
                std::string dataType = comboEdit->currentText().toStdString(); 
                if(dataType == "int")
                {
                    data = std::make_shared<BaseNodeData>( IntData().type() );
                }
                else if(dataType == "float")
                {
                    data = std::make_shared<BaseNodeData>( FloatData().type() );
                }
                else if(dataType == "bool")
                {
                    data = std::make_shared<BaseNodeData>( BoolData().type() );
                }
                else if(dataType == "string")
                {
                    data = std::make_shared<BaseNodeData>( StringData().type() );
                }
                else if(dataType == "image")
                {
                    data = std::make_shared<BaseNodeData>( ImageData().type() );
                }
                else if(dataType == "imageGPU")
                {
                    data = std::make_shared<BaseNodeData>( ImageGPUData().type() );
                }
                else if(dataType == "imageType")
                {
                    data = std::make_shared<BaseNodeData>( ImageTypeData().type() );
                }
                else
                {
                    data = std::make_shared<BaseNodeData>("NULL", "NULL");
                }
                _inputPorts[0].type = data->type();
            });

            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 

            baseWidget = new QWidget();
            baseLayout = new QHBoxLayout(); 
            baseLayout->setMargin(0); 
            baseLayout->setSpacing(0);
            baseLayout->setContentsMargins(QMargins(0,0,0,0));
            baseLayout->setSizeConstraint(QLayout::SetFixedSize); 
            baseWidget->setLayout(baseLayout); 
            baseLayout->addWidget(lineEdit); 
            baseLayout->addWidget(comboEdit); 
            
        }

		virtual ~GraphOutputNode(){}

        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) 
            {
                modelJson["graphOutputInx"] = QString::number(graphOutputInx);
                modelJson["combo"] = comboEdit->currentText(); 
            }

            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["graphOutputInx"];
            if (!v.isUndefined())
            {
                QString strNum = v.toString();
                bool ok;
                int d = strNum.toInt(&ok);
                if (ok)
                {
                    graphOutputInx = d; 
                    lineEdit->setValue(d);
                    
                    QJsonValue vCombo = p["combo"];
                    if (!vCombo.isUndefined())
                    {
                        QString str = vCombo.toString();
                        comboEdit->setCurrentText(str); 
                    }
                }
            }
        }

		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return nullptr; }
		QWidget* embeddedWidget() { return baseWidget; }

        inline int GetGraphOutputInx() { return graphOutputInx; }
};

}
}
}
}