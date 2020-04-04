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
        std::vector<Node*> nodes; 

	public:
        virtual void Serialize(SerializedObject* sObj)
        {
            std::map<std::string, int> typeCount;
            for(int i = 0; i < nodes.size(); i++)
            {
                std::string typeName = nodes[i]->GetTypeName(); 
                typeCount[typeName] = 0; 
            }
            std::map<Node*, std::string> idmap; 
            for(int i = 0; i < nodes.size(); i++)
            {
                std::string typeName = nodes[i]->GetTypeName(); 
                std::string id = typeName + std::to_string(typeCount[typeName]); 
                typeCount[typeName]++; 
                idmap[nodes[i]] = id;
            }

            std::vector<SerializedObject*> nodesParam;
            for(int i = 0; i < nodes.size(); i++)
            {
                SerializedObject* nodeSObj = new SerializedObject();
                std::string typeName = nodes[i]->GetTypeName(); 
                nodeSObj->SetString("type", typeName);
                std::string id = idmap[ nodes[i] ]; 
                nodeSObj->SetString("id", id);
                SerializedObject* nodeParamsSObj = new SerializedObject();
                nodes[i]->Serialize(nodeParamsSObj);
                nodeSObj->SetSerializedObject("params", nodeParamsSObj);  
                nodesParam.push_back(nodeSObj);
            }
            sObj->SetSerializedObjectArray("nodes", nodesParam);
        }

        virtual void Deserialize(SerializedObject* sObj)
        {
        }
};
	
}
}