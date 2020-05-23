#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"
#include "../../IO/ImageFile.h"

#include "../../Process/AdaptiveThreshold.h"
#include "../../Process/ApproxDistanceTransform.h"
#include "../../Process/AverageFilter.h"
#include "../../Process/Blend.h"
#include "../../Process/BrightnessContrast.h"
#include "../../Process/CameraDistortion.h"
#include "../../Process/ChannelDemux.h"
#include "../../Process/ChannelMapper.h"
#include "../../Process/ChannelMux.h"
#include "../../Process/CircleHoughTransform.h"
#include "../../Process/ClearColor.h"
#include "../../Process/Convolution.h"
#include "../../Process/CopyImage.h"
#include "../../Process/Crop.h"
#include "../../Process/Downsample.h"
#include "../../Process/FindContours.h"
#include "../../Process/FixedRadiusNN.h"
#include "../../Process/GaussianBlur.h"
#include "../../Process/GaussianDerivative.h"
#include "../../Process/GaussianFunction.h"
#include "../../Process/GrayScale.h"
#include "../../Process/HarrisShiTomasiResponse.h"
#include "../../Process/HighLowThreshold.h"
#include "../../Process/HSVToRGB.h"
#include "../../Process/HysteresisEdgeThreshold.h"
#include "../../Process/InRange.h"
#include "../../Process/Invert.h"
#include "../../Process/LocalMaxima.h"
#include "../../Process/MarkerBitExtract.h"
#include "../../Process/MarkerDictionary.h"
#include "../../Process/MedianFilter.h"
#include "../../Process/Mipmaps.h"
#include "../../Process/MorphRect.h"
#include "../../Process/NonMaximumEdgeSuppression.h"
#include "../../Process/Normalize.h"
#include "../../Process/OtsuThreshold.h"
#include "../../Process/Perspective3Point.h"
#include "../../Process/Renderer.h"
#include "../../Process/RGBToHSV.h"
#include "../../Process/Rotate.h"
#include "../../Process/Sobel.h"
#include "../../Process/StereoMatchSAD.h"
#include "../../Process/StructureTensor.h"
#include "../../Process/TemplateMatch.h"
#include "../../Process/Thinning.h"
#include "../../Process/Threshold.h"
#include "../../Process/Translate.h"
#include "../../Process/Upsample.h"


namespace TnkrVis
{
namespace ComputeGraph
{
namespace Nodes
{

template<class T>
class BaseProcess1In1OutCPUOnly: public Node
{
    protected:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/
        T proc;
    
    public: 
        Data GetOutput(int inx){ return outImageData; }

        inline virtual void SetParams()
        {
            
        }

        void Run()
        {
            SetParams(); 
            
            Image* inDstAsimage = GetInputData(0).AsType<Image>(ImageData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);  
                        
            if(inSrcAsimage != NULL && inDstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, inDstAsimage);
                outImageData = Data(DataType::ImageData, inDstAsimage); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

template<class T>
class BaseProcess1In1Out: public Node
{
    protected:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/
        T proc;
    
    public: 
        Data GetOutput(int inx){ return outImageData; }

        inline virtual void SetParams()
        {
            
        }

        void Run()
        {
            SetParams(); 

            Image* inDstAsimage = GetInputData(0).AsType<Image>(ImageData);
            ImageGPU* inDstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);  
            ImageGPU* inSrcAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  
                        
            if(inSrcAsimage != NULL && inDstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, inDstAsimage);
                outImageData = Data(DataType::ImageData, inDstAsimage); 
            }
            else if(inSrcAsimageGPU != NULL && inDstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, inDstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, inDstAsimageGPU);
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 







class AdaptiveThreshold: public BaseProcess1In1Out<Process::AdaptiveThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(AdaptiveThreshold) 
    public:
        void SetParams() 
        {
            proc.SetThreshold( GetInputData(2).DerefAsType<float>(FloatData, 0.5f) );
            proc.SetSize( GetInputData(3).DerefAsType<int>(IntData, 7) );
        }
};

class ApproxDistanceTransform: public BaseProcess1In1OutCPUOnly<Process::ApproxDistanceTransform>
{
    TNKRVIS_CLONEABLE_MACRO(ApproxDistanceTransform) 
}; 

class AverageFilter: public BaseProcess1In1Out<Process::AverageFilter>
{
    TNKRVIS_CLONEABLE_MACRO(AverageFilter) 
    public:
        void SetParams() 
        {
            proc.SetSize( GetInputData(2).DerefAsType<int>(IntData, 0) );
        }
}; 

class Blend: public Node
{
    TNKRVIS_CLONEABLE_MACRO(Blend) 
    private:
        Data outImageData;
        Process::Blend proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetMode( (Process::Blend::BlendMode)GetInputData(4).DerefAsType<int>(IntData, 0) ); 

            Image* bottomAsimage = GetInputData(0).AsType<Image>(ImageData);
            ImageGPU* bottomAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* topAsimage = GetInputData(1).AsType<Image>(ImageData);  
            ImageGPU* topAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* dstAsimage = GetInputData(2).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

            Image* maskAsimage = GetInputData(3).AsType<Image>(ImageData);  
            ImageGPU* maskAsimageGPU = GetInputData(3).AsType<ImageGPU>(ImageGPUData);  
                        
