#include "Renderer.h"

#include "../Core/ComputeShader.h"
#include "../Core/ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <random>

namespace Viso
{
namespace Process
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

        struct PolyLine
        {
            glm::vec4 color; 
            std::vector<glm::vec2> verts;
            float lineWidth; 
            int closed; 
        }; 

        struct Particle
        {
            glm::vec4 color; 
            glm::vec2 centre;
            float radius;
            int renderImagesInx; 
        }; 

        std::vector<Image> renderImages; 
        std::vector<Circle> circles; 
        std::vector<PolyLine> polyLines; 
        std::vector<Particle> particles; 

        inline void DrawLine(unsigned char* data, ImageType imageType, int width, int height, int startX, int startY, int endX, int endY, glm::vec4 color)
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
        }

        inline void DrawPolyLine(unsigned char* data, ImageType imageType, int width, int height, PolyLine& pl)
        {
            if(pl.verts.size() <= 1)
                return ;

            for(int l = 1; l < pl.verts.size(); l++)
            {
                glm::vec2 lStart = pl.verts[l-1]; 
                glm::vec2 lEnd = pl.verts[l]; 
                DrawLine(data, imageType, width, height, lStart.x, lStart.y, lEnd.x, lEnd.y, pl.color); 
            }

            if(pl.closed != 0)
            {
                glm::vec2 lStart = pl.verts[0];
                glm::vec2 lEnd = pl.verts[pl.verts.size()-1];
                DrawLine(data, imageType, width, height, lStart.x, lStart.y, lEnd.x, lEnd.y, pl.color); 
            }
        }

        inline void DrawCircle(unsigned char* data, ImageType imageType, int width, int height, Circle& c)
        {
            glm::vec2 centre = c.centre;
            float radius = c.radius; 
            int x = radius;
            int y = 0; 
            float r2 = radius * radius;

            if(radius < 1)
            {
                //SetPixel(output, centre.x, centre.y, c.color);
                SetPixel(data, imageType, width, height, centre.x, centre.y, c.color); 
                return;
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
        }

        inline void DrawImage(unsigned char* data, ImageType imageType, int width, int height, Image& im, int x, int y)
        {
            int wh = im.GetWidth(); 
            int hh = im.GetHeight(); 
            for(int i = 0; i < im.GetWidth(); i++)
            {
                for(int j = 0; j < im.GetHeight(); j++)
                {
                    glm::vec4 c = GetPixel(&im, i, j); 
                    SetPixel(data, imageType, width, height, i + x - wh, j + y - hh, c); 
                }
            }
        }

    public:
        Internal(); 
        void Run(Image* input, Image* output);
        void Run(Image* dst);
        void Clear(); 
        void AddCircle(glm::vec2 centre, float radius, glm::vec4 color=glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), bool filled=false, float borderWidth=1); 
        void AddPolyLine(std::vector<Vec2>* pl, glm::vec4 color, float lineWidth, bool closed);
        int AddRenderImage(Image* image);
        void AddParticle(glm::vec2 centre, glm::vec4 color, float radius); 
};

Renderer::Internal::Internal()
{
}

void Renderer::Internal::Run(Image* input, Image* output)
{
    
    ReallocateIfNotSameSize(output, input); 
    
    //copy input image to output
    output->Copy(input); 

    unsigned char* data = output->GetData(); 
    ImageType imageType = output->GetType();
    int width = output->GetWidth();
    int height = output->GetHeight();
    
    for(int inx = 0; inx < polyLines.size(); inx++)
    {
        PolyLine& pl = polyLines[inx]; 
        DrawPolyLine(data, imageType, width, height, pl); 
    }

    //Draw circles
    for(int inx = 0; inx < circles.size(); inx++)
    {
        Circle c = circles[inx];
        DrawCircle(data, imageType, width, height, c); 
    }
}

void Renderer::Internal::Run(Image* dst)
{
    unsigned char* data = dst->GetData(); 
    ImageType imageType = dst->GetType();
    int width = dst->GetWidth();
    int height = dst->GetHeight();
    
    for(int inx = 0; inx < polyLines.size(); inx++)
    {
        PolyLine& pl = polyLines[inx]; 
        DrawPolyLine(data, imageType, width, height, pl); 
    }

    //Draw circles
    for(int inx = 0; inx < circles.size(); inx++)
    {
        Circle c = circles[inx];
        DrawCircle(data, imageType, width, height, c); 
    }
}

void Renderer::Internal::Clear()
{
    circles.clear(); 
    circles.resize(0); 

    polyLines.clear(); 
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

int Renderer::Internal::AddRenderImage(Image* image)
{
    return 0; 
}

void Renderer::Internal::AddParticle(glm::vec2 centre, glm::vec4 color, float radius)
{
    
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

void Renderer::Run(Image* dst)
{
    internal->Run(dst); 
}

void Renderer::AddPolyLine(std::vector<Vec2>* pl, Color color, float lineWidth, bool closed)
{
    internal->AddPolyLine(pl, glm::vec4(color.r, color.g, color.b, color.a), lineWidth, closed); 
}

void Renderer::AddCircle(Vec2 centre, float radius, Color color, bool filled, float borderWidth)
{
    internal->AddCircle(glm::vec2(centre.x, centre.y), radius, glm::vec4(color.r, color.g, color.b, color.a), filled, borderWidth); 
}

void Renderer::AddContour(Contour* contour, Color color, bool renderVerts, float vertRad, bool renderLines, bool closed)
{
    if(renderVerts)
    {
        for(int j = 0; j < (*contour).verticies.size(); j++)
        {
            AddCircle( (*contour).verticies[j], vertRad, color, false, 1);
        }
    } 
    if(renderLines)
        AddPolyLine(&((*contour).verticies), color, 1, closed); 
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

void Renderer::Add3DAxis(float fovy, Vec3 pos)
{//TODO
    Vec3 orig = pos;
    Vec2 origTransformed; 

    std::vector<Vec2> xaxis;
    std::vector<Vec2> yaxis;
    std::vector<Vec2> zaxis;
    xaxis.push_back(origTransformed); 
    yaxis.push_back(origTransformed); 
    zaxis.push_back(origTransformed); 

    //AddPolyLine(); 
}

}
}