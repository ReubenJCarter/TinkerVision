#pragma once

#include "Visi_export.h"

#include "../Core/VectorMath.h"

#include <string>
#include <vector>

namespace Visi
{
namespace ComputeGraph
{
		
/**
 */
	
class VISI_EXPORT SerializedObject
{
    private: 
        class Internal;
        Internal* internal; 

    public:
        SerializedObject();
        SerializedObject(std::string str);
        ~SerializedObject();

        void Destroy(); 

        std::string ToString();
        void FromString(std::string str); 
         
        void SetFloat(std::string name, float v); 
        void SetInt(std::string name, int v);
        void SetBool(std::string name, bool v);
        void SetString(std::string name, std::string v); 
        void SetVec2(std::string name, Vec2 v);
        void SetVec3(std::string name, Vec3 v);
        void SetVec4(std::string name, Vec4 v);
        void SetColor(std::string name, Color v);
        void SetMat2(std::string name, Mat2 v);
        void SetMat3(std::string name, Mat3 v);
        void SetSerializedObject(std::string name, SerializedObject* v);
        
        void SetFloatArray(std::string name, std::vector<float>& vec); 
        void SetIntArray(std::string name, std::vector<int>& vec); 
        void SetBoolArray(std::string name, std::vector<bool>& vec);
        void SetStringArray(std::string name, std::vector<std::string>& vec);  
        void SetVec2Array(std::string name, std::vector<Vec2>& vec); 
        void SetVec3Array(std::string name, std::vector<Vec3>& vec); 
        void SetVec4Array(std::string name, std::vector<Vec4>& vec); 
        void SetMat2Array(std::string name, std::vector<Mat2>& vec); 
        void SetMat3Array(std::string name, std::vector<Mat3>& vec); 
        void SetSerializedObjectArray(std::string name, std::vector<SerializedObject*>& vec); 

        float GetFloat(std::string name, float v=0.0f); 
        int GetInt(std::string name, int v=0);
        bool GetBool(std::string name, bool v=false);
        std::string GetString(std::string name, std::string v=""); 
        Vec2 GetVec2(std::string name, Vec2 v=Vec2(0, 0));
        Vec3 GetVec3(std::string name, Vec3 v=Vec3(0, 0, 0));
        Vec4 GetVec4(std::string name, Vec4 v=Vec4(0, 0, 0, 0));
        Color GetColor(std::string name, Color v=Color(0, 0, 0, 1));
        Mat2 GetMat2(std::string name, Mat2 v=Mat2(1.0f));
        Mat3 GetMat3(std::string name, Mat3 v=Mat3(1.0f));
        SerializedObject* GetSerializedObject(std::string name, SerializedObject* v=NULL);

        bool GetFloatArray(std::string name, std::vector<float>& vec); 
        bool GetIntArray(std::string name, std::vector<int>& vec); 
        bool GetBoolArray(std::string name, std::vector<bool>& vec);
        bool GetStringArray(std::string name, std::vector<std::string>& vec);  
        bool GetVec2Array(std::string name, std::vector<Vec2>& vec); 
        bool GetVec3Array(std::string name, std::vector<Vec3>& vec); 
        bool GetVec4Array(std::string name, std::vector<Vec4>& vec); 
        bool GetMat2Array(std::string name, std::vector<Mat2>& vec); 
        bool GetMat3Array(std::string name, std::vector<Mat3>& vec); 
        bool GetSerializedObjectArray(std::string name, std::vector<SerializedObject*>& vec); 
};
	
}
}