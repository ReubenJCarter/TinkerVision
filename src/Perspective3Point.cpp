#include "Perspective3Point.h"

#include "ComputeShader.h"
#include "ProcessHelper.h"

#include <string>
#include <iostream>
#include <map>

namespace Visi
{

class Perspective3Point::Internal
{   
    public:
        Internal(); 
};

Perspective3Point::Internal::Internal()
{
}



Perspective3Point::Perspective3Point()
{
    internal = new Internal(); 
}

Perspective3Point::~Perspective3Point()
{ 
    delete internal; 
}


}