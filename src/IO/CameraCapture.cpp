#include "CameraCapture.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

namespace TnkrVis
{
namespace IO
{

class CameraCapture::Internal
{   
    private:
        bool isOpen;

    public:
        Internal(); 
        ~Internal(); 
        bool Open();
        bool Close();
        bool LoadNextFrame(); 
        void SwapBuffers(); 
        bool GetFrame(Image* frameImage);
        int GetFrameWidth();
        int GetFrameHeight();  
        bool AtEnd(); 
        bool IsOpen(); 
};

CameraCapture::Internal::Internal()
{
    isOpen = false; 
}

CameraCapture::Internal::~Internal()
{
    if(isOpen)
    {
        Close(); 
    }
}

bool CameraCapture::Internal::Open()
{
    isOpen = true;     
    std::cout << "TnkrVis:CameraCapture:Open:Done \n";
    return true; 
}

bool CameraCapture::Internal::Close()
{
    
    isOpen = false; 
    return true;
}

bool CameraCapture::Internal::LoadNextFrame()
{
    
    return true; 
}

void CameraCapture::Internal::SwapBuffers()
{
    
}

bool CameraCapture::Internal::GetFrame(Image* frameImage)
{
    
    return true; 
}

int CameraCapture::Internal::GetFrameWidth()
{
    return 0; 
}

int CameraCapture::Internal::GetFrameHeight()
{
    return 0; 
}

bool CameraCapture::Internal::AtEnd()
{
    return true; 
}

bool CameraCapture::Internal::IsOpen()
{
    return isOpen; 
}




CameraCapture::CameraCapture()
{
    internal = new Internal(); 
}

CameraCapture::~CameraCapture()
{ 
    delete internal; 
}

bool CameraCapture::Open()
{
    return internal->Open(); 
}

bool CameraCapture::Close()
{
    return internal->Close(); 
}

bool CameraCapture::LoadNextFrame()
{
    return internal->LoadNextFrame(); 
}

void CameraCapture::SwapBuffers()
{
    internal->SwapBuffers(); 
}

bool CameraCapture::GetFrame(Image* frameImage)
{
    return internal->GetFrame(frameImage); 
}

int CameraCapture::GetFrameWidth()
{
    return internal->GetFrameWidth(); 
}

int CameraCapture::GetFrameHeight()
{
    return internal->GetFrameHeight(); 
}

bool CameraCapture::AtEnd()
{
    return internal->AtEnd(); 
}

bool CameraCapture::IsOpen()
{
    return internal->IsOpen(); 
}

}
}