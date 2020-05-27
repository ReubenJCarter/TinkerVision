#include "Graph.h"

#include "../ComputeGraph/SourceNodes.h"
#include "../ComputeGraph/ImageNodes.h"

#include "../IO/ImageFile.h"

#include "../Process/AdaptiveThreshold.h"
#include "../Process/ApproxDistanceTransform.h"
#include "../Process/AverageFilter.h"
#include "../Process/BilateralFilter.h"
#include "../Process/Blend.h"
#include "../Process/BrightnessContrast.h"
#include "../Process/CameraDistortion.h"
#include "../Process/ChannelDemux.h"
#include "../Process/ChannelMapper.h"
#include "../Process/ChannelMux.h"
#include "../Process/CircleHoughTransform.h"
#include "../Process/ClearColor.h"
#include "../Process/Convolution.h"
#include "../Process/CopyImage.h"
#include "../Process/Crop.h"
#include "../Process/Downsample.h"
#include "../Process/FindContours.h"
#include "../Process/GaussianBlur.h"
#include "../Process/GaussianDerivative.h"
#include "../Process/GaussianFunction.h"
#include "../Process/GrayScale.h"
#include "../Process/HarrisShiTomasiResponse.h"
#include "../Process/HighLowThreshold.h"
#include "../Process/HSVToRGB.h"
#include "../Process/HysteresisEdgeThreshold.h"
#include "../Process/InRange.h"
#include "../Process/Invert.h"
#include "../Process/LocalMaxima.h"
#include "../Process/MarkerBitExtract.h"
#include "../Process/MarkerDictionary.h"
#include "../Process/MedianFilter.h"
#include "../Process/Mipmaps.h"
#include "../Process/MorphRect.h"
#include "../Process/MorphThinning.h"
#include "../Process/NonMaximumEdgeSuppression.h"
#include "../Process/Normalize.h"
#include "../Process/OtsuThreshold.h"
#include "../Process/Renderer.h"
#include "../Process/RGBToHSV.h"
#include "../Process/Rotate.h"
#include "../Process/Sobel.h"
#include "../Process/StereoMatchSAD.h"
#include "../Process/StructureTensor.h"
#include "../Process/TemplateMatch.h"
#include "../Process/Threshold.h"
#include "../Process/Translate.h"
#include "../Process/Upsample.h"

namespace TnkrVis
{
namespace ComputeGraph
{

std::map<std::string, Node*>  Graph::nodeTypes; 

void Graph::RegisterNodes()
{
    //Image nodes
    RegisterType<Nodes::ImageAllocate>(); 
    RegisterType<Nodes::ImageDeallocate>(); 
    RegisterType<Nodes::ImageGetDims>(); 
    RegisterType<Nodes::ImageCopy>(); 

    //IONodes
    RegisterType<Nodes::ImageFileRead>(); 
    RegisterType<Nodes::ImageFileWrite>();

    //Process nodes
    RegisterType<Nodes::AdaptiveThreshold>(); 
    RegisterType<Nodes::ApproxDistanceTransform>();
    RegisterType<Nodes::AverageFilter>(); 
    RegisterType<Nodes::BilateralFilter>(); 
    RegisterType<Nodes::Blend>();
    RegisterType<Nodes::BrightnessContrast>();
    RegisterType<Nodes::CameraDistortion>();
    RegisterType<Nodes::ChannelDemux>();
    RegisterType<Nodes::ChannelMapper>();
    RegisterType<Nodes::ChannelMux>();
    RegisterType<Nodes::CircleHoughTransform>();
    RegisterType<Nodes::ClearColor>();
    RegisterType<Nodes::Convolution>();
    RegisterType<Nodes::CopyImage>();
    RegisterType<Nodes::Crop>();
    RegisterType<Nodes::Downsample>();
    RegisterType<Nodes::FindContours>(); 
    RegisterType<Nodes::GaussianBlur>(); 
    RegisterType<Nodes::GaussianDerivative>(); 
    RegisterType<Nodes::GaussianFunction>(); 
    RegisterType<Nodes::GrayScale>(); 
    RegisterType<Nodes::HarrisShiTomasiResponse>(); 
    RegisterType<Nodes::HighLowThreshold>(); 
    RegisterType<Nodes::HSVToRGB>(); 
    RegisterType<Nodes::HysteresisEdgeThreshold>(); 
    RegisterType<Nodes::InRange>(); 
    RegisterType<Nodes::Invert>(); 
    RegisterType<Nodes::LocalMaxima>(); 
    RegisterType<Nodes::MarkerBitExtract>(); 
    RegisterType<Nodes::MedianFilter>(); 
    RegisterType<Nodes::Mipmaps>(); 
    RegisterType<Nodes::MorphRect>(); 
    RegisterType<Nodes::MorphThinning>(); 
    RegisterType<Nodes::NonMaximumEdgeSuppression>(); 
    RegisterType<Nodes::OtsuThreshold>(); 
    RegisterType<Nodes::RGBToHSV>(); 
    RegisterType<Nodes::Rotate>(); 
    RegisterType<Nodes::Sobel>(); 
    RegisterType<Nodes::StereoMatchSAD>(); 
    RegisterType<Nodes::StructureTensor>(); 
    RegisterType<Nodes::TemplateMatch>(); 
    RegisterType<Nodes::Threshold>(); 
    RegisterType<Nodes::Translate>(); 
    RegisterType<Nodes::Upsample>(); 

    //Source nodes
    RegisterType<Nodes::StringSource>(); 
    RegisterType<Nodes::IntSource>(); 
    RegisterType<Nodes::FloatSource>(); 
    RegisterType<Nodes::BoolSource>(); 
    RegisterType<Nodes::Vec2Source>(); 
    RegisterType<Nodes::Vec3Source>(); 
    RegisterType<Nodes::Vec4Source>(); 
    RegisterType<Nodes::Mat2Source>(); 
    RegisterType<Nodes::Mat3Source>(); 
    RegisterType<Nodes::ColorSource>(); 
    RegisterType<Nodes::ImageTypeSource>(); 
    RegisterType<Nodes::ImageSource>(); 
    RegisterType<Nodes::ImageGPUSource>(); 
}

}
}