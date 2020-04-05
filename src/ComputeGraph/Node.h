#pragma once

#include "Visi_export.h"

#include <vector>
#include <map>

#include "../Core/VectorMath.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "SerializedObject.h"

#define VISI_CLONEABLE_MACRO(TYPE)\
public:\
TYPE* CloneType() { return new TYPE(); }\
std::string GetTypeName() { return #TYPE;}\
private:


namespace Visi
{
namespace ComputeGraph
{
		
/**
 * Node of a compute graph.
 */
	
class Node
{
    private:
		static std::map<std::string, Node*> nodeTypes; 

	public:
		template<class T> static void RegisterType()
		{
			T* n = new T();
			nodeTypes[n->GetTypeName()] = n;
		}

		static Node* Create(std::string typeName)
        {
            if(nodeTypes.count(typeName) != 0)
            {
                return nodeTypes[typeName]->CloneType();
            }
            return NULL;
        } 

        virtual Node* CloneType() = 0; 
		virtual std::string GetTypeName() = 0; 

    public:
        enum DataType //could be replaced with typeinfo?
        {
           NullData,

           ImageData,
           ImageGPUData, 
           ImageTypeData, 

           IntData,
           FloatData,  
           BoolData, 
           Vec2Data,
           Vec3Data,
           Vec4Data, 
           Mat2Data,
           Mat3Data,
           ColorData, 
           
           StringData

        }; 

        class Data
        {
            public:
                DataType type; /**The type of the data.*/
                void* ptr; /**pointer to some data or object*/
                
                Data() 
                {
                    type = NullData;
                    ptr = NULL;
                }

                Data(DataType t, void* p) 
                {
                    type = t; 
                    ptr = p;
                }

                inline bool IsType(DataType ty)
                {
                    return type == ty;
                }

                inline bool IsNull()
                {
                    return type == NullData;
                }

                template<class T>
                T* AsType(DataType ty) //Can we get rid of the Datatype param in the future by using c++ stuff?
                {
                    if(ty == type)
                    {
                        return (T*)ptr; 
                    }
                    return NULL; 
                }

                template<class T>
                T DerefAsType(DataType ty, T defaultValue)
                {
                    if(ty == type)
                    {
                        return *((T*)ptr); 
                    }
                    return defaultValue; 
                }
        };

        struct Connection
        {
            Node* node; 
            int outputInx;
        };

    protected:    
        /**Run the node. can be overloaded to use the inputconnections.GetOutput functions to get data.*/
        virtual void Run() {}; 

    public:
         /**connection list*/    
        std::vector<Connection> inputConnection; 

        /**id of node*/
        std::string id; 

        /** Gets the input value at a particular index. fromt the output of an input connection*/
        inline Data GetInputData(int inx)
        {
            if(inx < 0 || inx >= inputConnection.size()) //range check on input array
            {
                return Data(NullData, NULL); 
            }

            return inputConnection[inx].node->GetOutput(inputConnection[inx].outputInx); 
        }

        /**Gets a pointer to a generic value at the output index of the node. */
        virtual Data GetOutput(int inx) { return Data(NullData, NULL); }

        /**Setup and input to this node.*/
        inline void SetInput(Node* innode, int outinx=0)
        {
            inputConnection.push_back({innode, outinx});
        }
        
        virtual void Serialize(SerializedObject* sObj){}
        
        virtual void Deserialize(SerializedObject* sObj){}
};

std::map<std::string, Node*>  Node::nodeTypes; 
	
}
}