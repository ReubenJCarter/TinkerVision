#include "ReadImageFile.h"

#include <FreeImage.h>

#include <iostream>

namespace Visi 
{
	
bool ReadImageFile(std::string fileName, Image* image)
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

    unsigned char* pixelData = (unsigned char*)FreeImage_GetBits(dib);
    unsigned char* imageData = image->GetData(); 
    for(unsigned int i = 0; i < width * height * bypp; i++)
    {
        imageData[i] = pixelData[i]; 
    }

    FreeImage_Unload(dib);
    return true; 
}
	
}