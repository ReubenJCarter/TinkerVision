#include "Context.h"
#include "Image.h"
#include "ImageGPU.h"
#include "WriteImageFile.h"
#include "ReadImageFile.h"
#include "BrightnessContrast.h"
#include "Threshold.h"
#include "GrayScale.h"
#include "RGBToHSV.h"
#include "HSVToRGB.h"
#include "AdaptiveThreshold.h"
#include "MedianFilter.h"
#include "GaussianBlur.h"
#include "Sobel.h"

#include <iostream>

int main()
{
	Visi::Context context; 
	context.MakeCurrent(); 
	
	std::cout << "inited\n"; 


	Visi::Image image1; 
	Visi::Image image2; 
	Visi::Image image3; 

	Visi::ImageGPU imageGPU1;
	Visi::ImageGPU imageGPU2;
	Visi::ImageGPU imageGPU3;

	//
	//Pixel access
	//
	

	image1.Allocate(256, 256, Visi::ImageType::RGBA8);
	for(int i = 0; i < 256; i++)
	{
		for(int j = 0; j < 256; j++)
		{
			image1.GetData()[(i * 256 + j) * 4 + 0]  = i; 
			image1.GetData()[(i * 256 + j) * 4 + 1]  = j; 
			image1.GetData()[(i * 256 + j) * 4 + 2]  = (i + j) / 2; 
			image1.GetData()[(i * 256 + j) * 4 + 3] = 255;
		}
	}

	Visi::ImageGPU imageGPU;
	imageGPU.Copy(&image1); 
	image1.Copy(&imageGPU); 
	std::cout << "writing image file\n"; 
	Visi::WriteImageFile("image0Test.png", &image1);


	//
	//Processes
	//

	Visi::ReadImageFile("aruco.png", &image1);
	imageGPU1.Copy(&image1);

	//Birghtness Contrast
	Visi::BrightnessContrast brightnessContrast; 
	brightnessContrast.SetBrightness(-1); 
	brightnessContrast.SetContrast(3); 
	brightnessContrast.Run(&imageGPU1, &imageGPU2);
	
	
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image1Test.png", &image2);

	//Threshold
	Visi::Threshold threshold; 
	threshold.SetThreshold(0.5); 
	threshold.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image2Test.png", &image2);

	//GrayScale
	Visi::GrayScale grayScale; 
	grayScale.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image3Test.png", &image2);

	//RGBToHSV vv HSVToRGB
	Visi::RGBToHSV rgbtohsv; 
	rgbtohsv.Run(&imageGPU1, &imageGPU2); 
	Visi::HSVToRGB hsvtorgb; 
	hsvtorgb.Run(&imageGPU2, &imageGPU3); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image4_0Test.png", &image2);
	image3.Copy(&imageGPU3);
	Visi::WriteImageFile("image4_1Test.png", &image3);

	//Adaptive Threshold
	grayScale.Run(&imageGPU1, &imageGPU2); 
	
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image5_1Test.png", &image2);
	
	Visi::AdaptiveThreshold adaptiveThreshold; 
	adaptiveThreshold.SetThreshold(0.02); 
	adaptiveThreshold.SetSize(7); 
	adaptiveThreshold.Run(&imageGPU2, &imageGPU3); 

	image3.Copy(&imageGPU3);
	Visi::WriteImageFile("image5_2Test.png", &image3);

	//Median Filter
	Visi::MedianFilter medianFilter; 
	medianFilter.SetSize(3); 
	medianFilter.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image6_1Test.png", &image2);

	medianFilter.SetSize(5); 
	medianFilter.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image6_2Test.png", &image2);

	//Gaussian Blur
	Visi::GaussianBlur gaussianBlur; 
	gaussianBlur.SetSigma(10); 
	gaussianBlur.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image7Test.png", &image2);

	//Sobel
	Visi::Sobel sobel; 
	sobel.Run(&imageGPU1, &imageGPU2); 

	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image8Test.png", &image2);

	return 1; 
}