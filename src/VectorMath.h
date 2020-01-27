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

class Mat3
{
	public:
		Vec3 col[3]; 
		
		Mat3(Vec3 col0, Vec3 col1, Vec3 col2){col[0] = col0; col[1] = col1; col[2] = col2;}
		Mat3(){}; 
};

class CameraPose 
{
        Mat3 rotation;
        Vec3 translation;
};

}