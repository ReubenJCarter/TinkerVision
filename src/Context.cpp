#include "Context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Visi 
{

class Context::ContextInternal
{
    public:
        GLFWwindow* window;

        ContextInternal(); 
        void Create();
        void MakeCurrent(); 
        void Destroy(); 
};

Context::ContextInternal::ContextInternal()
{
    window = NULL; 
}

void Context::ContextInternal::Create() 
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1, 1, "", NULL, NULL);
	glfwHideWindow(window);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
    
	glewInit();
}

void Context::ContextInternal::MakeCurrent()
{
    glfwMakeContextCurrent(window);
}

void Context::ContextInternal::Destroy() 
{
    glfwDestroyWindow(window);
}


Context::Context()
{
    contextInternal = new ContextInternal(); 
    contextInternal->Create(); 
}

Context::~Context()
{
    contextInternal->Destroy(); 
    delete contextInternal;
}

void Context::MakeCurrent()
{
    contextInternal->MakeCurrent(); 
}



	
}