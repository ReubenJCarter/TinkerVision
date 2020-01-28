#pragma once 

#include <math.h>

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

        float Length() {return sqrt(x*x + y*y);}
        float Length2(){return (x*x + y*y);}
        Vec2 Normalize(){ float len = sqrt(x*x + y*y); return Vec2(x/len, y/len); }
        static float Dot(Vec2& a, Vec2& b){ return a.x*b.x + a.y*b.y; }

        friend Vec2 operator+(Vec2& a, Vec2& b){ return Vec2(a.x+b.x, a.y+b.y); }
        friend Vec2 operator-(Vec2& a, Vec2& b){ return Vec2(a.x-b.x, a.y-b.y); }
        friend Vec2 operator*(Vec2& a, Vec2& b){ return Vec2(a.x*b.x, a.y*b.y); }
        friend Vec2 operator/(Vec2& a, Vec2& b){ return Vec2(a.x/b.x, a.y/b.y); }
};

class Vec3
{
    public: 
        float x, y, z; 
		
        Vec3(float fx, float fy, float fz){x = fx; y = fy; z = fz;}
        Vec3(){};

        float Length() { return sqrt(x*x + y*y + z*z); }
        float Length2(){ return (x*x + y*y + z*z); }
        Vec3 Normalize(){ float len = sqrt(x*x + y*y + z*z); return Vec3(x/len, y/len, z/len); }
        static float Dot(Vec3& a, Vec3& b){ return a.x*b.x + a.y*b.y + a.z*b.z; }

        friend Vec3 operator+(Vec3& a, Vec3& b){ return Vec3(a.x+b.x, a.y+b.y, a.z+b.z); }
        friend Vec3 operator-(Vec3& a, Vec3& b){ return Vec3(a.x-b.x, a.y-b.y, a.z-b.z); }
        friend Vec3 operator*(Vec3& a, Vec3& b){ return Vec3(a.x*b.x, a.y*b.y, a.z*b.z); }
        friend Vec3 operator/(Vec3& a, Vec3& b){ return Vec3(a.x/b.x, a.y/b.y, a.z/b.z); }
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
    public:
        Mat3 rotation;
        Vec3 translation;
};

}