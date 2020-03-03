#include "ARUCOBitExtract.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include "../Core/ParallelFor.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{
namespace Process
{

class ARUCOBitExtract::Internal
{
    private:
        float marginSize; 
        int gridW;
        int gridH;

    public:
        Internal(); 
        void Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages);

        void SetGridSize(int W, int H); 
        void SetMarginSize(float s); 
};

ARUCOBitExtract::Internal::Internal()
{
    marginSize = 0;
    gridW = 8;
    gridH = 8; 
}

void ARUCOBitExtract::Internal::Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages)
{
    ParallelFor& pf = ParallelFor::GetInstance(); 

    //Allocate bit image
    if(bitImages->size() < contours->size())    
    {
        bitImages->resize(contours->size()); 
    }
    for(int i = 0 ; i < bitImages->size(); i++)
    {
        bitImages->at(i).Allocate(gridW, gridH, Visi::ImageType::GRAYSCALE8); 
    }
    
    //for each contour
    for(int c = 0; c < contours->size(); c++)
    {
        std::vector<glm::vec2> verts(contours->at(c).verticies.size()); 
        for(int i = 0; i < contours->at(c).verticies.size(); i++)
        {
            verts.push_back(glm::vec2(contours->at(c).verticies[i].x, contours->at(c).verticies[i].y)); 
        }

        //if not 4 verts continue
        if(verts.size() != 4)
        {
            continue; 
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
        glm::vec2 gridCellSizePx = glm::vec2( (std::max)(glm::length(maxX[0] - minX[0]), glm::length(maxX[1] - minX[1])) / gridW,
                                              (std::max)(glm::length(minX[1] - minX[0]), glm::length(maxX[1] - maxX[0])) / gridH
                                           ); 
        
        //run grid cell value kernel
        auto kernel = [this, input, contours, bitImages, gridCellSizePx, minX, maxX, c](int gx, int gy)
        {
            glm::vec2 gridCoordStart = glm::vec2((float)gx / (float)gridW, (float)gy / (float)gridH); 
            glm::vec2 gridCoordEnd = glm::vec2(((float)gx+1) / (float)gridW, ((float)gy+1) / (float)gridH); 

            glm::vec4 sum = glm::vec4(0, 0, 0, 0); 
            int count = 0; 

            for(float j = 0; j < 1.0f; j += 1.0f / gridCellSizePx.y)
            {
               
                for(float i = 0; i < 1.0f; i += 1.0f / gridCellSizePx.x)
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
            if(sum.r > 0.5f)
                SetPixel(&(bitImages->at(c)), gx, gy, glm::vec4(1, 1, 1, 1));  
            else
                SetPixel(&(bitImages->at(c)), gx, gy, glm::vec4(0, 0, 0, 0));  
        };

        pf.Run(gridW, gridH, kernel);
    }
}

void ARUCOBitExtract::Internal::SetGridSize(int W, int H)
{
    gridW = W; 
    gridH = H; 
}

void ARUCOBitExtract::Internal::SetMarginSize(float s)
{
    marginSize = s; 
}




ARUCOBitExtract::ARUCOBitExtract()
{
    internal = new Internal(); 
}

ARUCOBitExtract::~ARUCOBitExtract()
{
    delete internal; 
}

void ARUCOBitExtract::SetGridSize(int W, int H)
{
    internal->SetGridSize(W, H); 
}

void ARUCOBitExtract::SetMarginSize(float s)
{
    internal->SetMarginSize(s); 
}

void ARUCOBitExtract::Run(Image* input, std::vector<Contour>* contours,  std::vector<Image>* bitImages)
{
    internal->Run(input, contours, bitImages); 
}

}
}