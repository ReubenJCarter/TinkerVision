#pragma once

#include "../BaseNode.h"

#include "../DataTypes.h"

#include "../../../ComputeGraph/Nodes/ImageNodes.h"

namespace TnkrVis
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
            std::vector<BaseNode::InputPortInfo> inputPorts( { {"im", true, ImageData().type(), true}, 
                                                          {"W", true, IntData().type(), true}, 
                                                          {"H", true, IntData().type(), true}, 
                                                          {"T", true, ImageTypeData().type(), true}} );

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ImageAllocate", inputPorts, outputPorts, true, "Image Allocate", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageAllocate(){}	

        ComputeGraph::Node* GetComputeNode() 
        {
            return new ComputeGraph::Nodes::ImageAllocate;  
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
            std::vector<BaseNode::InputPortInfo> inputPorts( { {"im", true, ImageData().type(), true} } );
            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ImageDeallocate", inputPorts, outputPorts, true, "Image Deallocate", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageDeallocate(){}

        ComputeGraph::Node* GetComputeNode() 
        {
            return new ComputeGraph::Nodes::ImageDeallocate; 
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
            std::vector<BaseNode::InputPortInfo> inputPorts( { {"im", true, ImageData().type(), true} } );
            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"W", true, IntData().type()}, 
                                                                {"H", true, IntData().type()}, 
                                                                {"T", true, ImageTypeData().type()} }); 
            
            Init("ImageGetDims", inputPorts, outputPorts, true, "Image Get Dims", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }

		virtual ~ImageGetDims(){}

        ComputeGraph::Node* GetComputeNode() 
        {
            return new ComputeGraph::Nodes::ImageGetDims; 
        }
};

}
}
}
}