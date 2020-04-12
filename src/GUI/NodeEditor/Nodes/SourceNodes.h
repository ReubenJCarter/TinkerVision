#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include <QLineEdit>
#include <QDoubleValidator>

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
            lineEdit->setValidator(new QDoubleValidator());
            lineEdit->setMaximumSize(lineEdit->sizeHint());
            lineEdit->setText("0.0");
            connect(lineEdit, &QLineEdit::textChanged, [this](QString const &string)
            {
                std::cout << "IntSource::OnTextEdited\n";
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
            if(data)
            {
                modelJson["data"] = QString::number(data->Data());
            }

            return modelJson;
        }

		void restore(QJsonObject const &p)
        {
            QJsonValue v = p["number"];
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

}
}
}
}