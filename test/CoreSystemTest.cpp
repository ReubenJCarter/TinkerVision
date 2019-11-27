#include "Context.h"
#include "Image.h"
#include "ImageGPU.h"
#include "WriteImageFile.h"
#include "ReadImageFile.h"
#include "BrightnessContrast.h"

#include <iostream>

int main()
{
	Visi::Context context; 
	context.MakeCurrent(); 
	
	std::cout << "inited\n"; 

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

	Visi::ImageGPU imageGPU0;
	imageGPU0.Copy(&image0); 
	image0.Copy(&imageGPU0); 
	std::cout << "writing image file\n"; 
	Visi::WriteImageFile("image0Test.png", &image0);


	Visi::Image image1; 
	Visi::ReadImageFile("aruco.png", &image1);
	Visi::ImageGPU imageGPU1;
	Visi::ImageGPU imageGPU1_1;
	imageGPU1.Copy(&image1);
	Visi::BrightnessContrast bc; 
	bc.SetBrightness(0.0f); 
	bc.SetContrast(0.1f); 
	bc.Run(&imageGPU1, &imageGPU1_1);
	Visi::Image image1_1; 
	image1_1.Copy(&imageGPU1_1);
	Visi::WriteImageFile("image1Test.png", &image1_1);

	return 1; 
}