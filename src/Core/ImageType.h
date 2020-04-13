#pragma once

#include "Visi_export.h"

#include <string>

namespace Visi
{
	
enum VISI_EXPORT ImageType {
    GRAYSCALE8, 
    GRAYSCALE16, 
    GRAYSCALE32F, 
    RGB8, 
    RGB32F, 
    RGBA8, 
    RGBA32F
};

class ImageTypehelper
{
    public:
        static std::string ImageTypeToString(ImageType imType)
        {
            if(imType == GRAYSCALE8) return "GRAYSCALE8"; 
            else if(imType == GRAYSCALE16) return "GRAYSCALE16"; 
            else if(imType == GRAYSCALE16) return "GRAYSCALE16"; 
            else if(imType == GRAYSCALE32F) return "GRAYSCALE32F"; 
            else if(imType == RGB8) return "RGB8"; 
            else if(imType == RGB32F) return "RGB32F"; 
            else if(imType == RGBA8) return "RGBA8"; 
            else if(imType == RGBA32F) return "RGBA32F"; 

            return "NULL"; 
        }

        static ImageType ImageTypeFromString(std::string imType)
        {
            if(imType == "GRAYSCALE8") return ImageType::GRAYSCALE8; 
            else if(imType == "GRAYSCALE16") return ImageType::GRAYSCALE16; 
            else if(imType == "GRAYSCALE16") return ImageType::GRAYSCALE16; 
            else if(imType == "GRAYSCALE32F") return ImageType::GRAYSCALE32F; 
            else if(imType == "RGB8") return ImageType::RGB8; 
            else if(imType == "RGB32F") return ImageType::RGB32F; 
            else if(imType == "RGBA8") return ImageType::RGBA8; 
            else if(imType == "RGBA32F") return ImageType::RGBA32F; 
            return RGB8; 
        }
};

}