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
                outImageData = Data(NullData, NULL); 
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
                outImageData = Data(NullData, NULL); 
            }
            else if(inSrcAsimageGPU != NULL && inDstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, inDstAsimageGPU);
                outImageData = Data(NullData, NULL);
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





}
}
}