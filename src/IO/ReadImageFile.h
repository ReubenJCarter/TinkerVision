#pragma once

#include <string>

#include "../Core/Image.h"

namespace Visi
{
namespace IO
{
	
bool VISI_EXPORT ReadImageFile(std::string fileName, Image* image); 

}
}