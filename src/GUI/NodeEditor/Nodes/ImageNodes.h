
#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{

class ImageAllocate: public BaseNode
{
	private:
        std::weak_ptr<ImageData> imageIn;
		std::weak_ptr<IntData> width;
		std::weak_ptr<IntData> height;
        std::weak_ptr<ImageTypeData> type;

        std::shared_ptr<ImageData> imageOut;
	 
	public:
		ImageAllocate()
        {
            BaseNode::PortInfo inPort = {"im", true, ImageData().type()};
            BaseNode::PortInfo WidthPort = {"W", true, IntData().type()};
            BaseNode::PortInfo heightPort = {"H", true, IntData().type()};
            BaseNode::PortInfo typePort = {"T", true, ImageTypeData().type()};
            std::vector<BaseNode::PortInfo> inputPorts( {inPort, WidthPort, heightPort, typePort} );
            BaseNode::PortInfo outPort = {"im", true, ImageData().type()};
            std::vector<BaseNode::PortInfo> outputPorts({outPort}); 
            
            Init("ImageAllocate", inputPorts, outputPorts, true, "Image Allocate", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageAllocate(){}
		
        void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex)
        {
            if(portIndex == 0)
            {
                imageIn = std::dynamic_pointer_cast<ImageData>(portData);
            }
            else if(portIndex == 1)
            {
                width = std::dynamic_pointer_cast<IntData>(portData); 
            }
            else if(portIndex == 2)
            {
                height = std::dynamic_pointer_cast<IntData>(portData); 
            }
            else if(portIndex == 3)
            {
                type = std::dynamic_pointer_cast<ImageTypeData>(portData); 
            } 
            
            auto imageInL = imageIn.lock();
            auto widthL = width.lock();
            auto heightL = height.lock();
            auto typeL = type.lock();
            
            if(imageInL && widthL && heightL && typeL)
            {
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                imageOut = std::make_shared<ImageData>();
            }
            else
            {
                SetValidationState(QtNodes::NodeValidationState::Error, "input error");
                imageOut.reset(); 
            }
            Q_EMIT dataUpdated(0);
        }

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex)
        {
            return std::static_pointer_cast<QtNodes::NodeData>(imageOut);
        }
};

class ImageDeallocate: public BaseNode
{
	private:
        std::weak_ptr<ImageData> imageIn;

        std::shared_ptr<ImageData> imageOut;

	public:
		ImageDeallocate()
        {
            BaseNode::PortInfo inPort = {"im", true, ImageData().type()};
            std::vector<BaseNode::PortInfo> inputPorts( {inPort} );
            BaseNode::PortInfo outPort = {"im", true, ImageData().type()};
            std::vector<BaseNode::PortInfo> outputPorts({outPort}); 
            
            Init("ImageDeallocate", inputPorts, outputPorts, true, "Image Deallocate", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageDeallocate(){}
		
        void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex)
        {
            imageIn = std::dynamic_pointer_cast<ImageData>(portData);
            auto imageInL = imageIn.lock();
            if(imageInL)
            {
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                imageOut = std::make_shared<ImageData>();
            }
            else
            {
                SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
                imageOut.reset();
            }
            Q_EMIT dataUpdated(0);
        }

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex)
        {
            return std::static_pointer_cast<QtNodes::NodeData>(imageOut);
        }
};

class ImageGetDims: public BaseNode
{
	private:
        std::weak_ptr<ImageData> imageIn;

		std::shared_ptr<IntData> width;
		std::shared_ptr<IntData> height;
        std::shared_ptr<ImageTypeData> type;

	public:
		ImageGetDims()
        {
            BaseNode::PortInfo inPort = {"im", true, ImageData().type()};
            std::vector<BaseNode::PortInfo> inputPorts( {inPort} );
            BaseNode::PortInfo outWidthPort = {"W", true, IntData().type()};
            BaseNode::PortInfo outHeightPort = {"H", true, IntData().type()};
            BaseNode::PortInfo outTypePort = {"T", true, ImageTypeData().type()};
            std::vector<BaseNode::PortInfo> outputPorts({outWidthPort, outHeightPort, outTypePort}); 
            
            Init("ImageGetDims", inputPorts, outputPorts, true, "Image Get Dims", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageGetDims(){}
		
        void setInData(std::shared_ptr<QtNodes::NodeData> portData, int portIndex)
        {
            imageIn = std::dynamic_pointer_cast<ImageData>(portData);
            auto imageInL = imageIn.lock();
            if(imageInL)
            {
                SetValidationState(QtNodes::NodeValidationState::Valid, ""); 
                width = std::make_shared<IntData>();
                height = std::make_shared<IntData>();
                type = std::make_shared<ImageTypeData>(); 
            }
            else
            {
                SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
                width.reset();
                height.reset(); 
                type.reset(); 
            }
            Q_EMIT dataUpdated(0);
            Q_EMIT dataUpdated(1);
            Q_EMIT dataUpdated(2);
        }

        std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex portIndex)
        {
            if(portIndex == 0 )
            {
                return std::static_pointer_cast<QtNodes::NodeData>(width);
            }
            else if(portIndex == 1 )
            {
                return std::static_pointer_cast<QtNodes::NodeData>(height);
            }
            else if(portIndex == 2 )
            {
                return std::static_pointer_cast<QtNodes::NodeData>(type);
            }
        }
};

}
}
}
}