#include "Context.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

namespace TnkrVis 
{

class Context::ContextInternal
{
    public:
        GLFWwindow* window;

        ContextInternal(); 
        void Create();
        void MakeCurrent(); 
        void Destroy(); 
        static void GLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
        static void GLFWErrorMessageCallback(int errorCode, const char* message); 
		
		void* GetUnderlyingWindow(); 

};

Context::ContextInternal::ContextInternal()
{
    window = NULL; 
}

void Context::ContextInternal::Create() 
{
    glfwInit();
    glfwSetErrorCallback(&Context::ContextInternal::GLFWErrorMessageCallback); 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    
	window = glfwCreateWindow(1, 1, "", NULL, NULL);
	
    if(window == NULL)
    {
        std::cerr << "TnkrVis:Context:Error creating context\n"; 
        return; 
    }
    
    glfwHideWindow(window);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0);
    
	glewInit();

    

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
    glDebugMessageCallback(&Context::ContextInternal::GLErrorMessageCallback, 0);
}

void Context::ContextInternal::MakeCurrent()
{
    glfwMakeContextCurrent(window);
}

void Context::ContextInternal::Destroy() 
{
    glfwDestroyWindow(window);
}

void Context::ContextInternal::GLErrorMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    std::cerr << 
        "GL CALLBACK:" << 
        ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << 
        " type:" << type << " severity:" << severity << " message:" <<  message << "\n"; 
}

void Context::ContextInternal::GLFWErrorMessageCallback(int errorCode, const char* message)
{
    std::cerr << 
        "GLFW CALLBACK:" << 
        "Error:" << errorCode <<
        " message:\n" << message << 
        "\n"; 
}

void* Context::ContextInternal::GetUnderlyingWindow()
{
	return window; 
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

void* Context::GetUnderlyingWindow()
{
    return contextInternal->GetUnderlyingWindow(); 
}


	
}