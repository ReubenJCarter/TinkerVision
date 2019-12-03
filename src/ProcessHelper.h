#pragma once

#include "Visi_export.h"

#include <map>

#include <glm/glm.hpp>

namespace Visi
{

class ComputeShader; 
class Image;
enum ImageType;

void CompileImageComputeShaders(std::map<ImageType, ComputeShader>& comuteShaderMap, std::string shaderSrc);
glm::ivec2 ComputeWorkGroupCount(glm::ivec2 dim, glm::ivec2 groupSize);
glm::vec4 GetPixel(Image* image, int x, int y); 
glm::ivec4 GetPixelUI(Image* image, int x, int y); 
glm::ivec4 GetPixelI(Image* image, int x, int y); 

void SetPixel(Image* image, int x, int y, glm::vec4 v); 

}