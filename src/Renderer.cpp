#include "Renderer.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <random>

namespace Visi
{

class Renderer::Internal
{
    private:
        struct Circle
        {
            glm::vec4 color;
            glm::vec2 centre;
            float radius;
            float borderWidth; 
            int filled; 
            glm::vec3 pad;
        }; 

        struct Line
        {
            glm::vec4 color; 
            glm::vec2 start;
            glm::vec2 end; 
            float lineWidth; 
        }; 

        struct PolyLine
        {
            glm::vec4 color; 
            std::vector<glm::vec2> verts;
            float lineWidth; 
            int closed; 
        }; 


        bool circlesDirty; 
        std::vector<Circle> circles; 
        bool polyLinesDirty;
        std::vector<PolyLine> polyLines; 

    public:
        Internal(); 
        void Run(Image* input, Image* output);
        void Clear(); 
        void AddCircle(glm::vec2 centre, float radius, glm::vec4 color=glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), bool filled=false, float borderWidth=1); 
        void AddPolyLine(std::vector<Vec2>* pl, glm::vec4 color, float lineWidth, bool closed);
};

Renderer::Internal::Internal()
{
    circlesDirty = true; 
    polyLinesDirty = true; 
}

void Renderer::Internal::Run(Image* input, Image* output)
{
    if(!output->IsSameDimensions(input)) 
    {
        output->Allocate(input->GetWidth(), input->GetHeight(), input->GetType()); 
    }
    
    //copy input image to output
    output->Copy(input); 

    unsigned char* data = output->GetData(); 
    ImageType imageType = output->GetType();
    int width = output->GetWidth();
    int height = output->GetHeight();
    
    //Draw Lines
    auto DrawLine = [data, imageType, width, height](int startX, int startY, int endX, int endY, glm::vec4 color)
    {
            int x2, y2, x1, y1; 
            if(startX <= endX)
            {
                x1 = startX;
                x2 = endX;

                y1 = startY;
                y2 = endY;
            }
            else
            {
                x2 = startX;
                x1 = endX;
                
                y2 = startY;
                y1 = endY;
            }

            int dx = x2 - x1;
            int dy = y2 - y1;

            if(dx == 0 && dy == 0)
                return; 

            float m; 
            if(dx >= abs(dy))
            {
                m = (float)dy / (float)dx; 
            }
            else
            {
                m = abs((float)dx / (float)dy); 
            }

            if(dx >= abs(dy))
            {
                float y = y1; 
                for (int x = x1; x <= x2; x++) 
                {    
                    y += m;
                    SetPixel(data, imageType, width, height, x, y, color); 
                }
            }
            else 
            {
                float x = x1; 
                if(y2 > y1)
                {
                    for (int y = y1; y <= y2; y++) 
                    {    
                        x += m;
                        SetPixel(data, imageType, width, height, x, y, color); 
                    }
                }
                else
                {
                    for (int y = y1; y >= y2; y--) 
                    {    
                        x += m;
                        SetPixel(data, imageType, width, height, x, y, color); 
                    }
                }
            }
    };
    for(int inx = 0; inx < polyLines.size(); inx++)
    {
        PolyLine& pl = polyLines[inx]; 
        if(pl.verts.size() <= 1)
            continue;

        for(int l = 1; l < pl.verts.size(); l++)
        {
            glm::vec2 lStart = pl.verts[l-1]; 
            glm::vec2 lEnd = pl.verts[l]; 
            DrawLine(lStart.x, lStart.y, lEnd.x, lEnd.y, pl.color); 
        }

        if(pl.closed != 0)
        {
            glm::vec2 lStart = pl.verts[0];
            glm::vec2 lEnd = pl.verts[pl.verts.size()-1];
            DrawLine(lStart.x, lStart.y, lEnd.x, lEnd.y, pl.color); 
        }
    }

    //Draw circles
    for(int inx = 0; inx < circles.size(); inx++)
    {
        Circle c = circles[inx];
        glm::vec2 centre = c.centre;
        float radius = c.radius; 
        int x = radius;
        int y = 0; 
        float r2 = radius * radius;

        if(radius < 1)
        {
            //SetPixel(output, centre.x, centre.y, c.color);
            SetPixel(data, imageType, width, height, centre.x, centre.y, c.color); 
            continue;
        }

        //Simple brute algo 
        for(int i = -radius; i < radius; i++)
        {
            for(int j = -radius; j < radius; j++)
            {
                glm::vec2 a = glm::vec2(i, j);
                float aLen = glm::length( a); 
             
                if(c.filled == 0)
                {
                    if((c.radius - aLen) <= c.borderWidth && aLen < c.radius)
                    {
                        int x = j + centre.x; 
                        int y = i + centre.y;
                        SetPixel(data, imageType, width, height, x, y, c.color); 
                    }
                }
                else 
                {
                    if(aLen <= c.radius)
                    {
                        int x = j + centre.x; 
                        int y = i + centre.y;
                        SetPixel(data, imageType, width, height, x, y, c.color); 
                    }
                }
            }
        }


        /*
        //B  algo
        while(x > y)
        {
            //Draw Pixel
            glm::vec4 p = GetPixel(output, x, y);
            SetPixel(output, centre.x + x, centre.y + y, c.color);

            //SetPixel(output, centre.x - x, centre.y + y, c.color);
            
            //SetPixel(output, centre.x + x, centre.y - y, c.color);

            //Advance
            int x2 = x * x;
            x = sqrt(x2 - 2 * y - 1);
            y++;
        }*/
    }
}

