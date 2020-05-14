#include "VideoFile.h"

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

#ifdef USE_FFMPEG

extern "C" 
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class VideoFile::Internal
{   
    private:
        bool isOpen;
        
        AVFormatContext* pFormatContext; 
        float frameDuration; 
        struct StreamData
        {
            AVCodecParameters* pCodecParameters;
            AVCodec* pCodec;
            AVCodecContext* pCodecContext;
            int pingPongInx;
            AVPacket* pPacket;
            AVFrame* pFrame[2];
            AVFrame* frameRGB; 
            int frameRGBBufferSize;
			unsigned char* frameRGBBuffer;
            bool atEnd;
            int width; 
            int height; 
            bool isVideo; 
        };
        std::vector<StreamData> videoStreamDatas; 
        std::vector<StreamData> audioStreamDatas; 

        std::vector<StreamData> allStreamDatas; 

    public:
        Internal(); 
        ~Internal(); 
        bool Open(std::string fileSrc);
        int GetStreamCount(); 
        bool Close();
        bool LoadNextFrame(); 
        void SwapBuffers(); 
        bool GetFrame(Image* frameImage, int streamInx);
        int GetFrameWidth(int streamInx);
        int GetFrameHeight(int streamInx);  
        bool AtEnd(int streamInx); 
        bool IsOpen(); 
};

VideoFile::Internal::Internal()
{
    isOpen = false; 
}

VideoFile::Internal::~Internal()
{
    if(isOpen)
    {
        Close(); 
    }
}

bool VideoFile::Internal::Open(std::string fileSrc)
{
    av_log_set_level(AV_LOG_FATAL );
    std::cout << "TnkrVis:VideoFile: Opeing: " << fileSrc << "\n"; 
    char errCharBuf[10000];

    //open input file
    pFormatContext = NULL;
    int ret = avformat_open_input(&pFormatContext, fileSrc.c_str(), NULL, NULL);
    
    if(ret != 0)
    {
        av_strerror(ret, errCharBuf, 10000); 
        std::cout << "TnkrVis:VideoFile:Open Input: " << ret << " " << errCharBuf <<  "\n";
        return false;
    }
    
    std::cout << "TnkrVis:VideoFile:OpenFile:Format is " << pFormatContext->iformat->long_name << " duration is " << pFormatContext->duration << "\n";
    frameDuration = pFormatContext->duration; 
    
    //for each stream in the container file
    for (int i = 0; i < pFormatContext->nb_streams; i++)
    {
        std::cout << "TnkrVis:VideoFile:OpenFile: stream found... " << "\n";

        //get the codec parameters
        AVCodecParameters* pLocalCodecParameters = pFormatContext->streams[i]->codecpar;
        AVCodec* pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

        // print specific info for video and audio
        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) 
        {
            std::cout << "Video Codec: resolution " << pLocalCodecParameters->width << " x " <<  pLocalCodecParameters->height << "\n";
        } 
        else if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) 
        {
            std::cout << "Audio Codec: " << pLocalCodecParameters->channels << "channels, sample rate" << pLocalCodecParameters->sample_rate << "\n";
        }
        else 
        {
            continue; 
        }
        // print general info
        std::cout << "Codec " << pLocalCodec->long_name << " ID " << pLocalCodec->id << " bit_rate " << pLocalCodecParameters->bit_rate << "\n";

        //Open the Codec
        AVCodecContext* pCodecContext = avcodec_alloc_context3(pLocalCodec);
        avcodec_parameters_to_context(pCodecContext, pLocalCodecParameters);
        avcodec_open2(pCodecContext, pLocalCodec, NULL);

        //allocate packet ad frame
        AVPacket* pPacket = av_packet_alloc();
        AVFrame* pFrame0 = av_frame_alloc();
        AVFrame* pFrame1 = av_frame_alloc();

        //copy all into stream data object
        StreamData sd;
        sd.pCodec = pLocalCodec;
        sd.pCodecContext = pCodecContext;
        sd.pCodecParameters = pLocalCodecParameters; 
        sd.pPacket = pPacket; 
        sd.pFrame[0] = pFrame0;
        sd.pFrame[1] = pFrame1;
        sd.pingPongInx = 0;

        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) 
        {
            sd.isVideo = true; 
            sd.frameRGB = av_frame_alloc();
            sd.frameRGBBufferSize = avpicture_get_size(AV_PIX_FMT_BGR24, pCodecContext->width, pCodecContext->height);
            sd.frameRGBBuffer = (unsigned char*) av_malloc(sd.frameRGBBufferSize * sizeof(unsigned char));
            avpicture_fill((AVPicture*)(sd.frameRGB), sd.frameRGBBuffer, AV_PIX_FMT_BGR24, pCodecContext->width, pCodecContext->height);	
        }
        else
        {
            sd.isVideo = false; 
        }
        sd.atEnd = false;
        sd.width = pCodecContext->width; 
        sd.height = pCodecContext->height;


        if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) 
        {
            videoStreamDatas.push_back(sd); 
        }
        else if(pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO)
        {
            audioStreamDatas.push_back(sd); 
        }
        allStreamDatas.push_back(sd); 
    }
    isOpen = true; 
     
    std::cout << "TnkrVis:VideoFile:OpenFile:Done \n";
    return true; 
}

