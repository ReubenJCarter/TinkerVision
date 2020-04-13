#pragma once

#include "DataTypes.h"

namespace Visi
{
namespace GUI
{
namespace NodeEditor
{
	
template<class T>
class ToTextConverter
{
	private:
		std::shared_ptr<QtNodes::NodeData> str;
		
	public:
		std::shared_ptr<QtNodes::NodeData> operator()(std::shared_ptr<QtNodes::NodeData> data)
		{
			
			auto inData = std::dynamic_pointer_cast<T>(data);
			if(inData)
			{
				std::string s = inData->ToString();
				str = std::make_shared<StringData>(s);
			}
			else
			{
				str.reset();
			}
			return str;
		}
};

}
}
}