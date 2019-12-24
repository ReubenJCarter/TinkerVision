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
#include "Blend.h"
#include "ChannelDemux.h"
#include "HighLowThreshold.h"
#include "InRange.h"
#include "CannyEdgeDetect.h"
#include "MedianFilter.h"
#include "Morph.h"

#include <iostream>

void ARUCOMarkerDetection(Visi::Image& image1)
{
	Visi::Image image2; 
	Visi::Image image3; 

	Visi::ImageGPU imageGPU1;
	Visi::ImageGPU imageGPU2;
	Visi::ImageGPU imageGPU3;
	Visi::ImageGPU imageGPU4;

	imageGPU1.Copy(&image1);

	//
	//Processes
	//
	//Greyscale
	Visi::GrayScale grayScale;
	grayScale.Run(&imageGPU1, &imageGPU2); 
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("MarkerTestGrey.png", &image2);

	//Adaptive Threshold
	Visi::AdaptiveThreshold adaptiveThreshold; 
	adaptiveThreshold.SetThreshold(0.02); 
	adaptiveThreshold.SetSize(7); 
	adaptiveThreshold.Run(&imageGPU2, &imageGPU3); 

	//Normal threshold
	Visi::Threshold threshold; 
	threshold.SetThreshold(0.5); 
	threshold.Run(&imageGPU2, &imageGPU3); 

	//Invert
	Visi::Invert invert;
	invert.Run(&imageGPU3, &imageGPU2); 
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("MarkerTest1.png", &image2);

	//Find Contours
	Visi::FindContours findContours; 
	std::vector<Visi::Contour> contours; 
	findContours.Run(&image2, &image3, &contours); 
	Visi::WriteImageFile("MarkerTest2.png", &image3);
	
	std::vector<Visi::Contour> contoursFiltered; 
	Visi::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 100);
	std::vector<Visi::Contour> contoursSimplified; 
	Visi::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 6);
	std::vector<Visi::Contour> contoursMerged; 
	Visi::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
	Visi::Contour::ContoursSimplify(&contoursMerged, &contoursSimplified, 5);
	std::vector<Visi::Contour> contoursQuads; 
	Visi::Contour::ContoursVertCountFilter(&contoursSimplified, &contoursQuads, 4, 5);

	Visi::Renderer renderer; 
	renderer.Clear(); 
	renderer.AddContours(&contoursFiltered, true, 1, false, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTest3.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&contoursSimplified, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTest4.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&contoursMerged, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTest5.png", &image2);

	renderer.Clear();
	renderer.AddContours(&contoursQuads, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTest6.png", &image2);


	Visi::Contour::ContoursToFile("MarkerTest.contours", &contoursQuads); 
}

void BarCodeDetect(Visi::Image& image1)
{	
	Visi::Image image2; 
	Visi::Image image3; 

	Visi::ImageGPU imageGPU1;
	Visi::ImageGPU imageGPU2;
	Visi::ImageGPU imageGPU3;
	Visi::ImageGPU imageGPU4;


	imageGPU1.Copy(&image1);

	//
	//Processes
	//

	Visi::Image imageCanny; 
	Visi::CannyEdgeDetect canny; 
	canny.SetBlurSigma(2); 
	canny.SetLowEdgeThreshold(0.01);
	canny.SetHighEdgeThreshold(0.06);

	canny.Run(&imageGPU1, &imageCanny); 
	Visi::WriteImageFile("MarkerTestCanny.png", &imageCanny);

	//Median filter
	Visi::MedianFilter medianFilter;
	medianFilter.SetSize(5); 
	medianFilter.Run(&imageGPU1, &imageGPU4); 
	medianFilter.Run(&imageGPU4, &imageGPU2); 
	medianFilter.Run(&imageGPU2, &imageGPU4); 
	image2.Copy(&imageGPU4);
	Visi::WriteImageFile("MarkerTestMedian.png", &image2);

	//RGBToHSV vv HSVToRGB
	Visi::RGBToHSV rgbtohsv; 
	rgbtohsv.Run(&imageGPU4, &imageGPU2); 
	image2.Copy(&imageGPU2);
	Visi::WriteImageFile("MarkerTestHSV.png", &image2);

	//Blur
	Visi::GaussianBlur blur; 
	blur.SetSigma(3);
	//blur.Run(&imageGPU4, &imageGPU2);  

	//inrange on hsv to pull out the red color of the tape
	Visi::InRange inRange; 
	inRange.SetLowThreshold(glm::vec3(0.8, 0.3, 0.3));
	inRange.SetHighThreshold(glm::vec3(1.0, 1.0, 1.0) ); 
	inRange.Run(&imageGPU2, &imageGPU3); 
	image2.Copy(&imageGPU3);
	Visi::WriteImageFile("MarkerTestHSVRedThres.png", &image2);

	//in range on hsv to pull out black stripes of barcode
	inRange.SetLowThreshold(glm::vec3(0.0, 0.0, 0.0));
	inRange.SetHighThreshold(glm::vec3(1.0, 1.0, 0.4) ); 
	inRange.Run(&imageGPU2, &imageGPU4); 
	image3.Copy(&imageGPU4);
	Visi::WriteImageFile("MarkerTestHSVBlackThres.png", &image3);

	//Morph and dillate
	Visi::Morph morph; 
	morph.SetMode(Visi::Morph::ERODE);
	morph.SetKernel(5, Visi::Morph::SQUARE);
	morph.Run(&imageGPU3, &imageGPU2);  
	image3.Copy(&imageGPU2);
	Visi::WriteImageFile("MarkerTestErode0.png", &image3);
	morph.Run(&imageGPU2, &imageGPU3);  
	image3.Copy(&imageGPU3);
	Visi::WriteImageFile("MarkerTestErode1.png", &image3);
	morph.Run(&imageGPU3, &imageGPU2);  
	image3.Copy(&imageGPU2);
	Visi::WriteImageFile("MarkerTestErode2.png", &image3);

	//Find Contours
	Visi::FindContours findContours; 
	std::vector<Visi::Contour> contours; 
	findContours.Run(&image2, &image3, &contours); 
	Visi::WriteImageFile("MarkerTestFindContours.png", &image3);
	
	std::vector<Visi::Contour> contoursFiltered; 
	Visi::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 5);
	std::vector<Visi::Contour> contoursSimplified; 
	Visi::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 3);
	std::vector<Visi::Contour> contoursMerged; 
	Visi::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
	Visi::Contour::ContoursSimplify(&contoursMerged, &contoursSimplified, 5);
	//std::vector<Visi::Contour> contoursQuads; 
	//Visi::Contour::ContoursVertCountFilter(&contoursSimplified, &contoursQuads, 4, 5);

	Visi::Renderer renderer; 
	renderer.Clear(); 
	renderer.AddContours(&contoursFiltered, true, 1, false, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTestContours.png", &image2);

	renderer.Clear(); 
	renderer.AddContours(&contoursSimplified, true, 4, true, true); 
	renderer.Run(&image1, &image2); 
	Visi::WriteImageFile("MarkerTestSimplifiedContours.png", &image2);

}

int main(int argc, char *argv[])
{
	if(argc <= 1)
	{
		std::cerr << "To Few Arguments \n"; 
		return 0 ; 
	}

	Visi::Context context; 
	context.MakeCurrent(); 
	
	std::cout << "Running Test\n"; 
	
	Visi::Image image1; 
	Visi::ReadImageFile(argv[1], &image1);

	//ARUCOMarkerDetection(image1); 
	BarCodeDetect(image1); 
	
	std::cout << "DONE\n";
	return 1; 
}