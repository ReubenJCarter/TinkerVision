#pragma once 

#include <math.h>

namespace TnkrVis
{

class Color
{
    public:
        float r, g, b, a;
		
        Color(float fr, float fg, float fb, float fa=1){r=fr; g=fg; b=fb; a=fa;}
        Color(){};
};

class Vec2
{
    public: 
        float x, y; 
		
        Vec2(float fx, float fy){x = fx; y = fy;}
        Vec2(){}
        Vec2(float v){x = v; y = v;}

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
        Vec3(){}
        Vec3(float v){x = v; y = v; z = v;}

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
        Vec4(){}
        Vec4(float v){x = v; y = v; z = v; w = v;}

        float Length() { return sqrt(x*x + y*y + z*z + w*w); }
        float Length2(){ return (x*x + y*y + z*z + w*w); }
        Vec4 Normalize(){ float len = sqrt(x*x + y*y + z*z + w*w); return Vec4(x/len, y/len, z/len, w/len); }
        static float Dot(Vec4& a, Vec4& b){ return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w; }

        friend Vec4 operator+(Vec4& a, Vec4& b){ return Vec4(a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w); }
        friend Vec4 operator-(Vec4& a, Vec4& b){ return Vec4(a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w); }
        friend Vec4 operator*(Vec4& a, Vec4& b){ return Vec4(a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w); }
        friend Vec4 operator/(Vec4& a, Vec4& b){ return Vec4(a.x/b.x, a.y/b.y, a.z/b.z, a.w/b.w); }
};

class Mat2 
{
    public: 
        Vec2 col[2]; 

        Mat2(Vec2 col0, Vec2 col1){col[0] = col0; col[1] = col1;}
        Mat2(){}
        Mat2(float v){col[0] = Vec2(v, 0); col[1] = Vec2(0, v);}

        friend Vec2 operator*(Mat2& a, Vec2& b)
        {
            Vec2 ret;
            ret.x = a.col[0].x*b.x + a.col[1].x*b.y; 
            ret.y = a.col[0].y*b.x + a.col[1].y*b.y; 
            return ret; 
        }
}; 

class Mat3
{
	public:
		Vec3 col[3]; 
		
		Mat3(Vec3 col0, Vec3 col1, Vec3 col2){col[0] = col0; col[1] = col1; col[2] = col2;}
		Mat3(){}
        Mat3(float v){col[0] = Vec3(v, 0, 0); col[1] = Vec3(0, v, 0); col[2] = Vec3(0, 0, v);}

        friend Mat3 operator*(Mat3& a, Mat3& b)
        {
            Mat3 ret; 
            return ret; 
        }

        friend Vec3 operator*(Mat3& a, Vec3& b)
        {
            Vec3 ret;
            ret.x = a.col[0].x*b.x + a.col[1].x*b.y + a.col[2].x*b.z; 
            ret.y = a.col[0].y*b.x + a.col[1].y*b.y + a.col[2].y*b.z; 
            ret.z = a.col[0].z*b.x + a.col[1].z*b.y + a.col[2].z*b.z; 
            return ret; 
        }
};

class CameraPose 
{
    public:
        Mat3 rotation;
        Vec3 translation;
};


}