#pragma once

#include "Visi_export.h"

#include <vector>

#include "../Core/VectorMath.h"
#include "../Core/Image.h"
#include "../Core/ImageGPU.h"

namespace Visi
{

namespace ComputeGraph
{
		
/**
 * Node of a compute graph.
 */
	
class VISI_EXPORT Node
{
    public:
        enum DataType
        {
           NullDate,

           ImageData,
           ImageGPUData, 
           ImageTypeData, 
           IntData,
           FloatData,  
           BoolData
        }; 

        class Data
        {
            public:
                DataType type; /**The type of the data.*/
                void* ptr; /**pointer to some data or object*/
                
                Data() 
                {

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
                    return type == NullDate;
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
                    return NULL; 
                }
        };

        struct Connection
        {
            Node* node; 
            int outputInx;
        };

    protected:
        /**connection list*/
        std::vector<Connection> inputConnection; 


        /**Gets a pointer to a generic value at the output index of the node. */
        virtual Data GetOutput(int inx) { return {NullDate, NULL}; }

         /**Sets directly the value of the output of the node from a value pointer */
        virtual void SetOutput(int inx, Data nodeData) {}

        /**Run the node. can be overloaded to use the inputconnections.GetOutput functions to get data.*/
        virtual void Run() {}; 

        /** Gets the input value at a particular index. fromt the output of an input connection*/
        inline Data GetInputData(int inx)
        {
            if(inx < 0 || inx >= inputConnection.size()) //range check on input array
            {
                return {NullDate, NULL}; 
            }

            return inputConnection[inx].node->GetOutput(inputConnection[inx].outputInx); 
        }

    public:
        /**The constructor.*/
        Node(); 

        /**The destructor.*/
        ~Node();       

        /**Setup and input to this node.*/
        inline void SetInput(Node* innode, int outinx=0)
        {
            inputConnection.push_back({innode, outinx});
        }
};
	
}

}