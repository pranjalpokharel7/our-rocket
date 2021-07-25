
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <string_view>

struct GLFWwindow;

class ShaderProgram
{
 public:
  GLuint shader_program;
  GLuint vertex_shader;
  GLuint fragment_shader;

 public:
  ShaderProgram() = default;
  ShaderProgram(const std::string_view& vs_path, const std::string_view& fs_path);
};

int CompileAndLogShader(const std::string_view &shader_path,GLuint shader_type);

struct Renderer
{
  GLFWwindow* window = nullptr;
  GLuint VBO;
  unsigned vbo_offset = 0;
  unsigned max_allocated = 0;
  ShaderProgram shader_program;

};

namespace Render {
  Renderer CreateRenderer(int width = 800, int height = 800);
    void DestroyRenderer(Renderer* );
    } // namespace Render

// Implement texture class here
class Texture2D
{
public:
  GLuint texture_id{};
  GLint no_of_channels;
  GLint width,height; 
public:
  Texture2D(std::string_view path);
};

    
  
    
#endif
