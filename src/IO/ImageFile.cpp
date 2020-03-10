#include "ImageFile.h"

#include <FreeImage.h>

#include <iostream>

namespace Visi 
{
namespace IO
{
	
bool ImageFile::Write(std::string fileName, Image* image)
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
        return false;
    }
    //write the file
    int flag = 0;

    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;

	if(!dib)
    {
        std::cerr << "Visi:WriteImageFile: invalid bitmap:" << fileName << "\n";
        return false; 
    } 

    // try to guess the file format from the file extension
    fif = FreeImage_GetFIFFromFilename(fileName.c_str());
    if(fif == FIF_UNKNOWN ) 
    {
        std::cerr << "Visi:WriteImageFile: Unknown file type:" << fileName << "\n";
        return false; 
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
    return true; 
}
	
bool ImageFile::Read(std::string fileName, Image* image)
{
    //load the file
    int flag = 0;
    //load anynumber of file formats
    FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
    fif = FreeImage_GetFileType(fileName.c_str(), 0);
    if(fif == FIF_UNKNOWN) 
    {
        fif = FreeImage_GetFIFFromFilename(fileName.c_str());
    }

    //check if the file is supported 
    if((fif == FIF_UNKNOWN)) 
    {
        std::cerr << "Visi:ReadImageFile: LoadFileGeneric Unknown file type:" << fileName << "\n";
        return false; 
    }

    if(!FreeImage_FIFSupportsReading(fif))
    {
        std::cerr << "Visi:ReadImageFile: LoadFileGeneric Unknown file type:" << fileName << "\n";
        return false; 
    }

    //load the file
    FIBITMAP* dib = FreeImage_Load(fif, fileName.c_str(), flag);
    if(dib == NULL)
    {
        std::cerr << "Visi:ReadImageFile: Failed to load image:" << fileName << "\n";
        return false;
    }

    //image info
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);     
    unsigned int width = FreeImage_GetWidth(dib);    
    unsigned int height = FreeImage_GetHeight(dib);     
    unsigned int bpp = FreeImage_GetBPP(dib); 
    unsigned int bypp;
    

    ImageType imageType;
    bool imageTypeFound = false;  

    if(type == FREE_IMAGE_TYPE::FIT_BITMAP)
    {
        if(bpp == 8)
        {
            imageType = ImageType::GRAYSCALE8;
            imageTypeFound = true; 
            bypp = 1;
        }
        else if(bpp == 16)
        {
            imageType = ImageType::GRAYSCALE16;
            imageTypeFound = true;
            bypp = 2;
        }
        else if(bpp == 24)
        {
            imageType = ImageType::RGB8; 
            imageTypeFound = true;
            bypp = 3;
        }
        else if(bpp == 32)
        {
            imageType = ImageType::RGBA8;
            imageTypeFound = true;
            bypp = 4;
        }
    }
    
    if(!imageTypeFound)
    {
        std::cerr << "Visi:ReadImageFile: Failed to find correct type for image:Failed to load image:" << fileName << "\n";
        return false;
    }
    image->Allocate(width, height, imageType); 
/*
    unsigned char* pixelData = (unsigned char*)FreeImage_GetBits(dib);
    unsigned char* imageData = image->GetData(); 
    for(unsigned int i = 0; i < width * height * bypp; i++)
    {
        imageData[i] = pixelData[i]; 
    }
*/

    unsigned char* imageData = image->GetData();
    int counter = 0;  
    for(int j = 0; j < height; j++ )
    {
        unsigned char* pixelData = (unsigned char*)FreeImage_GetScanLine(dib, j); 
        for(int i = 0; i < width * bypp; i++)
        {
            imageData[counter] = pixelData[i]; 
            counter++;
        } 
    }

    FreeImage_Unload(dib);
    return true; 
}
	
}
}