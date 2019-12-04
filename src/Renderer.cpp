#include "Renderer.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>

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
        void AddPolyLine(std::vector<glm::vec2>* pl, glm::vec4 color, float lineWidth, bool closed);
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
    for(int i = 0; i < input->GetHeight(); i++)
    {
        for(int j = 0; j < input->GetWidth(); j++)
        {
            glm::vec4 p = GetPixel(input, j, i);
            SetPixel(output, j, i, p);
        } 
    } 

    //Draw Lines
    auto DrawLine = [output](int startX, int startY, int endX, int endY, glm::vec4 color)
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
                    SetPixel(output, x, y, color); 
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
                        SetPixel(output, x, y, color); 
                    }
                }
                else
                {
                    for (int y = y1; y >= y2; y--) 
                    {    
                        x += m;
                        SetPixel(output, x, y, color); 
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
            SetPixel(output, centre.x, centre.y, c.color);
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
                        SetPixel(output, x, y, c.color);
                    }
                }
                else 
                {
                    if(aLen <= c.radius)
                    {
                        int x = j + centre.x; 
                        int y = i + centre.y;
                        SetPixel(output, x, y, c.color);
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

void Renderer::Internal::AddPolyLine(std::vector<glm::vec2>* pl, glm::vec4 color, float lineWidth, bool closed)
{
    polyLines.push_back(PolyLine());  
    PolyLine& polyLine = polyLines[polyLines.size()-1];
    polyLine.color = color;
    polyLine.lineWidth = lineWidth; 
    polyLine.closed = closed ? 1:0; 
    polyLine.verts.resize(pl->size()); 
    for(int i = 0; i < pl->size(); i++)
        polyLine.verts[i] = (pl->at(i));
    
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

void Renderer::AddPolyLine(std::vector<glm::vec2>* pl, glm::vec4 color, float lineWidth, bool closed)
{
    internal->AddPolyLine(pl, color, lineWidth, closed); 
}

void Renderer::AddCircle(glm::vec2 centre, float radius, glm::vec4 color, bool filled, float borderWidth)
{
    internal->AddCircle(centre, radius, color, filled, borderWidth); 
}

}