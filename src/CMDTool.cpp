#include "ComputeGraph/RegisterNodes.h"
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
    std::string fileName(argv[1]); 

    //Read json string from file
    std::ifstream fs; 
    fs.open(argv[1]);
    if(!fs.is_open())
    {
        std::cout << "Failed to open JSON file " << fileName << "\n";
        return 1; 
    }

    fs.seekg(fs.end);  
    unsigned int byteCount = fs.tellg();
    fs.seekg(fs.beg);  
    char* fBuffer = new char [byteCount];
    fs.read(fBuffer, byteCount); 
    std::string jsonStr(fBuffer, byteCount); 


    //Deserialize the graph and run
    TnkrVis::ComputeGraph::RegisterNodes(); 

    TnkrVis::SerializedObject graphJson; 
    graphJson.FromString(jsonStr); 
    TnkrVis::ComputeGraph::Graph graph;
    graph.Deserialize(&graphJson); 

    graph.Run(); 

    return 0; 
}