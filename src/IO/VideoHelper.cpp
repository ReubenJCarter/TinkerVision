#include "VideoHelper.h"

#include "VideoFile.h"

#include <thread>
#include <atomic>

namespace TnkrVis
{
namespace IO
{

class VideoHelper::Internal
{
    private:
        VideoFile videoFile; 
        std::atomic<bool> videoDecodeRunning;
        std::atomic<bool> getFrameDataRunning;
        std::atomic<bool> running;

        TnkrVis::Image image[2]; 
        TnkrVis::ImageGPU imageGPU;
        int pingpong; 

    public:
        Internal(); 
        ~Internal(); 
        bool Open(std::string fileSrc); 
        int GetFrameWidth();
        int GetFrameHeight(); 
        bool Close(); 
        bool NextFrame(std::function<void(TnkrVis::ImageGPU*, TnkrVis::Image*)> useFrame);
        bool IsOpen(); 
};

VideoHelper::Internal::Internal()
{
    pingpong = 0; 
    running = false; 
    videoDecodeRunning = false; 
    getFrameDataRunning = false; 
}

VideoHelper::Internal::~Internal()
{
    Close(); 
}

bool VideoHelper::Internal::Open(std::string fileSrc)
{
    videoFile.Open(fileSrc);

    //Init
    videoFile.LoadNextFrame(); 
	videoFile.SwapBuffers(); 

    running = true; 
    pingpong = 0; 

    videoDecodeRunning = false; 
    std::thread videoDecodeThread([&]()
    {
        while(running)
        {
            while(!videoDecodeRunning && running){}
            if(!running)
            {
                videoDecodeRunning = false;
                break; 
            }
            videoFile.LoadNextFrame(); 
            videoDecodeRunning = false;
        }
    });
    videoDecodeThread.detach(); 

    getFrameDataRunning = false; 
    std::thread getFrameDataThread([&]()
    {
        while(running)
        {
            while(!getFrameDataRunning && running){}
            if(!running)
            {
                getFrameDataRunning = false;
                break; 
            }
            videoFile.GetFrame(&image[pingpong]);
            getFrameDataRunning = false;
        }
    });
    getFrameDataThread.detach(); 


    return true; 
}

int VideoHelper::Internal::GetFrameWidth()
{
    return videoFile.GetFrameWidth();
}

int VideoHelper::Internal::GetFrameHeight()
{
    return videoFile.GetFrameHeight(); 
}

bool VideoHelper::Internal::Close()
{
    running = false;
    videoDecodeRunning = false; 
    getFrameDataRunning = false; 
    videoFile.Close(); 
    return false; 
}

bool VideoHelper::Internal::NextFrame(std::function<void(TnkrVis::ImageGPU*, TnkrVis::Image*)> useFrame)
{
    if(!running)
    {
        return false; 
    }

    videoDecodeRunning = true; 
    
    getFrameDataRunning = true; 

    if(image[ (pingpong+1)%2 ].GetWidth() != 0 && image[ (pingpong+1)%2 ].GetHeight() != 0)
    {
        imageGPU.Copy(&image[ (pingpong+1)%2 ]);
        useFrame( &(imageGPU), &(image[ (pingpong+1)%2 ]) ); 
    }
    //wait for all to finish before continue
    while(videoDecodeRunning || getFrameDataRunning ){}

    videoFile.SwapBuffers(); 
    pingpong = (pingpong + 1) % 2;

    return true; 
}

bool VideoHelper::Internal::IsOpen()
{
    return videoFile.IsOpen(); 
}



VideoHelper::VideoHelper()
{
    internal = new Internal(); 
}

VideoHelper::~VideoHelper()
{
    delete internal; 
}

bool VideoHelper::Open(std::string fileSrc)
{
    return internal->Open(fileSrc); 
}

int VideoHelper::GetFrameWidth()
{
    return internal->GetFrameWidth(); 
}

int VideoHelper::GetFrameHeight()
{
    return internal->GetFrameHeight(); 
}

bool VideoHelper::Close()
{
    return internal->Close(); 
}

bool VideoHelper::NextFrame(std::function<void(TnkrVis::ImageGPU*, TnkrVis::Image*)> useFrame)
{
    return internal->NextFrame(useFrame); 
}

bool VideoHelper::IsOpen()
{
    return internal->IsOpen();
}

}
}