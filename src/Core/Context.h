#pragma once

#include "TinkerVision_export.h"

namespace TnkrVis
{
		
/**
 *A helpful class to start an OpenGL context without a window. 
 *This class attempts to start an OpenGL 4.3 Core profile context. 
 */
	
class TINKERVISION_EXPORT Context
{        
    private:
        class ContextInternal; /**Forward declaration of internal Implementation*/
        ContextInternal* contextInternal; /**A pointer to the internal Implementation.*/

	public:
        /**The constructor.*/
        Context(); 

        /**The destructor.*/
        ~Context(); 
        
        /**Makes this context current. Subsequent OpenGL related calls will use this context*/
        void MakeCurrent();
		
		void* GetUnderlyingWindow();
        
};
	
}