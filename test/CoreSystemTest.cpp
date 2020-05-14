#include "Core/Context.h"
#include "Core/Image.h"
#include "Core/ImageGPU.h"
#include "Core/VectorMath.h"
#include "Core/Contour.h"
#include "Core/Window.h"

#include "IO/ImageFile.h"
#include "IO/VideoFile.h"
#include "IO/VideoHelper.h"

#include "Process/BrightnessContrast.h"
#include "Process/Threshold.h"
#include "Process/GrayScale.h"
#include "Process/RGBToHSV.h"
#include "Process/HSVToRGB.h"
#include "Process/AdaptiveThreshold.h"
#include "Process/MedianFilter.h"
#include "Process/GaussianBlur.h"
#include "Process/Sobel.h"
#include "Process/Renderer.h"
#include "Process/FindContours.h"
#include "Process/Invert.h"
#include "Process/ChannelDemux.h"
#include "Process/NonMaximumEdgeSuppression.h"
#include "Process/CopyImage.h"
#include "Process/GaussianDerivative.h"
#include "Process/Normalize.h"
#include "Process/Thinning.h"
#include "Process/ApproxDistanceTransform.h"
#include "Process/InRange.h"
#include "Process/LocalMaxima.h"
#include "Process/CameraDistortion.h"
#include "Process/AverageFilter.h"
#include "Process/ChannelMapper.h"
#include "Process/Downsample.h"
#include "Process/Upsample.h"
#include "Process/StereoMatchSAD.h"
#include "Process/Mipmaps.h"
#include "Process/Rotate.h"
#include "Process/Translate.h"
#include "Process/ClearColor.h"
#include "Process/TemplateMatch.h"
#include "Process/GaussianFunction.h"

#include "CompositeProcess/CannyEdgeDetect.h"
#include "CompositeProcess/CornerDetector.h"
#include "CompositeProcess/ARUCODetector.h"
#include "CompositeProcess/Sift.h"

#include "ComputeGraph/RegisterNodes.h"
#include "ComputeGraph/Node.h"
#include "ComputeGraph/Graph.h"
#include "ComputeGraph/Nodes/SourceNodes.h"
#include "ComputeGraph/Nodes/ImageNodes.h"
#include "ComputeGraph/Nodes/IONodes.h"
#include "ComputeGraph/Nodes/ProcessNodes.h"

#include <iostream>
#include <thread>
#include <atomic>
#include <sstream>


