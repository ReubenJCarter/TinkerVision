#include "SerializedObject.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <map>

using namespace rapidjson;

namespace Visi
{

class SerializedObject::Internal
{
    public:

        void ParseJson(const GenericValue<rapidjson::UTF8<>>* parent)
        {
            floats.clear();
            ints.clear();
            bools.clear(); 
            strings.clear(); 
            objs.clear(); 
            floatArrays.clear(); 
            intArrays.clear(); 
            boolArrays.clear(); 
            stringArrays.clear(); 
            objArrays.clear(); 

            if(!parent->IsObject())
            {
                return;
            }

            for(Value::ConstMemberIterator itr = parent->MemberBegin();
                itr != parent->MemberEnd(); ++itr)
            {
                const GenericValue<rapidjson::UTF8<>>* v = &(itr->value);
                std::string name = itr->name.GetString();

                //Floats
                if(v->IsFloat())
                {
                    floats[name] = v->GetFloat();
                }

                //Ints
                if(v->IsInt())
                {
                    ints[name] = v->GetInt();
                }

                //Bools
                if(v->IsBool())
                {
                    bools[name] = v->GetBool();
                }

                //Strings
                if(v->IsString())
                {
                    strings[name] = v->GetString(); 
                }

                //obj
                if(v->IsObject())
                {
                    SerializedObject* so = new SerializedObject(); 
                    so->internal->ParseJson(v); 
                    objs[name] = so; 
                }

                //Arrays
                if(v->IsArray())
                {
                    int len = v->GetArray().Size(); 
                    if(len > 0)
                    {
                        //Floats
                        if(v->Begin()->IsFloat())
                        {
                            std::vector<float> vec; 
                            for (Value::ConstValueIterator itr = v->Begin(); itr != v->End(); ++itr)
                            {
                                vec.push_back(itr->GetFloat()); 
                            }
                            floatArrays[name] = vec; 
                        }

                        //Ints
                        if(v->Begin()->IsInt())
                        {
                            std::vector<int> vec; 
                            for (Value::ConstValueIterator itr = v->Begin(); itr != v->End(); ++itr)
                            {
                                vec.push_back(itr->GetInt()); 
                            }
                            intArrays[name] = vec; 
                        }

                        //Bools
                        if(v->Begin()->IsBool())
                        {
                            std::vector<bool> vec; 
                            for (Value::ConstValueIterator itr = v->Begin(); itr != v->End(); ++itr)
                            {
                                vec.push_back(itr->GetBool()); 
                            }
                            boolArrays[name] = vec; 
                        }

                        //strings
                        if(v->Begin()->IsString())
                        {
                            std::vector<std::string> vec; 
                            for (Value::ConstValueIterator itr = v->Begin(); itr != v->End(); ++itr)
                            {
                                vec.push_back(itr->GetString()); 
                            }
                            stringArrays[name] = vec; 
                        }

                        //objs
                        if(v->Begin()->IsObject())
                        {
                            std::vector<SerializedObject*> vec; 
                            for (Value::ConstValueIterator itr = v->Begin(); itr != v->End(); ++itr)
                            {
                                SerializedObject* so = new SerializedObject(); 
                                so->internal->ParseJson(&(*itr)); 
                                vec.push_back(so); 
                            }
                            objArrays[name] = vec; 
                        }
                    }
                }
            }
        }

