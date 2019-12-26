#pragma once 

namespace Visi
{

class Color
{
    public:
        float r, g, b, a;
        Color(float fr, float fg, float fb, float fa=1){r=fr; g=fg; b=fb; a=fa;}; 
        Color(){};
};

class Vec2
{
    public: 
        float x, y; 
        Vec2(float fx, float fy){x = fx; y = fy;}
        Vec2(){};
};

class Vec3
{
    public: 
        float x, y, z; 
        Vec3(float fx, float fy, float fz){x = fx; y = fy; z = fz;}
        Vec3(){};
};

class Vec4
{
    public: 
        float x, y, z, w; 
        Vec4(float fx, float fy, float fz, float fw){x = fx; y = fy; z = fz; w = fw;}
        Vec4(){};
};

}