void Renderer::Internal::Clear()
{
    circlesDirty = true; 
    circles.clear(); 
    circles.resize(0); 

    polyLines.clear(); 
    polyLinesDirty= true;
    polyLines.resize(0);
}

void Renderer::Internal::AddCircle(glm::vec2 centre, float radius, glm::vec4 color, bool filled, float borderWidth)
{
    Circle c;
    c.borderWidth = borderWidth;
    c.centre = centre;
    c.color = color; 
    c.filled = filled ? 1:0;
    c.radius = radius; 
    circles.push_back(c); 
    circlesDirty = true;
}

void Renderer::Internal::AddPolyLine(std::vector<Vec2>* pl, glm::vec4 color, float lineWidth, bool closed)
{
    polyLines.push_back(PolyLine());  
    PolyLine& polyLine = polyLines[polyLines.size()-1];
    polyLine.color = color;
    polyLine.lineWidth = lineWidth; 
    polyLine.closed = closed ? 1:0; 
    polyLine.verts.resize(pl->size()); 
    for(int i = 0; i < pl->size(); i++)
        polyLine.verts[i] = glm::vec2(pl->at(i).x, pl->at(i).y);
    
}



Renderer::Renderer()
{
    internal = new Internal(); 
}

Renderer::~Renderer()
{
    delete internal; 
}

void Renderer::Clear()
{
    internal->Clear(); 
}

void Renderer::Run(Image* input, Image* output)
{
    internal->Run(input, output); 
}

void Renderer::AddPolyLine(std::vector<Vec2>* pl, Color color, float lineWidth, bool closed)
{
    internal->AddPolyLine(pl, glm::vec4(color.r, color.g, color.b, color.a), lineWidth, closed); 
}

void Renderer::AddCircle(Vec2 centre, float radius, Color color, bool filled, float borderWidth)
{
    internal->AddCircle(glm::vec2(centre.x, centre.y), radius, glm::vec4(color.r, color.g, color.b, color.a), filled, borderWidth); 
}

void Renderer::AddContours(std::vector<Contour>* contours, bool renderVerts, float vertRad, bool renderLines, bool closed)
{
    std::default_random_engine randgenerator;
    randgenerator.seed(0);
    std::uniform_real_distribution<float> randdistribution(0.0f, 1.0f);
    for(int i = 0; i < contours->size(); i++)
	{
		Color color; 
		color.r = randdistribution(randgenerator); 
		color.g = randdistribution(randgenerator);
		color.b = randdistribution(randgenerator);
        color.a = 1; 
        if(renderVerts)
        {
            for(int j = 0; j < (*contours)[i].verticies.size(); j++)
            {
                AddCircle((*contours)[i].verticies[j], vertRad, color, false, 1);
            }
        } 
        if(renderLines)
		    AddPolyLine(&((*contours)[i].verticies), color, 1, closed); 
	}
}

void Renderer::AddBoundingBox(BoundingBox* boundingBox, Color color)
{
    AddCircle(boundingBox->position, 4, color, false, 1);

    std::vector<Vec2> pl;

    Mat3 tr = boundingBox->GetTransform(); 
    /*
    std::cout << "bbTrans:\n" << tr.col[0].x << " " << tr.col[1].x << " " << tr.col[2].x << "\n" 
                              << tr.col[0].y << " " << tr.col[1].y << " " << tr.col[2].y << "\n" 
                              << tr.col[0].z << " " << tr.col[1].z << " " << tr.col[2].z << "\n" ;
                              */
    glm::mat3 trG = glm::mat3(glm::vec3(tr.col[0].x, tr.col[0].y, tr.col[0].z), 
                              glm::vec3(tr.col[1].x, tr.col[1].y, tr.col[1].z),
                              glm::vec3(tr.col[2].x, tr.col[2].y, tr.col[2].z)); 

    glm::vec3 a = trG * glm::vec3(-boundingBox->extends.x, -boundingBox->extends.y, 1); 
    glm::vec3 b = trG * glm::vec3(boundingBox->extends.x, -boundingBox->extends.y, 1); 
    glm::vec3 c = trG * glm::vec3(boundingBox->extends.x, boundingBox->extends.y, 1); 
    glm::vec3 d = trG * glm::vec3(-boundingBox->extends.x, boundingBox->extends.y, 1); 

    pl.push_back(Vec2(a.x, a.y)); 
    pl.push_back(Vec2(b.x, b.y)); 
    pl.push_back(Vec2(c.x, c.y)); 
    pl.push_back(Vec2(d.x, d.y)); 

    AddCircle(Vec2(a.x, a.y), 2, color, false, 1);
    AddCircle(Vec2(b.x, b.y), 2, color, false, 1);
    AddCircle(Vec2(c.x, c.y), 2, color, false, 1);
    AddCircle(Vec2(d.x, d.y), 2, color, false, 1);
    AddPolyLine(&pl, color, 1, true); 
}

}