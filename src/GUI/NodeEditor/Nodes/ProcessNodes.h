#pragma once

#include "../BaseNode.h"

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
namespace Nodes 
{

class AdaptiveThreshold: public BaseProcess1In1Out
{
	public:
		AdaptiveThreshold()
        {
            
            inputPorts.push_back( {"thresh", true, FloatData().type(), true} );
            inputPorts.push_back( {"size", true, IntData().type(), true}  );
            
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
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"thresh", true, FloatData().type(), true},
                                                        {"size", true, IntData().type(), true} });

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
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"size", true, IntData().type(), true} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("AverageFilter", inputPorts, outputPorts, true, "Average Filter", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~AverageFilter(){}	
};

class Blend: public BaseNode
{
	public:
		Blend()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"bottom", true, ImageData().type(), true},
                                                        {"top", true, ImageData().type(), true},
                                                        {"mask", true, ImageData().type(), false},
                                                        {"mode", true, IntData().type(), true} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("Blend", inputPorts, outputPorts, true, "Blend", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~Blend(){}	
};

class BrightnessContrast: public BaseNode
{
	public:
		BrightnessContrast()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"brightness", true, IntData().type(), true},
                                                        {"contrast", true, IntData().type(), true} });

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
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"k0", true, FloatData().type(), true},
                                                        {"k1", true, FloatData().type(), true},
                                                        {"k2", true, FloatData().type(), true},
                                                        {"p0", true, FloatData().type(), true},
                                                        {"p1", true, FloatData().type(), true},
                                                        {"fl", true, FloatData().type(), true} });

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
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"rSrc", true, IntData().type(), true},
                                                        {"gSrc", true, IntData().type(), true},
                                                        {"bSrc", true, IntData().type(), true},
                                                        {"aSrc", true, IntData().type(), true} });

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
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"color", true, IntData().type(), true} });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("ClearColor", inputPorts, outputPorts, true, "Clear Color", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ClearColor(){}	
};

class CopyImage: public BaseNode
{
	public:
		CopyImage()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"format translate", true, BoolData().type(), true}, 
                                                        {"offsetX", true, IntData().type(), false}, 
                                                        {"offsetY", true, IntData().type(), false}, 
                                                        });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("CopyImage", inputPorts, outputPorts, true, "Copy Image", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~CopyImage(){}	
};

}
}
}
}