        void BuildJson(Value* parent, Document::AllocatorType& allocator)
        {

            //Floats
            for (auto it = floats.begin(); it != floats.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                parent->AddMember(key, it->second, allocator);
            }
            for (auto it = floatArrays.begin(); it != floatArrays.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value a(kArrayType);
                for(int i = 0; i < it->second.size(); i++)
                {
                    float val = it->second[i];
                    a.PushBack(val, allocator); 
                }
                parent->AddMember(key, a, allocator);
            }

            //Ints
            for (auto it = ints.begin(); it != ints.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                parent->AddMember(key, it->second, allocator);
            }
            for (auto it = intArrays.begin(); it != intArrays.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value a(kArrayType);
                for(int i = 0; i < it->second.size(); i++)
                {
                    int val = it->second[i];
                    a.PushBack(val, allocator); 
                }
                parent->AddMember(key, a, allocator);
            }

            //bools
            for (auto it = bools.begin(); it != bools.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                parent->AddMember(key, it->second, allocator);
            }
            for (auto it = boolArrays.begin(); it != boolArrays.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value a(kArrayType);
                for(int i = 0; i < it->second.size(); i++)
                {
                    bool val = it->second[i];
                    a.PushBack(val, allocator); 
                }
                parent->AddMember(key, a, allocator);
            }

            //strings
            for (auto it = strings.begin(); it != strings.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value strVal;
                strVal.SetString(StringRef(it->second.c_str()));
                parent->AddMember(key, strVal, allocator);
            }
            for (auto it = stringArrays.begin(); it != stringArrays.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value a(kArrayType);
                for(int i = 0; i < it->second.size(); i++)
                {
                    Value strVal;
                    strVal.SetString(StringRef(it->second[i].c_str()));
                    a.PushBack(strVal, allocator); 
                }
                parent->AddMember(key, a, allocator);
            }

            //Objs
            for (auto it = objs.begin(); it != objs.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                
                Value objVal(kObjectType); 
                it->second->internal->BuildJson(&objVal, allocator);
                parent->AddMember(key, objVal, allocator);
            }
            for (auto it = objArrays.begin(); it != objArrays.end(); ++it)   
            {
                Value key(it->first.c_str(), allocator);
                Value a(kArrayType);
                for(int i = 0; i < it->second.size(); i++)
                {
                    Value objVal(kObjectType); 
                    it->second[i]->internal->BuildJson(&objVal, allocator);
                    a.PushBack(objVal, allocator); 
                }
                parent->AddMember(key, a, allocator);
            }
            
        }

        std::map<std::string, float> floats;
        std::map<std::string, std::vector<float>> floatArrays;
		std::map<std::string, int> ints;
        std::map<std::string, std::vector<int>> intArrays;
        std::map<std::string, bool> bools;
        std::map<std::string, std::vector<bool>> boolArrays;
        std::map<std::string, std::string> strings;
        std::map<std::string, std::vector<std::string>> stringArrays;
        std::map<std::string, SerializedObject*> objs;
        std::map<std::string, std::vector<SerializedObject*>> objArrays;
        
        Internal()
        {
        }