int main(int argc, char *argv[])
{
	std::string testFileName = "aruco.png"; 
	TnkrVis::Context context; 
	context.MakeCurrent(); 

	TnkrVis::ComputeGraph::RegisterNodes(); 

	if(argc == 2)
	{
		testFileName = argv[1]; 
	
		std::cout << "Running Test\n"; 

		TnkrVis::Image image1; 
		TnkrVis::Image image2; 
		TnkrVis::Image image3; 

		TnkrVis::ImageGPU imageGPU1;
		TnkrVis::ImageGPU imageGPU2;
		TnkrVis::ImageGPU imageGPU3;
		TnkrVis::ImageGPU imageGPU4;

		//
		//Pixel access
		//
		

		image1.Allocate(256, 256, TnkrVis::ImageType::RGBA8);
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

		TnkrVis::ImageGPU imageGPU;
		imageGPU.Copy(&image1); 
		image1.Copy(&imageGPU); 
		TnkrVis::IO::ImageFile::Write("image0Test.png", &image1);


		//
		//Processes
		//

		TnkrVis::IO::ImageFile::Read(testFileName, &image1);
		imageGPU1.Copy(&image1);

		//Birghtness Contrast
		std::cout << "Brightness Contrast\n"; 
		TnkrVis::Process::BrightnessContrast brightnessContrast; 
		brightnessContrast.SetBrightness(-1); 
		brightnessContrast.SetContrast(3); 
		brightnessContrast.Run(&imageGPU1, &imageGPU2);
		
		
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image1_1Test.png", &image2);

		brightnessContrast.Run(&image1, &image2); 

		TnkrVis::IO::ImageFile::Write("image1_2Test.png", &image2);

		//Threshold
		std::cout << "Threshold\n"; 
		TnkrVis::Process::Threshold threshold; 
		threshold.SetThreshold(0.5); 
		threshold.Run(&imageGPU1, &imageGPU2); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image2Test.png", &image2);

		//Thin
		std::cout << "Thin\n"; 
		TnkrVis::Process::Thinning thinning; 
		//thinning.Run(&image2, &image3); 
		//TnkrVis::ImageFile::Write("image2_1Test.png", &image3);

		//GrayScale
		std::cout << "Grayscale\n"; 
		TnkrVis::Process::GrayScale grayScale; 
		grayScale.Run(&imageGPU1, &imageGPU2); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image3Test.png", &image2);

		//Aprox Distance Transform
		std::cout << "Aprox Distance Transform\n"; 
		TnkrVis::Process::InRange inRange; 
		inRange.SetLowThreshold(TnkrVis::Vec3(0.5, 0.0, 0.0));
		inRange.SetHighThreshold(TnkrVis::Vec3(1.0, 1.0, 1.0) ); 
		inRange.Run(&imageGPU2, &imageGPU3); 
		TnkrVis::Process::Invert inv;
		inv.Run(&imageGPU3, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image3_1Test.png", &image2);
		TnkrVis::Process::ApproxDistanceTransform adt;
		adt.Run(&image2, &image3); 
		TnkrVis::Process::Normalize nrm;
		nrm.Run(&image3, &image2); 
		TnkrVis::IO::ImageFile::Write("image3_2Test.png", &image2);

		//LocalMaxima
		std::cout << "Local Maxima\n"; 
		TnkrVis::Process::GaussianBlur gb; 
		gb.SetSigma(3); 
		imageGPU2.Copy(&image2); 
		gb.Run(&imageGPU2, &imageGPU3); 
		image2.Copy(&imageGPU3); 
		TnkrVis::IO::ImageFile::Write("image3_3Test.png", &image2);
		TnkrVis::Process::LocalMaxima localMax; 
		localMax.Run(&image2, &image3);
		TnkrVis::IO::ImageFile::Write("image3_4Test.png", &image3);

		//RGBToHSV vv HSVToRGB
		std::cout << "RGBToHSV vv HSVToRGB\n"; 
		TnkrVis::Process::RGBToHSV rgbtohsv; 
		rgbtohsv.Run(&imageGPU1, &imageGPU2); 
		TnkrVis::Process::HSVToRGB hsvtorgb; 
		hsvtorgb.Run(&imageGPU2, &imageGPU3); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image4_0Test.png", &image2);
		image3.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image4_1Test.png", &image3);

		//Adaptive Threshold
		std::cout << "Adaptive THreshold\n"; 
		grayScale.Run(&imageGPU1, &imageGPU2); 
		
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image5_1Test.png", &image2);
		
		TnkrVis::Process::AdaptiveThreshold adaptiveThreshold; 
		adaptiveThreshold.SetThreshold(0.02); 
		adaptiveThreshold.SetSize(7); 
		adaptiveThreshold.Run(&imageGPU2, &imageGPU3); 

		image3.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image5_2Test.png", &image3);

		adaptiveThreshold.Run(&image2, &image3);
		TnkrVis::IO::ImageFile::Write("image5_3Test.png", &image3);

		//Median Filter
		std::cout << "Median Filter\n"; 
		TnkrVis::Process::MedianFilter medianFilter; 
		medianFilter.SetSize(3); 
		medianFilter.Run(&imageGPU1, &imageGPU2); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image6_1Test.png", &image2);

		medianFilter.SetSize(5); 
		medianFilter.Run(&imageGPU1, &imageGPU2); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image6_2Test.png", &image2);

		//Gaussian Blur
		std::cout << "Guassian Blur\n"; 
		TnkrVis::Process::GaussianBlur gaussianBlur; 
		gaussianBlur.SetSigma(10); 
		gaussianBlur.Run(&imageGPU1, &imageGPU2); 

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image7Test.png", &image2);

		//Sobel
		std::cout << "Sobel\n"; 
		TnkrVis::Process::Sobel sobel; 
		sobel.Run(&imageGPU1, &imageGPU2); 

		imageGPU3.Allocate(imageGPU1.GetWidth(), imageGPU1.GetHeight(), TnkrVis::ImageType::GRAYSCALE8); 
		TnkrVis::Process::ChannelDemux demux; 
		demux.SetChannel(2);
		demux.Run(&imageGPU2, &imageGPU3);

		image2.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image8_1Test.png", &image2);

		//NMS
		imageGPU3.Allocate(imageGPU2.GetWidth(), imageGPU2.GetHeight(), imageGPU2.GetType()); 
		std::cout << "NonMaximumEdgeSuppression\n"; 
		TnkrVis::Process::NonMaximumEdgeSuppression nms; 
		nms.Run(&imageGPU2, &imageGPU3); 

		imageGPU2.Allocate(imageGPU2.GetWidth(), imageGPU2.GetHeight(), TnkrVis::ImageType::GRAYSCALE8); 
		demux.SetChannel(2);
		demux.Run(&imageGPU3, &imageGPU2);

		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image8_2Test.png", &image2);

		//Renderer
		std::cout << "Renderer\n"; 
		TnkrVis::Process::Renderer renderer; 
		renderer.AddCircle(TnkrVis::Vec2(100, 100), 20);
		renderer.AddCircle(TnkrVis::Vec2(150, 100), 10, TnkrVis::Color(1, 0, 0, 1), false, 1); 
		std::vector<TnkrVis::Vec2> pl = {{100, 100}, {100, 140}, {300, 300}, {100, 300}};
		renderer.AddPolyLine(&pl, TnkrVis::Color(0, 0, 1, 1)); 
		renderer.Run(&image1, &image2); 
		
		TnkrVis::IO::ImageFile::Write("image9Test.png", &image2);

		//Contour Simplify Test
		std::cout << "Contour Simplify Test\n"; 
		TnkrVis::Contour c;
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

		std::vector<TnkrVis::Contour> cs;
		std::vector<TnkrVis::Contour> cs2;
		std::vector<TnkrVis::Contour> cs3;
		cs.push_back(c);
		TnkrVis::Contour::ContoursSimplify(&cs, &cs2, 4);

		cs3.resize(cs2.size());
		for(int i = 0; i < cs2.size(); i++)
		{ 
			auto c = &(cs2[i]); 
			std::vector<int> cInx;
			TnkrVis::Contour::FindConvexHull(c, &cInx);
			for(int j = 0; j < cInx.size(); j++)
			{
				cs3[i].verticies.push_back(c->verticies[cInx[j]]); 
			}
		}

		TnkrVis::BoundingBox bb = TnkrVis::Contour::FindBoundingBox(&c); 

		renderer.Clear(); 
		renderer.AddContours(&cs, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image11_1Test.png", &image2);

		renderer.Clear(); 
		renderer.AddContours(&cs2, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image11_2Test.png", &image2);

		renderer.Clear(); 
		renderer.AddContours(&cs3, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image11_3Test.png", &image2);

		renderer.Clear(); 
		renderer.AddBoundingBox(&bb, TnkrVis::Color(1, 0, 1, 1)); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image11_4Test.png", &image2);

		//Find Contours
		std::cout << "Find Contours\n"; 
		TnkrVis::Process::FindContours findContours; 
		imageGPU2.AllocateLike(&imageGPU1);
		imageGPU3.AllocateLike(&imageGPU1);
		imageGPU4.AllocateLike(&imageGPU1);
		grayScale.Run(&imageGPU1, &imageGPU2); 
		adaptiveThreshold.SetThreshold(0.02); 
		adaptiveThreshold.SetSize(7); 
		adaptiveThreshold.Run(&imageGPU2, &imageGPU3); 
		TnkrVis::Process::Invert invert;
		invert.Run(&imageGPU3, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image10_1Test.png", &image2);
		std::vector<TnkrVis::Contour> contours; 
		findContours.Run(&image2, &image3, &contours); 
		TnkrVis::IO::ImageFile::Write("image10_2Test.png", &image3);
		
		std::vector<TnkrVis::Contour> contoursFiltered; 
		TnkrVis::Contour::ContoursVertCountFilter(&contours, &contoursFiltered, 100);
		std::vector<TnkrVis::Contour> contoursSimplified; 
		TnkrVis::Contour::ContoursSimplify(&contoursFiltered, &contoursSimplified, 3);
		std::vector<TnkrVis::Contour> contoursMerged; 
		TnkrVis::Contour::ContoursMergeVerticies(&contoursSimplified, &contoursMerged, 4);
		TnkrVis::Contour::ContoursSimplify(&contoursMerged, &contoursSimplified, 5);
		std::vector<TnkrVis::Contour> contoursQuads; 
		TnkrVis::Contour::ContoursVertCountFilter(&contoursSimplified, &contoursQuads, 4, 5);

		renderer.Clear(); 
		renderer.AddContours(&contoursFiltered, true, 1, false, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image10_3Test.png", &image2);

		renderer.Clear(); 
		renderer.AddContours(&contoursSimplified, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image10_4Test.png", &image2);

		renderer.Clear(); 
		renderer.AddContours(&contoursMerged, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image10_5Test.png", &image2);

		renderer.Clear();
		/*
		std::vector<TnkrVis::Contour> conxHullContours;
		conxHullContours.resize(contoursQuads.size()); 
		for(int i = 0; i < contoursQuads.size(); i++)
		{
			std::vector<int> cInx;
			TnkrVis::Contour::FindConvexHull(&(contoursQuads[i]), &cInx);
			for(int j = 0; j < cInx.size(); j++)
			{
				conxHullContours[i].verticies.push_back(contoursQuads[i].verticies[cInx[j]]); 
			}

			TnkrVis::BoundingBox bb = TnkrVis::Contour::FindBoundingBox(&(contoursQuads[i])); 
			renderer.AddBoundingBox(&bb, TnkrVis::Color(1, 0, 1, 1)); 
		}
		renderer.AddContours(&contoursQuads, true, 4, true, true); 
		renderer.Run(&image1, &image2); 
		TnkrVis::IO::ImageFile::Write("image10_6Test.png", &image2);
		*/

		TnkrVis::Contour::ContoursToFile("image10_3Test.contours", &contoursFiltered); 

		//Canny Edges
		std::cout << "Canny Edges\n"; 
		TnkrVis::CompositeProcess::CannyEdgeDetect canny; 
		canny.SetBlurSigma(0.5); 
		canny.SetLowEdgeThreshold(0.1);
		canny.SetHighEdgeThreshold(0.2);
		image2.Allocate(imageGPU1.GetWidth(), imageGPU1.GetHeight(), TnkrVis::ImageType::GRAYSCALE16); 
		canny.Run(&imageGPU1, &image2); 
		TnkrVis::IO::ImageFile::Write("image12_1Test.png", &image2);

		//GaussianDerivative
		std::cout << "GaussianDerivative\n"; 
		grayScale.Run(&imageGPU1, &imageGPU2); 

		TnkrVis::Process::GaussianDerivative guassDeriv; 
		guassDeriv.SetDirection(TnkrVis::Process::GaussianDerivative::Direction::HORIZONTAL); 
		guassDeriv.Run(&imageGPU2, &imageGPU3); 

		image2.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image13_1Test.png", &image2);

		guassDeriv.SetDirection(TnkrVis::Process::GaussianDerivative::Direction::VERTICAL); 
		guassDeriv.Run(&imageGPU2, &imageGPU3); 

		image2.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image13_2Test.png", &image2);
		
		//Corner detector
		std::cout << "Corner detector\n"; 
		grayScale.Run(&imageGPU1, &imageGPU2); 

		TnkrVis::ImageGPU imageGPU5; 
		TnkrVis::CompositeProcess::CornerDetector cornerDetector; 
		cornerDetector.Run(&imageGPU2, &imageGPU5);

		imageGPU4.Allocate(imageGPU5.GetWidth(), imageGPU5.GetHeight(), TnkrVis::ImageType::RGBA32F); 
		demux.SetChannel(0);
		demux.Run(&imageGPU5, &imageGPU4);
		image2.Copy(&imageGPU4);
		TnkrVis::Process::Normalize normalize; 
		normalize.Run(&image2, &image3); 
		imageGPU3.Copy(&image3);
		imageGPU2.Allocate(imageGPU3.GetWidth(), imageGPU3.GetHeight(), TnkrVis::ImageType::GRAYSCALE8); 
		TnkrVis::Process::CopyImage copyImage; 
		copyImage.Run(&imageGPU3, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image14_1Test.png", &image2);

		demux.SetChannel(1);
		demux.Run(&imageGPU5, &imageGPU4);
		image2.Copy(&imageGPU4);
		normalize.Run(&image2, &image3); 
		imageGPU3.Copy(&image3);
		imageGPU2.Allocate(imageGPU3.GetWidth(), imageGPU3.GetHeight(), TnkrVis::ImageType::GRAYSCALE8); 
		copyImage.Run(&imageGPU3, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image14_2Test.png", &image2);

		//Camera Distortion
		std::cout << "Camera Distortion\n"; 
		TnkrVis::Process::CameraDistortion camDistort;
		camDistort.SetRadialCoefs(-0.1f, 0, 0);
		camDistort.SetTangentialCoefs(0, 0); 
		camDistort.SetFocalLength(1); 
		camDistort.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image15_1Test.png", &image2);

		//AverageFilter
		std::cout << "Average Filter\n"; 
		TnkrVis::Process::AverageFilter avFilter;
		avFilter.SetSize(5); 
		avFilter.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image16_1Test.png", &image2);

		//ChannelMapper
		std::cout << "Channel Mapper\n"; 
		TnkrVis::Process::ChannelMapper chMapr;
		chMapr.SetChannelMap(2, -1, -1, 3); 
		imageGPU2.Allocate(imageGPU1.GetWidth(), imageGPU1.GetHeight(), TnkrVis::ImageType::GRAYSCALE8); 
		chMapr.Run(&imageGPU1, &imageGPU2);
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image17_1Test.png", &image2);

		//DownSample
		std::cout << "Downsample\n"; 
		TnkrVis::Process::Downsample dwnSmpl;
		float dsScale = 0.5f;
		imageGPU2.Allocate(imageGPU1.GetWidth() * dsScale, imageGPU1.GetHeight() * dsScale, imageGPU1.GetType()); 
		dwnSmpl.SetMode(TnkrVis::Process::Downsample::Mode::NEAREST); 
		dwnSmpl.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image18_1Test.png", &image2);
		dwnSmpl.SetMode(TnkrVis::Process::Downsample::Mode::BOX); 
		dwnSmpl.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image18_2Test.png", &image2);
		dwnSmpl.SetMode(TnkrVis::Process::Downsample::Mode::BILINEAR); 
		dwnSmpl.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image18_3Test.png", &image2);

		//UpSample
		std::cout << "Upsample\n"; 
		TnkrVis::Process::Upsample upSmpl;
		float usScale = 4.0f;
		imageGPU2.Allocate(imageGPU1.GetWidth() * usScale, imageGPU1.GetHeight() * usScale, imageGPU1.GetType()); 
		upSmpl.SetMode(TnkrVis::Process::Upsample::Mode::BILINEAR); 
		upSmpl.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image19_1Test.png", &image2);
		upSmpl.SetMode(TnkrVis::Process::Upsample::Mode::NEAREST); 
		upSmpl.Run(&imageGPU1, &imageGPU2); 
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image19_2Test.png", &image2);

		//Mipmaps
		std::cout << "Mipmaps\n"; 
		TnkrVis::Process::Mipmaps mmaps;
		std::vector<TnkrVis::ImageGPU> mmstack; 
		std::vector<TnkrVis::Image> mmstackCPU; 
		mmaps.Run(&imageGPU1, &mmstack); 
		for(int i = 0; i < mmstack.size(); i++)
		{
			std::stringstream ss;
			ss << "image20_" << i << ".png";
			image2.Copy(&(mmstack[i])); 
			TnkrVis::IO::ImageFile::Write(ss.str(), &image2);
			if(i > 3)
				break; //break after saving first 4 images
		}
		mmaps.Run(&image1, &mmstackCPU); 
		for(int i = 0; i < mmstackCPU.size(); i++)
		{
			std::stringstream ss;
			ss << "image21_" << i << ".png";
			TnkrVis::IO::ImageFile::Write(ss.str(), &(mmstackCPU[i]));
			if(i > 3)
				break; //break after saving first 4 images
		}

		//Sift
		std::cout << "Sift\n"; 
		TnkrVis::CompositeProcess::Sift sift;
		sift.Run(&imageGPU1, &imageGPU2);
		std::vector<TnkrVis::ImageGPU>* pyramid = sift.GetPyramid(); 

		for(int i = 0; i < pyramid->size(); i++)
		{
			TnkrVis::ImageGPU* py = &( pyramid->at(i) );
			image2.Copy(py);
			std::stringstream ss;
			ss << "image22_0_" << i << ".png";
			//TnkrVis::IO::ImageFile::Write(ss.str(), &image2 );
		}

		//Rotate
		std::cout << "Rotate\n"; 
		TnkrVis::Process::Rotate rotate;
		rotate.SetRotation(0); 
		imageGPU2.AllocateLike(&imageGPU1);
		rotate.Run(&imageGPU1, &imageGPU2);
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image23_1Test.png", &image2);

		//Translate
		std::cout << "Translate\n"; 
		TnkrVis::Process::Translate transl;
		transl.SetTranslation(100, 200); 
		imageGPU2.AllocateLike(&imageGPU1);
		transl.Run(&imageGPU1, &imageGPU2);
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image24_1Test.png", &image2);
		transl.SetTranslation(-100, -200); 
		transl.Run(&imageGPU1, &imageGPU2);
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image24_2Test.png", &image2);

		//CopyImage
		std::cout << "CopyImage\n"; 

		imageGPU2.Allocate(imageGPU1.GetWidth()*2, imageGPU1.GetHeight(), imageGPU1.GetType()); 
		imageGPU3.Allocate(imageGPU1.GetWidth()/2, imageGPU1.GetHeight(), imageGPU1.GetType()); 

		TnkrVis::Process::CopyImage cpIm;
		cpIm.UseOutputSize(true);
		cpIm.SetOffset(0, 0);
		cpIm.Run(&imageGPU1, &imageGPU2) ;
		cpIm.SetOffset(imageGPU1.GetWidth(), 0);
		cpIm.Run(&imageGPU1, &imageGPU2) ;
		image2.Copy(&imageGPU2);
		TnkrVis::IO::ImageFile::Write("image25_1Test.png", &image2);

		cpIm.SetOffset( -imageGPU1.GetWidth()/2, 0);
		cpIm.Run(&imageGPU1, &imageGPU3) ;
		image2.Copy(&imageGPU3);
		TnkrVis::IO::ImageFile::Write("image25_2Test.png", &image2);

		//Template matching
		TnkrVis::ImageGPU templateImage;
		TnkrVis::Process::GaussianFunction gaussFunc; 
		gaussFunc.SetGenerateMode(TnkrVis::Process::GaussianFunction::SIZE_TO_SIGMA); 
		gaussFunc.SetSigma(20); 
		templateImage.Allocate(0, 0, imageGPU1.GetType()); 
		gaussFunc.Run(&templateImage); 
		TnkrVis::IO::ImageFile::Write("image26_1Test.png", &templateImage);
		TnkrVis::Process::TemplateMatch tmplMatch;
		tmplMatch.SetNormalized(true);
		tmplMatch.SetMatchMode(TnkrVis::Process::TemplateMatch::MATCH_CORR);
		tmplMatch.Run(&imageGPU1, &templateImage, &imageGPU2);
		TnkrVis::IO::ImageFile::Write("image26_2Test.png", &imageGPU2);

		std::cout << "DONE\n";

	}
	else if(argc == 3)
	{
		if(std::string( argv[1] ) == "-v")
		{
			std::cout << "Starting video test\n"; 
			TnkrVis::IO::VideoHelper videoHelper; 
			videoHelper.Open(argv[2]); 
			TnkrVis::Window TnkrVisWindow(videoHelper.GetFrameWidth(), videoHelper.GetFrameHeight(), &context);
			TnkrVis::ImageGPU filterIm;

			while(!TnkrVisWindow.ShouldClose())
			{
				videoHelper.NextFrame([&](TnkrVis::ImageGPU* imageGPU, TnkrVis::Image* image)
				{
					TnkrVis::Process::MedianFilter medianFilter; 
					medianFilter.SetSize(3); 
					medianFilter.Run(imageGPU, &filterIm); 
					TnkrVisWindow.DrawImage(&filterIm); 
					TnkrVisWindow.Refresh();
				}); 
			}
			videoHelper.Close(); 
		}
		else if(std::string( argv[1] ) == "-aruco")
		{
			std::cout << "Starting aruco test\n"; 
			TnkrVis::Image input;
			TnkrVis::IO::ImageFile::Read(argv[2], &input);
			TnkrVis::CompositeProcess::ARUCODetector arucoDetector;
			TnkrVis::ImageGPU inputGPU; 
			inputGPU.Copy(&input);


			std::vector<bool> bitsequence0 = {0, 0, 0, 0, 0, 0, 0, 0,
											  0, 0, 0, 0, 0, 0, 0, 0,
											  0, 1, 0, 1, 1, 0, 0, 0, 
											  0, 0, 0, 1, 0, 0, 1, 0, 
											  0, 1, 1, 1, 0, 0, 1, 0, 
											  0, 1, 1, 1, 0, 1, 1, 0, 
											  0, 1, 0, 1, 0, 1, 1, 0, 
											  0, 0, 0, 0, 0, 0, 0, 0};
			int markerId0 = 123; 								 
			arucoDetector.AddDictionaryEntry(&bitsequence0, 8, markerId0); 

			

			std::vector<TnkrVis::Contour> markerQuads; 
			std::vector<int> markerIds;
			arucoDetector.Run(&inputGPU, &markerQuads, &markerIds, false);
			
			
			
			TnkrVis::Process::Renderer renderer;
			for(int i = 0; i < markerQuads.size(); i++)
			{
				TnkrVis::Color color; 
				if(markerIds[i] == markerId0)
					color = TnkrVis::Color(1, 0, 0); 
				
				renderer.AddContour(&(markerQuads[i]), color, true, 4, true, true); 
			}
			
			TnkrVis::Image image2;
			renderer.Run(&input, &image2); 
			TnkrVis::IO::ImageFile::Write("arcuo1Test.png", &image2); 
		}
		else if(std::string( argv[1] ) == "-computegraph")
		{
			std::cout << "Starting ComputeGraph test\n\n"; 

			TnkrVis::SerializedObject graphJson; 

			graphJson.FromString(R"(
			{
				"nodes":[
					{
						"type":"ImageSource", 
						"id":"ImageSource1", 
						"params":{}
					}, 
					{
						"type":"StringSource", 
						"id":"StringSource1", 
						"params":{"val":"aruco.png"}
					}, 
					{
						"type":"StringSource", 
						"id":"StringSource2", 
						"params":{"val":"aruco_NODETEST.png"}
					}, 
					{
						"type":"ImageFileRead", 
						"id":"ImageFileRead1", 
						"params":{},
						"connections": [
							{"id": "ImageSource1", "outinx": 0},
							{"id": "StringSource1", "outinx": 0}
						]
					}, 
					{
						"type":"ImageFileWrite", 
						"id":"ImageFileWrite1", 
						"params":{},
						"connections": [
							{"id": "ImageFileRead1", "outinx": 0}, 
							{"id": "StringSource2", "outinx": 0}
						]
					}
				]	
			}
			)"); 

			TnkrVis::ComputeGraph::Graph graph;
			graph.Deserialize(&graphJson); 
			TnkrVis::SerializedObject outJson; 
			graph.Serialize(&outJson); 
			std::cout << outJson.ToString() << "\n"; 

			graph.Run();
		}
		
	}
	else if(argc == 4)
	{
		if(std::string(argv[1]) == "-d")
		{
			//Try out data: http://vision.middlebury.edu/stereo/data/scenes2003/    
			//Dataset created by created by Daniel Scharstein, Alexander Vandenberg-Rodes, and Rick Szeliski.
			std::cout << "Starting depth from stereo test\n"; 
			TnkrVis::Image left;
			TnkrVis::Image right;
			TnkrVis::Image leftGrey;
			TnkrVis::Image rightGrey;
			TnkrVis::Image output; 
			TnkrVis::IO::ImageFile::Read(argv[2], &left);
			TnkrVis::IO::ImageFile::Read(argv[3], &right);
			TnkrVis::Process::GrayScale gs;
			leftGrey.Allocate(left.GetWidth(), left.GetHeight(), TnkrVis::ImageType::GRAYSCALE16); 
			rightGrey.Allocate(right.GetWidth(), right.GetHeight(), TnkrVis::ImageType::GRAYSCALE16); 
			gs.Run(&left, &leftGrey);
			gs.Run(&right, &rightGrey);

			TnkrVis::ImageGPU leftGPU;
			TnkrVis::ImageGPU rightGPU; 
			TnkrVis::ImageGPU outputGPU; 

			leftGPU.Copy(&leftGrey); 
			rightGPU.Copy(&rightGrey); 


			TnkrVis::Process::StereoMatchSAD smsad;
			smsad.Run(&leftGPU, &rightGPU, &outputGPU);	

			output.Copy(&outputGPU);
			TnkrVis::IO::ImageFile::Write("imageDepth1Test.png", &output);
		}
	}

	return 1; 
}