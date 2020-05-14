#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"

namespace TnkrVis
{
namespace ComputeGraph
{
namespace Nodes
{

class MathExpression: public Node
{
    TNKRVIS_CLONEABLE_MACRO(MathExpression) 
    private:
        Data outData; /** cache of the output data, also null data when no image avilible.*/

	public:
        Data GetOutput(int inx){ return outData; }

        void Run()
        {
            std::string inFileName = GetInputData(0).DerefAsType<std::string>(StringData, "y=x");            
            
            {
                outData = Data(NullData, NULL); 
            }
        }
};

}
}
}