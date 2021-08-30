
#include "../include/renderer.h"
#include "../include/model.hpp"
#include "../include/stb_image.h"
#include "../include/helpers.hpp"
#include "../include/cube.hpp"
#include "../include/skybox.hpp"
#include "../Maths/Matrix.hpp"
#include "../Maths/vec.hpp"
#include "../include/zbuffer.hpp"

#include <GLFW/glfw3.h>
#include <chrono>
#include <string_view>

// #define NR_LIGHT_CUBES 3

static Render::State state;

static void handleEvents(GLFWwindow *window);
// Lets try without header and matching function defintion
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

int main(int argc, char **argv)
{
  auto render = Render::CreateRenderer(state.screen_width,state.screen_height);
  render.shader_program = ShaderProgram("./shader/model-vertex.glsl", "./shader/model-fragment.glsl");
  
  render.light_cube_shader_program = ShaderProgram("./shader/light-cube-vertex.glsl","./shader/light-cube-fragment.glsl");
  render.skybox_shader_program = ShaderProgram("./shader/skybox-vertex.glsl","./shader/skybox-fragment.glsl");

  stbi_set_flip_vertically_on_load(true);

  //Model mainModel("./models/cyborg/cyborg.obj");
  //Model mainModel("./models/rocket/rocket.obj");
  //  Model mainModel("./Models/cubie/cubie.obj");
   Model mainModel("./Models/Final/rocket.obj");
  //Model mainModel("./models/backpack/backpack.obj");
   Model floorModel("./Models/Final/Floor/floor.obj");
  
  FMath::Vec3<float> cube_positions[] = {
      //FMath::Vec3(0.6964f,  1.666f, 0.706876f),
      //FMath::Vec3(1.06075f, 1.01993f, -0.6604f),
      //FMath::Vec3(1.1641f, 1.01472f, 1.03914f),


      FMath::Vec3(2.19173f, 1.93018f, 2.25142f),
      FMath::Vec3(2.15991f, 1.89708f, -1.57857f),
      FMath::Vec3(1.32463f, 5.01879f, 2.60398f),
      FMath::Vec3(-0.746886f, 1.90037f, 2.15353f),
      FMath::Vec3(-0.705121f, 1.94047f, -1.54101f),

      // Lighting for four corners
      FMath::Vec3(6.0f,0.45f,-6.0f),
      FMath::Vec3(-6.0f,0.45f,6.0f),
      FMath::Vec3(-6.0f,0.45f,-6.0f),
      FMath::Vec3(6.0f,0.45f,6.0f),
  };

  
  FMath::Vec3<float> cube_colors[] = {
      //FMath::Vec3(1.0f, 1.0f, 1.0f),
      //FMath::Vec3(0.498f, 1.000f, 0.831f),
      //FMath::Vec3(0.863f, 0.078f, 0.235f),
      //FMath::Vec3(1.000f, 0.843f, 0.000f),
      
      FMath::Vec3(1.0f, 1.0f, 1.0f),
      FMath::Vec3(0.0f, 0.2f, 0.0f),
      FMath::Vec3(1.0f, 1.0f, 1.0f),
      FMath::Vec3(1.0f, 0.0f, 0.0f),
      FMath::Vec3(1.0f, 1.0f, 1.0f),

      FMath::Vec3(1.0f,0.0f,1.0f),
      FMath::Vec3(1.0f,0.0f,0.0f),
      FMath::Vec3(0.0f,0.0f,1.0f),
      FMath::Vec3(1.0f,1.0f,0.0f)
  };
  int NR_LIGHT_CUBES = sizeof(cube_positions) / (3 * sizeof(float));

  Cube cubes[NR_LIGHT_CUBES];
  for (int i = 0; i < NR_LIGHT_CUBES; i++){
    cubes[i].position = cube_positions[i];
    cubes[i].scale = FMath::Vec3<float>(0.08f, 0.08f, 0.08f);
    cubes[i].color = cube_colors[i];
  }

  // Manual depth buffering
  auto zrender = Render::ZRender(state.screen_width,state.screen_height);
  auto zprogram = glCreateProgram();
  auto zvertex  = CompileAndLogShader("./shader/zvertex.glsl",GL_VERTEX_SHADER);
  glAttachShader(zprogram,zvertex);
  zrender.attachDepthFragment(zprogram);
  
  
  const char* skybox_textures[6] = {
    "./resources/skybox/right.png",
    "./resources/skybox/left.png",
    "./resources/skybox/top.png",
    "./resources/skybox/bottom.png",
    "./resources/skybox/back.png",
    "./resources/skybox/front.png"
  };
  SkyBox skybox;
  skybox.load_skybox_textures(skybox_textures);
  glUseProgram(render.skybox_shader_program.shader_program);
  update_uniform_1i("skybox", render.skybox_shader_program.shader_program, 0);

  // glEnable(GL_DEPTH_TEST);

  //std::cout << "Width " << state.screen_width << " Height -> " << state.screen_height << std::endl;
  // pre-main loop optimization
  FMath::Mat4 model(1.0f), inverse_model(1.0f);
  model = model.translate({0.0f, 0.0f, 0.0f});
  model = model.scale({0.2f, 0.2f, 0.2});
  inverse_model = inverse_model.model_mat_inverse(model);
  inverse_model = inverse_model.transpose();

  // projection matrix does not change for us since it is only first person view
  FMath::Mat4 projection(1.0f);
  projection = projection.perspective(1.0f, 30.0f,0.01f,100.0f); // 30 degrees so 30.0f

  // auto project = glm::perspective(glm::radians(30.0f),1.0f,0.1f,10.0f);

  glEnable(GL_CULL_FACE);
  auto now = std::chrono::steady_clock::now();
  auto then = now;

  FMath::Vec3<float> atten_vec[9] =
    {
      FMath::Vec3(1.0f,0.44f,0.35f),
      FMath::Vec3(1.0f,0.44f,0.35f),
      FMath::Vec3(1.0f,0.44f,0.35f),
      FMath::Vec3(1.0f,0.44f,0.35f),
      FMath::Vec3(1.0f,0.44f,0.35f),
      
      FMath::Vec3(0.1f,0.1f,0.1f),
      FMath::Vec3(0.1f,0.1f,0.1f),
      FMath::Vec3(0.1f,0.1f,0.1f),
      FMath::Vec3(0.1f,0.1f,0.1f)
    };
      
  
  while (!glfwWindowShouldClose(render.window)) {


    zrender.clearBG(0.25f,0.25f,0.25f,1.0f);
    zrender.clearDepth();

    glUseProgram(render.shader_program.shader_program);
    FMath::Mat4 view(1.0f), cube_model(1.0f);

    view = state.camera.ViewMatrix();

    update_uniform_3f("viewPos", render.shader_program.shader_program,
                      state.camera.camera_pos);
    for (int i = 0; i < NR_LIGHT_CUBES; i++) {
      update_uniform_3f(
          std::string("light[" + std::to_string(i) + "].position").c_str(),
          render.shader_program.shader_program, cubes[i].position);

      update_uniform_3f(std::string("light[" + std::to_string(i) + "].ambient").c_str(), render.shader_program.shader_program,
                        ambient(cubes[i].color));
      update_uniform_3f(std::string("light[" + std::to_string(i) + "].diffuse").c_str(), render.shader_program.shader_program,
                        diffuse(cubes[i].color));
      update_uniform_3f(std::string("light[" + std::to_string(i) + "].specular").c_str(), render.shader_program.shader_program,
                        specular(cubes[i].color));

      // TODO: maybe use default values for this one? just an optimization thought
      // update_uniform_1f(std::string("light[" + std::to_string(i) + "].constantAtten").c_str(),
      //                   render.shader_program.shader_program, 1.0f);
      // update_uniform_1f(std::string("light[" + std::to_string(i) + "].linearAtten").c_str(),
      //                   render.shader_program.shader_program, 0.35f);
      // update_uniform_1f(std::string("light[" + std::to_string(i) + "].quadraticAtten").c_str(),
      //                   render.shader_program.shader_program, 0.44f);

      update_uniform_1f(std::string("light[" + std::to_string(i) + "].constantAtten").c_str(),
                        render.shader_program.shader_program, atten_vec[i].x);
      update_uniform_1f(std::string("light[" + std::to_string(i) + "].linearAtten").c_str(),
                        render.shader_program.shader_program, atten_vec[i].y);
      update_uniform_1f(std::string("light[" + std::to_string(i) + "].quadraticAtten").c_str(),
                        render.shader_program.shader_program, atten_vec[i].z);

    }


    glUseProgram(render.shader_program.shader_program);
    update_uniform_matrix_4f("model", render.shader_program.shader_program, &model[0][0]);
    update_uniform_matrix_4f("inv_model", render.shader_program.shader_program, &inverse_model[0][0]);
    update_uniform_matrix_4f("projection", render.shader_program.shader_program,projection.value_ptr());
    update_uniform_matrix_4f("view", render.shader_program.shader_program, &view[0][0]);

    
    glUseProgram(zprogram);
    update_uniform_matrix_4f("model", zprogram, model.value_ptr());
    update_uniform_matrix_4f("projection", zprogram, projection.value_ptr());//projection.value_ptr());
    update_uniform_matrix_4f("view", zprogram, view.value_ptr());
    
    


    glUseProgram(render.skybox_shader_program.shader_program);



    update_uniform_matrix_4f("projection", render.skybox_shader_program.shader_program, &projection[0][0]);
    update_uniform_matrix_4f("view", render.skybox_shader_program.shader_program, &view[0][0]);

    glBindFramebuffer(GL_FRAMEBUFFER,zrender.colorFBO);
    skybox.draw_skybox();
    
    glUseProgram(zprogram);

    zrender.clearDepth();
     
    // zrender.clearDepth();
    // // mainModel.draw_model(render.shader_program.shader_program);

    //  modal.draw_model(render.shader_program.shader_program,zprogram,zrender);
    mainModel.draw_model(render.shader_program.shader_program,zprogram,zrender);
    // update_uniform_matrix_4f("model", zprogram, model.scale({5.0f,1.0f,5.0f}).value_ptr());
    floorModel.draw_model(render.shader_program.shader_program,zprogram,zrender);
    // // // light cube render
    // // glUseProgram(render.light_cube_shader_program.shader_program);
    // // update_uniform_matrix_4f("projection", render.light_cube_shader_program.shader_program, &projection[0][0]);
    // // update_uniform_matrix_4f("view", render.light_cube_shader_program.shader_program, &view[0][0]);

    // // // TODO: use instanced cubes
    // // glBindVertexArray(cubes[0].VAO);
    // // for (int i = 0; i < NR_LIGHT_CUBES; i++){
    // //   cube_model = FMath::Mat4<float>(1.0f);
    // //   cube_model = cube_model.translate(cubes[i].position);
    // //   cube_model = cube_model.scale(cubes[i].scale);

    // //   update_uniform_3f("cubeColor", render.light_cube_shader_program.shader_program, cubes[i].color);
    // //   update_uniform_matrix_4f("model", render.light_cube_shader_program.shader_program, &cube_model[0][0]);
    // //   glDrawArrays(GL_TRIANGLES, 0, 36);
    // // }
    
    zrender.draw();

    handleEvents(render.window);
    now = std::chrono::steady_clock::now();
    //std::cout << "FPS is -> " << 1000.0f/std::chrono::duration_cast<std::chrono::milliseconds>(now-then).count() << std::endl;
    then = now;
    glfwPollEvents();
    glfwSwapBuffers(render.window);
  }

  glDeleteProgram(render.shader_program.shader_program);
  glDeleteProgram(render.light_cube_shader_program.shader_program);
  glDeleteProgram(render.skybox_shader_program.shader_program);
  Render::DestroyRenderer(&render);
  return 0;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  // Doubt xoffset will be produced
  state.fovy -= yoffset;
  if (state.fovy > 90)
    state.fovy = 89.0f;
  else
    state.fovy = 0.0f;
}

