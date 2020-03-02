#pragma once

#include <string>

#include "../Core/Image.h"

namespace Visi
{
namespace IO
{
	
void VISI_EXPORT WriteImageFile(std::string fileName, Image* image);
	
}
}