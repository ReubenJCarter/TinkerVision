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
    protected:
        std::vector<Node> nodes; 

	public:
        /**Reads a json string and creates the appropriate nodes and connects them up */
        virtual void ReadFromJSON(char* str)
        {
        }

        /**write to a json string from the nodes.*/
        virtual void WriteToJSON(char* str)
        {
        }
};
	
}

}