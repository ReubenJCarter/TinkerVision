#include "ReadImageFile.h"

#include <FreeImage.h>

#include <iostream>

namespace Visi 
{
	
void ReadImageFile(std::string fileName, Image* image)
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
        std::cerr << "QTEngine3D:ImageReader:LoadFileGeneric Unknown file type:" << fileName << "\n";
        return ; 
    }

    if(!FreeImage_FIFSupportsReading(fif))
    {
        std::cerr << "QTEngine3D:ImageReader:LoadFileGeneric Unknown file type:" << fileName << "\n";
        return ; 
    }

    //load the file
    FIBITMAP* dib = FreeImage_Load(fif, fileName.c_str(), flag);
    if(dib == NULL)
    {
        std::cerr << "QTEngine3D:ImageReader:LoadRGBA2D: Failed to load image:" << fileName << "\n";
        return ;
    }

    //process the image data make sure its bitmap 32bit 
    FIBITMAP* temp = dib; 
    dib = FreeImage_ConvertTo32Bits(dib);
    FreeImage_Unload(temp);

    //image info
    FREE_IMAGE_TYPE type = FreeImage_GetImageType(dib);     
    unsigned width = FreeImage_GetWidth(dib);    
    unsigned height = FreeImage_GetHeight(dib);     
    unsigned bpp = FreeImage_GetBPP(dib); 
    image->Allocate(width, height, ImageType::RGBA8); 

    //get actual image data  freeimage gives brg data need to swap to rgb
    unsigned char* pixelData = (unsigned char*)FreeImage_GetBits(dib);
    //for(int i = 0; i < width * height * )

    FreeImage_Unload(dib);
    return ; 
}
	
}