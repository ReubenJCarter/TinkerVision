#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"
#include "../../Core/VectorMath.h"

namespace Visi
{
namespace ComputeGraph
{
namespace Nodes
{

/** 
 */

template<class T>
class BaseSource: public Node
{
    protected:
        Data data; 
        T value; 

    public:
        BaseSource(DataType type)
        {
            data.ptr = &value; 
            data.type = type; 
        }

        Data GetOutput(int inx)
        {
            return data;
        }
}; 

/**
 */
	
class IntSource: public BaseSource<int>
{        
    public:
        IntSource(): BaseSource<int>(IntData){}
};

/**
 */
	
class FloatSource: public BaseSource<float>
{        
    public:
        FloatSource(): BaseSource<float>(FloatData){}
};

/**
 */
	
class BoolSource: public BaseSource<bool>
{        
	public:
        BoolSource(): BaseSource<bool>(BoolData){}
};

/**
 */
	
class Vec2Source: public BaseSource<Vec2>
{        
	public:
        Vec2Source(): BaseSource<Vec2>(Vec2Data){}
};

/**
 */
	
class Vec3Source: public BaseSource<Vec3>
{        
	public:
        Vec3Source(): BaseSource<Vec3>(Vec3Data){}
};

/**
 */
	
class Vec4Source: public BaseSource<Vec4>
{        
	public:
        Vec4Source(): BaseSource<Vec4>(Vec4Data){}
};

/**
 */
	
class Mat2Source: public BaseSource<Mat2>
{        
	public:
        Mat2Source(): BaseSource<Mat2>(Mat2Data){}
};

/**
 */
	
class Mat3Source: public BaseSource<Mat3>
{        
	public:
        Mat3Source(): BaseSource<Mat3>(Mat3Data){}
};

/**
 */
	
class ColorSource: public BaseSource<Color>
{        
	public:
        ColorSource(): BaseSource<Color>(ColorData){}
};

/**
 */
	
class ImageTypeSource: public BaseSource<ImageType>
{        
	public:
        ImageTypeSource(): BaseSource<ImageType>(ImageTypeData){}
};

/**
 */
	
class ImageSource: public BaseSource<Image>
{        
	public:
        ImageSource(): BaseSource<Image>(ImageData){}
};

/**
 */
	
class ImageGPUSource: public BaseSource<ImageGPU>
{        
	public:
        ImageGPUSource(): BaseSource<ImageGPU>(ImageGPUData){}
};

}
}
}