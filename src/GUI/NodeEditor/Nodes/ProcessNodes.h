#pragma once

#include "../BaseNode.h"

#include "../../../Process/AdaptiveThreshold.h"
#include "../../../Process/ApproxDistanceTransform.h"
#include "../../../Process/AverageFilter.h"
#include "../../../Process/BilateralFilter.h"
#include "../../../Process/Blend.h"
#include "../../../Process/BrightnessContrast.h"
#include "../../../Process/CameraDistortion.h"
#include "../../../Process/ChannelDemux.h"
#include "../../../Process/ChannelMapper.h"
#include "../../../Process/ChannelMux.h"
#include "../../../Process/CircleHoughTransform.h"
#include "../../../Process/ClearColor.h"
#include "../../../Process/Convolution.h"
#include "../../../Process/CopyImage.h"
#include "../../../Process/Crop.h"
#include "../../../Process/Downsample.h"
#include "../../../Process/FindContours.h"
#include "../../../Process/GaussianBlur.h"
#include "../../../Process/GaussianDerivative.h"
#include "../../../Process/GaussianFunction.h"
#include "../../../Process/GrayScale.h"
#include "../../../Process/HarrisShiTomasiResponse.h"
#include "../../../Process/HighLowThreshold.h"
#include "../../../Process/HSVToRGB.h"
#include "../../../Process/HysteresisEdgeThreshold.h"
#include "../../../Process/InRange.h"
#include "../../../Process/Invert.h"
#include "../../../Process/LocalMaxima.h"
#include "../../../Process/MarkerBitExtract.h"
#include "../../../Process/MarkerDictionary.h"
#include "../../../Process/MedianFilter.h"
#include "../../../Process/Mipmaps.h"
#include "../../../Process/MorphRect.h"
#include "../../../Process/MorphThinning.h"
#include "../../../Process/NonMaximumEdgeSuppression.h"
#include "../../../Process/Normalize.h"
#include "../../../Process/OtsuThreshold.h"
#include "../../../Process/Renderer.h"
#include "../../../Process/RGBToHSV.h"
#include "../../../Process/Rotate.h"
#include "../../../Process/Sobel.h"
#include "../../../Process/StereoMatchSAD.h"
#include "../../../Process/StructureTensor.h"
#include "../../../Process/TemplateMatch.h"
#include "../../../Process/Threshold.h"
#include "../../../Process/Translate.h"
#include "../../../Process/Upsample.h"

namespace TnkrVis
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::AdaptiveThreshold; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ApproxDistanceTransform; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::AverageFilter; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::Blend; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::BrightnessContrast; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::CameraDistortion; }
};

class ChannelDemux: public BaseProcess1In1Out
{
	public:
		ChannelDemux()
        {
            inputPorts.push_back({"ch", true, IntData().type(), true});
            
            Init("ChannelDemux", inputPorts, outputPorts, true, "Channel Demux", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ChannelDemux(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ChannelDemux; }
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
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ChannelMapper; }
};

class ChannelMux: public BaseProcess1In1Out
{
	public:
		ChannelMux()
        {
            inputPorts.push_back({"ch", true, IntData().type(), true});
            
            Init("ChannelMux", inputPorts, outputPorts, true, "Channel Mux", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ChannelMux(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ChannelMux; }
};

class CircleHoughTransform: public BaseProcess1In1Out
{
	public:
		CircleHoughTransform()
        {            
            Init("CircleHoughTransform", inputPorts, outputPorts, true, "Circle Hough Transform", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~CircleHoughTransform(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::CircleHoughTransform; }
};

class ClearColor: public BaseProcess1In1Out
{
	public:
		ClearColor()
        {
            inputPorts.push_back({"r", true, FloatData().type(), true});
            inputPorts.push_back({"g", true, FloatData().type(), true});
            inputPorts.push_back({"b", true, FloatData().type(), true});
            inputPorts.push_back({"a", true, FloatData().type(), false});
            
            Init("ClearColor", inputPorts, outputPorts, true, "Clear Color", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~ClearColor(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::ClearColor; }
};

class Convolution: public BaseNode
{        
	public:
		Convolution()
        {
            std::vector<BaseNode::InputPortInfo> inputPorts({{"dst", true, ImageData().type(), true},
                                                        {"src", true, ImageData().type(), true},
                                                        {"conv", true, ImageData().type(), true}
                                                        });

            std::vector<BaseNode::OutputPortInfo> outputPorts({ {"im", true, ImageData().type()} }); 
            
            Init("Convolution", inputPorts, outputPorts, true, "Convolution", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~Convolution(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::Convolution; }
};

class CopyImage: public BaseProcess1In1Out
{
	public:
		CopyImage()
        {
           
            inputPorts.push_back({"format translate", true, BoolData().type(), false}); 
            inputPorts.push_back({"use out's size", true, BoolData().type(), false}); 
            inputPorts.push_back({"offsetX", true, IntData().type(), false}); 
            inputPorts.push_back({"offsetY", true, IntData().type(), false});
            
            Init("CopyImage", inputPorts, outputPorts, true, "Copy Image", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~CopyImage(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::CopyImage; }
};

class Crop: public BaseProcess1In1Out
{
	public:
		Crop()
        {
            inputPorts.push_back({"offsetX", true, IntData().type(), false}); 
            inputPorts.push_back({"offsetY", true, IntData().type(), false});
            
            Init("Crop", inputPorts, outputPorts, true, "Crop", false); 
            SetValidationState(QtNodes::NodeValidationState::Error, "input error"); 
        }
		virtual ~Crop(){}	
        ComputeGraph::Node* GetComputeNode() { return new ComputeGraph::Nodes::Crop; }
};


}
}
}
}