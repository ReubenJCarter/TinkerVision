#pragma once

#include "Visi_export.h"

namespace Visi
{
	
/*
 *Context
 *
 */
	
class VISI_EXPORT Context
{        
    private:
        class ContextInternal;
        ContextInternal* contextInternal;

	public:
        Context(); 
        ~Context(); 
        void MakeCurrent();
        
};
	
}