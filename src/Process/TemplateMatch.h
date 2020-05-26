#pragma once

#include "TinkerVision_export.h"

#include "../Core/ImageType.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "../Core/VectorMath.h"

#include "../ComputeGraph/Node.h"

namespace TnkrVis
{
namespace Process
{
	
class TINKERVISION_EXPORT TemplateMatch
{
    public:
        enum MatchMode{MATCH_SAD, MATCH_CORR};

    private: 
        class Internal;
        Internal* internal;

	public:
		TemplateMatch(); 
        ~TemplateMatch(); 
        void SetMatchMode(MatchMode mm); 
        void SetNormalized(bool n); 
		void Run(ImageGPU* input, ImageGPU* match, ImageGPU* output); 
        void Run(Image* input, Image* match, Image* output); 
};
	
}

namespace ComputeGraph
{
namespace Nodes
{

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

}
}

}