#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QDoubleValidator>
#include <QIntValidator>

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{


class IntSource: public BaseNode
{
	private:
		std::shared_ptr<IntData> data;
		QLineEdit* lineEdit;
	 
	public:
		IntSource()
        {
            BaseNode::PortInfo outPort = {"out", true, IntData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("IntSource", inputPorts, outputPorts, true, "Int Source", false); 
            
            lineEdit = new QLineEdit(); 
            lineEdit->setValidator(new QIntValidator());
            lineEdit->setMaximumSize( QSize(50, 50) );
            lineEdit->setText("0");
            connect(lineEdit, &QLineEdit::textChanged, [this](QString const &string)
            {
	            Q_UNUSED(string);
                bool ok = false;
                int n = lineEdit->text().toInt(&ok);
                if (ok)
                {
                    data = std::make_shared<IntData>(n);
                    SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                    Q_EMIT dataUpdated(0);
                }
                else
                {
                    data = nullptr; 
                    SetValidationState(QtNodes::NodeValidationState::Error, "value error"); 
                    Q_EMIT dataUpdated(0);
                    Q_EMIT dataInvalidated(0);
                }
            });
            
            data = std::make_shared<IntData>(0);
        }

		virtual ~IntSource(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) modelJson["data"] = QString::number(data->Data());
            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["data"];
            if (!v.isUndefined())
            {
                QString strNum = v.toString();
                bool ok;
                int d = strNum.toInt(&ok);
                if (ok)
                {
                    data = std::make_shared<IntData>(d);
                    lineEdit->setText(strNum);
                }
            }
        }

		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
		QWidget* embeddedWidget() { return lineEdit; };
};

class FloatSource: public BaseNode
{
	private:
		std::shared_ptr<FloatData> data;
		QLineEdit* lineEdit;
	 
	public:
		FloatSource()
        {
            BaseNode::PortInfo outPort = {"out", true, FloatData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("FloatSource", inputPorts, outputPorts, true, "Float Source", false); 
            
            lineEdit = new QLineEdit(); 
            lineEdit->setValidator(new QDoubleValidator());
            lineEdit->setMaximumSize( QSize(50, 50) );
            lineEdit->setText("0.0");
            connect(lineEdit, &QLineEdit::textChanged, [this](QString const &string)
            {
	            Q_UNUSED(string);
                bool ok = false;
                float n = (float)lineEdit->text().toDouble(&ok);
                if (ok)
                {
                    data = std::make_shared<FloatData>(n);
                    SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                    Q_EMIT dataUpdated(0);
                }
                else
                {
                    data = nullptr; 
                    SetValidationState(QtNodes::NodeValidationState::Error, "value error"); 
                    Q_EMIT dataUpdated(0);
                    Q_EMIT dataInvalidated(0);
                }
            });
            
            data = std::make_shared<FloatData>(0);
        }

		virtual ~FloatSource(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) modelJson["data"] = QString::number(data->Data());
            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["data"];
            if (!v.isUndefined())
            {
                QString strNum = v.toString();
                bool ok;
                float d = (float)strNum.toDouble(&ok);
                if (ok)
                {
                    data = std::make_shared<FloatData>(d);
                    lineEdit->setText(strNum);
                }
            }
        }
        
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
		QWidget* embeddedWidget() { return lineEdit; };
};

class BoolSource: public BaseNode
{
	private:
		std::shared_ptr<BoolData> data;
		QCheckBox* checkEdit;
	 
	public:
		BoolSource()
        {
            BaseNode::PortInfo outPort = {"out", true, BoolData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("BoolSource", inputPorts, outputPorts, true, "Bool Source", false); 
            
            checkEdit = new QCheckBox(); 
            connect(checkEdit, &QCheckBox::stateChanged, [this](int state)
            {
	            Q_UNUSED(state);
                bool checked = checkEdit->checkState() == Qt::Checked; 
                data = std::make_shared<BoolData>(checked);
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                Q_EMIT dataUpdated(0);
            });
            
            data = std::make_shared<BoolData>(0);
        }

		virtual ~BoolSource(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) modelJson["data"] = QString::number(data->Data());
            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["data"];
            if (!v.isUndefined())
            {
                QString strNum = v.toString();
                bool ok;
                bool d = strNum.toInt(&ok) != 0;
                if (ok)
                {
                    data = std::make_shared<BoolData>(d);
                    if(d)
                        checkEdit->setCheckState(Qt::Checked);
                    else
                        checkEdit->setCheckState(Qt::Unchecked);
                }
            }
        }
        
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
		QWidget* embeddedWidget() { return checkEdit; };
};

class StringSource: public BaseNode
{
	private:
		std::shared_ptr<StringData> data;
		QLineEdit* lineEdit;
	 
	public:
		StringSource()
        {
            BaseNode::PortInfo outPort = {"out", true, StringData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("StringSource", inputPorts, outputPorts, true, "String Source", false); 
            
            lineEdit = new QLineEdit(); 
            lineEdit->setMaximumSize( QSize(120, 50) );
            lineEdit->setText("");
            connect(lineEdit, &QLineEdit::textChanged, [this](QString const &string)
            {
	            Q_UNUSED(string);
                std::string n = lineEdit->text().toStdString();
                data = std::make_shared<StringData>(n);
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                Q_EMIT dataUpdated(0);
            });
            
            data = std::make_shared<StringData>("");
        }

		virtual ~StringSource(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) modelJson["data"] = QString( data->Data().c_str() );
            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["data"];
            if (!v.isUndefined())
            {
                QString str = v.toString();
                std::string d = str.toStdString();
                data = std::make_shared<StringData>(d);
                lineEdit->setText(str);
            }
        }
        
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
		QWidget* embeddedWidget() { return lineEdit; };
};

class ImageSource: public BaseNode
{
	private:
		std::shared_ptr<ImageData> data;
	 
	public:
		ImageSource()
        {
            BaseNode::PortInfo outPort = {"out", true, ImageData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("ImageSource", inputPorts, outputPorts, true, "Image Source", false); 
            data = std::make_shared<ImageData>();
        }

		virtual ~ImageSource(){}
	
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
};

class ImageGPUSource: public BaseNode
{
	private:
		std::shared_ptr<ImageGPUData> data;
	 
	public:
		ImageGPUSource()
        {
            BaseNode::PortInfo outPort = {"out", true, ImageGPUData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("ImageGPUSource", inputPorts, outputPorts, true, "ImageGPU Source", false); 
            data = std::make_shared<ImageGPUData>();
        }
        
		virtual ~ImageGPUSource(){}
	
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
};

class ImageTypeSource: public BaseNode
{
	private:
		std::shared_ptr<ImageTypeData> data;
		QComboBox* comboEdit;
	 
	public:
		ImageTypeSource()
        {
            BaseNode::PortInfo outPort = {"out", true, ImageTypeData().type()};
            std::vector<BaseNode::PortInfo> outputPorts( {outPort} );
            std::vector<BaseNode::PortInfo> inputPorts;
            Init("ImageTypeSource", inputPorts, outputPorts, true, "ImageType Source", false); 
            
            comboEdit = new QComboBox(); 
            comboEdit->setMaximumSize( QSize(130, 50) );

            comboEdit->addItem("GRAYSCALE8");
            comboEdit->addItem("GRAYSCALE16");
            comboEdit->addItem("GRAYSCALE32F");
            comboEdit->addItem("RGB8");
            comboEdit->addItem("RGB32F"); 
            comboEdit->addItem("RGBA8");
            comboEdit->addItem("RGBA32F"); 

            comboEdit->setCurrentText("RGB8"); 

            connect(comboEdit, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int inx)
            {
                Q_UNUSED(inx);
                data = std::make_shared<ImageTypeData>( comboEdit->currentText().toStdString() );
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                Q_EMIT dataUpdated(0);
            });
            
            data = std::make_shared<ImageTypeData>("RGB8");
        }

		virtual ~ImageTypeSource(){}
		
        QJsonObject save() const 
        {
            QJsonObject modelJson = QtNodes::NodeDataModel::save();
            if(data) modelJson["data"] = QString( data->Data().c_str() );
            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["data"];
            if (!v.isUndefined())
            {
                QString str = v.toString();
                std::string d = str.toStdString();
                data = std::make_shared<ImageTypeData>(d);
                comboEdit->setCurrentText(str); 
            }
        }
        
		std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex) { return std::static_pointer_cast<QtNodes::NodeData>(data);}
		QWidget* embeddedWidget() { return comboEdit; };
};

}
}
}
}