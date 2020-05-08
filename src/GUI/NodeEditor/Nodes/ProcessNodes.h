#pragma once

#include "../BaseNode.h"

#include "../../../ComputeGraph/Nodes/ProcessNodes.h"

namespace Viso
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

class ApproxDistanceTransform: public BaseProcess1In1Out
{
	public:
		ApproxDistanceTransform()
        {
            cpuOnly = true;
            inputPorts.push_back( {"thresh", true, FloatData().type(), true} );
            inputPorts.push_back( {"size", true, IntData().type(), true} );
            
            Init("ApproxDistanceTransform", inputPorts, outputPorts, true, "Approx Distance Transform", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ApproxDistanceTransform(){}	
};

class AverageFilter: public BaseProcess1In1Out
{
	public:
		AverageFilter()
        {
            inputPorts.push_back(  {"size", true, IntData().type(), true} );
            
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

class BrightnessContrast: public BaseProcess1In1Out
{
	public:
		BrightnessContrast()
        {
            inputPorts.push_back( {"brightness", true, IntData().type(), true} );
            inputPorts.push_back( {"contrast", true, IntData().type(), true} );
            
            Init("BrightnessContrast", inputPorts, outputPorts, true, "Brightness Contrast", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~BrightnessContrast(){}	
};

class CameraDistortion: public BaseProcess1In1Out
{
	public:
		CameraDistortion()
        {
            inputPorts.push_back({"k0", true, FloatData().type(), true});
            inputPorts.push_back({"k1", true, FloatData().type(), true});
            inputPorts.push_back({"k2", true, FloatData().type(), true});
            inputPorts.push_back({"p0", true, FloatData().type(), true});
            inputPorts.push_back({"p1", true, FloatData().type(), true});
            inputPorts.push_back({"fl", true, FloatData().type(), true});
            
            Init("CameraDistortion", inputPorts, outputPorts, true, "Camera Distortion", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~CameraDistortion(){}	
};

class ChannelMapper: public BaseProcess1In1Out
{
	public:
		ChannelMapper()
        {
            inputPorts.push_back({"rSrc", true, IntData().type(), true});
            inputPorts.push_back({"gSrc", true, IntData().type(), true});
            inputPorts.push_back({"bSrc", true, IntData().type(), true});
            inputPorts.push_back({"aSrc", true, IntData().type(), true});
            
            Init("ChannelMapper", inputPorts, outputPorts, true, "Channel Mapper", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ChannelMapper(){}	
};

class ClearColor: public BaseProcess1In1Out
{
	public:
		ClearColor()
        {
            inputPorts.push_back({"color", true, IntData().type(), true});
            
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