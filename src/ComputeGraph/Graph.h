#pragma once

#include "Visi_export.h"

#include "Node.h"

namespace Visi
{

namespace ComputeGraph
{
		
/**
 */
	
class VISI_EXPORT Graph: public Node
{        
	public:
        /**The constructor.*/
        Graph(); 

        /**The destructor.*/
        ~Graph();     
};
	
}

}