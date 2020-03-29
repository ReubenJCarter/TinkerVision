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
        struct Connection
        {
            Node* node; 
            int outputInx;
        };

    protected:
        /**connection list*/
        std::vector<Connection> inputConnection; 

        /**Gets a pointer to a generic value at the output index of the node. */
        virtual void* GetOutput(int inx) {return NULL;}

         /**Sets directly the value of the output of the node from a value pointer */
        virtual void SetOutput(int inx, void* valptr) {}

        /**Run the node. can be overloaded to use the inputconnections.GetOutput functions to get data.*/
        virtual void Run() {}; 
        
    public:
        /**The constructor.*/
        Node(); 

        /**The destructor.*/
        ~Node();       

        /**Setup and input to this node.*/
        void SetInput(Node* innode, int outinx=0); 
};
	
}

}