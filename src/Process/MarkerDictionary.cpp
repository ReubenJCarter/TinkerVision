#include "MarkerDictionary.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class MarkerDictionary::Internal
{
    private:
        struct Entry{Image im; int id; }; 
        std::vector<Entry> dict;
        int maxBitError ; 

    public:
        Internal(); 
        int Lookup(Image* input);
        void AddEntry(Image* im, int id); 
        void SetMaxBitError(int mbe); 
};

MarkerDictionary::Internal::Internal()
{
    maxBitError = 3; 
}

int MarkerDictionary::Internal::Lookup(Image* input)
{
    if(input->GetWidth() == 0 && input->GetHeight() == 0)
    {
        return -1;
    }

    for(int d = 0; d < dict.size(); d++)
    {
        if(input->GetWidth() != dict[d].im.GetWidth() && input->GetHeight() != dict[d].im.GetHeight())
        {
            continue; 
        }
        //0 deg
        int missCount = 0; 
        bool match = true; 
        for(int j = 0; j < input->GetHeight(); j++)
        {
            for(int i = 0; i < input->GetWidth(); i++)
            {
                glm::ivec4 pixin = GetPixelUI(input, i, j);
                glm::ivec4 pixDic = GetPixelUI(&(dict[d].im), i, j);
                if(pixin.r != pixDic.r)
                {
                    missCount++;
                    if(missCount > maxBitError)
                    {
                        match = false; 
                        break; 
                    }
                }
            }
            if(!match)
            {
                break; 
            }
        }
        if(match)
        {
            return dict[d].id; 
        }

        //rotate 90 deg
        missCount = 0; 
        match = true; 
        for(int j = 0; j < input->GetHeight(); j++)
        {
            for(int i = 0; i < input->GetWidth(); i++)
            {
                glm::ivec4 pixin = GetPixelUI(input, i, j);
                glm::ivec4 pixDic = GetPixelUI(&(dict[d].im), j, input->GetHeight() - i - 1);
                if(pixin.r != pixDic.r)
                {
                    missCount++;
                    if(missCount > maxBitError)
                    {
                        match = false; 
                        break; 
                    }
                }
            }
            if(!match)
            {
                break; 
            }
        }
        if(match)
        {
            return dict[d].id; 
        }

        //rotate -90 deg
        missCount = 0; 
        match = true; 
        for(int j = 0; j < input->GetHeight(); j++)
        {
            for(int i = 0; i < input->GetWidth(); i++)
            {
                glm::ivec4 pixin = GetPixelUI(input, i, j);
                glm::ivec4 pixDic = GetPixelUI(&(dict[d].im), input ->GetHeight() - j - 1, i);
                if(pixin.r != pixDic.r)
                {
                    missCount++;
                    if(missCount > maxBitError)
                    {
                        match = false; 
                        break; 
                    }
                }
            }
            if(!match)
            {
                break; 
            }
        }
        if(match)
        {
            return dict[d].id; 
        }

        //rotate 180 deg
        missCount = 0; 
        match = true; 
        for(int j = 0; j < input->GetHeight(); j++)
        {
            for(int i = 0; i < input->GetWidth(); i++)
            {
                glm::ivec4 pixin = GetPixelUI(input, i, j);
                glm::ivec4 pixDic = GetPixelUI(&(dict[d].im), input->GetWidth() - i - 1, 
                                                              input->GetHeight() - j - 1);
                if(pixin.r != pixDic.r)
                {
                    missCount++;
                    if(missCount > maxBitError)
                    {
                        match = false; 
                        break; 
                    }
                }
            }
            if(!match)
            {
                break; 
            }
        }
        if(match)
        {
            return dict[d].id; 
        }

    }

    return -1;
}

void MarkerDictionary::Internal::AddEntry(Image* im, int id)
{
    Entry entry; 
    dict.push_back(entry);
    Entry& e = dict[dict.size()-1]; 

    e.im.Copy(im);
    e.id = id; 
}

void MarkerDictionary::Internal::SetMaxBitError(int mbe)
{
    maxBitError = mbe; 
}




MarkerDictionary::MarkerDictionary()
{
    internal = new Internal(); 
}

MarkerDictionary::~MarkerDictionary()
{
    delete internal; 
}

int MarkerDictionary::Lookup(Image* input)
{
    return internal->Lookup(input); 
}

void MarkerDictionary::AddEntry(Image* im, int id)
{
    internal->AddEntry(im, id);
}

void MarkerDictionary::SetMaxBitError(int mbe)
{
    internal->SetMaxBitError(mbe);
}

}
}