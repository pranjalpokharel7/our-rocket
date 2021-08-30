
#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <glad/glad.h>
#include <string_view>
#include "camera.hpp"

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
  ShaderProgram light_cube_shader_program;
  ShaderProgram skybox_shader_program;
};

namespace Render {
  Renderer CreateRenderer(int width = 800, int height = 800);
    void DestroyRenderer(Renderer* );

  // The struct state will only have one instance of it and will store the current state of the renderer
  struct State
  {
    // Adding camera here may not be the best idea but .. for now 
    Camera camera;
    static constexpr double angle_constant = (double)1/1000;
    bool first_mouse = true;
    float fovy = 45.0f;
    float yaw = 0, pitch = 0;
    double last_xpos = 0.0, last_ypos = 0;
    int screen_width = 1000;
    int screen_height = 1000;

    float delta_time = 0.f;
    float last_frame = 0.f;

    // Need some way to handle this first frame thingy
    bool first_frame = true;
  };
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
