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
#include "Renderer.h"
#include "FindContours.h"
#include "Invert.h"
#include "Contour.h"
#include "ChannelDemux.h"
#include "NonMaximumSuppression.h"
#include "CopyImage.h"
#include "CannyEdgeDetect.h"

#include <iostream>

int main(int argc, char *argv[])
{
	std::string testFileName = "aruco.png"; 
	if(argc > 1)
	{
		testFileName = argv[1]; 
	}



	Visi::Context context; 
	context.MakeCurrent(); 
	
	std::cout << "Running Test\n"; 


	Visi::Image image1; 
	Visi::Image image2; 
	Visi::Image image3; 

	Visi::ImageGPU imageGPU1;
	Visi::ImageGPU imageGPU2;
	Visi::ImageGPU imageGPU3;
	Visi::ImageGPU imageGPU4;

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
	Visi::WriteImageFile("image0Test.png", &image1);


	//
	//Processes
	//

	Visi::ReadImageFile(testFileName, &image1);
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

	Visi::ChannelDemux demux; 
	demux.SetChannel(2);
	demux.Run(&imageGPU2, &imageGPU3);

	Visi::CopyImage copyImage; 
	imageGPU4.Allocate(imageGPU3.GetWidth(), imageGPU3.GetWidth(), Visi::ImageType::GRAYSCALE8);
	copyImage.Run(&imageGPU3, &imageGPU4); 	

	image2.Copy(&imageGPU4);
	Visi::WriteImageFile("image8_1Test.png", &image2);

	//NMS
	Visi::NonMaximumSuppression nms; 
	nms.Run(&imageGPU2, &imageGPU3); 

	demux.SetChannel(2);
	demux.Run(&imageGPU3, &imageGPU2);

	imageGPU4.Allocate(imageGPU2.GetWidth(), imageGPU2.GetWidth(), Visi::ImageType::GRAYSCALE8);
	copyImage.Run(&imageGPU2, &imageGPU4); 	

	image2.Copy(&imageGPU4);
	Visi::WriteImageFile("image8_2Test.png", &image2);

	//Renderer
	Visi::Renderer renderer; 
	renderer.AddCircle(glm::vec2(100, 100), 20);
	renderer.AddCircle(glm::vec2(150, 100), 10, glm::vec4(1, 0, 0, 1), false, 1); 
	std::vector<glm::vec2> pl = {{100, 100}, {100, 140}, {300, 300}, {100, 300}};
	renderer.AddPolyLine(&pl, glm::vec4(0, 0, 1, 1)); 
	renderer.Run(&image1, &image2); 
	
	Visi::WriteImageFile("image9Test.png", &image2);

	//Contour Simplify Test
	Visi::Contour c;
	c.verticies = {
				   {11, 16}, {11, 17}, {11, 18}, {11, 19}, {20, 20}, {21, 20},

				   {22, 20}, {23, 21}, {25, 19}, {26, 21}, {27, 20}, {28, 20},
				   {29, 19}, {30, 20}, {31, 21}, {31, 21}, {33, 20}, {34, 20}, 
				   {35, 20}, {36, 20}, {37, 20}, {38, 19}, {39, 21}, {40, 20}, 
				   {42, 20}, {46, 20}, {47, 20}, {48, 19}, {52, 21}, {55, 20},

				   {55, 19}, {55, 18}, {55, 17}, {56, 16}, {55, 15}, {54, 14},

				   {53, 19}, {52, 18}, {51, 17}, {50, 16}, {49, 15}, {46, 14},

				   {46, 13}, {46, 12}, {46, 11}, {56, 10}, {46, 9}, {46, 8}, 
				   {46, 7}, {46, 6}, {47, 5}, {49, 4}, {51, 3}, {52, 2},

				   {53, 2}, {52, 3}, {51, 2}, {50, 3}, {49, 3}, {48, 2},
				   {47, 2}, {46, 3}, {45, 2}, {44, 2}, {43, 3}, {42, 2}, 
				   {41, 2}, {40, 3}, {38, 2}, {36, 3}, {35, 3}, {33, 2}, 
				   {32, 2}, {30, 3}, {27, 2}, {26, 2}, {25, 3}, {23, 2},
				   {21, 2}, {20, 3}, {19, 2}, {18, 2}, {16, 3}, {14, 2},  
				   {12, 2}, {10, 2},
				   {10, 3}, {10, 4}, {10, 5}, {10, 6}, {10, 7}, {11, 8},
				   {11, 9}, {11, 10}, {10, 11}, {10, 12}, {10, 13}, {11, 14},
				   {11, 17}
				   };

	std::vector<Visi::Contour> cs;
	std::vector<Visi::Contour> cs2;
	cs.push_back(c);
	Visi::Contour::ContoursSimplify(&cs, &cs2, 4);


	renderer.Clear(); 
	renderer.AddContours(&cs, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image11_1Test.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&cs2, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image11_2Test.png", &image2);

	//Find Contours
	Visi::FindContours findContours; 
	grayScale.Run(&imageGPU1, &imageGPU2); 
	adaptiveThreshold.SetThreshold(0.02); 
	adaptiveThreshold.SetSize(7); 
	adaptiveThreshold.Run(&imageGPU2, &imageGPU3); 
	threshold.Run(&imageGPU2, &imageGPU3); 
	Visi::Invert invert;
	invert.Run(&imageGPU3, &imageGPU2); 
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("image10_1Test.png", &image2);
	std::vector<Visi::Contour> contours; 
	findContours.Run(&image2, &image3, &contours); 
	Visi::WriteImageFile("image10_2Test.png", &image3);
	
	std::vector<Visi::Contour> contoursFiltered; 
	Visi::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 100);
	std::vector<Visi::Contour> contoursSimplified; 
	Visi::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 3);
	std::vector<Visi::Contour> contoursMerged; 
	Visi::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
	std::vector<Visi::Contour> contoursQuads; 
	Visi::Contour::ContoursVertCountFilter(&contoursMerged, &contoursQuads, 4, 5);

	renderer.Clear(); 
	renderer.AddContours(&contoursFiltered, true, 1, false, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image10_3Test.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&contoursSimplified, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image10_4Test.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&contoursMerged, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image10_5Test.png", &image2);

	renderer.Clear();
	renderer.AddContours(&contoursQuads, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("image10_6Test.png", &image2);


	Visi::Contour::ContoursToFile("image10_3Test.contours", &contoursFiltered); 

	//Canny Edges
	Visi::CannyEdgeDetect canny; 
	canny.SetBlurSigma(0.5); 
	canny.SetLowEdgeThreshold(0.25);
	canny.SetHighEdgeThreshold(0.5);

	canny.Run(&imageGPU1, &image2); 
	Visi::WriteImageFile("image12_1Test.png", &image2);

	
	std::cout << "DONE\n";
	return 1; 
}