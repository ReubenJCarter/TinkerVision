#include "ARUCODetector.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Process/GaussianBlur.h"
#include "../Process/GrayScale.h"
#include "../Process/AdaptiveThreshold.h"
#include "../Process/FindContours.h"
#include "../Process/MarkerBitExtract.h"
#include "../Process/MarkerDictionary.h"
#include "../Process/Renderer.h"

#include "../IO/ImageFile.h"

#include <string>
#include <iostream>
#include <map>

namespace Viso
{
namespace CompositeProcess
{

using namespace Process;

class ARUCODetector::Internal
{
    private:
        ImageGPU tempGPU[4]; 
        Image temp[3];

        std::vector<Contour> contours;
        std::vector<Viso::Contour> contoursFiltered; 
        std::vector<Viso::Contour> contoursSimplified; 
        std::vector<Viso::Contour> contoursMerged; 
        std::vector<Viso::Contour> contoursQuads; 
        std::vector<Viso::Contour> arucoQuads; 
        std::vector<Viso::Image> bitImages;  

        GrayScale gray; 
        AdaptiveThreshold adaptiveThreshold; 
        FindContours findContours; 
        MarkerBitExtract markerBitExtract; 
        MarkerDictionary markerDictionary; 
        Renderer render;

    public:
        Internal(); 
        void Run(ImageGPU* input, std::vector<Viso::Contour>* markerQuads, std::vector<int>* markerIds, bool retAllQuads);
        void AddDictionaryEntry(Image* entry, int id);
        void AddDictionaryEntry(std::vector<bool>* bitsequence, int size, int id);
        void SetMaxBitError(int mbe); 
};


ARUCODetector::Internal::Internal()
{
}

void ARUCODetector::Internal::Run(ImageGPU* input, std::vector<Viso::Contour>* markerQuads, std::vector<int>* markerIds, bool retAllQuads)
{
    bool grayConverted = false; 
    if(input->GetType() != ImageType::GRAYSCALE8 && input->GetType() != ImageType::GRAYSCALE16 && input->GetType() != ImageType::GRAYSCALE32F)
    {
        gray.Run(input, &tempGPU[0]); 
        input = &tempGPU[0]; 
    }

    adaptiveThreshold.SetThreshold(0.02); 
	adaptiveThreshold.SetSize(7); 
    adaptiveThreshold.Run(input, &tempGPU[1]);

    temp[0].Copy(&tempGPU[1]); 

    findContours.Run(&temp[0], &temp[1], &contours); 

    Viso::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 100);
    Viso::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 3);
    Viso::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
    Viso::Contour::ContoursSimplify(&contoursMerged, &contoursSimplified, 5);
    Viso::Contour::ContoursVertCountFilter(&contoursSimplified, &contoursQuads, 4, 4);

    temp[1].Copy(input); 
    markerBitExtract.Run(&temp[1], &contoursQuads, &bitImages);

    markerQuads->clear();
    markerIds->clear();
    for(int i = 0; i < bitImages.size(); i++)
    {
        int id = markerDictionary.Lookup(&bitImages[i]); 
        if(id != -1 || retAllQuads)
        {
            markerQuads->push_back(contoursQuads.at(i)); 
            markerIds->push_back(id); 
        } 
    }
    /*
    for(int i = 0; i < bitImages.size(); i++)
    {
        std::string fn = "bitImage";
        fn += std::to_string(i); 
        fn += ".png"; 
        Viso::IO::ImageFile::Write(fn, &bitImages[i]); 
    }*/
    
    /*
    render.AddContours(&contoursQuads);
    render.Run(&temp[0], &temp[2]); 
    Viso::IO::ImageFile::Write("testTemp.png", &temp[2]); 
    */
}

void ARUCODetector::Internal::AddDictionaryEntry(Image* entry, int id)
{
    markerDictionary.AddEntry(entry, id);
}

void ARUCODetector::Internal::AddDictionaryEntry(std::vector<bool>* bitsequence, int size, int id)
{
    Image entry; 
    entry.Allocate(size, size, ImageType::GRAYSCALE8); 
    unsigned char* d = entry.GetData();
    for(int i = 0; i < size*size; i++)
    {
        if(i < bitsequence->size())
        {
            d[i] = bitsequence->at(i) ? 255 : 0;
        }
        else
        {
            d[i] = 0;
        }
    }
    markerDictionary.AddEntry(&entry, id); 
}

void ARUCODetector::Internal::SetMaxBitError(int mbe)
{
    markerDictionary.SetMaxBitError(mbe); 
}



ARUCODetector::ARUCODetector()
{
    internal = new Internal(); 
}

ARUCODetector::~ARUCODetector()
{ 
    delete internal; 
}

void ARUCODetector::Run(ImageGPU* input, std::vector<Viso::Contour>* markerQuads, std::vector<int>* markerIds, bool retAllQuads)
{
    internal->Run(input, markerQuads, markerIds, retAllQuads); 
}

void ARUCODetector::AddDictionaryEntry(Image* entry, int id)
{
    internal->AddDictionaryEntry(entry, id);
}

void ARUCODetector::AddDictionaryEntry(std::vector<bool>* bitsequence, int size, int id)
{
    internal->AddDictionaryEntry(bitsequence, size, id);
}

void ARUCODetector::SetMaxBitError(int mbe)
{
    internal->SetMaxBitError(mbe);
}


}
}