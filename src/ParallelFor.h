#pragma once

#include "Visi_export.h"

#include <functional> 

namespace Visi
{
	
class VISI_EXPORT ParallelFor
{
    private: 
        class Internal;
        Internal* internal;

	public:
		ParallelFor(); 
        ~ParallelFor(); 
        void Run(int itemCount, std::function<void(int)> innerForLoop);
};
	
}