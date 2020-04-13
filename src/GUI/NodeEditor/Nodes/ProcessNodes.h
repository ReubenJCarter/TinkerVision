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

class AdaptiveThreshold: public BaseNode
{
	public:
		AdaptiveThreshold()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"thresh", true, FloatData().type()},
                                                        {"size", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("AdaptiveThreshold", inputPorts, outputPorts, true, "Adaptive Threshold", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~AdaptiveThreshold(){}	
};

class ApproxDistanceTransform: public BaseNode
{
	public:
		ApproxDistanceTransform()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"thresh", true, FloatData().type()},
                                                        {"size", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ApproxDistanceTransform", inputPorts, outputPorts, true, "Approx Distance Transform", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ApproxDistanceTransform(){}	
};

class AverageFilter: public BaseNode
{
	public:
		AverageFilter()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"size", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("AverageFilter", inputPorts, outputPorts, true, "Average Filter", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~AverageFilter(){}	
};

class BrightnessContrast: public BaseNode
{
	public:
		BrightnessContrast()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"brightness", true, IntData().type()},
                                                        {"contrast", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("BrightnessContrast", inputPorts, outputPorts, true, "Brightness Contrast", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~BrightnessContrast(){}	
};

class CameraDistortion: public BaseNode
{
	public:
		CameraDistortion()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"k0", true, FloatData().type()},
                                                        {"k1", true, FloatData().type()},
                                                        {"k2", true, FloatData().type()},
                                                        {"p0", true, FloatData().type()},
                                                        {"p1", true, FloatData().type()},
                                                        {"fl", true, FloatData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("CameraDistortion", inputPorts, outputPorts, true, "Camera Distortion", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~CameraDistortion(){}	
};

class ChannelMapper: public BaseNode
{
	public:
		ChannelMapper()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"rSrc", true, IntData().type()},
                                                        {"gSrc", true, IntData().type()},
                                                        {"bSrc", true, IntData().type()},
                                                        {"aSrc", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ChannelMapper", inputPorts, outputPorts, true, "Channel Mapper", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ChannelMapper(){}	
};

class ClearColor: public BaseNode
{
	public:
		ClearColor()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type()},
                                                        {"src", true, ImageData().type()},
                                                        {"color", true, IntData().type()} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ClearColor", inputPorts, outputPorts, true, "Clear Color", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ClearColor(){}	
};

}
}
}
}