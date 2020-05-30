#include "CameraCapture.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>













/*
 *Direct show capture API
 */

#define USE_DSHOW
#ifdef USE_DSHOW

#include <windows.h>
#include <dshow.h>
//#pragma comment(lib, "strmiids")

#define DsHook(a,b,c) if (!c##_) { INT_PTR* p = b + *(INT_PTR**)a ; VirtualProtect(&c##_, 4, PAGE_EXECUTE_READWRITE, &no); \
                                    *(INT_PTR*)&c##_=*p;   \
                                    VirtualProtect(p,    4,PAGE_EXECUTE_READWRITE,&no);   \
                                    *p=(INT_PTR)c; \
                                    }


namespace TnkrVis
{
namespace IO
{

HRESULT ( __stdcall *Receive_ ) ( void* inst, IMediaSample *smp ) ; 

static HRESULT   __stdcall   Receive    ( void* inst, IMediaSample *smp ) 
{     
    BYTE*     buf;    
    smp->GetPointer(&buf); 
    DWORD len = smp->GetActualDataLength();
    std::cout << "Buffer:" << len << "\n"; 
    HRESULT   ret  =  Receive_   ( inst, smp );   
    return    ret; 
}

class CameraCapture::Internal
{   
    private:
        bool isOpen;

        std::string BStrToStdStr(BSTR& s); 
        bool DisplayInformation(IMoniker *pMoniker);

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

std::string CameraCapture::Internal::BStrToStdStr(BSTR& s)
{
    std::string str = "";
    int maxLen = 200;
    int inx = 0; 
    while( s[inx] != 0x00 && inx < maxLen) 
    {
        str += (char)s[inx];
        inx++;
    }
    return str; 
}

bool CameraCapture::Internal::DisplayInformation(IMoniker *pMoniker)
{
    IPropertyBag *pPropBag;
    HRESULT hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
    if (FAILED(hr))
    {
        return false;
    } 

    VARIANT var;
    VariantInit(&var);

    // Get description or friendly name.
    hr = pPropBag->Read(L"Description", &var, 0);
    if (FAILED(hr))
    {
        hr = pPropBag->Read(L"FriendlyName", &var, 0);
    }
    if (SUCCEEDED(hr))
    {
        std::cout << "FriendlyName: " << BStrToStdStr(var.bstrVal)  << "\n";
        VariantClear(&var); 
    }

    hr = pPropBag->Write(L"FriendlyName", &var);

    // WaveInID applies only to audio capture devices.
    hr = pPropBag->Read(L"WaveInID", &var, 0);
    if (SUCCEEDED(hr))
    {
        std::cout << "WaveInID: " <<  var.lVal << "\n";
        VariantClear(&var); 
    }

    hr = pPropBag->Read(L"DevicePath", &var, 0);
    if (SUCCEEDED(hr))
    {
        // The device path is not intended for display.
        std::cout << "DevicePath: " <<  BStrToStdStr(var.bstrVal) << "\n";
        VariantClear(&var); 
    }

    pPropBag->Release();
    return true;
}

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

    isOpen = false;    

    //
    //Init Com Library
    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (! SUCCEEDED(hr))
    {
        std::cerr << "CameraCapture:Internal:Open:Failed to intit com library\n";
        return false; 
    }
    

    //
    //Enumerate

    // Create the System Device Enumerator.
    IEnumMoniker *pEnum;
    ICreateDevEnum *pDevEnum;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum));
    if (!SUCCEEDED(hr))
    {
        std::cerr << "CameraCapture:Internal:CoCreateInstance(CLSID_SystemDeviceEnum failed.\n";
        return false; 
    }

    // Create an enumerator for the category.
    hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
    if (!SUCCEEDED(hr))
    {
        pDevEnum->Release();
        hr = VFW_E_NOT_FOUND;  // The category is empty. Treat as an error.
        std::cerr << "CameraCapture:Internal:Open:CreateClassEnumerator(CLSID_VideoInputDeviceCategory failed.\n";
        return false; 
    }


    //
    //Build the graph for each device and display info
    IMoniker *pMoniker = NULL;
    if (pEnum->Next(1, &pMoniker, NULL) == S_OK)  //First webcam
    {
        DisplayInformation(pMoniker); 


        //
        //Init the graph
        ICaptureGraphBuilder2* pCaptureGraphBuilder; // Capture Graph Builder
        IGraphBuilder* pGraphBuilder;
        // Create the Capture Graph Builder.
        hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2, (void**)&pCaptureGraphBuilder );
        if (!SUCCEEDED(hr))
        {
            std::cerr << "CameraCapture:Create CLSID_CaptureGraphBuilder2 failed\n"; 
            pMoniker->Release(); 
            return false; 
        }
        // Create the Filter Graph Manager.
        hr = CoCreateInstance(CLSID_FilterGraph, 0, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraphBuilder);
        if (SUCCEEDED(hr))
        {
            // Initialize the Capture Graph Builder.
            pCaptureGraphBuilder->SetFiltergraph(pGraphBuilder);
        }
        else
        {
            std::cerr << "CameraCapture:Create CLSID_FilterGraph failed\n"; 
            pCaptureGraphBuilder->Release();
            pMoniker->Release(); 
            return false; 
        }
        

