#include "RegisterNodes.h"

#include "Node.h"

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

void RegisterNodes()
{
    //Image nodes
    Node::RegisterType<Nodes::ImageAllocate>(); 
    Node::RegisterType<Nodes::ImageDeallocate>(); 
    Node::RegisterType<Nodes::ImageGetDims>(); 
    Node::RegisterType<Nodes::ImageCopy>(); 

    //IONodes
    Node::RegisterType<Nodes::ImageFileRead>(); 
    Node::RegisterType<Nodes::ImageFileWrite>();

    //Process nodes
    Node::RegisterType<Nodes::AdaptiveThreshold>(); 
    Node::RegisterType<Nodes::ApproxDistanceTransform>();
    Node::RegisterType<Nodes::AverageFilter>(); 
    Node::RegisterType<Nodes::BilateralFilter>(); 
    Node::RegisterType<Nodes::Blend>();
    Node::RegisterType<Nodes::BrightnessContrast>();
    Node::RegisterType<Nodes::CameraDistortion>();
    Node::RegisterType<Nodes::ChannelDemux>();
    Node::RegisterType<Nodes::ChannelMapper>();
    Node::RegisterType<Nodes::ChannelMux>();
    Node::RegisterType<Nodes::CircleHoughTransform>();
    Node::RegisterType<Nodes::ClearColor>();
    Node::RegisterType<Nodes::Convolution>();
    Node::RegisterType<Nodes::CopyImage>();
    Node::RegisterType<Nodes::Crop>();
    Node::RegisterType<Nodes::Downsample>();
    Node::RegisterType<Nodes::FindContours>(); 
    Node::RegisterType<Nodes::GaussianBlur>(); 
    Node::RegisterType<Nodes::GaussianDerivative>(); 
    Node::RegisterType<Nodes::GaussianFunction>(); 
    Node::RegisterType<Nodes::GrayScale>(); 
    Node::RegisterType<Nodes::HarrisShiTomasiResponse>(); 
    Node::RegisterType<Nodes::HighLowThreshold>(); 
    Node::RegisterType<Nodes::HSVToRGB>(); 
    Node::RegisterType<Nodes::HysteresisEdgeThreshold>(); 
    Node::RegisterType<Nodes::InRange>(); 
    Node::RegisterType<Nodes::Invert>(); 
    Node::RegisterType<Nodes::LocalMaxima>(); 
    Node::RegisterType<Nodes::MarkerBitExtract>(); 
    Node::RegisterType<Nodes::MedianFilter>(); 
    Node::RegisterType<Nodes::Mipmaps>(); 
    Node::RegisterType<Nodes::MorphRect>(); 
    Node::RegisterType<Nodes::MorphThinning>(); 
    Node::RegisterType<Nodes::NonMaximumEdgeSuppression>(); 
    Node::RegisterType<Nodes::OtsuThreshold>(); 
    Node::RegisterType<Nodes::RGBToHSV>(); 
    Node::RegisterType<Nodes::Rotate>(); 
    Node::RegisterType<Nodes::Sobel>(); 
    Node::RegisterType<Nodes::StereoMatchSAD>(); 
    Node::RegisterType<Nodes::StructureTensor>(); 
    Node::RegisterType<Nodes::TemplateMatch>(); 
    Node::RegisterType<Nodes::Threshold>(); 
    Node::RegisterType<Nodes::Translate>(); 
    Node::RegisterType<Nodes::Upsample>(); 

    //Source nodes
    Node::RegisterType<Nodes::StringSource>(); 
    Node::RegisterType<Nodes::IntSource>(); 
    Node::RegisterType<Nodes::FloatSource>(); 
    Node::RegisterType<Nodes::BoolSource>(); 
    Node::RegisterType<Nodes::Vec2Source>(); 
    Node::RegisterType<Nodes::Vec3Source>(); 
    Node::RegisterType<Nodes::Vec4Source>(); 
    Node::RegisterType<Nodes::Mat2Source>(); 
    Node::RegisterType<Nodes::Mat3Source>(); 
    Node::RegisterType<Nodes::ColorSource>(); 
    Node::RegisterType<Nodes::ImageTypeSource>(); 
    Node::RegisterType<Nodes::ImageSource>(); 
    Node::RegisterType<Nodes::ImageGPUSource>(); 
}

}
}