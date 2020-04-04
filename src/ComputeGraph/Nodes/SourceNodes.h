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
        
    public:
        T value; 

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
    VISI_CLONEABLE_MACRO(IntSource)
    public:
        IntSource(): BaseSource<int>(IntData){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetInt("val", value);}
};

/**
 */
	
class FloatSource: public BaseSource<float>
{   
    VISI_CLONEABLE_MACRO(FloatSource)     
    public:
        FloatSource(): BaseSource<float>(FloatData){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetFloat("val", value);}
};

/**
 */
	
class BoolSource: public BaseSource<bool>
{        
    VISI_CLONEABLE_MACRO(BoolSource)  
	public:
        BoolSource(): BaseSource<bool>(BoolData){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetBool("val", value);}
};

/**
 */
	
class Vec2Source: public BaseSource<Vec2>
{   
    VISI_CLONEABLE_MACRO(Vec2Source)       
	public:
        Vec2Source(): BaseSource<Vec2>(Vec2Data){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetVec2("val", value);}
};

/**
 */
	
class Vec3Source: public BaseSource<Vec3>
{      
    VISI_CLONEABLE_MACRO(Vec3Source)    
	public:
        Vec3Source(): BaseSource<Vec3>(Vec3Data){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetVec3("val", value);}
};

/**
 */
	
class Vec4Source: public BaseSource<Vec4>
{      
    VISI_CLONEABLE_MACRO(Vec4Source)    
	public:
        Vec4Source(): BaseSource<Vec4>(Vec4Data){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetVec4("val", value);}
};

/**
 */
	
class Mat2Source: public BaseSource<Mat2>
{     
    VISI_CLONEABLE_MACRO(Mat2Source)     
	public:
        Mat2Source(): BaseSource<Mat2>(Mat2Data){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetMat2("val", value);}
};

/**
 */
	
class Mat3Source: public BaseSource<Mat3>
{   
    VISI_CLONEABLE_MACRO(Mat3Source)       
	public:
        Mat3Source(): BaseSource<Mat3>(Mat3Data){}
        virtual void Serialize(SerializedObject* sObj){sObj->SetMat3("val", value);}
};

/**
 */
	
class ColorSource: public BaseSource<Color>
{    
    VISI_CLONEABLE_MACRO(ColorSource)      
	public:
        ColorSource(): BaseSource<Color>(ColorData){}
};

/**
 */
	
class ImageTypeSource: public BaseSource<ImageType>
{        
    VISI_CLONEABLE_MACRO(ImageTypeSource)  
	public:
        ImageTypeSource(): BaseSource<ImageType>(ImageTypeData){}
};

/**
 */
	
class ImageSource: public BaseSource<Image>
{   
    VISI_CLONEABLE_MACRO(ImageSource)       
	public:
        ImageSource(): BaseSource<Image>(ImageData){}
};

/**
 */
	
class ImageGPUSource: public BaseSource<ImageGPU>
{       
    VISI_CLONEABLE_MACRO(ImageGPUSource)   
	public:
        ImageGPUSource(): BaseSource<ImageGPU>(ImageGPUData){}
};

}
}
}