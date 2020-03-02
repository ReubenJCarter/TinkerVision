#include "WriteImageFile.h"

#include <FreeImage.h>

#include <iostream>

namespace Visi 
{
namespace IO
{
	
void WriteImageFile(std::string fileName, Image* image)
{
    unsigned char* data = image->GetData(); 
    unsigned int width = image->GetWidth(); 
    unsigned int height = image->GetHeight(); 
    ImageType t = image->GetType(); 

    int bypp = 1; 
    int bits = 8;
    FREE_IMAGE_TYPE fit = FREE_IMAGE_TYPE::FIT_BITMAP; 
    
    if(t == ImageType::GRAYSCALE8)
    {
        fit = FREE_IMAGE_TYPE::FIT_BITMAP; 
        bits = 8;
        bypp = 1;   
    }
    else if(t == ImageType::GRAYSCALE16)
    {
        fit = FREE_IMAGE_TYPE::FIT_UINT16;
        bits = 16;
        bypp = 2;
    }
    else if(t == ImageType::GRAYSCALE32F)
    {
        fit = FREE_IMAGE_TYPE::FIT_FLOAT;
        bits = 32;
        bypp = 4;
    }
    else if(t == ImageType::RGB8)
    {
        fit = FREE_IMAGE_TYPE::FIT_BITMAP; 
        bits = 24;
        bypp = 3;
    }
    else if(t == ImageType::RGB32F)
    {
        fit = FREE_IMAGE_TYPE::FIT_FLOAT;
        bits = 96;
        bypp = 12;
    }
    else if(t == ImageType::RGBA8)
    {
        fit = FREE_IMAGE_TYPE::FIT_BITMAP; 
        bits = 32;
        bypp = 4;  
    }
    else if(t == ImageType::RGBA32F)
    {
        fit = FREE_IMAGE_TYPE::FIT_FLOAT;
        bits = 128;
        bypp = 16;  
    }


    FIBITMAP* dib;
    if(t == ImageType::GRAYSCALE16)
    {
        
        dib = FreeImage_ConvertFromRawBitsEx(FALSE, data, fit, width, height, width * bypp, bits,  
                                   0xFFFF, 0xFFFF, 0xFFFF, FALSE); 
    }
    else if(t == ImageType::GRAYSCALE8)
    {
        dib = FreeImage_ConvertFromRawBitsEx(FALSE, data, fit, width, height, width * bypp, bits,  
                                   0xFF, 0xFF, 0xFF, FALSE); 
    }
    else
    {
        dib = FreeImage_ConvertFromRawBitsEx(FALSE, data, fit, width, height, width * bypp, bits,  
                                   FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, FALSE); 
    }



    if(dib == NULL)
    {
        std::cerr << "Visi:WriteImageFile: Failed to create image:" << fileName << "\n";
        FreeImage_Unload(dib);
        return ;
    }
    //write the file
    int flag = 0;

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	if(!dib)
    {
        std::cerr << "Visi:WriteImageFile: invalid bitmap:" << fileName << "\n";
        return ; 
    } 

    // try to guess the file format from the file extension
    fif = FreeImage_GetFIFFromFilename(fileName.c_str());
    if(fif == FIF_UNKNOWN ) 
    {
        std::cerr << "Visi:WriteImageFile: Unknown file type:" << fileName << "\n";
        return ; 
    }
/*
    // check that the plugin has sufficient writing and export capabilities ...
    WORD bpp = FreeImage_GetBPP(dib);
    if(!FreeImage_FIFSupportsWriting(fif) || !FreeImage_FIFSupportsExportBPP(fif, bpp)) 
    {
        
        std::cerr << "Visi:WriteImageFile: can't write to the file type:" << fileName << "  ";
        std::cerr << "bits:" << bits << " bpp:" << bpp << " fif:" << fif << "\n"; 
        return ;
    }*/
    // ok, we can save the file
    BOOL bSuccess = FALSE;
    bSuccess = FreeImage_Save(fif, dib, fileName.c_str(), flag);
    // unless an abnormal bug, we are done !

    FreeImage_Unload(dib);
    return ; 
}
	
}
}