void handleEvents(GLFWwindow *window) {
  // This particular part could be made event but it doesn't matter much here
  double xPos, yPos;
  glfwGetCursorPos(window, &xPos, &yPos);
  auto delta_xPos = -xPos + state.last_xpos;
  auto delta_yPos = yPos - state.last_ypos;

  if (state.first_mouse) {
    delta_yPos = delta_xPos = 0;
    state.first_mouse = false;
  }

  state.last_xpos = xPos;
  state.last_ypos = yPos;
  state.pitch += -delta_yPos * state.angle_constant;
  state.yaw -= delta_xPos * state.angle_constant;

  if (state.pitch > 1.5)
    state.pitch = 1.5;
  else if (state.pitch < -1.5)
    state.pitch = -1.5;

  state.camera.camera_front.x = std::sin(state.yaw) * std::cos(state.pitch);
  state.camera.camera_front.y = std::sin(state.pitch);
  state.camera.camera_front.z = -std::cos(state.pitch) * std::cos(state.yaw);

  state.camera.camera_front = state.camera.camera_front.unitVec();

  // Now handle the movement
  // Have to handle the first frame here
  constexpr float move_speed = 0.1f;
  auto current_frame = glfwGetTime();
  state.delta_time = current_frame - state.last_frame;
  state.last_frame = current_frame;

  if (state.first_frame) {
    state.delta_time = 0.0f;
    state.first_frame = false;
  }

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    state.camera.camera_pos =
        state.camera.camera_pos + state.camera.camera_front * state.delta_time * 0.6f;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    state.camera.camera_pos =
        state.camera.camera_pos - state.camera.camera_front * state.delta_time * 0.6f;
  }
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS){
    std::cout << state.camera.camera_pos << std::endl;
  }
}
