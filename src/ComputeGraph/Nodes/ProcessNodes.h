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
#include "../../Process/MorphThinning.h"
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

class GaussianBlur: public BaseProcess1In1Out<Process::GaussianBlur>
{
    TNKRVIS_CLONEABLE_MACRO(GaussianBlur) 
    public:
        void SetParams() 
        {
            proc.SetSigma( GetInputData(2).DerefAsType<int>(FloatData, 1.6f) ); 
        }
};

class GaussianDerivative: public BaseProcess1In1Out<Process::GaussianDerivative>
{
    TNKRVIS_CLONEABLE_MACRO(GaussianDerivative) 
    public:
        void SetParams() 
        {
            proc.SetSigma( GetInputData(2).DerefAsType<int>(FloatData, 1.6f) ); 
            proc.SetDirection( (Process::GaussianDerivative::Direction)GetInputData(3).DerefAsType<int>(IntData, (int)Process::GaussianDerivative::Direction::HORIZONTAL ) ); 
        }
};

class GaussianFunction: public Node
{
    TNKRVIS_CLONEABLE_MACRO(GaussianFunction) 
    private:
        Data outImageData;
        Process::GaussianFunction proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetSigma(GetInputData(1).DerefAsType<float>(FloatData, 0), 
                          GetInputData(2).DerefAsType<float>(FloatData, 0)); 
            proc.SetGenerateMode( (Process::GaussianFunction::GenerateMode)GetInputData(3).DerefAsType<int>(IntData, 0)); 

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

class GrayScale: public BaseProcess1In1Out<Process::GrayScale>
{
    TNKRVIS_CLONEABLE_MACRO(GrayScale) 
    public:
        void SetParams() {}
}; 

class HarrisShiTomasiResponse: public BaseProcess1In1Out<Process::HarrisShiTomasiResponse>
{
    TNKRVIS_CLONEABLE_MACRO(HarrisShiTomasiResponse) 
    public:
        void SetParams() {}
}; 

class HighLowThreshold: public BaseProcess1In1Out<Process::HighLowThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(HighLowThreshold) 
    public:
        void SetParams() 
        {            
            proc.SetLowThreshold( GetInputData(2).DerefAsType<Color>(ColorData, Color(0, 0, 0) ) ); 
            proc.SetHighThreshold( GetInputData(3).DerefAsType<Color>(ColorData, Color(1, 1, 1) ) ); 
        }
}; 

class HSVToRGB: public BaseProcess1In1Out<Process::HSVToRGB>
{
    TNKRVIS_CLONEABLE_MACRO(HSVToRGB) 
    public:
        void SetParams() {}
}; 

class HysteresisEdgeThreshold: public BaseProcess1In1OutCPUOnly<Process::HysteresisEdgeThreshold>
{
    TNKRVIS_CLONEABLE_MACRO(HysteresisEdgeThreshold) 
    public:
        void SetParams() 
        {
            proc.SetHighThreshold( GetInputData(2).DerefAsType<float>(FloatData, 1.0f ) ); 
            proc.SetLowThreshold( GetInputData(3).DerefAsType<float>(FloatData, 0.0f) ); 
        }
}; 

class InRange: public BaseProcess1In1Out<Process::InRange>
{
    TNKRVIS_CLONEABLE_MACRO(InRange) 
    public:
        void SetParams() 
        {
            proc.SetHighThreshold( GetInputData(2).DerefAsType<Vec3>(Vec3Data, Vec3(1, 1, 1) ) ); 
            proc.SetLowThreshold( GetInputData(3).DerefAsType<Vec3>(Vec3Data, Vec3(0, 0, 0)) ); 

            proc.SetInvertRange( GetInputData(4).DerefAsType<bool>(BoolData, false),
                                 GetInputData(5).DerefAsType<bool>(BoolData, false),
                                 GetInputData(6).DerefAsType<bool>(BoolData, false)); 
        }
}; 

class Invert: public BaseProcess1In1Out<Process::Invert>
{
    TNKRVIS_CLONEABLE_MACRO(Invert) 
    public:
        void SetParams() {}
}; 

class LocalMaxima: public BaseProcess1In1OutCPUOnly<Process::LocalMaxima>
{
    TNKRVIS_CLONEABLE_MACRO(LocalMaxima) 
    public:
        void SetParams() 
        { //TODO Array of centers support (needs general array support)
            proc.SetThreshold( GetInputData(2).DerefAsType<float>(FloatData, 1.0f )  ); 
            proc.SetSize( GetInputData(3).DerefAsType<float>(FloatData, 1.0f )  );
        }
}; 

