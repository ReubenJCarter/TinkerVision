#pragma once

#include "Node.h"

#include "Nodes/SourceNodes.h"
#include "Nodes/ImageNodes.h"
#include "Nodes/IONodes.h"
#include "Nodes/ProcessNodes.h"

namespace Visi
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
    Node::RegisterType<Nodes::AdaptiveThresholdNode>(); 
    Node::RegisterType<Nodes::ApproxDistanceTransformNode>();
    Node::RegisterType<Nodes::BrightnessContrastNode>();
    Node::RegisterType<Nodes::CameraDistortionNode>();

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