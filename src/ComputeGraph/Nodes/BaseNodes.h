#pragma once

#include "Visi_export.h"

#include "Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"

namespace Visi
{

namespace ComputeGraph
{
		
/**
 */
	
class VISI_EXPORT IntSource: public Node
{        
    private:
        int value;

	public:
        Data GetOutput(int inx)
        {
            return {IntData, &value};
        }
};

/**
 */
	
class VISI_EXPORT FloatSource: public Node
{        
    private:
        float value;

	public:
        Data GetOutput(int inx)
        {
            return {FloatData, &value};
        }
};

/**
 */
	
class VISI_EXPORT BoolSource: public Node
{        
    private:
        bool value;

	public:
        Data GetOutput(int inx)
        {
            return {BoolData, &value};
        }
};

	
}

}