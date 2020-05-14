#pragma once

#include "TinkerVision_export.h"

#include "Node.h"

#include "Nodes/SourceNodes.h"

#include "../Core/SerializedObject.h"

#include <iostream>
#include <functional>

namespace TnkrVis
{
namespace ComputeGraph
{
		
/**
 */
	
class Graph: public Node
{
    TNKRVIS_CLONEABLE_MACRO(Graph) 

    protected:
        std::vector<Node*> nodes; 
        std::vector<Connection> graphOutputMapping;
        Nodes::InputCopySource graphInputSource;    

        bool dirty; 
        bool circularDependency;
        bool externalDependency;
        std::vector<int> callOrder; 

	public:
        Graph(): graphInputSource(this) 
        {
            dirty = true; 
        }
        
        ~Graph()
        {
            Clear(); 
        }

        inline void Clear()
        {
            graphOutputMapping.clear(); 
            for(int i = 0; i < nodes.size(); i++)
            {
                delete nodes[i]; 
            }
            nodes.clear();
            dirty = true; 
        }

        void AddNode(Node* n)
        {
            nodes.push_back(n); 
        }
        
        int NodeCount()
        {
            return nodes.size(); 
        }

        Node* GetNode(int inx)
        {
            return nodes[inx]; 
        }

        inline void AddOutputMapping(Node* n, int outinx=0)
        {
            Connection c;  
            c.outputInx = outinx; 
            c.node = n; 
            graphOutputMapping.push_back(c);
        }

        inline void ClearOutputMappings()
        {
            graphOutputMapping.clear(); 
        }

        Data GetOutput(int inx)
        {
            if(inx < 0 || inx >= graphOutputMapping.size()) //range check on input array
            {
                return Data(NullData, NULL); 
            }

            return graphOutputMapping[inx].node->GetOutput(graphOutputMapping[inx].outputInx); 
        }

        inline void AddInputMapping(Node* n, int graphInInx)
        {
            n->AddInputConnection(&graphInputSource, graphInInx); 
        }

        void RecalculateCallOrder()
        {
            // node pointer to index in nodes
            std::map<Node*, int> nodeInxMap; 
            std::vector<bool> nodeIsOutput(nodes.size(), true );
            for(int i = 0; i < nodes.size(); i++)
                nodeInxMap[nodes[i]] = i; 

            //add extra element for graph input source 
            nodeInxMap[&graphInputSource] = nodes.size(); 

            //check for connected nodes outside the graph
            externalDependency = false; 
            for(int i = 0; i < nodes.size(); i++)
            {
                for(int c = 0; c < nodes[i]->GetInputConnectionNumber(); c++)
                {
                    Connection conec = nodes[i]->GetInputConnection(c); 
                    //if one of the connections is not present in the graph there is externalDependency
                    if(!nodeInxMap.count(conec.node))
                    {
                        externalDependency = true; 
                        dirty = false; 
                        return;
                    }
                }
            }

            //find the output nodes search all nodes input connections (if a node is an input it cannot be edge output)
            for(int i = 0; i < nodes.size(); i++)
            {
                for(int c = 0; c < nodes[i]->GetInputConnectionNumber(); c++)
                {
                    Connection conec = nodes[i]->GetInputConnection(c); 
                    nodeIsOutput[ nodeInxMap[conec.node] ] = false; //isoutput
                }
            }
            std::vector<int> outputNodes;
            for(int i = 0; i < nodeIsOutput.size(); i++)
            {
                if(nodeIsOutput[i])
                    outputNodes.push_back(i);
            }

            //recursivly traverse graph from outputs
            circularDependency = false; 
            std::vector<bool> nodeTouched(nodes.size(), false );
            std::vector<bool> nodeAdded (nodes.size(), false );
            std::function<void(Node*)> recureTrav;
			recureTrav = [this, &recureTrav, &nodeInxMap, &nodeAdded, &nodeTouched](Node* n)
			{
                int inx = nodeInxMap[n];

                //check if the node is the graphinputsource node and return
                if(inx == nodes.size())
                    return; 

                //check for circular dep(if node has been touched before but not yet added )
                if(nodeTouched[inx] && !nodeAdded[inx])
                {
                    circularDependency = true; 
                    return; 
                }
                //we have been down this path before so just return 
                if(nodeAdded[inx])
                {
                    return; 
                }


                nodeTouched[inx] = true;
                
                //recure on all nbs
                for(int c = 0; c < n->GetInputConnectionNumber(); c++)
                {
                    Connection conec = n->GetInputConnection(c);  
                    recureTrav(conec.node);
                }
                
                //add node
                nodeAdded[inx] = true; 
                callOrder.push_back(inx); 
			};
            //start recursion on all output nodes
            for(int i = 0; i < outputNodes.size(); i++)
            {
                recureTrav( nodes[ outputNodes[i] ] );      
            }
            
            //Done
            dirty = false;

        }

        void Run()
        {
            if(dirty)
                RecalculateCallOrder(); 

            if(!(circularDependency || externalDependency))
            {
                for(int i = 0; i < callOrder.size(); i++)
                {
                    nodes[ callOrder[i] ]->Run();
                }
            }
            else
            {
                if(circularDependency)
                    std::cerr << "TnkrVis:ComputeGraph:Graph: Circular deppendency! FAIL\n"; 
                if(externalDependency)
                    std::cerr << "TnkrVis:ComputeGraph:Graph: External deppendency! FAIL\n"; 
            }
        }

        virtual void Serialize(SerializedObject* sObj)
        {
            sObj->Destroy(); 
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
                for(int c = 0; c < nodes[i]->GetInputConnectionNumber(); c++)
                {
                    SerializedObject* connectionSObj = new SerializedObject();
                    int outInx = nodes[i]->GetInputConnection(c).outputInx;
                    connectionSObj->SetInt("outinx", outInx);
                    std::string connectionId = idmap[nodes[i]->GetInputConnection(c).node];
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
            dirty = true; 

            //Destroy whole current graph 
            Clear(); 

            //
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
                        n->SetId( id ); 
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
                    
                    n->ClearInputConnections(); //needed??

                    for (int c = 0; c < connectionsArraySObj.size(); c++)
                    {
                        std::string connectionId = connectionsArraySObj[c]->GetString("id"); 
                        int outInx = connectionsArraySObj[c]->GetInt("outinx");  
                        n->AddInputConnection(ptrmap[connectionId], outInx); 
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