        //
        //Add device capture filter 
        IBaseFilter* pCaptureFilter; // Video capture filter.
        hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCaptureFilter);
        if (!SUCCEEDED(hr))
        {
            std::cerr << "CameraCapture:Create capture filter failed\n"; 
            pCaptureGraphBuilder->Release();
            pMoniker->Release(); 
            return false; 
        }
         
        hr = pGraphBuilder->AddFilter(pCaptureFilter, L"Capture Filter");
        if (!SUCCEEDED(hr))
        {
            std::cerr << "CameraCapture:Add capture filter failed\n"; 
            pCaptureGraphBuilder->Release();
            pMoniker->Release(); 
            return false; 
        }

        
        //
        //Find the interface
        IAMStreamConfig* streamConf; 
        GUID captureMode = PIN_CATEGORY_PREVIEW;
        hr = pCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCaptureFilter, IID_IAMStreamConfig, (void **)&streamConf);
        if(!SUCCEEDED(hr) || !streamConf)
        {
            //fall back to capture mode is no preview mode avalible
            hr = pCaptureGraphBuilder->FindInterface(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, pCaptureFilter, IID_IAMStreamConfig, (void **)&streamConf);
            captureMode = PIN_CATEGORY_CAPTURE;
            if(!SUCCEEDED(hr) || !streamConf)
            {
                std::cerr << "CameraCapture:FindInterface failed\n"; 
                pCaptureGraphBuilder->Release();
                pMoniker->Release(); 
                return false;
            }
        }

        //
        //GetFomat stuff
        AM_MEDIA_TYPE* pAmMediaType;
        hr = streamConf->GetFormat(&pAmMediaType);
        if(!SUCCEEDED(hr))
        {
            std::cerr << "CameraCapture:GetFormat failed\n"; 
            pCaptureGraphBuilder->Release();
            pMoniker->Release(); 
            return false;
        }

        VIDEOINFOHEADER* pVih = reinterpret_cast<VIDEOINFOHEADER*>(pAmMediaType->pbFormat);
        int W = HEADER(pVih)->biWidth;
        int H = HEADER(pVih)->biHeight;

        std::cout << "Camera format:" << W << " X " << H << "\n"; 

        //
        //
        IEnumPins*      pins = 0;  
        hr = pCaptureFilter?pCaptureFilter->EnumPins(&pins):0;   // we need output pin to autogenerate rest of the graph
        IPin*           pin  = 0;  
        hr = pins?pins->Next(1,&pin, 0):0; // via graph->Render
        hr = pGraphBuilder->Render(pin); // graph builder now builds whole filter chain including MJPG decompression on some webcams

        IEnumFilters*   fil  = 0;  
        hr = pGraphBuilder->EnumFilters(&fil); // from all newly added filters
        IBaseFilter*    rnd  = 0;  
        hr = fil->Next(1, &rnd, 0); // we find last one (renderer)
        hr = rnd->EnumPins(&pins);  // because data we are intersted in are pumped to renderers input pin 
        hr = pins->Next(1,&pin, 0); // via Receive member of IMemInputPin interface
        IMemInputPin*   mem  = 0; 
        hr = pin->QueryInterface(IID_IMemInputPin,(void**)&mem);

        MSG msg={0}; 
        DWORD no;

        INT_PTR* p = 6 + *(INT_PTR**)mem; 
        VirtualProtect(&Receive_, 4, PAGE_EXECUTE_READWRITE, &no); 
        *(INT_PTR*)&Receive_=*p;   
        VirtualProtect(p, 4, PAGE_EXECUTE_READWRITE, &no);   
        *p = (INT_PTR)(&Receive); 

        IMediaControl*  ctrl = 0;  
        hr = pGraphBuilder->QueryInterface( IID_IMediaControl, (void **)&ctrl );
        hr = ctrl->Run();   

        while ( GetMessage(   &msg, 0, 0, 0 ) ) 
        {  
            TranslateMessage( &msg );   
            DispatchMessage(  &msg ); 
        }

/*
        //
        //put together
        hr = pCaptureGraphBuilder->RenderStream(&PIN_CATEGORY_PREVIEW, &MEDIATYPE_Video, pCaptureFilter, NULL, NULL);
        if (!SUCCEEDED(hr))
        {
            std::cerr << "CameraCapture:RenderStream failed\n"; 
            pCaptureGraphBuilder->Release();
            pMoniker->Release(); 
            return false; 
        }
*/

        std::cout << "TnkrVis:CameraCapture:Capture device opened successfully!\n"; 
        
        pMoniker->Release(); 
    }

    pEnum->Release();
    pDevEnum->Release();
        
    CoUninitialize();
        
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

#else

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
    isOpen = false;     
    std::cerr << "TnkrVis:CameraCapture:Open:No capture API present.\n";
    return false; 
}

bool CameraCapture::Internal::Close()
{
    std::cerr << "TnkrVis:CameraCapture:Close:No capture API present.\n";
    isOpen = false; 
    return false;
}

bool CameraCapture::Internal::LoadNextFrame()
{
    std::cerr << "TnkrVis:CameraCapture:LoadNextFrame:No capture API present.\n";
    return false; 
}

void CameraCapture::Internal::SwapBuffers()
{
    std::cerr << "TnkrVis:CameraCapture:SwapBuffers:No capture API present.\n";
}

bool CameraCapture::Internal::GetFrame(Image* frameImage)
{
    std::cerr << "TnkrVis:CameraCapture:GetFrame:No capture API present.\n";
    return false; 
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

#endif











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