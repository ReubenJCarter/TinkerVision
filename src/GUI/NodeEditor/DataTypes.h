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
	private: 
		QtNodes::NodeDataType _type; 

    public:
		BaseNodeData(QtNodes::NodeDataType t ){_type = t;}
		BaseNodeData(std::string id, std::string name){_type = {id.c_str(), name.c_str()};}
		BaseNodeData(){_type = {"BaseNodeData", "BaseNodeData"};}
        virtual std::string ToString(){ return ""; }
        virtual QtNodes::NodeDataType type() const {return _type;};
		std::string TypeId(); 
};

class BaseImageData: public BaseNodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"baseImageData", "BaseImageData"};};
        std::string ToString(){ return "BaseImageData"; }
};

class ImageData: public BaseImageData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"baseImageData", "ImageData"};};
        std::string ToString(){ return "ImageData"; }
};

class ImageGPUData: public BaseImageData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"baseImageData", "ImageGPUData"};};
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