int VideoFile::Internal::GetStreamCount()
{
    return videoStreamDatas.size();
}

bool VideoFile::Internal::Close()
{
    for(int i = 0; i < allStreamDatas.size(); i++)
    {
        avformat_close_input(&pFormatContext);
        avformat_free_context(pFormatContext);
        av_packet_free(&allStreamDatas[i].pPacket);
        av_frame_free(&allStreamDatas[i].pFrame[0]);
        av_frame_free(&allStreamDatas[i].pFrame[1]);
        if(allStreamDatas[i].isVideo)
        {
            av_frame_free(&allStreamDatas[i].frameRGB);
            av_free(&allStreamDatas[i].frameRGBBuffer);
        }
        avcodec_free_context(&allStreamDatas[i].pCodecContext);
        allStreamDatas[i].atEnd = false;
    }
    allStreamDatas.clear();
    isOpen = false; 

    return true;
}

bool VideoFile::Internal::LoadNextFrame()
{
    if(allStreamDatas.size() == 0)
        return false; 

    for(int i = 0; i < allStreamDatas.size(); i++)
    {
        if(!allStreamDatas[i].isVideo)
        {
            continue; 
        }

        int retcode; 
        int pingPongInx = allStreamDatas[i].pingPongInx;
        retcode = av_read_frame(pFormatContext, allStreamDatas[i].pPacket);
        
        if(retcode < 0)
        {
            allStreamDatas[i].atEnd = true; 
            continue; 
        }
        
/*
        retcode = avcodec_send_packet(videoStreamDatas[i].pCodecContext, videoStreamDatas[i].pPacket[pingPongInx]);

        if(retcode != 0)
        { 
            return false;
        }
  
        while (retcode >= 0) 
        {
            retcode = avcodec_receive_frame(videoStreamDatas[i].pCodecContext, videoStreamDatas[i].pFrame[pingPongInx]);
            if (retcode == AVERROR(EAGAIN) || retcode == AVERROR_EOF) 
            {
                break;
            } 
            else if (retcode < 0) 
            {
                return false;
            }
            av_frame_unref(videoStreamDatas[i].pFrame[pingPongInx]);
        }

        av_packet_unref(videoStreamDatas[i].pPacket[pingPongInx]);
*/
        int got_picture; 
        retcode = avcodec_decode_video2(allStreamDatas[i].pCodecContext, allStreamDatas[i].pFrame[pingPongInx], &got_picture, allStreamDatas[i].pPacket); 
        int timeout = 0; 
        while (retcode < 0 && timeout < 100)
        {
            retcode = av_read_frame(pFormatContext, allStreamDatas[i].pPacket);
        
            if(retcode < 0)
            {
                allStreamDatas[i].atEnd = true; 
                break; 
            }
            
            retcode = avcodec_decode_video2(allStreamDatas[i].pCodecContext, allStreamDatas[i].pFrame[pingPongInx], &got_picture, allStreamDatas[i].pPacket); 
            timeout++;
        }
    }
    return true; 
}

void VideoFile::Internal::SwapBuffers()
{
    for(int i = 0; i < allStreamDatas.size(); i++)
    {
        allStreamDatas[i].pingPongInx++; 
        if(allStreamDatas[i].pingPongInx > 1)
        {
            allStreamDatas[i].pingPongInx = 0; 
        }
    }
}

