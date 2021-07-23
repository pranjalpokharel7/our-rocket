

#include "../include/renderer.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GLFW/glfw3.h>

ShaderProgram::ShaderProgram(const std::string_view &vs_path,
                             const std::string_view &fs_path)
{
    // this procedure compiles both fragment and vertex shader
    vertex_shader = CompileAndLogShader(vs_path, GL_VERTEX_SHADER);
    fragment_shader = CompileAndLogShader(fs_path, GL_FRAGMENT_SHADER);

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    int flag;
    glGetProgramiv(shader_program,GL_LINK_STATUS, &flag);
    if (!flag)
    {
	char infoLog[512];
	glGetProgramInfoLog(shader_program,512,NULL,infoLog);
	std::cerr << "Failed to link shader program .. " << infoLog;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

int CompileAndLogShader(const std::string_view& path, GLuint shader_type)
{
    std::ifstream read_file {path.data(), std::ios::in};
    if (read_file.fail())
    {
	std::cout << "Failed to open shader file " << path.data() << std::endl;
	return -1;
    }

    std::string temp;
    std::stringstream content;
    while (std::getline(read_file,temp))
	content << temp << '\n';

    auto shader = glCreateShader(shader_type);
    auto str = content.str();
    auto c_str = str.c_str();
    glShaderSource(shader,1,&c_str,NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
    if (!success)
    {
	char infoLog[512];
	glGetShaderInfoLog(shader,512,nullptr,infoLog);
	std::cerr << "\nFailed to compile " << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader" << infoLog << std::endl;
	return -1;
    }
    else
	std::cerr << '\n' << (shader_type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader compilation passed." << std::endl;
    return shader;
}
    
namespace Render
{
    Renderer CreateRenderer()
    {
	if (!glfwInit())
	{
	    std::cerr << "Failed to load GLFW API. Exiting \n";
	    return {};
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800,800,"Model rendering",nullptr,nullptr);
	if (!window)
	{
	    std::cerr << "Failed to create window. Exiting\n" << std::endl;
	    return Renderer{};
	}

	glfwMakeContextCurrent(window);
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    // could use optional here but leave it for now
	    std::cerr << "Failed to load glad loader... Exiting" <<  std::endl;
	    return Renderer{};
	}
	auto render = Renderer();
	render.window = window;
	

	// Allocate the initial VBO
	glGenBuffers(1,&render.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, render.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * 2048, nullptr, GL_DYNAMIC_DRAW);
	render.max_allocated = 4 * 2048;
	render.vbo_offset = 0;
	return render;		
    }

    void DestroyRenderer(Renderer* render)
    {
	glfwDestroyWindow(render->window);
	glfwTerminate();
    }
}