        ~Internal()
        {
            for (auto it = objs.begin(); it != objs.end(); ++it)   
            {
                delete it->second;
            }

            for (auto it = objArrays.begin(); it != objArrays.end(); ++it)   
            {
                for(int i = 0; i < it->second.size(); i++)
                {
                    delete it->second[i];
                }
            }
        }
}; 

SerializedObject::SerializedObject()
{
    internal = new Internal;    
}

SerializedObject::SerializedObject(std::string str)
{
    internal = new Internal;    
    FromString(str); 
}

SerializedObject::~SerializedObject()
{
    delete internal; 
}

void SerializedObject::Destroy()
{
    for (auto it = internal->objs.begin(); it != internal->objs.end(); ++it)   
    {
        delete it->second;
    }

    for (auto it = internal->objArrays.begin(); it != internal->objArrays.end(); ++it)   
    {
        for(int i = 0; i < it->second.size(); i++)
        {
            delete it->second[i];
        }
    }

    internal->floats.clear(); 
    internal->floatArrays.clear(); 
	internal->ints.clear(); 
    internal->intArrays.clear(); 
    internal->bools.clear(); 
    internal->boolArrays.clear(); 
    internal->strings.clear(); 
    internal->stringArrays.clear(); 
    internal->objs.clear(); 
    internal->objArrays.clear(); 
}

std::string SerializedObject::ToString()
{
    Document doc;
    doc.SetObject();
    Document::AllocatorType& allocator = doc.GetAllocator();

    internal->BuildJson(&doc, allocator);

    StringBuffer buffer;
    buffer.Clear();
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    return std::string( buffer.GetString() );
}

void SerializedObject::FromString(std::string str)
{
    Destroy(); 
    
    Document doc;
    doc.Parse(str.c_str());
    internal->ParseJson(&doc);
}

void SerializedObject::SetFloat(std::string name, float v)
{
    internal->floats[name] = v; 
} 

void SerializedObject::SetInt(std::string name, int v)
{
    internal->ints[name] = v; 
}

void SerializedObject::SetBool(std::string name, bool v)
{
    internal->bools[name] = v; 
}

void SerializedObject::SetString(std::string name, std::string v)
{
    internal->strings[name] = v;
}

void SerializedObject::SetVec2(std::string name, Vec2 v)
{
    internal->floatArrays[name] = {v.x, v.y}; 
}

void SerializedObject::SetVec3(std::string name, Vec3 v)
{
    internal->floatArrays[name] = {v.x, v.y, v.z}; 
}

void SerializedObject::SetVec4(std::string name, Vec4 v)
{
    internal->floatArrays[name] = {v.x, v.y, v.z, v.w}; 
}

void SerializedObject::SetColor(std::string name, Color v)
{
    internal->floatArrays[name] = {v.r, v.g, v.b, v.a}; 
}

void SerializedObject::SetMat2(std::string name, Mat2 v)
{  
    internal->floatArrays[name] = {v.col[0].x, v.col[0].y, 
                                   v.col[1].x, v.col[1].y}; 
}

void SerializedObject::SetMat3(std::string name, Mat3 v)
{
    internal->floatArrays[name] = {v.col[0].x, v.col[0].y, v.col[0].z,
                                   v.col[1].x, v.col[1].y, v.col[1].z,
                                   v.col[2].x, v.col[2].y, v.col[2].z};
}

void SerializedObject::SetSerializedObject(std::string name, SerializedObject* v)
{
    internal->objs[name] = v; 
}

void SerializedObject::SetFloatArray(std::string name, std::vector<float>& vec)
{
    internal->floatArrays[name] = vec; 
} 

void SerializedObject::SetIntArray(std::string name, std::vector<int>& vec)
{
    internal->intArrays[name] = vec; 
}

void SerializedObject::SetBoolArray(std::string name, std::vector<bool>& vec)
{
    internal->boolArrays[name] = vec; 
}

void SerializedObject::SetStringArray(std::string name, std::vector<std::string>& vec)
{
    internal->stringArrays[name] = vec; 
}

void SerializedObject::SetVec2Array(std::string name, std::vector<Vec2>& vec)
{
    std::vector<float> vTemp;
    vTemp.reserve(vec.size() * 2); 
    for(int i = 0; i < vec.size(); i++)
    {
        vTemp.push_back(vec[i].x);
        vTemp.push_back(vec[i].y);
    }
    internal->floatArrays[name] = vTemp; 
} 

void SerializedObject::SetVec3Array(std::string name, std::vector<Vec3>& vec)
{
    std::vector<float> vTemp;
    vTemp.reserve(vec.size() * 3); 
    for(int i = 0; i < vec.size(); i++)
    {
        vTemp.push_back(vec[i].x);
        vTemp.push_back(vec[i].y);
        vTemp.push_back(vec[i].z);
    }
    internal->floatArrays[name] = vTemp; 
}

void SerializedObject::SetVec4Array(std::string name, std::vector<Vec4>& vec)
{
    std::vector<float> vTemp;
    vTemp.reserve(vec.size() * 3); 
    for(int i = 0; i < vec.size(); i++)
    {
        vTemp.push_back(vec[i].x);
        vTemp.push_back(vec[i].y);
        vTemp.push_back(vec[i].z);
        vTemp.push_back(vec[i].w);
    }
    internal->floatArrays[name] = vTemp; 
}

void SerializedObject::SetMat2Array(std::string name, std::vector<Mat2>& vec)
{
    std::vector<float> vTemp;
    vTemp.reserve(vec.size() * 4); 
    for(int i = 0; i < vec.size(); i++)
    {
        vTemp.push_back(vec[i].col[0].x);
        vTemp.push_back(vec[i].col[0].y);
        vTemp.push_back(vec[i].col[1].x);
        vTemp.push_back(vec[i].col[1].y);
    }
    internal->floatArrays[name] = vTemp;
}

void SerializedObject::SetMat3Array(std::string name, std::vector<Mat3>& vec)
{
    std::vector<float> vTemp;
    vTemp.reserve(vec.size() * 9); 
    for(int i = 0; i < vec.size(); i++)
    {
        vTemp.push_back(vec[i].col[0].x);
        vTemp.push_back(vec[i].col[0].y);
        vTemp.push_back(vec[i].col[0].z);
        vTemp.push_back(vec[i].col[1].x);
        vTemp.push_back(vec[i].col[1].y);
        vTemp.push_back(vec[i].col[1].z);
        vTemp.push_back(vec[i].col[2].x);
        vTemp.push_back(vec[i].col[2].y);
        vTemp.push_back(vec[i].col[2].z);
    }
    internal->floatArrays[name] = vTemp;
}

void SerializedObject::SetSerializedObjectArray(std::string name, std::vector<SerializedObject*>& vec)
{    
    internal->objArrays[name] = vec; 
}

float SerializedObject::GetFloat(std::string name, float v)
{
    if(internal->floats.count(name) == 0) return v;
    return internal->floats[name];
}

int SerializedObject::GetInt(std::string name, int v)
{
    if(internal->ints.count(name) == 0) return v;
    return internal->ints[name];
}

bool SerializedObject::GetBool(std::string name, bool v)
{
    if(internal->bools.count(name) == 0) return v;
    return internal->bools[name];
}

std::string SerializedObject::GetString(std::string name, std::string v)
{
    if(internal->strings.count(name) == 0) return v;
    return internal->strings[name];
}

Vec2 SerializedObject::GetVec2(std::string name, Vec2 v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 2)
    {
        return Vec2((*vec)[0], (*vec)[1]);
    } 
    return v;
}

