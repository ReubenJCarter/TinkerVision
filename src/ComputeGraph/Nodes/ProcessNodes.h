#pragma once

#include "../Node.h"

#include "../../Core/Image.h"
#include "../../Core/ImageGPU.h"
#include "../../IO/ImageFile.h"

#include "../../Process/AdaptiveThreshold.h"
#include "../../Process/ApproxDistanceTransform.h"

namespace Visi
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

        void Run()
        {
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

        void SetParams()
        {
            
        }

        void Run()
        {
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

class AdaptiveThresholdNode: public BaseProcess1In1Out<Process::AdaptiveThreshold>
{
    VISI_CLONEABLE_MACRO(AdaptiveThresholdNode) 
};

class ApproxDistanceTransformNode: public BaseProcess1In1OutCPUOnly<Process::ApproxDistanceTransform>
{
    VISI_CLONEABLE_MACRO(ApproxDistanceTransformNode) 
}; 


}
}
}