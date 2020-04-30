#include "BarcodeDetector.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace CompositeProcess
{

//using namespace Process;

class BarcodeDetector::Internal
{
    private:
        

    public:
        Internal(); 
        void Run(ImageGPU* input, std::vector<Visi::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData);
};


BarcodeDetector::Internal::Internal()
{
}

void BarcodeDetector::Internal::Run(ImageGPU* input, std::vector<Visi::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData)
{

}



BarcodeDetector::BarcodeDetector()
{
    internal = new Internal(); 
}

BarcodeDetector::~BarcodeDetector()
{ 
    delete internal; 
}

void BarcodeDetector::Run(ImageGPU* input, std::vector<Visi::Contour>* barcodeQuads, std::vector< std::vector<unsigned char> >* barcodeData)
{
    internal->Run(input, barcodeQuads, barcodeData); 
}


}
}