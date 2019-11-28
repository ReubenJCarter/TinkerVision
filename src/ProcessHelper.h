#pragma once

#include "Visi_export.h"

#include <map>

#include <glm/glm.hpp>

namespace Visi
{

class ComputeShader; 
enum ImageType;

void CompileImageComputeShaders(std::map<ImageType, ComputeShader>& comuteShaderMap, std::string shaderSrc);
glm::ivec2 ComputeWorkGroupCount(glm::ivec2 dim, glm::ivec2 groupSize);

}