class MarkerBitExtract: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(MarkerBitExtract) 
    private:
        Data outImageData;
        Process::MarkerBitExtract proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            proc.SetGridSize(GetInputData(3).DerefAsType<int>(IntData, 0), 
                             GetInputData(4).DerefAsType<int>(IntData, 0)); 
            proc.SetMarginSize( GetInputData(5).DerefAsType<float>(FloatData, 0)); 

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
                        
            if(dstAsimage != NULL)
            {
                //proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

class MedianFilter: public BaseProcess1In1Out<Process::MedianFilter>
{
    TNKRVIS_CLONEABLE_MACRO(MedianFilter) 
    public:
        void SetParams() 
        {
            proc.SetSize( GetInputData(2).DerefAsType<int>(IntData, 3 ) ); 
        }
}; 

class Mipmaps: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(Mipmaps) 
    private:
        Data outImageData;
        Process::Mipmaps proc; 

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  
                        
            if(dstAsimage != NULL)
            {
                //proc.Run( dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(dstAsimageGPU != NULL)
            {
                //proc.Run( dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
};

class MorphRect: public BaseProcess1In1Out<Process::MorphRect>
{
    TNKRVIS_CLONEABLE_MACRO(MorphRect) 
    public:
        void SetParams() 
        {
            proc.SetMode( (Process::MorphRect::Mode)GetInputData(2).DerefAsType<int>(IntData, (int)Process::MorphRect::Mode::DILATE) );
            proc.SetKernel( GetInputData(3).DerefAsType<int>(IntData, 3),
                            GetInputData(4).DerefAsType<int>(IntData, 3) );  
        }
}; 

class MorphThinning: public BaseProcess1In1OutCPUOnly<Process::MorphThinning>
{
    TNKRVIS_CLONEABLE_MACRO(MorphThinning) 
    public:
        void SetParams() { }
}; 

class NonMaximumEdgeSuppression: public BaseProcess1In1Out<Process::NonMaximumEdgeSuppression>
{
    TNKRVIS_CLONEABLE_MACRO(NonMaximumEdgeSuppression) 
    public:
        void SetParams() { }
}; 

class Normalize: public BaseProcess1In1OutCPUOnly<Process::Normalize>
{
    TNKRVIS_CLONEABLE_MACRO(Normalize) 
    public:
        void SetParams() { }
}; 

class OtsuThreshold: public Node //TODO (with array support)
{
    TNKRVIS_CLONEABLE_MACRO(OtsuThreshold) 
    private:
        float threshold; 
        Data outThresholdData;
        Process::OtsuThreshold proc; 

    public:
        Data GetOutput(int inx){ return outThresholdData; }

        void Run()
        {
            Image* srcAsimage = GetInputData(0).AsType<Image>(ImageData);  
                        
            if(srcAsimage != NULL)
            {
                threshold = proc.Run( srcAsimage);
                outThresholdData = Data(FloatData, &threshold); 
            }
            else
            {
                outThresholdData = Data(NullData, NULL); 
            }
        }
};

class RGBToHSV: public BaseProcess1In1Out<Process::RGBToHSV>
{
    TNKRVIS_CLONEABLE_MACRO(RGBToHSV) 
    public:
        void SetParams() { }
}; 

class Rotate: public BaseProcess1In1Out<Process::Rotate>
{
    TNKRVIS_CLONEABLE_MACRO(Rotate) 
    public:
        void SetParams() 
        { 
            proc.SetRotation( GetInputData(2).DerefAsType<float>(FloatData, 0));
            proc.SetResizeToFit( GetInputData(3).DerefAsType<bool>(BoolData, true)); 
        }
}; 

class Sobel: public BaseProcess1In1Out<Process::Sobel>
{
    TNKRVIS_CLONEABLE_MACRO(Sobel) 
    public:
        void SetParams() 
        { 
            proc.SetMode( (Process::Sobel::Mode)GetInputData(2).DerefAsType<int>(IntData, Process::Sobel::Mode::FULL));
        }
}; 

class StereoMatchSAD: public Node
{
    TNKRVIS_CLONEABLE_MACRO(StereoMatchSAD) 
    private:
        Data outImageData;
        Process::StereoMatchSAD proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {

            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcLAsimage = GetInputData(1).AsType<Image>(ImageData);
            ImageGPU* inSrcLAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcRAsimage = GetInputData(2).AsType<Image>(ImageData);
            ImageGPU* inSrcRAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

            proc.SetBlockSize( GetInputData(3).DerefAsType<int>(IntData, 32),
                               GetInputData(4).DerefAsType<int>(IntData, 32));
            proc.SetMaxK( GetInputData(5).DerefAsType<int>(IntData, 80));
            proc.SetSADThreshold( GetInputData(6).DerefAsType<float>(FloatData, 50)); 
                        
            if(inSrcLAsimage != NULL && inSrcRAsimage != NULL && dstAsimage != NULL)
            {
                proc.Run(inSrcLAsimage, inSrcRAsimage, dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(inSrcLAsimageGPU != NULL && inSrcRAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                proc.Run(inSrcLAsimageGPU, inSrcRAsimageGPU, dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

class StructureTensor: public BaseProcess1In1Out<Process::StructureTensor>
{
    TNKRVIS_CLONEABLE_MACRO(StructureTensor) 
    public:
        void SetParams() { }
}; 

class TemplateMatch:  public Node
{
    TNKRVIS_CLONEABLE_MACRO(TemplateMatch) 
    private:
        Data outImageData;
        Process::TemplateMatch proc;

    public:
        Data GetOutput(int inx){ return outImageData; }

        void Run()
        {
            Image* dstAsimage = GetInputData(0).AsType<Image>(ImageData);  
            ImageGPU* dstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);
            ImageGPU* inSrcAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  

            Image* matchAsimage = GetInputData(2).AsType<Image>(ImageData);  
            ImageGPU* matchAsimageGPU = GetInputData(2).AsType<ImageGPU>(ImageGPUData);  

            proc.SetMatchMode( (Process::TemplateMatch::MatchMode)GetInputData(3).DerefAsType<int>(IntData, (int)Process::TemplateMatch::MatchMode::MATCH_SAD)); 
            proc.SetNormalized( GetInputData(4).DerefAsType<bool>(BoolData, true)); 
                        
            if(inSrcAsimage != NULL && matchAsimage != NULL && dstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, matchAsimage, dstAsimage);
                outImageData = Data(DataType::ImageData, dstAsimage); 
            }
            else if(inSrcAsimageGPU != NULL && matchAsimageGPU != NULL && dstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, matchAsimageGPU, dstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, dstAsimageGPU); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

class Threshold: public BaseProcess1In1Out<Process::Threshold>
{
    TNKRVIS_CLONEABLE_MACRO(Threshold) 
    public:
        void SetParams() 
        { 
            //proc.SetThreshold( GetInputData(2).DerefAsType<Color>(ColorData));
        }
}; 

}
}
}