Vec3 SerializedObject::GetVec3(std::string name, Vec3 v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 3)
    {
        return Vec3((*vec)[0], (*vec)[1], (*vec)[2]);
    } 
    return v;
}

Vec4 SerializedObject::GetVec4(std::string name, Vec4 v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 4)
    {
        return Vec4((*vec)[0], (*vec)[1], (*vec)[2], (*vec)[3]);
    } 
    return v;
}

Color SerializedObject::GetColor(std::string name, Color v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 4)
    {
        return Color((*vec)[0], (*vec)[1], (*vec)[2], (*vec)[3]);
    } 
    return v;
}

Mat2 SerializedObject::GetMat2(std::string name, Mat2 v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 4)
    {
        Mat2 m;
        m.col[0].x = (*vec)[0]; 
        m.col[0].y = (*vec)[1]; 
        m.col[1].x = (*vec)[2]; 
        m.col[1].y = (*vec)[3]; 
        return m;
    } 
    return v;
}

Mat3 SerializedObject::GetMat3(std::string name, Mat3 v)
{
    if(internal->floatArrays.count(name) == 0) return v;
    auto vec = &(internal->floatArrays[name]);
    if(vec->size() >= 9)
    {
        Mat3 m;
        m.col[0].x = (*vec)[0]; 
        m.col[0].y = (*vec)[1]; 
        m.col[0].z = (*vec)[2];
        m.col[1].x = (*vec)[3]; 
        m.col[1].y = (*vec)[4];
        m.col[1].z = (*vec)[5];
        m.col[2].x = (*vec)[6]; 
        m.col[2].y = (*vec)[7];
        m.col[2].z = (*vec)[8];
        return m;
    } 
    return v;
}

SerializedObject* SerializedObject::GetSerializedObject(std::string name, SerializedObject* v)
{
    if(internal->objs.count(name) == 0) return v;
    return internal->objs[name];
}

bool SerializedObject::GetFloatArray(std::string name, std::vector<float>& vec)
{
    if(internal->floatArrays.count(name) == 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size());
    for(int i = 0; i < v->size(); i++)
    {
        vec.push_back((*v)[i]); 
    } 
    return true;
}

bool SerializedObject::GetIntArray(std::string name, std::vector<int>& vec)
{
    if(internal->intArrays.count(name) == 0) return false;
    auto v = &(internal->intArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size());
    for(int i = 0; i < v->size(); i++)
    {
        vec.push_back((*v)[i]); 
    } 
    return true;
}

bool SerializedObject::GetBoolArray(std::string name, std::vector<bool>& vec)
{
    if(internal->boolArrays.count(name) == 0) return false;
    auto v = &(internal->boolArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size());
    for(int i = 0; i < v->size(); i++)
    {
        vec.push_back((*v)[i]); 
    } 
    return true;
}

