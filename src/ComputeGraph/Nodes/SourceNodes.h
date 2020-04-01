#pragma once

#include "Visi_export.h"

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
	
class VISI_EXPORT IntSource: public BaseSource<int>
{        
    public:
        IntSource(): BaseSource<int>(IntData){}
};

/**
 */
	
class VISI_EXPORT FloatSource: public BaseSource<float>
{        
    public:
        FloatSource(): BaseSource<float>(FloatData){}
};

/**
 */
	
class VISI_EXPORT BoolSource: public BaseSource<bool>
{        
	public:
        BoolSource(): BaseSource<bool>(BoolData){}
};

/**
 */
	
class VISI_EXPORT Vec2Source: public BaseSource<Vec2>
{        
	public:
        Vec2Source(): BaseSource<Vec2>(Vec2Data){}
};

/**
 */
	
class VISI_EXPORT Vec3Source: public BaseSource<Vec3>
{        
	public:
        Vec3Source(): BaseSource<Vec3>(Vec3Data){}
};

/**
 */
	
class VISI_EXPORT Vec4Source: public BaseSource<Vec4>
{        
	public:
        Vec4Source(): BaseSource<Vec4>(Vec4Data){}
};

/**
 */
	
class VISI_EXPORT Mat2Source: public BaseSource<Mat2>
{        
	public:
        Mat2Source(): BaseSource<Mat2>(Mat2Data){}
};

/**
 */
	
class VISI_EXPORT Mat3Source: public BaseSource<Mat3>
{        
	public:
        Mat3Source(): BaseSource<Mat3>(Mat3Data){}
};

/**
 */
	
class VISI_EXPORT ColorSource: public BaseSource<Color>
{        
	public:
        ColorSource(): BaseSource<Color>(ColorData){}
};

/**
 */
	
class VISI_EXPORT ImageTypeSource: public BaseSource<ImageType>
{        
	public:
        ImageTypeSource(): BaseSource<ImageType>(ImageTypeData){}
};

/**
 */
	
class VISI_EXPORT ImageSource: public BaseSource<Image>
{        
	public:
        ImageSource(): BaseSource<Image>(ImageData){}
};

/**
 */
	
class VISI_EXPORT ImageGPUSource: public BaseSource<ImageGPU>
{        
	public:
        ImageGPUSource(): BaseSource<ImageGPU>(ImageGPUData){}
};

}

}

}