#include "Window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace TnkrVis 
{

class Window::Internal
{
    private:
		Context* originContext; 
        GLFWwindow* window;
        int width;
        int height;

        struct Vertex
        {
            glm::vec2 position; 
            glm::vec2 texco; 
        };
        unsigned int programObject; 
        unsigned int elementBuffer;
        unsigned int vertexArrayObject; 
        unsigned int vertexBuffer; 

        void CreateShaders(); 
        void CreateBuffers();
        void DrawVAO();  

    public:
        Internal(); 
        void Create(int w, int h, Context* shareContext, std::string title="");
        void MakeCurrent(); 
        void Destroy(); 
        void Clear(float r, float g, float b, float a); 
        void DrawImage(Image* image);
        void DrawImage(ImageGPU* image); 
        bool ShouldClose(); 
        void Refresh();
};

void Window::Internal::CreateShaders()
{
    std::string vSrc = R"(
    #version 330

    //attribs
    layout(location = 0) in vec2 vertexPosition;
    layout(location = 1) in vec2 vertexTexco;

    //transforms
    uniform mat3 mvpMatrix;
    
    //outputs
    out vec2 fragTexco;
    
    //main
    void main()
    {
        //compute outputs
        fragTexco = vertexTexco; 
        vec3 finalPosition = mvpMatrix * vec3(vertexPosition.x, vertexPosition.y, 1);
        gl_Position = vec4(finalPosition.x, finalPosition.y, 0, 1); 
    }
    )";

    std::string fSrc = R"(
    #version 330
    
    //inputs
    in vec2 fragTexco;
    
    //uniforms
    uniform sampler2D colorSampler;

    //output
    layout(location = 0) out vec4 outputColor;
    
    //main
    void main()
    {
        outputColor = texture(colorSampler, fragTexco.xy);
    }
    )";

    const char* vsrc[1];
	const char* fsrc[1];
	vsrc[0] = vSrc.c_str();
	fsrc[0] = fSrc.c_str();
	
	//compile vertex and fragment shaders from source
	unsigned int vertexObject = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexObject, 1, vsrc, NULL);
	glCompileShader(vertexObject);
	unsigned int fragmentObject = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentObject, 1, fsrc, NULL);
	glCompileShader(fragmentObject);
	programObject = glCreateProgram();
	glAttachShader(programObject, vertexObject);
	glAttachShader(programObject, fragmentObject);
	glLinkProgram(programObject);

	//Check status of shader and log any compile time errors
	int linkStatus;
	glGetProgramiv(programObject, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE) 
	{
		char log[5000];
		//for(int i = 0; i < 5000; i++) log[i] = (char)0; 
		int logLen; 
		glGetProgramInfoLog(programObject, 5000, &logLen, log);
		std::cerr << "TnkrVis:Window:Shader:Could not link program: " << std::endl;
		std::cerr << log << std::endl;
		glGetShaderInfoLog(vertexObject, 5000, &logLen, log);
		std::cerr << "TnkrVis:Window:vertex shader log:\n" << log << std::endl;
		glGetShaderInfoLog(fragmentObject, 5000, &logLen, log);
		std::cerr << "TnkrVis:Window:fragment shader log:\n" << log << std::endl;
		glDeleteProgram(programObject);
		programObject = 0;
	}
	else
	{
		std::cout << "TnkrVis:Window:Shader:compile success " << std::endl;
	}
}

void Window::Internal::CreateBuffers()
{
    //generate buffers
    glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &elementBuffer);
	glGenVertexArrays(1, &vertexArrayObject);

    //bind buffers
    glBindVertexArray(vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void*)((uintptr_t)0));//position
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(Vertex), (void*)( (uintptr_t)(8) ));//texco

    //Create quad
    Vertex a = {{-1, -1}, {0, 0}};
    Vertex b = {{1, -1}, {1, 0}};
    Vertex c = {{1, 1}, {1, 1}};
    Vertex d = {{-1, 1}, {0, 1}};
    std::vector<Vertex> vertexBuffer = {a, b, c, d};
    std::vector<unsigned int> elementBuffer = {0, 1, 2, 2, 3, 0};

	//copy data to buffers
	glBufferData(GL_ARRAY_BUFFER, vertexBuffer.size() * sizeof(Vertex), (uint8_t*)&vertexBuffer[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementBuffer.size() * sizeof(unsigned int), (uint8_t*)&elementBuffer[0], GL_STATIC_DRAW);
		
	//Unbind all
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Window::Internal::DrawVAO()
{   
    glUseProgram(programObject);
    glBindVertexArray(vertexArrayObject);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
    glUseProgram(0);
}

Window::Internal::Internal()
{
    window = NULL; 
}

void Window::Internal::Create(int w, int h, Context* shareContext, std::string title) 
{    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    width = w; 
    height = h; 
	window = glfwCreateWindow(w, h, title.c_str(), NULL, (GLFWwindow*)(shareContext->GetUnderlyingWindow()) );
	originContext = shareContext; 
    if(window == NULL)
    {
        std::cerr << "TnkrVis:Window:Error creating window\n"; 
        return; 
    }
    
    glfwShowWindow(window);
    glfwMakeContextCurrent(window);
	glfwSwapInterval(0);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    
    CreateShaders(); 
    CreateBuffers();

    glViewport(0, 0, width, height);
	
	originContext->MakeCurrent(); 
}

void Window::Internal::MakeCurrent()
{
    glfwMakeContextCurrent(window);
}

void Window::Internal::Destroy() 
{
    glfwDestroyWindow(window);
}

void Window::Internal::Clear(float r, float g, float b, float a)
{
    MakeCurrent(); 
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	originContext->MakeCurrent(); 
}

void Window::Internal::DrawImage(Image* image)
{
    MakeCurrent(); 
    glm::mat3 mvpMat = glm::mat3(1); 
    glProgramUniformMatrix3fv(programObject, 
                              glGetUniformLocation(programObject, "mvpMatrix"), 
                              1, 
                              false, 
                              glm::value_ptr(mvpMat));
    DrawVAO(); 
	originContext->MakeCurrent(); 
}

void Window::Internal::DrawImage(ImageGPU* image)
{
    MakeCurrent(); 
    int unit = 0; 
    glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, image->GetTexture());
    glProgramUniform1i(programObject, glGetUniformLocation(programObject, "colorSampler"), unit);

    glm::mat3 mvpMat = glm::mat3(1); 
    glProgramUniformMatrix3fv(programObject, 
                              glGetUniformLocation(programObject, "mvpMatrix"), 
                              1, 
                              false, 
                              glm::value_ptr(mvpMat));
    DrawVAO(); 
	originContext->MakeCurrent(); 
}

bool Window::Internal::ShouldClose()
{
    return glfwWindowShouldClose(window) != 0; 
}

void Window::Internal::Refresh()
{
    MakeCurrent(); 
    glViewport(0, 0, width, height);
    glfwSwapBuffers(window);
    glfwPollEvents();
	originContext->MakeCurrent(); 
}


Window::Window(int w, int h, Context* shareContext, std::string title)
{
    internal = new Internal(); 
    internal->Create(w, h, shareContext, title); 
}

Window::~Window()
{
    internal->Destroy(); 
    delete internal;
}

void Window::DrawImage(Image* image)
{
    internal->DrawImage(image);
}

void Window::DrawImage(ImageGPU* image)
{
    internal->DrawImage(image); 
}

void Window::Clear(float r, float g, float b, float a)
{
    internal->Clear(r, g, b, a); 
}

void Window::Refresh()
{
    internal->Refresh(); 
}

bool Window::ShouldClose()
{
    return internal->ShouldClose(); 
}

}