bool SerializedObject::GetStringArray(std::string name, std::vector<std::string>& vec)
{
    if(internal->stringArrays.count(name) == 0) return false;
    auto v = &(internal->stringArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size());
    for(int i = 0; i < v->size(); i++)
    {
        vec.push_back((*v)[i]); 
    } 
    return true;
} 

bool SerializedObject::GetVec2Array(std::string name, std::vector<Vec2>& vec)
{
    int vcount = internal->floatArrays.count(name); 
    if(vcount / 2 == 0 || vcount % 2 != 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size()/2);
    for(int i = 0; i < v->size()/2; i++)
    {
        Vec2 newV;
        newV.x = ((*v)[i * 2 + 0]); 
        newV.y = ((*v)[i * 2 + 1]);
        vec.push_back(newV);  
    } 
    return true;
} 

bool SerializedObject::GetVec3Array(std::string name, std::vector<Vec3>& vec)
{
    int vcount = internal->floatArrays.count(name); 
    if(vcount / 3 == 0 || vcount % 3 != 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size()/3);
    for(int i = 0; i < v->size()/3; i++)
    {
        Vec3 newV;
        newV.x = ((*v)[i * 3 + 0]); 
        newV.y = ((*v)[i * 3 + 1]);
        newV.z = ((*v)[i * 3 + 2]);
        vec.push_back(newV);  
    } 
    return true;
} 

bool SerializedObject::GetVec4Array(std::string name, std::vector<Vec4>& vec)
{
    int vcount = internal->floatArrays.count(name); 
    if(vcount / 4 == 0 || vcount % 4 != 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size()/4);
    for(int i = 0; i < v->size()/4; i++)
    {
        Vec4 newV;
        newV.x = ((*v)[i * 4 + 0]); 
        newV.y = ((*v)[i * 4 + 1]);
        newV.z = ((*v)[i * 4 + 2]);
        newV.w = ((*v)[i * 4 + 3]);
        vec.push_back(newV);  
    } 
    return true;
}

bool SerializedObject::GetMat2Array(std::string name, std::vector<Mat2>& vec)
{
    int vcount = internal->floatArrays.count(name); 
    if(vcount / 4 == 0 || vcount % 4 != 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size()/4);
    for(int i = 0; i < v->size()/4; i++)
    {
        Mat2 newV;
        newV.col[0].x = ((*v)[i * 4 + 0]); 
        newV.col[0].y = ((*v)[i * 4 + 1]);
        newV.col[1].x = ((*v)[i * 4 + 2]);
        newV.col[1].y = ((*v)[i * 4 + 3]);
        vec.push_back(newV);  
    } 
    return true;
}

bool SerializedObject::GetMat3Array(std::string name, std::vector<Mat3>& vec)
{
    int vcount = internal->floatArrays.count(name); 
    if(vcount / 9 == 0 || vcount % 9 != 0) return false;
    auto v = &(internal->floatArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size()/9);
    for(int i = 0; i < v->size()/9; i++)
    {
        Mat3 newV;
        newV.col[0].x = ((*v)[i * 9 + 0]); 
        newV.col[0].y = ((*v)[i * 9 + 1]);
        newV.col[0].z = ((*v)[i * 9 + 2]);
        newV.col[1].x = ((*v)[i * 9 + 3]);
        newV.col[1].y = ((*v)[i * 9 + 4]);
        newV.col[1].z = ((*v)[i * 9 + 5]);
        newV.col[2].x = ((*v)[i * 9 + 6]);
        newV.col[2].y = ((*v)[i * 9 + 7]);
        newV.col[2].z = ((*v)[i * 9 + 8]);
        vec.push_back(newV);  
    } 
    return true;
}

bool SerializedObject::GetSerializedObjectArray(std::string name, std::vector<SerializedObject*>& vec)
{
    if(internal->objArrays.count(name) == 0) return false;
    auto v = &(internal->objArrays[name]); 
    vec.clear(); 
    vec.reserve(v->size());
    for(int i = 0; i < v->size(); i++)
    {
        vec.push_back((*v)[i]); 
    } 
    return true;
}

}