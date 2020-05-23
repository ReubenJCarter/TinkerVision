#pragma once

#include "Node.h"

#include "Nodes/SourceNodes.h"
#include "Nodes/ImageNodes.h"
#include "Nodes/IONodes.h"
#include "Nodes/ProcessNodes.h"

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