#pragma once

#include "TinkerVision_export.h"

#include <vector>
#include <map>

#include "../Core/VectorMath.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

#include "Core/SerializedObject.h"

#define TNKRVIS_CLONEABLE_MACRO(TYPE)\
public:\
TYPE* CloneType() { return new TYPE(); }\
std::string GetTypeName() { return #TYPE;}\
private:


namespace TnkrVis
{
namespace ComputeGraph
{
		
/**
 * Node of a compute graph.
 */
	
class TINKERVISION_EXPORT Node
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

        class Connection
        {
            public:
                Connection()
                {
                    node = NULL;
                    outputInx = -1; 
                }

                Connection(Node* n, int outInx)
                {
                    node = n;
                    outputInx = outInx; 
                }

                Node* node; 
                int outputInx;
        };

    protected:    
        /**id of node*/
        std::string id; 

        /**connection list*/    
        std::vector<Connection> inputConnection; 

    public:
        virtual ~Node() {}

        /**Run the node. can be overloaded to use the inputconnections.GetOutput functions to get data.*/
        virtual void Run() {}; 

        /**Gets a pointer to a generic value at the output index of the node. */
        virtual Data GetOutput(int inx) { return Data(NullData, NULL); }

        virtual void Serialize(SerializedObject* sObj){}
        
        virtual void Deserialize(SerializedObject* sObj){}

        /** Gets the input value at a particular index. fromt the output of an input connection*/
        inline Data GetInputData(int inx)
        {
            if(inx < 0 || inx >= inputConnection.size()) //range check on input array
            {
                return Data(NullData, NULL); 
            }

            return inputConnection[inx].node->GetOutput(inputConnection[inx].outputInx); 
        }

        /**Setup and input to this node.*/
        inline void AddInputConnection(Node* innode, int outinx=0)
        {
            inputConnection.push_back(Connection(innode, outinx));
        }

        inline void ClearInputConnections()
        {
            inputConnection.clear(); 
        }

        inline int GetInputConnectionNumber()
        {
            return inputConnection.size();
        }

        inline Connection GetInputConnection(int inx)
        {
            if(inx < 0 || inx >= inputConnection.size()) //range check on input array
            {
                return Connection(); 
            }

            return inputConnection[inx]; 
        }

        inline void SetId(std::string idstr)
        {
            id = idstr;
        }

        inline std::string GetId()
        {
            return id; 
        }
};

template<class T>
class BaseProcess1In1OutCPUOnly: public Node
{
    protected:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/
        T proc;
    
    public: 
        Data GetOutput(int inx){ return outImageData; }

        inline virtual void SetParams()
        {
            
        }

        void Run()
        {
            SetParams(); 
            
            Image* inDstAsimage = GetInputData(0).AsType<Image>(ImageData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);  
                        
            if(inSrcAsimage != NULL && inDstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, inDstAsimage);
                outImageData = Data(DataType::ImageData, inDstAsimage); 
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

template<class T>
class BaseProcess1In1Out: public Node
{
    protected:
        Data outImageData; /** cache of the output data, also null data when no image avilible.*/
        T proc;
    
    public: 
        Data GetOutput(int inx){ return outImageData; }

        inline virtual void SetParams()
        {
            
        }

        void Run()
        {
            SetParams(); 

            Image* inDstAsimage = GetInputData(0).AsType<Image>(ImageData);
            ImageGPU* inDstAsimageGPU = GetInputData(0).AsType<ImageGPU>(ImageGPUData);  

            Image* inSrcAsimage = GetInputData(1).AsType<Image>(ImageData);  
            ImageGPU* inSrcAsimageGPU = GetInputData(1).AsType<ImageGPU>(ImageGPUData);  
                        
            if(inSrcAsimage != NULL && inDstAsimage != NULL)
            {
                proc.Run(inSrcAsimage, inDstAsimage);
                outImageData = Data(DataType::ImageData, inDstAsimage); 
            }
            else if(inSrcAsimageGPU != NULL && inDstAsimageGPU != NULL)
            {
                proc.Run(inSrcAsimageGPU, inDstAsimageGPU);
                outImageData = Data(DataType::ImageGPUData, inDstAsimageGPU);
            }
            else
            {
                outImageData = Data(NullData, NULL); 
            }
        }
}; 

}
}