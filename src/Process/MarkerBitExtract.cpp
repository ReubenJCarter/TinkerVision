#include "MarkerBitExtract.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include "OtsuThreshold.h"

#include <string>
#include <iostream>
#include <map>

namespace TnkrVis
{
namespace Process
{

class MarkerBitExtract::Internal
{
    private:
        OtsuThreshold otsuThreshold; 
        
        float marginSize; 
        int gridW;
        int gridH;

    public:
        Internal(); 
        void Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages);

        void SetGridSize(int W, int H); 
        void SetMarginSize(float s); 
};

MarkerBitExtract::Internal::Internal()
{
    marginSize = 0.0;//fraction of the size of the cell  
    gridW = 8;
    gridH = 8; 
}

void MarkerBitExtract::Internal::Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages)
{
    ParallelFor& pf = ParallelFor::GetInstance(); 

    //Allocate bit image
	//std::cout << "bitImages->size() " << bitImages->size() << " new size:" << contours->size() << "\n";

    if(bitImages->size() != contours->size())    
    {
        bitImages->resize(contours->size()); 
    }

    //std::cout << "Bit image array resized\n" ; 

    //Allocate histogram for the otsu threshold
    std::vector<int> hist(256); 
    
    //for each contour
    for(int c = 0; c < contours->size(); c++)
    {
        std::vector<glm::vec2> verts(contours->at(c).verticies.size()); 
        for(int i = 0; i < contours->at(c).verticies.size(); i++)
        {
            verts[i] = glm::vec2(contours->at(c).verticies[i].x, contours->at(c).verticies[i].y);
        }

        //if not 4 verts continue
        if(verts.size() != 4)
        {
            bitImages->at(c).Allocate(0, 0, TnkrVis::ImageType::GRAYSCALE8);
            continue; 
        }
        else
        {
            bitImages->at(c).Allocate(gridW, gridH, TnkrVis::ImageType::GRAYSCALE8);
        }

        //sort into lowest and highest two x values 
        glm::vec2 minX[2];
        glm::vec2 maxX[2]; 

        auto CompairAndSwap = [&verts](int a, int b) //a is lower b is higher index
        { 
            if(verts[a].x > verts[b].x)
            {
                glm::vec2 temp = verts[a]; 
                verts[a] = verts[b];
                verts[b] = temp; 
            }
        }; 
        CompairAndSwap(0, 1);
        CompairAndSwap(2, 3);
        CompairAndSwap(0, 2);
        CompairAndSwap(1, 3);
        CompairAndSwap(1, 2);

        minX[0] = verts[0];
        minX[1] = verts[1];
        maxX[0] = verts[2];
        maxX[1] = verts[3];

        //sort the x min and max in order of y
        if(minX[0].y > minX[1].y)
        {
            glm::vec2 temp = minX[1];
            minX[1] = minX[0]; 
            minX[0] = temp;
        }
        if(maxX[0].y > maxX[1].y)
        {
            glm::vec2 temp = maxX[1];
            maxX[1] = maxX[0]; 
            maxX[0] = temp;
        }

        //compute the number of samples to take per cell 
        glm::ivec2 gridSize = glm::ivec2((std::max)(glm::length(maxX[0] - minX[0]), glm::length(maxX[1] - minX[1])),
                                         (std::max)(glm::length(minX[1] - minX[0]), glm::length(maxX[1] - maxX[0]))); 

        glm::vec2 gridCellSizePx = glm::vec2((std::max)(glm::length(maxX[0] - minX[0]), glm::length(maxX[1] - minX[1])) / gridW,
                                             (std::max)(glm::length(minX[1] - minX[0]), glm::length(maxX[1] - maxX[0])) / gridH); 
        
        Image* bitImage = &(bitImages->at(c)); 
        
       
        //
        //Build otsu Threshold
        ////Information from here: http://www.labbookpages.co.uk/software/imgProc/otsuThreshold.html
        double otsuThreshold;

        {
            //Zero Histogram
            for(int i = 0; i < 256; i++)
            {
                hist[i] = 0; 
            }
            //Scan whoel aruco region of image and extract pixels
            int totalPixels = 0; 
            for(int j = 0; j < gridSize.y; j++)
            {
                for(int i = 0; i < gridSize.x; i++)
                {
                    glm::vec2 uv = glm::vec2((float)i / gridSize.x, (float)j / gridSize.y); 
                    glm::vec2 a = minX[0] + (minX[1] - minX[0]) * uv.y;
                    glm::vec2 b = maxX[0] + (maxX[1] - maxX[0]) * uv.y;
                    glm::vec2 imCoord = a + (b - a ) * uv.x; 
                    glm::vec4 pix = GetPixelBilinear(input, imCoord.x, imCoord.y); 
                    int r = pix.x * 255; 
                    if(r > 255) r = 255;
                    if(r < 0) r = 0; 
                    hist[r]++; 
                    totalPixels++; 
                }
            }
            //Sum of the pixels
            double pixelSum = 0; //this will get very big !
            for(int i =0; i < 256; i++)
            {
                pixelSum += i * hist[i]; 
            }
            
            //Compute the actual threshold
            double sumB = 0;
            int wB = 0;
            int wF = 0;

            double varMax = 0;
            int threshold = 0;

            for (int t = 0; t < 256; t++) 
            {
                wB += hist[t];// Weight Background
                if (wB == 0) continue;

                wF = totalPixels - wB; // Weight Foreground
                if (wF == 0) break;

                sumB += (double) (t * hist[t]);

                double mB = sumB / wB;// Mean Background
                double mF = (pixelSum - sumB) / wF;// Mean Foreground

                // Calculate Between Class Variance
                double varBetween = (double)wB * (double)wF * (mB - mF) * (mB - mF);

                // Check if new maximum found
                if (varBetween > varMax) 
                {
                    varMax = varBetween;
                    threshold = t;
                }
            }

            otsuThreshold = (double)threshold / 255.0f;
        }
        
        //extract marker bits
        //run grid cell value kernel
        auto kernel = [this, input, contours, gridCellSizePx, minX, maxX, bitImage, otsuThreshold](int gx, int gy)
        {
            glm::vec2 gridCoordStart = glm::vec2((float)gx / (float)gridW, (float)gy / (float)gridH); 
            glm::vec2 gridCoordEnd = glm::vec2(((float)gx+1) / (float)gridW, ((float)gy+1) / (float)gridH); 

            glm::vec4 sum = glm::vec4(0, 0, 0, 0); 
            int count = 0; 

            for(float j = marginSize; j < 1.0f-marginSize; j += 1.0f / gridCellSizePx.y)
            {
               
                for(float i = marginSize; i < 1.0f-marginSize; i += 1.0f / gridCellSizePx.x)
                {
                    glm::vec2 uv = gridCoordStart + (gridCoordEnd - gridCoordStart) * glm::vec2(i, j); 

                    glm::vec2 a = minX[0] + (minX[1] - minX[0]) * uv.y;
                    glm::vec2 b = maxX[0] + (maxX[1] - maxX[0]) * uv.y;
                    glm::vec2 imCoord = a + (b - a ) * uv.x; 
                    sum += GetPixelBilinear(input, imCoord.x, imCoord.y); 
                    count++; 
                }
            }   
            sum /= count; 
            if(sum.r > otsuThreshold)
                SetPixelUI(bitImage, gx, gy, glm::ivec4(255, 255, 255, 255));  
            else
                SetPixelUI(bitImage, gx, gy, glm::vec4(0, 0, 0, 1));  
        };

        pf.Run(gridW, gridH, kernel);
        
    }
}

void MarkerBitExtract::Internal::SetGridSize(int W, int H)
{
    gridW = W; 
    gridH = H; 
}

void MarkerBitExtract::Internal::SetMarginSize(float s)
{
    marginSize = s; 
}




MarkerBitExtract::MarkerBitExtract()
{
    internal = new Internal(); 
}

MarkerBitExtract::~MarkerBitExtract()
{
    delete internal; 
}

void MarkerBitExtract::SetGridSize(int W, int H)
{
    internal->SetGridSize(W, H); 
}

void MarkerBitExtract::SetMarginSize(float s)
{
    internal->SetMarginSize(s); 
}

void MarkerBitExtract::Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages)
{
    internal->Run(input, contours, bitImages); 
}

}
}