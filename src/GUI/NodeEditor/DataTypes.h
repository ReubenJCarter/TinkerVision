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
    private:
        std::string data; 

	public:
        ImageTypeData(){ data = ""; }
        ImageTypeData(std::string d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"imageTypeData", "ImageTypeData"};};
        std::string Data(){ return data; }
		std::string ToString(){ return data; }
};

class IntData: public BaseNodeData
{
    private:
        int data; 

	public:
        IntData(){ data = 0; }
        IntData(int d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"intData", "IntData"};};
        int Data(){ return data; }
		std::string ToString(){ return std::to_string(data); }
};

class FloatData: public BaseNodeData
{
    private:
        float data; 

	public:
        FloatData(){ data = 0; }
        FloatData(float d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"floatData", "FloatData"};};
        float Data(){ return data; }
		std::string ToString(){ return std::to_string(data); }
};

class BoolData: public BaseNodeData
{
    private:
        bool data; 
        
	public:
        BoolData(){ data = 0; }
        BoolData(bool d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"boolData", "BoolData"};};
        bool Data(){ return data; }
		std::string ToString(){ return std::to_string(data); }
};

class StringData: public BaseNodeData
{
    private:
        std::string data; 
        
	public:
        StringData(){ data = ""; }
        StringData(std::string d){ data = d; }
		QtNodes::NodeDataType type() const {return QtNodes::NodeDataType {"stringData", "StringData"};};
        std::string Data(){ return data; }
		std::string ToString(){ return data; }
};

}	
}
}