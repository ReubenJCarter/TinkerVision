#include "Node.h"

namespace Visi
{

namespace ComputeGraph
{

Node::Node()
{

}

Node::~Node()
{
    
}

void Node::SetInput(Node* innode, int outinx)
{
    inputConnection.push_back({innode, outinx});
}

}

}