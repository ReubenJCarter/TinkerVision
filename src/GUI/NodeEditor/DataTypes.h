#pragma once 

#include <nodes/NodeData>

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
    
class BaseNodeData : public QtNodes::NodeData
{
    public:
        virtual std::string ToString(){ return ""; }
        virtual QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"baseNodeData", "BaseNodeData"};};
};

class ImageData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageData", "ImageData"};};
        std::string ToString(){ return "ImageData"; }
};

class ImageGPUData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageGPUData", "ImageGPUData"};};
        std::string ToString(){ return "ImageGPUData"; }
};

class ImageTypeData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageTypeData", "ImageTypeData"};};
		std::string ToString(){ return "ImageTypeData"; }
};

class IntData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"intData", "IntData"};};
		std::string ToString(){ return "IntData"; }
};

class FloatData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"floatData", "FloatData"};};
		std::string ToString(){ return "FloatData"; }
};

class BoolData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"boolData", "BoolData"};};
		std::string ToString(){ return "BoolData"; }
};

class StringData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"stringData", "StringData"};};
		std::string ToString(){ return "StringData"; }
};

}	
}
}