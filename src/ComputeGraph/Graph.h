#pragma once

#include "Visi_export.h"

#include "Node.h"

#include "Nodes/SourceNodes.h"

#include "SerializedObject.h"

#include <iostream>

namespace Visi
{
namespace ComputeGraph
{
		
/**
 */
	
class Graph: public Node
{
    VISI_CLONEABLE_MACRO(Graph) 
	public:
        std::vector<Node*> nodes; 
        std::vector<Connection> graphOutputMapping;
        Nodes::InputCopySource graphInputSource;         

        Graph(): graphInputSource(this) {}

        inline void AddOutputMapping(Node* n, int outinx=0)
        {
            graphOutputMapping.push_back({n, outinx});
        }

        Data GetOutput(int inx)
        {
            if(inx < 0 || inx >= graphOutputMapping.size()) //range check on input array
            {
                return Data(NullData, NULL); 
            }

            return graphOutputMapping[inx].node->GetOutput(graphOutputMapping[inx].outputInx); 
        }

        virtual void Serialize(SerializedObject* sObj)
        {
            //count the nubmer of different types
            std::map<std::string, int> typeCount;
            for(int i = 0; i < nodes.size(); i++)
            {
                std::string typeName = nodes[i]->GetTypeName(); 
                typeCount[typeName] = 0; 
            }

            //generate id map of ptr to id and add graph inputs node
            std::map<Node*, std::string> idmap; 
            idmap[&graphInputSource] = "GraphInputSource";
            for(int i = 0; i < nodes.size(); i++)
            {
                std::string typeName = nodes[i]->GetTypeName(); 
                std::string id = typeName + std::to_string(typeCount[typeName]); 
                typeCount[typeName]++; 
                idmap[nodes[i]] = id;
            }

            //create a serialized object for each node
            std::vector<SerializedObject*> nodessobj;
            for(int i = 0; i < nodes.size(); i++)
            {
                //id and type
                SerializedObject* nodeSObj = new SerializedObject();
                std::string typeName = nodes[i]->GetTypeName(); 
                nodeSObj->SetString("type", typeName);
                std::string id = idmap[ nodes[i] ]; 
                nodeSObj->SetString("id", id);

                //create the connections array for each object
                std::vector<SerializedObject*> connectionsArraySObj;
                for(int c = 0; c < nodes[i]->inputConnection.size(); c++)
                {
                    SerializedObject* connectionSObj = new SerializedObject();
                    int outInx = nodes[i]->inputConnection[c].outputInx;
                    connectionSObj->SetInt("outinx", outInx);
                    std::string connectionId = idmap[nodes[i]->inputConnection[c].node];
                    connectionSObj->SetString("id", connectionId);
                    connectionsArraySObj.push_back(connectionSObj); 
                }
                nodeSObj->SetSerializedObjectArray("connections", connectionsArraySObj);

                //params serialize
                SerializedObject* nodeParamsSObj = new SerializedObject();
                nodes[i]->Serialize(nodeParamsSObj);
                nodeSObj->SetSerializedObject("params", nodeParamsSObj);  
                nodessobj.push_back(nodeSObj);
            }

            sObj->SetSerializedObjectArray("nodes", nodessobj);
            
            std::vector<SerializedObject*> graphOutputMappingSObj;
            for(int i = 0; i < graphOutputMapping.size(); i++)
            {
                SerializedObject* connectionSObj = new SerializedObject();
                int outInx = graphOutputMapping[i].outputInx;
                connectionSObj->SetInt("outinx", outInx);
                std::string connectionId = idmap[ graphOutputMapping[i].node ];
                connectionSObj->SetString("id", connectionId);
                graphOutputMappingSObj.push_back(connectionSObj); 
            }
            sObj->SetSerializedObjectArray("outmaps", graphOutputMappingSObj);
            
        }

        virtual void Deserialize(SerializedObject* sObj)
        {
            std::map<std::string, Node*> ptrmap; 
            ptrmap["GraphInputSource"] = &graphInputSource;

            std::vector<SerializedObject*> nodessobj;
            bool hasNodes = sObj->GetSerializedObjectArray("nodes", nodessobj);
            if(hasNodes)
            {
                //build the nodes and serialise
                for(int i = 0; i < nodessobj.size(); i++)
                {
                    std::string type = nodessobj[i]->GetString("type"); 
                    std::string id = nodessobj[i]->GetString("id"); 

                    Node* n = Node::Create(type); 
                    ptrmap[id] = n; 
                    if(n != NULL)
                    {
                        n->id = id; 
                    }
                   
                    SerializedObject* params = nodessobj[i]->GetSerializedObject("params"); 
                    if (params != NULL && n != NULL)
                    {
                        n->Deserialize(params);
                    }
                    nodes.push_back(n); 
                }

                //build  the connections 
                for(int i = 0; i < nodessobj.size(); i++)
                {
                    std::vector<SerializedObject*> connectionsArraySObj;
                    nodessobj[i]->GetSerializedObjectArray("connections", connectionsArraySObj); 
                    
                    Node* n = nodes[i]; 
                    
                    n->inputConnection.reserve(connectionsArraySObj.size()); 

                    for (int c = 0; c < connectionsArraySObj.size(); c++)
                    {
                        std::string connectionId = connectionsArraySObj[c]->GetString("id"); 
                        int outInx = connectionsArraySObj[c]->GetInt("outinx"); 
                        Connection conec;
                        conec.node = ptrmap[connectionId]; 
                        conec.outputInx = outInx; 
                        n->inputConnection.push_back(conec); 
                    }
                }

                //build the output mappings
                std::vector<SerializedObject*> graphOutputMappingSObj;
                bool hasOutputMappings = sObj->GetSerializedObjectArray("outmaps", graphOutputMappingSObj);
                graphOutputMapping.clear(); 
                if(hasOutputMappings)
                {
                    for(int i = 0; i < graphOutputMappingSObj.size(); i++)
                    {
                        std::string connectionId = graphOutputMappingSObj[i]->GetString("id"); 
                        int outInx = graphOutputMappingSObj[i]->GetInt("outinx"); 
                        Connection conec;
                        conec.node = ptrmap[connectionId]; 
                        conec.outputInx = outInx; 
                        graphOutputMapping.push_back(conec); 
                    }
                }           
            }
        }
};
	
}
}