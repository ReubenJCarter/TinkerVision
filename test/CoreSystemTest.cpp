#include "Context.h"
#include "Image.h"
#include "WriteImageFile.h"

int main()
{
	Visi::Context context; 
	context.MakeCurrent(); 

	Visi::Image image0; 
	image0.Allocate(256, 256, Visi::ImageType::RGBA8);
	for(int i = 0; i < 256; i++)
	{
		for(int j = 0; j < 256; j++)
		{
			image0.GetData()[(i * 256 + j) * 4 + 0]  = i; 
			image0.GetData()[(i * 256 + j) * 4 + 1]  = j; 
			image0.GetData()[(i * 256 + j) * 4 + 2]  = (i + j) / 2; 
			image0.GetData()[(i * 256 + j) * 4 + 3] = 255;
		}
	}
	Visi::WriteImageFile("image0Test.png", &image0);

	std::string shaderSrc0=
	R"(
	#version 430
	layout(rgba32f, binding=0) uniform image2D outputImage;
	layout (local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
	void main()
	{
		
	}
	)";

	return 1; 
}