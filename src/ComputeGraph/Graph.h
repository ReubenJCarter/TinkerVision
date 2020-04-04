#pragma once

#include "Visi_export.h"

#include "Node.h"

#include "SerializedObject.h"

namespace Visi
{
namespace ComputeGraph
{
		
/**
 */
	
class VISI_EXPORT Graph: public Node
{
    VISI_CLONEABLE_MACRO(Graph) 
    protected:
        std::vector<Node> nodes; 

	public:
        virtual void Serialize(SerializedObject* sObj)
        {
        }

        virtual void Deserialize(SerializedObject* sObj)
        {
        }
};
	
}
}