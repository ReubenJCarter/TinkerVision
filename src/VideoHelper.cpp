#include "VideoHelper.h"

#include "VideoFile.h"

#include <thread>
#include <atomic>

namespace Visi
{

class VideoHelper::Internal
{
    private:
        VideoFile videoFile; 
        std::atomic<bool> videoDecodeRunning;
        std::atomic<bool> getFrameDataRunning;
        std::atomic<bool> gpuCopyRunning;
        std::atomic<bool> running;

        Visi::Image image[2]; 
        Visi::ImageGPU imageGPU[2];
        int pingpong; 

    public:
        Internal(); 
        ~Internal(); 
        bool Open(std::string fileSrc); 
        int GetFrameWidth();
        int GetFrameHeight(); 
        bool Close(); 
        bool NextFrame(std::function<void(Visi::ImageGPU*)> useFrame);
};

VideoHelper::Internal::Internal()
{
    pingpong = 0; 
    running = false; 
    videoDecodeRunning = false; 
    getFrameDataRunning = false; 
    gpuCopyRunning = false; 
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

    gpuCopyRunning = false; 
    std::thread gpuCopyThread([&]()
    {
        while(running)
        {
            while(!gpuCopyRunning && running){}
            if(!running)
            {
                gpuCopyRunning = false; 
                break; 
            }
            imageGPU[pingpong].Copy(&image[ (pingpong+1)%2 ]);
            gpuCopyRunning = false; 
        }
    });
    gpuCopyThread.detach();

    

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
    gpuCopyRunning = false; 
    videoFile.Close(); 
    return false; 
}

bool VideoHelper::Internal::NextFrame(std::function<void(Visi::ImageGPU*)> useFrame)
{
    if(!running)
    {
        return false; 
    }

    videoDecodeRunning = true; 
    
    getFrameDataRunning = true; 

    imageGPU[pingpong].Copy(&image[ (pingpong+1)%2 ]);


    useFrame( &(imageGPU[(pingpong+1)%2]) ); 

    //wait for all to finish before continue
    while(videoDecodeRunning || getFrameDataRunning || gpuCopyRunning){}

    videoFile.SwapBuffers(); 
    pingpong = (pingpong + 1) % 2;

    return true; 
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

bool VideoHelper::NextFrame(std::function<void(Visi::ImageGPU*)> useFrame)
{
    return internal->NextFrame(useFrame); 
}

}