            if(bottomAsimage != NULL && topAsimage != NULL && dstAsimage != NULL)
            {
                if(maskAsimage != NULL)
                {
                    proc.Run(topAsimage, bottomAsimage, dstAsimage, maskAsimage);
                    outImageData = Data(DataType::ImageData, dstAsimage); 
                }
                else
                {
                    proc.Run(topAsimage, bottomAsimage, dstAsimage);
                    outImageData = Data(DataType::ImageData, dstAsimage); 
                }
            }
            else if(bottomAsimageGPU != NULL && topAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                if(maskAsimageGPU != NULL)
                {
                    proc.Run(topAsimageGPU, bottomAsimageGPU, dstAsimageGPU, maskAsimageGPU);
                    outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
                }
                else
                {
                    proc.Run(topAsimageGPU, bottomAsimageGPU, dstAsimageGPU);
                    outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
                }
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }

};

class BrightnessContrast: public BaseProcess1In1Out<Process::BrightnessContrast>
{
    TNKRVIS_CLONEABLE_MACRO(BrightnessContrast) 
    public:
        void SetParams() 
        {
            proc.SetBrightness( GetInputData(2).DerefAsType<float>(FloatData, 0) );
            proc.SetContrast( GetInputData(3).DerefAsType<float>(FloatData, 1) );
        }
}; 

class CameraDistortion: public BaseProcess1In1Out<Process::CameraDistortion>
{
    TNKRVIS_CLONEABLE_MACRO(CameraDistortion) 
    public:
        void SetParams() 
        {
            proc.SetRadialCoefs( GetInputData(2).DerefAsType<float>(FloatData, 0), 
                                 GetInputData(3).DerefAsType<float>(FloatData, 0), 
                                 GetInputData(4).DerefAsType<float>(FloatData, 0) );
            proc.SetTangentialCoefs( GetInputData(5).DerefAsType<float>(FloatData, 0),
                                     GetInputData(6).DerefAsType<float>(FloatData, 0) );
            proc.SetFocalLength( GetInputData(7).DerefAsType<float>(FloatData, 1) );
        }
}; 

class ChannelDemux: public BaseProcess1In1Out<Process::ChannelDemux>
{
    TNKRVIS_CLONEABLE_MACRO(ChannelDemux) 
    public:
        void SetParams() 
        {
            proc.SetChannel( GetInputData(2).DerefAsType<int>(IntData, 0));
        }
}; 

class ChannelMapper: public BaseProcess1In1Out<Process::ChannelMapper>
{
    TNKRVIS_CLONEABLE_MACRO(ChannelMapper) 
    public:
        void SetParams() 
        {
            proc.SetChannelMap( GetInputData(2).DerefAsType<int>(IntData, 0), 
                                GetInputData(3).DerefAsType<int>(IntData, 1), 
                                GetInputData(4).DerefAsType<int>(IntData, 2),
                                GetInputData(5).DerefAsType<int>(IntData, 3));
        }
}; 

class ChannelMux: public BaseProcess1In1Out<Process::ChannelMux>
{
    TNKRVIS_CLONEABLE_MACRO(ChannelMux) 
    public:
        void SetParams() 
        {
            proc.SetChannel( GetInputData(2).DerefAsType<int>(IntData, 0) );
        }
}; 

class CircleHoughTransform: public BaseProcess1In1Out<Process::CircleHoughTransform>
{
    TNKRVIS_CLONEABLE_MACRO(CircleHoughTransform) 
    public:
        void SetParams() 
        {
        }
}; 

class ClearColor: public Node
{
    TNKRVIS_CLONEABLE_MACRO(ClearColor) 
    private:
        Data outImageData;
        Process::ClearColor proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetColor(GetInputData(1).DerefAsType<float>(FloatData, 0), 
                          GetInputData(2).DerefAsType<float>(FloatData, 0), 
                          GetInputData(3).DerefAsType<float>(FloatData, 0),
                          GetInputData(4).DerefAsType<float>(FloatData, 1)); 

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
                        
            if(dstAsimage != NULL)
            {
                proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(dstAsimageGPU != NULL)
            {
                proc.Run( dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

class Convolution:  public Node
{
    TNKRVIS_CLONEABLE_MACRO(Convolution) 
    private:
        Data outImageData;
        Process::Convolution proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {

             Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);
            ImageGPU* inSrcAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* convAsimage = GetInputData(2).AsType<Image>(ImageData);  
            ImageGPU* convAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

           
                        
            if(inSrcAsimage != NULL && convAsimage != NULL && dstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, convAsimage, dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(inSrcAsimageGPU != NULL && convAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, convAsimageGPU, dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

class CopyImage: public BaseProcess1In1Out<Process::CopyImage>
{
    TNKRVIS_CLONEABLE_MACRO(CopyImage) 
    public:
        void SetParams() 
        {
            proc.SetFormatTranslate(GetInputData(2).DerefAsType<bool>(BoolData, true)); 

            proc.UseOutputSize(GetInputData(3).DerefAsType<bool>(BoolData, false)); 

            proc.SetOffset(GetInputData(4).DerefAsType<int>(IntData, 0),
                           GetInputData(5).DerefAsType<int>(IntData, 0)); 
        }
}; 

class Crop: public BaseProcess1In1Out<Process::Crop>
{
    TNKRVIS_CLONEABLE_MACRO(Crop) 
    public:
        void SetParams() 
        {
            proc.SetOffset(GetInputData(2).DerefAsType<int>(IntData, 0),
                           GetInputData(3).DerefAsType<int>(IntData, 0)); 
        }
}; 

class Downsample: public BaseProcess1In1Out<Process::Downsample>
{
    TNKRVIS_CLONEABLE_MACRO(Downsample) 
    public:
        void SetParams() 
        {
            proc.SetMode( (Process::Downsample::Mode)GetInputData(2).DerefAsType<int>(IntData, 0) ); 
        }
}; 

class FindContours: public BaseProcess1In1OutCPUOnly<Process::FindContours>
{
    TNKRVIS_CLONEABLE_MACRO(FindContours) 
    public:
        void SetParams() {}
}; 

}
}
}