#include "ComputeGraph/Graph.h"

#include <iostream>
#include <fstream>

int main(int argc, char *argv[])
{
    //cmd ARGS
    if(argc < 2)
    {
        std::cout << "Not correct inputs, use like: TinkerVisionCMD <filename>\n\n\nHave a wonderful day.";
        return 1; 
    }

    std::cout << "TinkerVision Starting\n"; 

    std::string fileName(argv[1]); 

    //Read json string from file
    std::cout << "Reading Json File\n"; 
    
    std::ifstream fs; 
    fs.open(argv[1], std::ifstream::binary);
    if(!fs.is_open())
    {
        std::cout << "Failed to open JSON file " << fileName << "\n";
        return 1; 
    }

    int fileBegin = fs.tellg();
    fs.seekg(0, std::ios::end);  
    int fileEnd = fs.tellg();
    int byteCount = fileEnd - fileBegin;
    fs.seekg(0, std::ios::beg);  
    char* fBuffer = new char [byteCount];
    fs.read(fBuffer, byteCount); 
    std::string jsonStr(fBuffer, byteCount); 

    //Deserialize the graph and run
    std::cout << "Registering Compute Nodes\n"; 

    TnkrVis::ComputeGraph::Graph::RegisterNodes(); 

    std::cout << "Loading json\n"; 
    TnkrVis::SerializedObject graphJson; 
    graphJson.FromString(jsonStr); 
    
    std::cout << "Deserializing graph\n"; 
    TnkrVis::ComputeGraph::Graph graph;
    graph.Deserialize(&graphJson); 

    std::cout << "Running graph\n"; 
    graph.Run(); 

    return 0; 
}