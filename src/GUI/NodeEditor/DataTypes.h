#pragma once 

#include <nodes/NodeData>

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
    
class ImageData : public QtNodes::NodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageData", "ImageData"};};
};

class ImageGPUData : public QtNodes::NodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageGPUData", "ImageGPUData"};};
};

class ImageTypeData : public QtNodes::NodeData
{
	public:
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageTypeData", "ImageTypeData"};};
};

class IntData : public QtNodes::NodeData
{
    private:
        int data; 

	public:
        IntData(){ data = 0; }
        IntData(int d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"intData", "IntData"};};
        int Data(){ return data; }
		std::string ToString(){ std::to_string(data); }
};

class FloatData : public QtNodes::NodeData
{
    private:
        float data; 

	public:
        FloatData(){ data = 0; }
        FloatData(int d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"floatData", "FloatData"};};
        float Data(){ return data; }
		std::string ToString(){ std::to_string(data); }
};

class BoolData : public QtNodes::NodeData
{
    private:
        bool data; 
        
	public:
        BoolData(){ data = 0; }
        BoolData(int d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"boolData", "BoolData"};};
        bool Data(){ return data; }
		std::string ToString(){ std::to_string(data); }
};

}	
}
}