bool VideoFile::Internal::GetFrame(Image* frameImage, int streamInx)
{
    if(streamInx >= videoStreamDatas.size())
    {
        return false; 
    }

    int inx = streamInx; 

    int pingPongInx = (videoStreamDatas[inx].pingPongInx  + 1) % 2;

    int width = videoStreamDatas[inx].pFrame[pingPongInx]->width;
    int height = videoStreamDatas[inx].pFrame[pingPongInx]->height;

    //Convert into RGB 
    AVCodecContext* c = videoStreamDatas[inx].pCodecContext;
    AVFrame* frame = videoStreamDatas[inx].pFrame[pingPongInx]; 
    AVFrame* frameRGB = videoStreamDatas[inx].frameRGB;
    struct SwsContext* imgConvertCtx = sws_getContext(c->width, c->height, c->pix_fmt, c->width, c->height, AV_PIX_FMT_BGR24, SWS_BICUBIC, NULL, NULL, NULL);
    sws_scale(imgConvertCtx, frame->data, frame->linesize, 0, frame->height, frameRGB->data, frameRGB->linesize);
    //Output data into image
    int index = 0;
    int wrap = frameRGB->linesize[0];

    if(frameImage->GetWidth() != width || frameImage->GetHeight() != height || frameImage->GetType() != ImageType::RGB8)
    {
        std::cout << "Allocating " << width << " " << height << "\n"; 
        frameImage->Allocate(width, height, ImageType::RGB8); 
    }
    unsigned char* d = frameImage->GetData(); 
    for(int i = 0; i < height; i++) 
    {
        index = i * wrap;
        int vecIndex = width * (height - i - 1);
        for(int j = 0; j < width; j++)
        {
            
            d[(vecIndex + j) * 3 + 0] = (float)((frameRGB->data[0])[i * wrap + j * 3 + 0]);
            d[(vecIndex + j) * 3 + 1] = (float)((frameRGB->data[0])[i * wrap + j * 3 + 1]);
            d[(vecIndex + j) * 3 + 2] = (float)((frameRGB->data[0])[i * wrap + j * 3 + 2]);
        }
    }
    return true; 
}

int VideoFile::Internal::GetFrameWidth(int streamInx)
{
    if(streamInx >= videoStreamDatas.size())
        return 0; 

    return videoStreamDatas[streamInx].width; 
}

int VideoFile::Internal::GetFrameHeight(int streamInx)
{
    if(streamInx >= videoStreamDatas.size())
        return 0; 

    return videoStreamDatas[streamInx].height; 
}

bool VideoFile::Internal::AtEnd(int streamInx)
{
    if(streamInx >= videoStreamDatas.size())
        return true; 

    return videoStreamDatas[streamInx].atEnd;
}

bool VideoFile::Internal::IsOpen()
{
    return isOpen; 
}

#else

class VideoFile::Internal
{   
    public:
        Internal(); 
        bool Open(std::string fileSrc);
        int GetStreamCount();
        bool Close();
        bool LoadNextFrame(); 
        void SwapBuffers(); 
        bool GetFrame(Image* frameImage, int streamInx);
        int GetFrameWidth(int streamInx);
        int GetFrameHeight(int streamInx);
        bool AtEnd(int streamInx); 
        bool IsOpen(); 
};

VideoFile::Internal::Internal()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
}

bool VideoFile::Internal::Open(std::string fileSrc)
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return false; 
}

int VideoFile::Internal::GetStreamCount()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return 0;
}

bool VideoFile::Internal::Close()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return false; 
}

bool VideoFile::Internal::LoadNextFrame()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return false; 
}

void VideoFile::Internal::SwapBuffers()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
}

bool VideoFile::Internal::GetFrame(Image* frameImage, int streamInx)
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return false; 
}

int VideoFile::Internal::GetFrameWidth(int streamInx)
{
    return 0;
}

int VideoFile::Internal::GetFrameHeight(int streamInx)
{
    return 0; 
}

bool VideoFile::Internal::AtEnd(int streamInx)
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return true; 
}

bool VideoFile::Internal::IsOpen()
{
    std::cerr << "TnkrVis:VideoFile: Cannont use VideoFile as TnkrVis has not been linked to Video library\n";
    return false; 
}

#endif





VideoFile::VideoFile()
{
    internal = new Internal(); 
}

VideoFile::~VideoFile()
{ 
    delete internal; 
}

bool VideoFile::Open(std::string fileSrc)
{
    return internal->Open(fileSrc); 
}

int VideoFile::GetStreamCount() 
{
    return internal->GetStreamCount(); 
}

bool VideoFile::Close()
{
    return internal->Close(); 
}

bool VideoFile::LoadNextFrame()
{
    return internal->LoadNextFrame(); 
}

void VideoFile::SwapBuffers()
{
    internal->SwapBuffers(); 
}

bool VideoFile::GetFrame(Image* frameImage, int streamInx)
{
    return internal->GetFrame(frameImage, streamInx); 
}

int VideoFile::GetFrameWidth(int streamInx)
{
    return internal->GetFrameWidth(streamInx); 
}

int VideoFile::GetFrameHeight(int streamInx)
{
    return internal->GetFrameHeight(streamInx); 
}

bool VideoFile::AtEnd(int streamInx)
{
    return internal->AtEnd(streamInx); 
}

bool VideoFile::IsOpen()
{
    return internal->IsOpen(); 
}

}
}